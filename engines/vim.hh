#ifndef REMOTEE_ENGINES_VIM_HH
#define REMOTEE_ENGINES_VIM_HH 1

#include "engine.hh"

class Vim
    : public Engine
{
public:
    Vim() = default;

    virtual std::string name() const;
    std::shared_ptr<const IDInfo> process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index);
};

#endif
