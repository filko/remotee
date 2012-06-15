#include "launchpad.hh"

#include <paludis/util/log.hh>
#include <paludis/util/make_null_shared_ptr.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

std::string Launchpad::name() const
{
    return "launchpad";
}

std::shared_ptr<const IDInfo> Launchpad::process(const std::shared_ptr<const paludis::PackageID> & id,
                                                 const std::string & index)
{
    using namespace paludis;
    Context context("When processing " + stringify(*id) + " with launchpad index '" + index + "'");

    auto content(Cache::get_instance()->get("launchpad", index,
                                            "https://api.launchpad.net/1.0/" + index + "/releases/"));
    if (content)
        try
        {
            auto doc(load_json_or_throw(*content));

            json_t * entries(json_object_get_or_throw(doc.get(), "entries"));
            std::size_t size(json_array_size_or_throw(entries));
            VersionSpec best("0", { });
            for (std::size_t i(0); i != size; ++i)
            {
                json_t * release(json_array_get_or_throw(entries, i));
                json_t * version(json_object_get_or_throw(release, "version"));
                try
                {
                    best = std::max(best, VersionSpec(json_get_string_or_throw(version), { }));
                }
                catch (const BadVersionSpecError & e)
                {
                    Log::get_instance()->message("launchpad.bad_version", ll_qa, lc_context) << e.message();
                }
            }

            if (VersionSpec("0", { }) < best)
                return std::make_shared<IDInfo>(best);
        }
        catch (const JsonException & e)
        {
            Log::get_instance()->message("launchpad.json", ll_qa, lc_context) << e.message();
        }

    Log::get_instance()->message("launchpad.no_version", ll_qa, lc_context) << "No version found.";
    return make_null_shared_ptr();
}
