#include "InclusionNotesSniffer.hpp"

#include "tools.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>


namespace charge
{


InclusionNotesSniffer::InclusionNotesSniffer(ReadableStream & source)
    :
    source_(source),
    note_inclusion_("Note: including file:"),
    note_inclusion_end_pos_(note_inclusion_.size())
{}


boost::optional<std::string> InclusionNotesSniffer::read()
{
    for (;;)
    {

        auto new_block = source_.read();

        if (!new_block)
        {
            if (accumulator_.empty())
            {
                return boost::optional<std::string>();
            }

            std::string output;
            swap(output, accumulator_);
            return output;
        }

        // We are guaranteed to receive at least one character.
        assert(!(*new_block).empty());
        accumulator_ += *new_block;

        auto output = extract_output_from_accumulator();

        if (!output.empty()) return output;
        // else, wait for more data.

    }
}


std::string InclusionNotesSniffer::extract_output_from_accumulator()
{
    std::string output;

    while (auto line = consume_line(accumulator_))
    {
        using boost::algorithm::starts_with;

        if (starts_with(*line, note_inclusion_))
        {
            note(*line);
        }
        else
        {
            output += *line;
        }
    }

    return output;
}


void InclusionNotesSniffer::note(std::string const & line)
{
    auto remaining_size = line.size() - note_inclusion_end_pos_;

    auto raw_header_fn = line.substr(note_inclusion_end_pos_, remaining_size);

    using boost::algorithm::trim_copy;
    auto header_fn = trim_copy(raw_header_fn);

    inclusions_.push_back(header_fn);
}


} // charge
