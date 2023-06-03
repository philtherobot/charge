
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

bool is_boost_test_not_found(LibraryNotConfiguredError const & ex)
{
    return
        ex.what() == "library boost_test is not configured"s &&
        ex.library() == "boost_test"s;
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

    BOOST_CHECK_EXCEPTION(
        find_imports(config, is),
        LibraryNotConfiguredError,
        is_boost_test_not_found
    );
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
