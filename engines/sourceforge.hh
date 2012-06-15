#ifndef REMOTEE_ENGINE_SOURCEFORGE_HH
#define REMOTEE_ENGINE_SOURCEFORGE_HH 1

#include "engine.hh"

class Sourceforge
    : public Engine
{
public:
    Sourceforge() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};


#endif
