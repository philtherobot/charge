
#include "process.hpp"
#include "tools.hpp"

#include "exception.hpp"

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


namespace charge
{

namespace
{

std::string get_error_message(DWORD code)
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
		0, NULL);

	std::string msg(static_cast<char const *>(msg_buf));

	LocalFree(msg_buf);

	boost::algorithm::erase_all(msg, "\r\n");

	return msg;
}


class Win32Error : public Exception
{
public:
	explicit Win32Error(std::string const & function, DWORD code = 0)
		:
		Exception("Win32 error \""s +
			get_error_message(code == 0 ? GetLastError() : code) +
			"\" in function " + function)
	{}
};


std::vector<char> strcpy(std::string const & str)
{
    std::vector<char> r;
    r.resize(str.size() + 1);
	std::copy(str.begin(), str.end(), r.begin());
	r.push_back(0);
    return r;
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
        throw Win32Error("CreatePipe");
    }
}


void Pipe::set_inherit(HANDLE h)
{
    if (!SetHandleInformation(h, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT))
    {
        throw Win32Error("SetHandleInformation");
    }
}


class StreamCookerReader : public ReadableStream
{
public:
	explicit StreamCookerReader(ReadableStream & binary_stream_source)
		: source_(binary_stream_source), state_(START)
	{}

	virtual boost::optional<std::string> read()
	{
		for (;;)
		{
			auto new_block_opt = source_.read();

			if (!new_block_opt) return boost::optional<std::string>();

			auto output = cook(*new_block_opt);

			if (!output.empty())
			{
				return output;
			}
			// Else, go read more...

		} // end forever
	}

private:
	std::string cook(std::string const & new_block)
	{
		std::string::const_iterator input = new_block.begin();

		std::string output;
		output.reserve(new_block.size());

		while (input != new_block.end())
		{
			char c = *input;
			++input;

			switch (state_)
			{
			case START:
				in_start_state(c, output);
				break;

			case CR_DETECTED:
				in_cr_detected_state(c, output);
				break;
			}
		} // end while

		return output;
	}

	void in_start_state(char c, std::string & output)
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

	void in_cr_detected_state(char c, std::string & output)
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


class Reader : public ReadableStream
{
public:
    explicit Reader(HANDLE h) : h_(h)
    {}

    virtual ~Reader()
    {
        CloseHandle(h_);
    }

    virtual boost::optional<std::string> read()
    {
        char buf[1024];
        DWORD nb_read = 0;
        BOOL success = ReadFile(h_, buf, sizeof(buf), &nb_read, NULL);
        if (!success)
        {
            DWORD code = GetLastError();
            if (code == ERROR_BROKEN_PIPE)
            {
                return boost::optional<std::string>();
            }
            throw Win32Error("ReadFile", code);
        }

        if (nb_read)
        {
            return std::string(buf, nb_read);
        }

        // 0 read bytes => end of file
        return boost::optional<std::string>();
    }

    HANDLE h_;
};


} // anonymous


class ShellProcess::Implementation
{
public:
    Implementation() : process_handle_(INVALID_HANDLE_VALUE) {}

    ~Implementation()
    {
        CloseHandle(process_handle_);
    }

	std::unique_ptr<Reader> raw_reader_;
    HANDLE process_handle_;
};


ShellProcess::ShellProcess()
    : impl_(new Implementation())
{}


ShellProcess::~ShellProcess()
{}

void ShellProcess::start(std::string const & shell_command)
{
    Pipe child_stdout;
    child_stdout.set_inherit(child_stdout.write_);


    std::string const cmd(getenv("COMSPEC"));

    std::string exec(cmd + " /C " + shell_command);


    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));


    STARTUPINFO start_info;

    ZeroMemory(&start_info, sizeof(STARTUPINFO));
    start_info.cb = sizeof(STARTUPINFO);
    start_info.hStdError = child_stdout.write_;
    start_info.hStdOutput = child_stdout.write_;
    start_info.dwFlags |= STARTF_USESTDHANDLES;


    BOOL success = FALSE;

    auto exec_buf = strcpy(exec);

    success = CreateProcess(
        NULL,
        exec_buf.data(),  // command line 
        NULL,             // process security attributes 
        NULL,             // primary thread security attributes 
        TRUE,             // handles are inherited 
        0,                // creation flags 
        NULL,             // use parent's environment 
        NULL,             // use parent's current directory 
        &start_info,      // STARTUPINFO pointer 
        &process_info);   // receives PROCESS_INFORMATION 

    // If an error occurs, exit the application. 
    if (!success)
    {
        throw Win32Error("CreateProcess");
    }

    impl_->process_handle_ = process_info.hProcess;

    CloseHandle(process_info.hThread);

    // Child process has the write handle, we
    // must not keep it also.
    CloseHandle(child_stdout.write_);

	impl_->raw_reader_.reset(new Reader(child_stdout.read_));
	Reader * raw_reader = impl_->raw_reader_.get();

    child_stdout_.reset(new StreamCookerReader(*raw_reader));
}


int ShellProcess::exit_code()
{
    DWORD code = 0;
    if (!GetExitCodeProcess(impl_->process_handle_, &code))
    {
        throw Win32Error("GetExitCodeProcess");
    }

    return int(code);
}


int exec(std::string const & pgm, StringList const & args)
{
	std::string cmdline = write_command_string(pgm, args);

	auto cmdline_buf = strcpy(cmdline);

	PROCESS_INFORMATION process_info;
	ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));

	STARTUPINFO start_info;

	ZeroMemory(&start_info, sizeof(STARTUPINFO));
	start_info.cb = sizeof(STARTUPINFO);

	BOOL success = FALSE;

	success = CreateProcess(
		pgm.c_str(),
		cmdline_buf.data(),  // command line 
		NULL,             // process security attributes 
		NULL,             // primary thread security attributes 
		FALSE,            // handles are inherited 
		0,                // creation flags 
		NULL,             // use parent's environment 
		NULL,             // use parent's current directory 
		&start_info,      // STARTUPINFO pointer 
		&process_info);   // receives PROCESS_INFORMATION 

	if (!success)
	{
		throw Win32Error("CreateProcess");
	}

	HANDLE proc_h = process_info.hProcess;

	WaitForSingleObject(proc_h, INFINITE);

	DWORD code = 0;
	if (!GetExitCodeProcess(proc_h, &code))
	{
		throw Win32Error("GetExitCodeProcess");
	}

	CloseHandle(proc_h);
	CloseHandle(process_info.hThread);

	return int(code);
}


std::string write_command_string(std::string const & pgm, StringList const & args)
{
	StringList strings(args);
	strings.insert(strings.begin(), pgm);

	boost::for_each(strings, 
		[](std::string & s) { s = quote_if_needed(s);  }
	);

	return boost::algorithm::join(strings, " ");
}


} // charge
