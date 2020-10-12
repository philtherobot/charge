#include <boost/test/unit_test.hpp>

#include "stirrup/stirrup.hpp"
#include "stirrup/StreamCookerReader.hpp"
#include "stirrup/test/FakeReadableStream.hpp"

namespace stirrup
{

using std::string;

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_stream_cooker_reader);

class ReaderFixture
{
public:
    ReaderFixture() : reader_(source_stream_) {}

    void queueReadResult(string const & read_result)
    {
        source_stream_.queueReadResult(read_result);
    }

    string read()
    {
        return reader_.read();
    }

private:
    FakeReadableStream source_stream_;
    StreamCookerReader reader_;
};

BOOST_AUTO_TEST_CASE(case_empty_stream)
{
    ReaderFixture fixture;

    BOOST_CHECK_EQUAL(fixture.read(), "");
}

BOOST_AUTO_TEST_CASE(case_only_crlf)
{
    ReaderFixture fixture;
    fixture.queueReadResult("we\r\ngot\r\nsome\r\nCRLF\r\n");

    BOOST_CHECK_EQUAL(fixture.read(), "we\ngot\nsome\nCRLF\n");
}

BOOST_AUTO_TEST_CASE(case_lone_cr)
{
    ReaderFixture fixture;
    fixture.queueReadResult("it is\ralone");

    BOOST_CHECK_EQUAL(fixture.read(), "it isalone");
}

BOOST_AUTO_TEST_CASE(case_crlf_in_two_reads)
{
    ReaderFixture fixture;
    fixture.queueReadResult("first\r");
    fixture.queueReadResult("\nthen");

    BOOST_CHECK_EQUAL(fixture.read(), "first");
    BOOST_CHECK_EQUAL(fixture.read(), "\nthen");
}

BOOST_AUTO_TEST_CASE(case_lone_lf)
{
    ReaderFixture fixture;
    fixture.queueReadResult("it is\nalone");

    BOOST_CHECK_EQUAL(fixture.read(), "it is\nalone");
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

}
