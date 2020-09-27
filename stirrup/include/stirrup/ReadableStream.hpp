#pragma once

#include <string>

namespace stirrup
{

class ReadableStream
{
public:
    virtual ~ReadableStream() {}
    virtual std::string read() = 0;
};

}
