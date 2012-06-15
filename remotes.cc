#include "remotes.hh"

#include <iostream>
#include <paludis/util/pimp-impl.hh>
#include <vector>

#include "engine_factory.hh"
#include "status_printer.hh"

using namespace paludis;
using std::cerr;
using std::endl;

namespace paludis
{

template <>
struct Imp<Remotes>
{
    typedef std::pair<std::string, std::shared_ptr<const PackageID>> name_id;

    std::map<std::string, std::vector<name_id>> type_name_id;

    void dump()
    {
        for (auto t(type_name_id.cbegin()), t_end(type_name_id.cend());
             t != t_end; ++t)
        {
            cerr << t->first << " = " << t->second.size() << endl;
            for (auto n(t->second.cbegin()), n_end(t->second.cend());
                 n != n_end; ++n)
            {
                cerr << '{' << n->first << ',' << *n->second << "} ";
            }
            cerr << endl << endl;
        }
    }

    IDRemoteMap perform(bool only_better)
    {
        std::size_t size(0);
        for (auto t : type_name_id)
        {
            size += t.second.size();
        }
        StatusPrinter sp(size);
        IDRemoteMap ret;
        EngineFactory engines;
        for (auto t(type_name_id.cbegin()), t_end(type_name_id.cend());
             t != t_end; ++t)
        {
            sp.current_engine(t->first, t->second.size());

            auto engine(engines.get(t->first));
            if (! engine)
            {
                sp.done(t->second.size());
                continue;
            }

            for (auto n(t->second.cbegin()), n_end(t->second.cend());
                 n != n_end; ++n)
            {
                auto info(engine->process(n->second, n->first));
                sp.done();
                if (! info)
                    continue;

                if (! only_better || n->second->version() < info->best_version)
                    ret[n->second].insert(std::make_pair(engine->name(), info->best_version));
            }
        }
        return ret;
    }
};

}

Remotes::Remotes() = default;
Remotes::~Remotes() = default;

void Remotes::insert(const std::string & type, const std::string & name,
                     const std::shared_ptr<const PackageID> & id)
{
    imp_->type_name_id[type].push_back({name, id});
}

void Remotes::dump()
{
    imp_->dump();
}

IDRemoteMap Remotes::perform(bool only_better)
{
    return imp_->perform(only_better);
}
