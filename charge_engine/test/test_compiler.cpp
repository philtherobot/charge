
#include "../src/compiler.hpp"
#include "../src/platform.hpp"
#include "../src/tools.hpp"

#include "helpers.hpp"

#include <boost/test/unit_test.hpp>


namespace
{


class CompilerFamiliesFixture
{
public:

    struct Family
    {
        std::string name_;
        std::string trigger_cmd_;
        std::string capture_;
        std::string cmd_;
    };

    CompilerFamiliesFixture()
    {
        compiler_families_["msvc"] = Family{
            "msvc",
            "cl",

            "Microsoft (R) C/C++ Optimizing Compiler Version 19.15.26732.1 for x64\n"
            "Copyright(C) Microsoft Corporation.All rights reserved.\n"
            "\n"
            "usage: cl[option...] filename...[/ link linkoption...]\n"
            "\n",

            "cl"
        };

        compiler_families_["gcc"] = Family{
            "gcc",
            "g++ --version",
            "GNU C++ compiler 8.2.0\n",
            "g++"
        };
    }

    class MockProgramDetector : public charge::ProgramDetector
    {
    public:

        MockProgramDetector(Family const & family) : family_(family)
        {}

        virtual boost::optional<Result> look_for_program(std::string const & cmd)
        {
            if (cmd == family_.trigger_cmd_) return Result{ family_.capture_, 0 };

            return boost::optional<Result>();
        }

    private:
        Family const family_;
    };


    class MockNoProgramDetector : public charge::ProgramDetector
    {
    public:
        virtual boost::optional<Result> look_for_program(std::string const & cmd)
        {
            // No program is ever found.
            return boost::optional<Result>();
        }
    };


    using AssertionResult = boost::test_tools::assertion_result;

    AssertionResult check_configuration(Family const & family, charge::Config const & conf)
    {
        if (conf["command"].as<std::string>() != family.cmd_)
        {
            AssertionResult result(false);
            result.message() << "command is incorrect";
            return result;
        }

        if (conf["family"].as<std::string>() != family.name_)
        {
            AssertionResult result(false);
            result.message() << "family is incorrect";
            return result;
        }

        return true;
    }


    std::map<std::string, Family> compiler_families_;


}; // CompilerFamiliesFixture

} // anonymous


BOOST_AUTO_TEST_SUITE(compiler);


BOOST_FIXTURE_TEST_CASE(configure_no_compiler_installed, CompilerFamiliesFixture)
{
    MockNoProgramDetector detector;

    BOOST_CHECK(
        charge::test::catch_exception<charge::CompilerDetectionError>(
            [&]() {
                charge::configure(detector);
            }
        )
    );
}


BOOST_FIXTURE_TEST_CASE(configure_finds_first_compiler, CompilerFamiliesFixture)
{
    auto order = charge::platform::compiler_detection_order();

    BOOST_REQUIRE_GE(order.size(), 1);

    auto first_compiler = order[0];

    BOOST_REQUIRE_GE(compiler_families_.count(first_compiler), 1);

    Family family = compiler_families_[first_compiler];

    MockProgramDetector detector(family);

    auto conf(charge::configure(detector));

    BOOST_CHECK( check_configuration(family, conf) );
}


BOOST_FIXTURE_TEST_CASE(configure_finds_alternate_compiler, CompilerFamiliesFixture)
{
    auto order = charge::platform::compiler_detection_order();

    BOOST_REQUIRE_GE(order.size(), 2);

    auto compiler = order[1];

    BOOST_REQUIRE_GE(compiler_families_.count(compiler), 1);

    Family family = compiler_families_[compiler];

    MockProgramDetector detector(family);

    auto conf(charge::configure(detector));

    BOOST_CHECK(check_configuration(family, conf));
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
