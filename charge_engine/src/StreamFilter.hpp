#ifndef GUARD_0853e544d3e846a1ade9df2cc2c99479
#define GUARD_0853e544d3e846a1ade9df2cc2c99479

#include "ReadableStream.hpp"
#include "tools.hpp"


namespace charge
{

template <typename Predicate>
class StreamFilter : public ReadableStream
{
public:
    StreamFilter(ReadableStream & source, Predicate const & predicate);

    std::string read();

private:

    std::string extract_output_from_accumulator();

    ReadableStream & source_;
    Predicate predicate_;
    std::string accumulator_;
};


template <typename Predicate>
StreamFilter<Predicate> make_stream_filter(ReadableStream & source, Predicate const & predicate)
{
    return StreamFilter<Predicate>(source, predicate);
}


template <typename Predicate>
StreamFilter<Predicate>::StreamFilter(ReadableStream & source, Predicate const & predicate)
: source_(source), predicate_(predicate)
{}


template <typename Predicate>
std::string StreamFilter<Predicate>::read()
{
    for (;;)
    {

        auto new_block = source_.read();

        if (new_block.empty())
        {
            if (accumulator_.empty())
            {
                return std::string();
            }

            std::string output;
            swap(output, accumulator_);
            return output;
        }

        // We are guaranteed to receive at least one character.
        accumulator_ += new_block;

        auto output = extract_output_from_accumulator();

        if (!output.empty()) return output;
        // else, wait for more data.

    }
}


template <typename Predicate>
std::string StreamFilter<Predicate>::extract_output_from_accumulator()
{
    std::string output;

    while (auto line = consume_line(accumulator_))
    {
        if (predicate_(*line))
        {
            output += *line;
        }
    }

    return output;
}


} // charge

#endif
