#ifndef GUARD_5ea4ecbeaba3491a899bb905eb7cd8de
#define GUARD_5ea4ecbeaba3491a899bb905eb7cd8de

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace charge
{

using FileList = std::vector<boost::filesystem::path>;

std::string getenv(std::string const & var);

std::string hostname();

boost::filesystem::path home_path();

void check_absolute(boost::filesystem::path const & p);

std::string quote(std::string const & str);
std::string quote_if_needed(std::string const & str);

boost::optional<std::string> consume_line(std::string & buf_inout);

std::string make_errno_message(std::string const & from_function, int err);

class ProgramDetector
{
public:
    struct Result
    {
        std::string const captured_;
        int const exit_code_;
    };

    virtual ~ProgramDetector() {}

    virtual boost::optional<Result> look_for_program(std::string const & cmd);
};


} // charge

#endif
