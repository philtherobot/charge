
#include "../src/command_line.hpp"
#include "../src/Commandable.hpp"

#include "helpers.hpp"

#include <charge/exception.hpp>
#include <charge/types.hpp>

#include <boost/test/unit_test.hpp>

using charge::StringList;

namespace
{

class MockCommander : public charge::Commandable
{
public:
    virtual int compile()
    {
        calls_.push_back("compile");
        return 0;
    }

    virtual int update()
    {
        calls_.push_back("update");
        return 0;
    }

    virtual int execute(StringList const & args)
    {
        calls_.push_back("execute");
        execute_args_ = args;
        return 0;
    }


    virtual int copy_executable()
    {
        calls_.push_back("copy_executable");
        return 0;
    }

    virtual int purge()
    {
        calls_.push_back("purge");
        return 0;
    }

    charge::StringList calls_;
    charge::StringList execute_args_;
};


class Fixture
{
public:

    int run_command_line(StringList const & args)
    {
        return charge::run_command_line(args, cmder_, console_);
    }

    MockCommander cmder_;
    std::ostringstream console_;
};

} // anonymous


BOOST_FIXTURE_TEST_SUITE(command_line, Fixture);


BOOST_AUTO_TEST_CASE(no_script)
{
    BOOST_CHECK(
        charge::test::catch_exception<charge::CommandLineArgumentError>(
            [&]() {
                run_command_line(StringList{});
            }
            )
    );
}


BOOST_FIXTURE_TEST_CASE(no_arguments, Fixture)
{
    run_command_line(StringList{ "script.cpp" });

    StringList expected{ "update", "execute" };

    BOOST_CHECK_EQUAL(cmder_.calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(force, Fixture)
{
    run_command_line(StringList{ "-f", "script.cpp" });

    StringList expected{ "compile" };

    BOOST_CHECK_EQUAL(cmder_.calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(force_execute, Fixture)
{
    run_command_line(StringList{ "--force", "-e", "script.cpp" });

    StringList expected{ "compile", "execute" };

    BOOST_CHECK_EQUAL(cmder_.calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(execute, Fixture)
{
    BOOST_CHECK(
        charge::test::catch_exception<charge::CommandLineArgumentError>(
            [&]() { run_command_line(StringList{ "--execute", "script.cpp" }); }
        )
    );
}


BOOST_FIXTURE_TEST_CASE(script_arguments, Fixture)
{
    run_command_line(StringList{ "script.cpp", "a", "b" });

    StringList expected{ "update", "execute" };
    StringList expected_args{ "a", "b" };

    BOOST_CHECK_EQUAL(cmder_.calls_, expected);
    BOOST_CHECK_EQUAL(cmder_.execute_args_, expected_args);
}


BOOST_FIXTURE_TEST_CASE(help, Fixture)
{
    run_command_line(StringList{ "--help" });

    BOOST_CHECK(!console_.str().empty());
}


BOOST_AUTO_TEST_SUITE_END();
