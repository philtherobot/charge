#include <catch2/catch.hpp>

#include <stirrup/error.hpp>
#include <stirrup/stream.hpp>

using namespace stirrup;

using std::begin;
using std::end;
using std::u32string;
using std::vector;

SCENARIO("Input memory stream")
{
    GIVEN("a memory stream on an input buffer")
    {
        u32string input_data = U"hello";

        input_stream s = create_memory_input_stream(input_data);

        WHEN("we read from the memory stream")
        {
            CHECK(s.read(2) == U"he");
            CHECK(s.read(2) == U"ll");
            CHECK(s.read(2) == U"o");
            CHECK(s.read(2).empty());
            CHECK(s.read(2).empty());
        }
    }

    GIVEN("a memory stream on an output buffer")
    {
        u32string output_buffer;

        output_stream s = create_memory_output_stream(output_buffer);

        WHEN("we write to the memory stream")
        {
            s.write(U"he");
            CHECK(output_buffer == U"he");

            s.write(U"llo");
            CHECK(output_buffer == U"hello");
        }
    }
}
