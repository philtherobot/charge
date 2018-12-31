
#include "../src/compiler.hpp"
#include "../src/tools.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(compiler);


class MockProgramDetector : public charge::ProgramDetector
{
public:
    virtual boost::optional<Result> look_for_program(std::string const & cmd)
    {
        return boost::optional<Result>();
    }
};


BOOST_AUTO_TEST_CASE(configure_gcc)
{
    MockProgramDetector detector;

    auto conf(charge::configure(detector));

    BOOST_CHECK_EQUAL(conf["compiler"].as<std::string>(), "/usr/bin/g++");
    BOOST_CHECK_EQUAL(conf["version"].as<std::string>(), "7.2.0");
    BOOST_CHECK_EQUAL(conf["family"].as<std::string>(), "g++");
}


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
        "\"C:\\Program Files\\cl.exe\" /nologo /TP /MD /showIncludes /EHsc"
        " /Fe:C:\\cache\\123\\executable.exe"
        " /Fo:C:\\cache\\123\\executable.obj"
        " C:\\user\\script.cpp"
    );
}


BOOST_AUTO_TEST_SUITE_END();
