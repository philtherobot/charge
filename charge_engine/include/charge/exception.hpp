#ifndef GUARD_310c5f06f04f4590a8d9bc3d57e5b43e
#define GUARD_310c5f06f04f4590a8d9bc3d57e5b43e


#include <boost/filesystem/path.hpp>

#include <exception>
#include <string>


namespace charge
{

class Exception : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};


class LibraryNotConfiguredError : public Exception
{
public:
	explicit LibraryNotConfiguredError(std::string const & lib);

	std::string const & library() const;

private:
	std::string lib_;
};


class CommandLineArgumentError : public Exception
{
public:
	explicit CommandLineArgumentError(std::string const & msg);
};


class UnsupportedFamilyError : public Exception
{
public:
	explicit UnsupportedFamilyError(std::string const & family);

	std::string const & family() const;

private:
	std::string family_;
};


class ErrnoError : public Exception
{
public:
	explicit ErrnoError(std::string const & function, int code = 0);

private:
	static std::string msg(std::string const & function, int code);
};


class AbsolutePathError : public Exception
{
public:
	explicit AbsolutePathError(boost::filesystem::path const & p);

private:
	static std::string msg(boost::filesystem::path const & p);
};


} // charge

#endif
