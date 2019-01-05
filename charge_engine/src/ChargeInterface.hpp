#ifndef GUARD_46eb9e2018fb40cca751a842508deeb1
#define GUARD_46eb9e2018fb40cca751a842508deeb1

#include <charge/types.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <memory>


namespace charge
{

class ScriptInterface;


class ChargeInterface : boost::noncopyable
{
public:
    virtual ~ChargeInterface() {}

    virtual std::shared_ptr<ScriptInterface> script(boost::filesystem::path const & script) = 0;
};


class ScriptInterface : boost::noncopyable
{
public:
    virtual ~ScriptInterface() {}

    virtual int compile() = 0;
    virtual int update() = 0;
    virtual int execute(StringList const & args) = 0;
};


} // charge

#endif
