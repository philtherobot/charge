#include "../src/cache.hpp"
#include "../src/header_dependencies.hpp"

#include "helpers.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/test/unit_test.hpp>

#include <regex>

using namespace charge;
using namespace std::string_literals;


BOOST_AUTO_TEST_SUITE(cache);


BOOST_AUTO_TEST_CASE(get_cache_pth)
{

    auto path = charge::get_cache_path("hostname",
        test::make_absolute_path("/Users/philippe"),
        test::make_absolute_path("/dev/exp/script.cpp"));

    auto hashed_host_and_script = path.filename();

    using boost::algorithm::all;
    using boost::algorithm::is_digit;

    BOOST_CHECK(all(hashed_host_and_script.string(), is_digit()));

    auto expected_root_path =
        test::make_absolute_path("/Users/philippe/.charge/cache");

    BOOST_CHECK_EQUAL(path.parent_path(), expected_root_path);

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
