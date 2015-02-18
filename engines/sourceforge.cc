#include "sourceforge.hh"

#include <boost/regex.hpp>
#include <jansson.h>
#include <paludis/name.hh>
#include <paludis/package_id.hh>
#include <paludis/util/log.hh>
#include <paludis/util/options.hh>
#include <paludis/util/stringify.hh>
#include <paludis/version_spec.hh>
#include <set>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

using namespace paludis;

std::string Sourceforge::name() const
{
    return "sourceforge";
}

std::shared_ptr<const IDInfo> Sourceforge::process(const std::shared_ptr<const PackageID> & id,
                                                   const std::string & index)
{
    Context context("When processing " + stringify(*id) + " with sourceforge index '" + index + "'");
    auto content(Cache::get_instance()->get("sourceforge", index + "---base",
                                         "http://sourceforge.net/projects/" + index + "/rss"));

    if (content)
    {
        boost::regex re("<link>http://sourceforge.net/projects/\\S*/" +
                        id->name().package().value() +
                        "-(\\S*)\\.(tar\\.(gz|bz2|xz)|tgz)/download</link>");

        boost::match_results<std::string::const_iterator> what;
        auto begin(content->cbegin()), end(content->cend());
        VersionSpec best("0", { });
        while (boost::regex_search(begin, end,
                                   what, re))
        {
            try
            {
                best = std::max(best, VersionSpec(what[1], { }));
            }
            catch (const BadVersionSpecError & e)
            {
                Log::get_instance()->message("sourceforge.bad_version", ll_qa, lc_context) << e.message();
            }
            begin = what[0].second;
        }

        if (best != VersionSpec("0", { }))
            return std::make_shared<IDInfo>(best);
    }

    return nullptr;
}
