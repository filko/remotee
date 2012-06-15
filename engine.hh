#ifndef REMOTEE_ENGINE_HH
#define REMOTEE_ENGINE_HH 1

#include <memory>
#include <paludis/package_id-fwd.hh>
#include <paludis/version_spec.hh>
#include <string>

struct IDInfo
{
    paludis::VersionSpec best_version;

    IDInfo(const paludis::VersionSpec & bv)
        : best_version(bv)
    {
    }
};

class Engine
{
protected:
    Engine() = default;

public:
    Engine(const Engine &) = delete;
    Engine & operator=(const Engine &) = delete;
    virtual ~Engine() { }

    virtual std::string name() const = 0;
    virtual std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id,
                                                  const std::string & index) = 0;
};

#endif
