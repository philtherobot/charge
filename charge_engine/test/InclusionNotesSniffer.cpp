
#include "../src/InclusionNotesSniffer.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(InclusionNotesSniffer);

namespace
{

class MockStream : public ReadableStream
{
public:
	explicit MockStream(StringList const & sl)
		: input_(sl)
	{}

	virtual boost::optional<std::string> read()
	{
		if (input_.empty()) return boost::optional<std::string>();

		return input_.pop_front();
	}
};


}

BOOST_AUTO_TEST_CASE(empty_input)
{
}

BOOST_AUTO_TEST_SUITE_END();
