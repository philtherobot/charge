
#include "../src/compiler.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(compiler);

BOOST_AUTO_TEST_CASE(configure_gcc)
{
    auto conf(charge::configure());

    BOOST_CHECK_EQUAL(conf["compiler"].as<std::string>(), "/usr/bin/g++");
    BOOST_CHECK_EQUAL(conf["version"].as<std::string>(), "7.2.0");
    BOOST_CHECK_EQUAL(conf["family"].as<std::string>(), "g++");
}

/*
No test because not implemented.
BOOST_AUTO_TEST_CASE(compile)
{
    YAML::Node conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";
    conf["family"] = "g++";

    charge::Compiler compiler(conf);

    charge::StringList nothing;

    auto deps =
        compiler.compile({ "source.cpp", nothing, nothing, nothing,
            boost::filesystem::path("/home/user/cache/source") });

    //TODO: from the mock, get...
    std::string cmd("/usr/bin/g++ source.cpp -o /home/user/cache/source");
    std::string const expect("/usr/bin/g++ source.cpp -o /home/user/cache/source");

    BOOST_CHECK_EQUAL(cmd, expect);

    BOOST_CHECK(deps.empty());
}
*/

BOOST_AUTO_TEST_CASE(msvc_compile_command)
{
	YAML::Node conf;

	conf["command"] = "C:\\Program Files\\cl.exe";
	conf["version"] = "14.0.0.0";
	conf["family"] = "msvc";

	charge::Compiler compiler(conf);

	charge::Compiler::Arguments args;
	args.source_ = "C:\\user\\script.cpp";
	args.executable_output_fn_ = "C:\\cache\\123\\executable.exe";
	
	BOOST_CHECK_EQUAL(
		compiler.msvc_command_line(args),
		"\"C:\\Program Files\\cl.exe\" /nologo /TP /MT /showIncludes /EHsc"
		" /Fe:C:\\cache\\123\\executable.exe" 
		" /Fo:C:\\cache\\123\\executable.obj" 
		" C:\\user\\script.cpp"
	);
}


BOOST_AUTO_TEST_SUITE_END();
