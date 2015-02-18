#include "engine_factory.hh"

#include <iostream>
#include <map>
#include <paludis/util/log.hh>
#include <paludis/util/pimp-impl.hh>
#include <string>

#include "engines/cpan.hh"
#include "engines/freecode.hh"
#include "engines/launchpad.hh"
#include "engines/pypi.hh"
#include "engines/rubyforge.hh"
#include "engines/sourceforge.hh"
#include "engines/vim.hh"

using namespace paludis;

namespace paludis
{

template <>
struct Imp<EngineFactory>
{
    std::map<std::string, std::shared_ptr<Engine>> engines_;

    Imp()
    {
        engines_["cpan"] = std::make_shared<Cpan>();
        engines_["freshmeat"] = engines_["freecode"] = std::make_shared<Freecode>();
        engines_["launchpad"] = std::make_shared<Launchpad>();
        engines_["pypi"] = std::make_shared<Pypi>();
        engines_["rubyforge"] = std::make_shared<Rubyforge>();
        engines_["sourceforge"] = std::make_shared<Sourceforge>();
        engines_["vim"] = std::make_shared<Vim>();
    }
};

}

EngineFactory::EngineFactory() = default;
EngineFactory::~EngineFactory() = default;

std::shared_ptr<Engine> EngineFactory::get(const std::string & name)
{
    auto i(imp_->engines_.find(name));
    if (imp_->engines_.end() != i)
        return i->second;

    Log::get_instance()->message("engine_factory.not_supported", ll_warning, lc_no_context)
        << "Engine " << name << " not supported.";
    return imp_->engines_[name] = nullptr;
}
