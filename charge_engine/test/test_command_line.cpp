
#include "../src/ChargeInterface.hpp"
#include "../src/command_line.hpp"

#include "helpers.hpp"

#include <charge/exception.hpp>
#include <charge/types.hpp>

#include <catch2/catch.hpp>

#include <algorithm>

using charge::StringList;

namespace
{

class MockScript : public charge::ScriptInterface
{
public:

    explicit MockScript(boost::filesystem::path const & path)
        : path_(path)
    {}

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

    boost::filesystem::path const path_;
    charge::StringList calls_;
    charge::StringList execute_args_;
};


class MockCharge : public charge::ChargeInterface
{
public:
    virtual ScriptInterfacePointer script(boost::filesystem::path const & path)
    {
        script_.reset(new MockScript(path));
        return script_;
    }

    std::shared_ptr<MockScript> script_;
};


class Fixture
{
public:

    int run_command_line(StringList const & args)
    {
        StringList args_with_arg0;
        args_with_arg0.push_back("charge_executable");
        std::copy(args.begin(), args.end(), std::back_inserter(args_with_arg0));

        return charge::run_command_line(args_with_arg0, charge_impl_, console_);
    }

    MockCharge charge_impl_;
    std::ostringstream console_;
};

} // anonymous


SCENARIO("[Charge] command_line") {
    
}
BOOST_FIXTURE_TEST_SUITE(command_line, Fixture);


BOOST_AUTO_TEST_CASE(case_no_script)
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

    BOOST_REQUIRE(charge_impl_.script_);

    StringList expected{ "update", "execute" };

    BOOST_CHECK_EQUAL(charge_impl_.script_->calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(force, Fixture)
{
    run_command_line(StringList{ "-f", "script.cpp" });

    BOOST_REQUIRE(charge_impl_.script_);

    StringList expected{ "compile", "execute" };

    BOOST_CHECK_EQUAL(charge_impl_.script_->calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(force_noexecute, Fixture)
{
    run_command_line(StringList{ "-fc", "script.cpp" });

    BOOST_REQUIRE(charge_impl_.script_);

    StringList expected{ "compile" };

    BOOST_CHECK_EQUAL(charge_impl_.script_->calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(compile_only, Fixture)
{
    run_command_line(StringList{ "--compile-only", "script.cpp" });

    BOOST_REQUIRE(charge_impl_.script_);

    StringList expected{ "update" };

    BOOST_CHECK_EQUAL(charge_impl_.script_->calls_, expected);
}


BOOST_FIXTURE_TEST_CASE(script_arguments, Fixture)
{
    run_command_line(StringList{ "script.cpp", "a", "b" });

    BOOST_REQUIRE(charge_impl_.script_);

    StringList expected{ "update", "execute" };
    StringList expected_args{ "a", "b" };

    BOOST_CHECK_EQUAL(charge_impl_.script_->calls_, expected);
    BOOST_CHECK_EQUAL(charge_impl_.script_->execute_args_, expected_args);
}


BOOST_FIXTURE_TEST_CASE(help, Fixture)
{
    run_command_line(StringList{ "--help" });

    BOOST_CHECK(!charge_impl_.script_);
    BOOST_CHECK(!console_.str().empty());
}

