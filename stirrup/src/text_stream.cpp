#include "stirrup/text_stream.hpp"


namespace stirrup
{

using std::u32string;
using std::size_t;

text_stream::text_stream(stream & binary_stream_source)
    : binary_stream_source_(binary_stream_source), state_(state::START)
{}

u32string text_stream::read(size_t read_size)
{
//    for (;;)
//    {
//        auto new_block = source_.read();
//
//        if (new_block.empty()) return string();
//
//        auto output = cook(new_block);
//
//        if (!output.empty())
//        {
//            return output;
//        }
//        // Else, go read more...
//
//    } // end forever
    return {};
}

//string text_stream::cook(string const & new_block)
//{
//    string::const_iterator input = new_block.begin();
//
//    string output;
//    output.reserve(new_block.size());
//
//    while (input != new_block.end())
//    {
//        char c = *input;
//        ++input;
//
//        switch (state_)
//        {
//            case state::START:in_start_state(c, output);
//                break;
//
//            case state::CR_DETECTED:in_cr_detected_state(c, output);
//                break;
//        }
//    } // end while
//
//    return output;
//}
//
//void text_stream::in_start_state(char c, string & output)
//{
//    if (c == '\r')
//    {
//        state_ = state::CR_DETECTED;
//    }
//    else
//    {
//        output.push_back(c);
//    }
//}
//
//void text_stream::in_cr_detected_state(char c, string & output)
//{
//    if (c == '\n')
//    {
//        output.push_back('\n');
//    }
//    else
//    {
//        output.push_back(c);
//    }
//    state_ = state::START;
//}

}
