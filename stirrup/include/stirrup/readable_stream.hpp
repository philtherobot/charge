#pragma once

#include <string>

namespace stirrup
{

class readable_stream
{
public:
    virtual ~readable_stream() = default;
    virtual std::string read() = 0;
};

}
