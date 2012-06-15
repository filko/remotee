#include "freecode.hh"

#include <paludis/util/log.hh>
#include <paludis/util/make_null_shared_ptr.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

std::string Freecode::name() const
{
    return "freecode";
}

std::shared_ptr<const IDInfo> Freecode::process(const std::shared_ptr<const paludis::PackageID> & id,
                                                const std::string & index)
{
    paludis::Context context("When processing " + stringify(*id) + " with freecode index '" + index + "'");

    auto content(Cache::get_instance()->get("freecode", index,
                                            "http://freecode.com/projects/" + index +
                                            "/releases.json?auth_code=ENh4x5lS309zY9Rc4Oj"));
    if (content)
        try
        {
            auto doc(load_json_or_throw(*content));

            json_t * first(json_array_get_or_throw(doc.get(), 0));
            json_t * release(json_object_get_or_throw(first, "release"));
            json_t * version(json_object_get_or_throw(release, "version"));

            try
            {
                return std::make_shared<IDInfo>(paludis::VersionSpec(json_get_string_or_throw(version), { }));
            }
            catch (const paludis::BadVersionSpecError & e)
            {
                using namespace paludis;
                Log::get_instance()->message("freecode.bad_version", ll_qa, lc_context) << e.message();
            }
        }
        catch (const JsonException & e)
        {
            using namespace paludis;
            Log::get_instance()->message("freecode.json", ll_qa, lc_context) << e.message();
        }

    return paludis::make_null_shared_ptr();
}
