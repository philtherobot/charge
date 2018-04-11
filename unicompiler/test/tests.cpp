
#define BOOST_TEST_MODULE unicompiler
#include <boost/test/included/unit_test.hpp>

#include "unicompiler/unicompiler.hpp"

BOOST_AUTO_TEST_CASE(configure_gcc)
{
    auto conf(unicompiler::configure());

    BOOST_CHECK_EQUAL(conf["compiler"].as<std::string>(), "/usr/bin/g++");
    BOOST_CHECK_EQUAL(conf["version"].as<std::string>(), "7.2.0");
    BOOST_CHECK_EQUAL(conf["family"].as<std::string>(), "g++");
}

BOOST_AUTO_TEST_CASE(compile)
{
    YAML::Node conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";
    conf["family"] = "g++";

    unicompiler::Compiler compiler(conf);

    unicompiler::StringList nothing;

    auto deps =
        compiler.compile({ "source.cpp", nothing, nothing, nothing,
            boost::filesystem::path("/home/user/cache/source") });

    //TODO: from the mock, get...
    std::string cmd("/usr/bin/g++ source.cpp -o /home/user/cache/source");
    std::string const expect("/usr/bin/g++ source.cpp -o /home/user/cache/source");

    BOOST_CHECK_EQUAL(cmd, expect);

    BOOST_CHECK(deps.empty());
}
