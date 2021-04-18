/*#include <fmt/core.h>

int main() {
    fmt::format("just one {}", "string");
    //fmt::format("just one {}", 24);
}

*/


#include <fmt/core.h>

int main() {
  std::string s = fmt::format("The answer is {}.", 42);
  fmt::print("{}", s);
}
