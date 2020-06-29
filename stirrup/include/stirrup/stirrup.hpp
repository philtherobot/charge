#pragma once

#include <string>

namespace stirrup
{

using str_char = unsigned char;

class str: public std::basic_string<str_char>
{
public:
    using std::basic_string<str_char>::basic_string;
    str(char const * system_encoded_char_array);

    bool operator==(char const * system_encoded_char_array) const;
};

std::ostream & operator<<(std::ostream & os, str const & s);

std::string getenv(std::string const & variable_name);

std::string make_errno_message(std::string const & from_function, int err);

namespace str_construction
{
str from(std::string const & source);

std::wstring wstring_from_system_string(std::string const & system_encoded_string);

}

class scoped_locale
{
public:
    explicit scoped_locale(int category, char const * locale);
    ~scoped_locale();
private:
    int category_;
    std::string previous_locale_;
};

}
