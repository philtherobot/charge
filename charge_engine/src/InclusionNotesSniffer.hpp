#ifndef GUARD_e0f0a46971434f5c854dc39060bb0652
#define GUARD_e0f0a46971434f5c854dc39060bb0652

#include "ReadableStream.hpp"
#include "tools.hpp"


namespace charge
{

class InclusionNotesSniffer : public ReadableStream
{
public:

    explicit InclusionNotesSniffer(ReadableStream & source);

    FileList inclusions_;

    virtual boost::optional<std::string> read();

private:
    std::string extract_output_from_accumulator();
    void note(std::string const & line);

    ReadableStream & source_;

    std::string const note_inclusion_;
    std::string::size_type const note_inclusion_end_pos_;
    std::string accumulator_;
};

} // charge

#endif
