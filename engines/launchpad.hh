#ifndef REMOTEE_ENGINES_LAUNCHPAD_HH
#define REMOTEE_ENGINES_LAUNCHPAD_HH 1

#include "engine.hh"

class Launchpad
    : public Engine
{
public:
    Launchpad() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
