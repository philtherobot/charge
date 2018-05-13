
#include "tools.hpp"

#include "exception.hpp"

#include <stdlib.h>

namespace charge
{

std::string getenv(std::string const & var)
{
	char * buf = 0;
	int err = _dupenv_s(&buf, NULL, var.c_str());
	if (err != 0) throw ErrnoError("_dupenv_s", err);

	if (!buf) throw Exception("failed to find environment variable " + var);

	std::string r{ buf };
	free(buf);

	return r;
}

} // charge