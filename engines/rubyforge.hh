#ifndef REMOTEE_ENGINES_RUBYFORGE_HH
#define REMOTEE_ENGINES_RUBYFORGE_HH 1

#include "engine.hh"

class Rubyforge
    : public Engine
{
public:
    Rubyforge() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
