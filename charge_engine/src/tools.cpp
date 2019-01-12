
#include "tools.hpp"

#include "process.hpp"

#include <charge/exception.hpp>

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <cctype>
#include <cerrno>
#include <sstream>


namespace charge
{

void check_absolute(boost::filesystem::path const & p)
{
    if (!p.is_absolute())
    {
        throw AbsolutePathError(p);
    }
}

std::string quote(std::string const & str)
{
    return std::string("\"") + str + std::string("\"");
}

std::string quote_if_needed(std::string const & str)
{
    auto isspace = [](unsigned char c){ return std::isspace(c); };

    if (boost::range::find_if(str, isspace) != str.end())
    {
        return quote(str);
    }

    return str;
}



boost::optional<std::string> consume_line(std::string & buf_inout)
{
    auto eol_iterator = boost::range::find(buf_inout, '\n');
    if (eol_iterator == buf_inout.end())
    {
        return boost::optional<std::string>();
    }

    auto eol_pos = std::distance(buf_inout.begin(), eol_iterator);
    auto after_eol_pos = eol_pos + 1;

    std::string line = buf_inout.substr(0, after_eol_pos);

    buf_inout.erase(0, after_eol_pos);

    return line;
}


std::string make_errno_message(std::string const & from_function, int err)
{
    std::ostringstream os;
    os << "system (errno) error " << err;
    os << " (" << strerror(err) << ") ";
    os << " in function " << from_function;
    return os.str();
}


boost::optional<ProgramDetector::Result> ProgramDetector::look_for_program(std::string const & cmd)
{
    ShellProcess process;

    process.start(cmd);

    std::string output;

    for (;;)
    {
        auto data = process.child_stdout_->read();
        if (data.empty()) break;
        output += data;
    }

    return Result{ output, process.exit_code() };
}


} // charge


namespace std
{

std::ostream & operator << (std::ostream & os, charge::FileList const & fl)
{
    os << '(';
    if (fl.size())
    {
        auto set_it = fl.begin();
        os << *set_it;
        while (++set_it != fl.end())
        {
            os << ',' << *set_it;
        }
    }
    os << ')';
    return os;
} 

} //std
