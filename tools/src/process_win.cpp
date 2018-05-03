
#include "tools/process.hpp"

#include <boost/algorithm/string/erase.hpp>

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include <windows.h>


using namespace std::string_literals;


namespace tools
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


class ErrnoError : public Exception
{
public:
	explicit ErrnoError(std::string const & function, int code = 0)
		:
		Exception( msg(function, code) )
	{}

private:
	static std::string msg(std::string const & function, int code)
	{
		std::ostringstream os;
		os << "system (errno) error " << code;
		os << " in function " << function;
		return os.str();
	}
};


std::vector<char> strcpy(std::string const & str)
{
    std::vector<char> r;
    r.resize(str.size() + 1);
	std::copy(str.begin(), str.end(), r.begin());
	r.push_back(0);
    return r;
}


std::string getenv(std::string const & var)
{
	char * buf = 0;
	int err = _dupenv_s(&buf, NULL, var.c_str());
	if (err != 0) throw ErrnoError("_dupenv_s", err);

	if (!buf) throw Exception("failed to find environment variable " + var);

	std::string r{ buf };
	free(buf);

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


class Process::Implementation
{
public:
    Implementation() : process_handle_(INVALID_HANDLE_VALUE) {}

    ~Implementation()
    {
        CloseHandle(process_handle_);
    }

    HANDLE process_handle_;
};


Process::Process()
    : impl_(new Implementation())
{}


Process::~Process()
{}


void Process::start(std::string const & shell_command)
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
        exec_buf.data(),      // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &start_info,  // STARTUPINFO pointer 
        &process_info);  // receives PROCESS_INFORMATION 

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

    child_stdout_.reset(new Reader(child_stdout.read_));
}


int Process::exit_code()
{
    DWORD code = 0;
    if (!GetExitCodeProcess(impl_->process_handle_, &code))
    {
        throw Win32Error("GetExitCodeProcess");
    }

    return int(code);
}

} //tools
