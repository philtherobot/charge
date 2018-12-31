
#include "tools.hpp"

#include "process.hpp"

#include <charge/exception.hpp>

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <cctype>


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
    if (boost::range::find_if(str, std::isspace) != str.end())
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


boost::optional<ProgramDetector::Result> ProgramDetector::look_for_program(std::string const & cmd)
{
    ShellProcess process;

    process.start(cmd);

    std::string output;

    while (auto data = process.child_stdout_->read())
    {
        output += *data;
    }

    return Result{ output, process.exit_code() };
}


} // charge
