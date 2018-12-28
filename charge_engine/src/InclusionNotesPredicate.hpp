#ifndef GUARD_e0f0a46971434f5c854dc39060bb0652
#define GUARD_e0f0a46971434f5c854dc39060bb0652

#include "tools.hpp"


namespace charge
{

class InclusionNotesPredicate
{
public:
    InclusionNotesPredicate();

    FileList inclusions_;

    bool operator() (std::string const & line);

private:
    void note(std::string const & line);
    std::string const note_inclusion_;
    std::string::size_type const note_inclusion_end_pos_;
};


} // charge

#endif
