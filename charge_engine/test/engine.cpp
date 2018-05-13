
#include "charge/charge.hpp"
#include "../src/cache.hpp"
#include "../src/config.hpp"
#include "../src/engine.hpp"
#include "../src/process.hpp"

#include <boost/test/unit_test.hpp>

#include <regex>
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


BOOST_AUTO_TEST_SUITE(engine);

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

BOOST_AUTO_TEST_CASE(get_cache_pth)
{

#if defined( CHARGE_WINDOWS )

	auto path{ charge::get_cache_path("hostname",
		"C:\\Users\\philippe", 
		"T:\\dev\\exp\\script.cpp") };

	auto backslash{ "\\\\"s };

	std::regex expect_re{ 
		"C:"s + backslash + 
		"Users"s + backslash +
		"philippe"s + backslash + 
		".charge" + backslash + 
		"cache" + backslash + 
		"\\d*" };

	BOOST_CHECK( std::regex_match(path.string(), expect_re) );

#elif defined( CHARGE_LINUX )

	auto path{ charge::get_cache_path("hostname",
		"/home/philippe",
		"/home/philippe/dev/exp/script.cpp") };

	std::regex expect_re{ "/home/philippe/.charge/cache/\\d*" };

	BOOST_CHECK(std::regex_match(path.string(), expect_re));

#else

#  error "no test for this platform"

#endif

}


BOOST_AUTO_TEST_CASE(decode_deps)
{
	BOOST_CHECK_EQUAL(decode_dependencies(std::string()), FileList{});

	std::string input{
		"/home/philippe/libs/include/tools.hpp\n"
		"/usr/local/include/yaml-cpp\n"
	};

	FileList expect;
	expect.push_back("/home/philippe/libs/include/tools.hpp"s);
	expect.push_back("/usr/local/include/yaml-cpp"s);

	BOOST_CHECK_EQUAL(decode_dependencies(input), expect);
}


BOOST_AUTO_TEST_CASE(write_arg_string)
{
	BOOST_CHECK_EQUAL(write_arguments_string(StringList{}), "");

	BOOST_CHECK_EQUAL(
		write_arguments_string(
			StringList{
				"pgm"
			}
		),

		"pgm"
	);

	BOOST_CHECK_EQUAL(
		write_arguments_string(
			StringList{
				"pgm",
				"arg1",
				"arg2"
			}
		),

		"pgm arg1 arg2"
	);

	BOOST_CHECK_EQUAL(
		write_arguments_string(
			StringList{
				"pgm",
				"arg1 space",
				"arg2"
			}
		),

		"pgm \"arg1 space\" arg2"
	);
}


BOOST_AUTO_TEST_SUITE_END();

