#ifndef REMOTEE_ENGINES_FREECODE_HH
#define REMOTEE_ENGINES_FREECODE_HH 1

#include "engine.hh"

class Freecode
    : public Engine
{
public:
    Freecode() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
