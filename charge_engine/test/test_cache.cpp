#include "../src/cache.hpp"
#include "../src/header_dependencies.hpp"
#include "../src/platform_config.hpp"

#include <boost/test/unit_test.hpp>

#include <regex>

using namespace charge;
using namespace std::string_literals;


BOOST_AUTO_TEST_SUITE(cache);


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

    BOOST_CHECK(std::regex_match(path.string(), expect_re));

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


BOOST_AUTO_TEST_CASE(encode_deps)
{
    BOOST_CHECK_EQUAL(encode_header_dependencies(FileList{}), std::string());

    FileList deps;
    deps.push_back("/home/philippe/libs/include/tools.hpp"s);
    deps.push_back("/usr/local/include/yaml-cpp"s);

    std::string expect{
        "/home/philippe/libs/include/tools.hpp\n"
        "/usr/local/include/yaml-cpp\n"
    };

    BOOST_CHECK_EQUAL(encode_header_dependencies(deps), expect);
}


BOOST_AUTO_TEST_CASE(decode_deps)
{
    BOOST_CHECK_EQUAL(decode_header_dependencies(std::string()), FileList{});

    std::string input{
        "/home/philippe/libs/include/tools.hpp\n"
        "/usr/local/include/yaml-cpp\n"
    };

    FileList expect;
    expect.push_back("/home/philippe/libs/include/tools.hpp"s);
    expect.push_back("/usr/local/include/yaml-cpp"s);

    BOOST_CHECK_EQUAL(decode_header_dependencies(input), expect);
}


BOOST_AUTO_TEST_SUITE_END();
