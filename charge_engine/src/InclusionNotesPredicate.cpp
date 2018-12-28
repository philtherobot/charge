#include "InclusionNotesPredicate.hpp"

#include "tools.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>


namespace charge
{


InclusionNotesPredicate::InclusionNotesPredicate()
    :
    note_inclusion_("Note: including file:"),
    note_inclusion_end_pos_(note_inclusion_.size())
{}


bool InclusionNotesPredicate::operator() (std::string const & line)
{
    using boost::algorithm::starts_with;

    if (!starts_with(line, note_inclusion_)) return true;

    note(line);
    return false;
}


void InclusionNotesPredicate::note(std::string const & line)
{
    auto remaining_size = line.size() - note_inclusion_end_pos_;

    auto raw_header_fn = line.substr(note_inclusion_end_pos_, remaining_size);

    using boost::algorithm::trim_copy;
    auto header_fn = trim_copy(raw_header_fn);

    inclusions_.push_back(header_fn);
}


} // charge
