#ifndef REMOTEE_ENGINES_PYPI_HH
#define REMOTEE_ENGINES_PYPI_HH 1

#include "engine.hh"

class Pypi
    : public Engine
{
public:
    Pypi() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
