#ifndef REMOTEE_ENGINE_FACTORY_HH
#define REMOTEE_ENGINE_FACTORY_HH 1

#include <memory>
#include <paludis/util/pimp.hh>

#include "engine.hh"

class EngineFactory
{
    paludis::Pimp<EngineFactory> imp_;

public:
    EngineFactory();
    ~EngineFactory();

    EngineFactory(const EngineFactory &) = delete;
    EngineFactory & operator=(const EngineFactory &) = delete;

    std::shared_ptr<Engine> get(const std::string & name);
};

#endif
