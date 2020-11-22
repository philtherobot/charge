#include "stirrup/environment_variable.hpp"

#include <cstdlib>
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

string getenv(string const & variable_name)
{
    char * buffer = nullptr;
    const auto error = _dupenv_s(&buffer, nullptr, variable_name.c_str());

    if (error)
    {
        // todo-php: use make_errno_error_message
        throw runtime_error("_dupenv_s failure");
    }

    if (!buffer)
    {
        return string();
    }

    auto_free_block auto_free(buffer);

    return buffer;
}

}
