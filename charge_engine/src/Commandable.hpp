#ifndef GUARD_46eb9e2018fb40cca751a842508deeb1
#define GUARD_46eb9e2018fb40cca751a842508deeb1

#include <charge/types.hpp>

namespace charge
{

class Commandable
{
public:
    virtual ~Commandable() {}

    virtual int compile() = 0;
    virtual int update() = 0;
    virtual int execute(StringList const & args) = 0;
    virtual int copy_executable() = 0;
    virtual int purge() = 0;
};

} // charge

#endif
