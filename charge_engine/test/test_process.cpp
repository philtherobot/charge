
#include "../src/process.hpp"

#include <boost/test/unit_test.hpp>

#include <string>

using namespace charge;
using namespace std::string_literals;


BOOST_AUTO_TEST_SUITE(engine);


BOOST_AUTO_TEST_CASE(write_cmd_string)
{
	BOOST_CHECK_EQUAL(write_command_string(std::string(), StringList{}), "");

	BOOST_CHECK_EQUAL(
		write_command_string(
			std::string("pgm"),
			StringList{}
		),

		"pgm"
	);

	BOOST_CHECK_EQUAL(
		write_command_string(
			std::string("pgm"),
			StringList{
				"arg1",
				"arg2"
			}
		),

		"pgm arg1 arg2"
	);

	BOOST_CHECK_EQUAL(
		write_command_string(
			std::string("pgm"),
			StringList{
				"arg1 space",
				"arg2"
			}
		),

		"pgm \"arg1 space\" arg2"
	);
}


BOOST_AUTO_TEST_SUITE_END();

