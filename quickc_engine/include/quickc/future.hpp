#ifndef GUARD_6ca432fbe12a4ef6bd579f4b9ca49e7c
#define GUARD_6ca432fbe12a4ef6bd579f4b9ca49e7c


#if 1

#   include <boost/optional.hpp>
    namespace quickc
    {
        namespace fu_optional = boost;
    }

#else

#   include <optional>
    namespace quickc
    {
        namespace future_optional = std;
    }

#endif



#if 1

#   include <boost/filesystem.hpp>
    namespace quickc
    {
        namespace fu_filesystem = boost::filesystem;
    }

#else

#   include <filesystem>
    namespace quickc
    {
        namespace future_filesystem = std::experimental::filesystem::v1;
    }

#endif

#if 1

#   include <boost/variant.hpp>
    namespace quickc
    {
        namespace fu_variant = boost;
    }

#else

#   include <variant>
    namespace quickc
    {
        namespace future_variant = std;
    }

#endif

#endif
