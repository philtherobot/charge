#include "stirrup/standard_streams.hpp"

#include "stirrup/file.hpp"

namespace stirrup
{

file stdin_file(stdin);
input_stream binary_stdin = stdin_file.input_stream();

file stdout_file(stdout);
output_stream binary_stdout = stdout_file.output_stream();

}
