#include <boost/test/unit_test.hpp>

#include "stirrup/stirrup.hpp"
#include "stirrup/StreamCookerReader.hpp"
#include "stirrup/test/FakeReadableStream.hpp"

namespace stirrup
{

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_stream_cooker_reader);

BOOST_AUTO_TEST_CASE(case_empty_stream)
{
    FakeReadableStream sourceStream;
    StreamCookerReader reader(sourceStream);

    BOOST_CHECK_EQUAL(reader.read(), "");
}

BOOST_AUTO_TEST_CASE(case_only_crlf)
{
    FakeReadableStream sourceStream;
    sourceStream.queueReadResult("we\r\ngot\r\nsome\r\nCRLF\r\n");
    StreamCookerReader reader(sourceStream);

    BOOST_CHECK_EQUAL(reader.read(), "we\ngot\nsome\nCRLF\n");
}

BOOST_AUTO_TEST_CASE(case_lone_cr)
{
    FakeReadableStream sourceStream;
    sourceStream.queueReadResult("it is\ralone");
    StreamCookerReader reader(sourceStream);

    BOOST_CHECK_EQUAL(reader.read(), "it isalone");
}

BOOST_AUTO_TEST_CASE(case_crlf_in_two_reads)
{
    FakeReadableStream sourceStream;
    sourceStream.queueReadResult("first\r");
    sourceStream.queueReadResult("\nthen");
    StreamCookerReader reader(sourceStream);

    BOOST_CHECK_EQUAL(reader.read(), "first");
    BOOST_CHECK_EQUAL(reader.read(), "\nthen");
}

BOOST_AUTO_TEST_CASE(case_lone_lf)
{
    FakeReadableStream sourceStream;
    sourceStream.queueReadResult("it is\nalone");
    StreamCookerReader reader(sourceStream);

    BOOST_CHECK_EQUAL(reader.read(), "it is\nalone");
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

}
