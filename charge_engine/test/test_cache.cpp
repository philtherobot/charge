#include "../src/cache.hpp"
#include "../src/header_dependencies.hpp"

#include "helpers.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <catch2/catch.hpp>

#include <regex>

using namespace charge;
using namespace std::string_literals;


SCENARIO("[Charge]  cache") {

WHEN("get_cache_pth")
{

    auto path = charge::get_cache_path("hostname",
        test::make_absolute_path("/Users/philippe"),
        test::make_absolute_path("/dev/exp/script.cpp"));

    auto hashed_host_and_script = path.filename();

    using boost::algorithm::all;
    using boost::algorithm::is_digit;

    CHECK(all(hashed_host_and_script.string(), is_digit()));

    auto expected_root_path =
        test::make_absolute_path("/Users/philippe/.charge/cache");

    CHECK(path.parent_path() == expected_root_path);

}


WHEN("case_encode_deps")
{
    CHECK(encode_header_dependencies(FileList{}).empty());

    FileList deps;
    deps.push_back("/home/philippe/libs/include/tools.hpp"s);
    deps.push_back("/usr/local/include/yaml-cpp"s);

    std::string expect{
        "/home/philippe/libs/include/tools.hpp\n"
        "/usr/local/include/yaml-cpp\n"
    };

    CHECK(encode_header_dependencies(deps), expect);
}


WHEN("case_decode_deps")
{
    CHECK(decode_header_dependencies(std::string()).empty());

    std::string input{
        "/home/philippe/libs/include/tools.hpp\n"
        "/usr/local/include/yaml-cpp\n"
    };

    FileList expect;
    expect.push_back("/home/philippe/libs/include/tools.hpp"s);
    expect.push_back("/usr/local/include/yaml-cpp"s);

    CHECK(decode_header_dependencies(input) == expect);
}


}
