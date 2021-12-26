#include "stirrup/environment_variable.hpp"

#include "stirrup/error.hpp"
#include "stirrup/string.hpp"

#include <stdlib.h>
#include <stdexcept>

using namespace std;

namespace stirrup
{

namespace
{

class auto_free_block
{
public:
    explicit auto_free_block(void * allocated_memory)
        : allocated_memory_(allocated_memory)
    {}

    ~auto_free_block()
    {
        free(allocated_memory_);
    }

private:
    void * const allocated_memory_;
};

}

u32string getenv(u32string const & variable_name)
{
    wchar_t * buffer = nullptr;
    const auto error = _wdupenv_s(&buffer, nullptr, transcode_to_wstring(variable_name).c_str());

    throw_on_errno(error);

    if (!buffer)
    {
        return {};
    }

    auto_free_block auto_free(buffer);

    return transcode_from(buffer);
}

void putenv(std::u32string const &variable_name, std::u32string const &value)
{
    const auto error = _wputenv_s(transcode_to_wstring(variable_name).c_str(), transcode_to_wstring(value).c_str());
    throw_on_errno(error);
}

}
