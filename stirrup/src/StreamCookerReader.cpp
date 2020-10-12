#include "stirrup/StreamCookerReader.hpp"


namespace stirrup
{

using std::string;

StreamCookerReader::StreamCookerReader(ReadableStream & binary_stream_source)
    : source_(binary_stream_source), state_(State::START)
{}

string StreamCookerReader::read()
{
    for (;;)
    {
        auto new_block = source_.read();

        if (new_block.empty()) return string();

        auto output = cook(new_block);

        if (!output.empty())
        {
            return output;
        }
        // Else, go read more...

    } // end forever
}

string StreamCookerReader::cook(string const & new_block)
{
    string::const_iterator input = new_block.begin();

    string output;
    output.reserve(new_block.size());

    while (input != new_block.end())
    {
        char c = *input;
        ++input;

        switch (state_)
        {
            case State::START:in_start_state(c, output);
                break;

            case State::CR_DETECTED:in_cr_detected_state(c, output);
                break;
        }
    } // end while

    return output;
}

void StreamCookerReader::in_start_state(char c, string & output)
{
    if (c == '\r')
    {
        state_ = State::CR_DETECTED;
    }
    else
    {
        output.push_back(c);
    }
}

void StreamCookerReader::in_cr_detected_state(char c, string & output)
{
    if (c == '\n')
    {
        output.push_back('\n');
    }
    else
    {
        output.push_back(c);
    }
    state_ = State::START;
}

}
