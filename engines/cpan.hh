#ifndef REMOTEE_ENGINES_CPAN_HH
#define REMOTEE_ENGINES_CPAN_HH 1

#include "engine.hh"

class Cpan
    : public Engine
{
public:
    Cpan() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
