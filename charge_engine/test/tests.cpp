
#define BOOST_TEST_MODULE charge_engine
#include <boost/test/included/unit_test.hpp>

#include "charge/charge.hpp"

#include <sstream>
#include <string>

using namespace charge;
using namespace std::string_literals;

template <typename ExceptionType, typename Callable>
boost::optional<ExceptionType> catch_exception(Callable c)
{
    try
    {
        c();
    }
    catch( ExceptionType const & ex )
    {
        return ex;
    }
    return boost::optional<ExceptionType>();
}

BOOST_AUTO_TEST_CASE(one_header_library)
{
    YAML::Node config( YAML::Load(
        "libraries:\n"
        "  boost_filesystem:\n"
        "    header: /home/phil/include/boost\n"
    ));

    auto pgm("// chargetrick import boost_filesystem\n"s);
    std::istringstream is(pgm);

    auto deps = find_dependencies(config, is);

    BOOST_CHECK_EQUAL( deps.libraries_.headers_, 
        StringList{"/home/phil/include/boost"});
}

BOOST_AUTO_TEST_CASE(library_not_found)
{
    YAML::Node config( YAML::Load(
        "libraries:\n"
        "  boost_filesystem:\n"
        "    header: /home/phil/include/boost\n"
    ));

    auto pgm("// chargetrick import boost_test\n"s);
    std::istringstream is(pgm);

    boost::optional<LibraryNotConfiguredError> opt_ex(
        catch_exception<LibraryNotConfiguredError>( 
            [&]{find_dependencies(config, is);}
        ) 
    );

    BOOST_REQUIRE(opt_ex);

    BOOST_CHECK_EQUAL( 
        std::string( (*opt_ex).what() ), 
        "library boost_test is not configured" );
    
    BOOST_CHECK_EQUAL( (*opt_ex).library(), "boost_test" );
}

BOOST_AUTO_TEST_CASE( library_not_configured_error )
{
    LibraryNotConfiguredError ex("curl");

    LibraryNotConfiguredError copy_cons(ex);

    BOOST_CHECK_EQUAL( copy_cons.what(), "library curl is not configured" );

    LibraryNotConfiguredError assign("a");
    assign = ex;

    BOOST_CHECK_EQUAL( assign.what(), "library curl is not configured" );
}
