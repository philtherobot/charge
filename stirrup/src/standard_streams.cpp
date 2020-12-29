#include "stirrup/standard_streams.hpp"

#include "stirrup/file.hpp"

namespace stirrup
{

file stdin_file(stdin);
input_stream sinp = stdin_file.input_stream();

file stdout_file(stdout);
output_stream sout = stdout_file.output_stream();

}
