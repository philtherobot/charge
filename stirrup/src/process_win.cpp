
#include "stirrup/process.hpp"

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include <windows.h>


using namespace std::string_literals;
using std::string;
using std::vector;


namespace stirrup
{

namespace
{

string get_command_line(string const & pgm, StringList const & args)
{
    StringList strings;

    strings.push_back(pgm);
    copy(args.begin(), args.end(), back_inserter(strings));

    boost::for_each(
        strings,
        [](string & s)
        { s = quote_if_needed(s); }
    );

    return boost::algorithm::join(strings, " ");
}

string get_error_message(DWORD code)
{
    LPVOID msg_buf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msg_buf,
        0, NULL
    );

    string msg(static_cast<char const *>(msg_buf));

    LocalFree(msg_buf);

    boost::algorithm::erase_all(msg, "\r\n");

    return msg;
}

string make_win32_error_message(string const & from_function, DWORD code = 0)
{
    return
        "Win32 error \""s +
            get_error_message(code == 0 ? GetLastError() : code) +
            "\" in function " + from_function;
}

vector<char> strcpy(string const & str)
{
    vector<char> r;
    r.resize(str.size() + 1);
    std::copy(str.begin(), str.end(), r.begin());
    r.push_back(0);
    return r;
}

HANDLE create_process(string const & pgm, StringList const & args, STARTUPINFO & start_info)
{
    string command_line = get_command_line(pgm, args);

    auto command_line_buffer = strcpy(command_line);

    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));

    BOOL success = FALSE;

    success = CreateProcess(
        NULL,
        command_line_buffer.data(),
        NULL,             // process security attributes
        NULL,             // primary thread security attributes
        FALSE,            // handles are inherited
        0,                // creation flags
        NULL,             // use parent's environment
        NULL,             // use parent's current directory
        &start_info,
        &process_info
    );

    if (!success)
    {
        auto const message = "In create_process: " + make_win32_error_message("CreateProcess") +
            " command " + pgm;

        throw std::runtime_error(message);
    }

    CloseHandle(process_info.hThread);

    return process_info.hProcess;
}

class Pipe
{
public:
    Pipe();

    HANDLE read_;
    HANDLE write_;

    static void set_inherit(HANDLE h);
};

Pipe::Pipe()
    :
    read_(INVALID_HANDLE_VALUE),
    write_(INVALID_HANDLE_VALUE)
{
    if (!CreatePipe(&read_, &write_, 0, 0))
    {
        throw std::runtime_error(make_win32_error_message("CreatePipe"));
    }
}

void Pipe::set_inherit(HANDLE h)
{
    if (!SetHandleInformation(h, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT))
    {
        throw std::runtime_error(make_win32_error_message("SetHandleInformation"));
    }
}

class StreamCookerReader: public ReadableStream
{
public:
    explicit StreamCookerReader(ReadableStream & binary_stream_source)
        : source_(binary_stream_source), state_(START)
    {}

    virtual string read()
    {
        for (;;)
        {
            auto new_block = source_.read();

            if (new_block.empty()) return string();

            auto output = cook(new_block);

            if (!output.empty())
            {
                return output;
            }
            // Else, go read more...

        } // end forever
    }

private:
    string cook(string const & new_block)
    {
        string::const_iterator input = new_block.begin();

        string output;
        output.reserve(new_block.size());

        while (input != new_block.end())
        {
            char c = *input;
            ++input;

            switch (state_)
            {
                case START:in_start_state(c, output);
                    break;

                case CR_DETECTED:in_cr_detected_state(c, output);
                    break;
            }
        } // end while

        return output;
    }

    void in_start_state(char c, string & output)
    {
        if (c == '\r')
        {
            state_ = CR_DETECTED;
        }
        else
        {
            output.push_back(c);
        }
    }

    void in_cr_detected_state(char c, string & output)
    {
        if (c == '\n')
        {
            output.push_back('\n');
        }
        else
        {
            output.push_back(c);
        }
        state_ = START;
    }

    ReadableStream & source_;

    enum
    {
        START,
        CR_DETECTED
    } state_;
};

class WindowsHandleReader: public ReadableStream
{
public:
    explicit WindowsHandleReader(HANDLE h)
        : h_(h)
    {}

    virtual ~WindowsHandleReader()
    {
        CloseHandle(h_);
    }

    virtual string read()
    {
        char buf[1024];
        DWORD nb_read = 0;
        BOOL success = ReadFile(h_, buf, sizeof(buf), &nb_read, NULL);
        if (!success)
        {
            DWORD code = GetLastError();
            if (code == ERROR_BROKEN_PIPE)
            {
                return string();
            }
            throw std::runtime_error(make_win32_error_message("ReadFile", code));
        }

        if (nb_read)
        {
            return string(buf, nb_read);
        }

        // 0 read bytes => end of file
        return string();
    }

    HANDLE h_;
};

STARTUPINFO getDefaultStartupInfo()
{
    STARTUPINFO startup_info;
    ZeroMemory(&startup_info, sizeof(STARTUPINFO));
    startup_info.cb = sizeof(STARTUPINFO);
    return startup_info;
}

} // anonymous


class SystemProcess::Implementation
{
public:
    Implementation()
        : process_handle_(INVALID_HANDLE_VALUE)
    {}

    ~Implementation()
    {
        CloseHandle(process_handle_);
    }

    std::unique_ptr<WindowsHandleReader> process_output_handle_reader_;
    HANDLE process_handle_;
};

SystemProcess::SystemProcess()
    : impl_(new Implementation())
{}

SystemProcess::~SystemProcess()
{}

void SystemProcess::start(string const & pgm, StringList const & args)
{
    STARTUPINFO startup_info = getDefaultStartupInfo();

    impl_->process_handle_ = create_process(pgm, args, startup_info);
}

std::shared_ptr<ReadableStream> SystemProcess::start_capture_output(string const & pgm, StringList const & args)
{

    STARTUPINFO startup_info = getDefaultStartupInfo();

    Pipe child_stdout;
    child_stdout.set_inherit(child_stdout.write_);

    startup_info.hStdError = child_stdout.write_;
    startup_info.hStdOutput = child_stdout.write_;
    startup_info.dwFlags |= STARTF_USESTDHANDLES;

    impl_->process_handle_ = create_process(pgm, args, startup_info);

    // Child process has the write handle, we
    // must not keep it also.
    CloseHandle(child_stdout.write_);

    impl_->process_output_handle_reader_.reset(new WindowsHandleReader(child_stdout.read_));

    return std::make_shared<StreamCookerReader>(*impl_->process_output_handle_reader_);
}

int SystemProcess::wait_for_exit_code()
{
    WaitForSingleObject(impl_->process_handle_, INFINITE);

    DWORD code = 0;
    if (!GetExitCodeProcess(impl_->process_handle_, &code))
    {
        throw std::runtime_error(make_win32_error_message("GetExitCodeProcess"));
    }

    return int(code);
}

}
