#ifndef GUARD_0803cfd1f23d4c1eb7f5a31e6f6d7362
#define GUARD_0803cfd1f23d4c1eb7f5a31e6f6d7362

#include <string>

namespace charge
{

class ReadableStream
{
public:
    virtual ~ReadableStream() {}
    virtual std::string read() = 0;
};


} // charge

#endif
