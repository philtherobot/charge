#include <locale>
#include <iterator>

using namespace std;

class numpunct_char : public std::numpunct<char> {};
class numpunct_wchar_t : public std::numpunct<wchar_t> {};
class numpunct_char32_t : public std::numpunct<char32_t> {};

int main(int argc, char * argv[])
{

    return 0;
}
