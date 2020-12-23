#include <catch2/catch.hpp>

#include "stirrup/text_stream.hpp"

using stirrup::text_stream;

namespace
{

//class fake_stream : public stirrup::stream
//{
//public:
//    void queue_read_result(string const & read_result)
//    {
//        source_stream_.queue_read_result(read_result);
//    }
//
//    string read()
//    {
//        return reader_.read();
//    }
//
//private:
//    fake_readable_stream source_stream_;
//    stream_cooker_reader reader_;
//};

}

SCENARIO("Text stream")
{
    //fake_stream source_stream;
    //text_stream reader(source_stream);

    //CHECK(reader.read().empty());
}


//#include <boost/test/unit_test.hpp>
//
//#include "stirrup/text_stream.hpp"
//#include "stirrup/test/fake_readable_stream.hpp"
//
//using namespace stirrup;
//using std::string;
//
//BOOST_AUTO_TEST_SUITE(suite_stirrup);
//BOOST_AUTO_TEST_SUITE(suite_stream_cooker_reader);
//
//class reader_fixture
//{
//public:
//    reader_fixture()
//        : reader_(source_stream_)
//    {}
//
//    void queue_read_result(string const & read_result)
//    {
//        source_stream_.queue_read_result(read_result);
//    }
//
//    string read()
//    {
//        return reader_.read();
//    }
//
//private:
//    fake_readable_stream source_stream_;
//    stream_cooker_reader reader_;
//};
//
//
//BOOST_AUTO_TEST_CASE(case_only_crlf)
//{
//    reader_fixture fixture;
//    fixture.queue_read_result("we\r\ngot\r\nsome\r\nCRLF\r\n");
//
//    BOOST_CHECK_EQUAL(fixture.read(), "we\ngot\nsome\nCRLF\n");
//}
//
//BOOST_AUTO_TEST_CASE(case_lone_cr)
//{
//    reader_fixture fixture;
//    fixture.queue_read_result("it is\ralone");
//
//    BOOST_CHECK_EQUAL(fixture.read(), "it isalone");
//}
//
//BOOST_AUTO_TEST_CASE(case_crlf_in_two_reads)
//{
//    reader_fixture fixture;
//    fixture.queue_read_result("first\r");
//    fixture.queue_read_result("\nthen");
//
//    BOOST_CHECK_EQUAL(fixture.read(), "first");
//    BOOST_CHECK_EQUAL(fixture.read(), "\nthen");
//}
//
//BOOST_AUTO_TEST_CASE(case_lone_lf)
//{
//    reader_fixture fixture;
//    fixture.queue_read_result("it is\nalone");
//
//    BOOST_CHECK_EQUAL(fixture.read(), "it is\nalone");
//}
//
//BOOST_AUTO_TEST_SUITE_END();
//
//BOOST_AUTO_TEST_SUITE_END();
