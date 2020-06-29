
#include "../src/imports.hpp"

#include "helpers.hpp"

#include <charge/exception.hpp>

#include <boost/test/unit_test.hpp>


using namespace charge;
using namespace std::string_literals;


BOOST_AUTO_TEST_SUITE(suite_imports);

BOOST_AUTO_TEST_CASE(case_one_header_library)
{
    Config config(YAML::Load(
        "libraries:\n"
        "  boost_filesystem:\n"
        "    header_path: /home/phil/include/boost\n"
    ));

    auto pgm("// chargetrick import boost_filesystem\n"s);
    std::istringstream is(pgm);

    auto libs = find_imports(config, is);

    BOOST_CHECK_EQUAL(libs.header_paths_,
        StringList{ "/home/phil/include/boost" });
}

BOOST_AUTO_TEST_CASE(case_library_not_found)
{
    Config config(YAML::Load(
        "libraries:\n"
        "  boost_filesystem:\n"
        "    header_path: /home/phil/include/boost\n"
    ));

    auto pgm("// chargetrick import boost_test\n"s);
    std::istringstream is(pgm);

    boost::optional<LibraryNotConfiguredError> opt_ex(
        test::catch_exception<LibraryNotConfiguredError>(
            [&] {find_imports(config, is); }
            )
    );

    BOOST_REQUIRE(opt_ex);

    BOOST_CHECK_EQUAL(
        std::string((*opt_ex).what()),
        "library boost_test is not configured");

    BOOST_CHECK_EQUAL((*opt_ex).library(), "boost_test");
}

BOOST_AUTO_TEST_CASE(case_library_not_configured_error)
{
    LibraryNotConfiguredError ex("curl");

    LibraryNotConfiguredError copy_cons(ex);

    BOOST_CHECK_EQUAL(copy_cons.what(), "library curl is not configured");

    LibraryNotConfiguredError assign("a");
    assign = ex;

    BOOST_CHECK_EQUAL(assign.what(), "library curl is not configured");
}

BOOST_AUTO_TEST_SUITE_END();

