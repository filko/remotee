#include "cpan.hh"

#include <paludis/util/log.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

std::string Cpan::name() const
{
    return "cpan";
}

std::shared_ptr<const IDInfo> Cpan::process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index)
{
    paludis::Context context("When processing " + stringify(*id) + " with cpan index '" + index + "'");

    auto content(Cache::get_instance()->get("cpan", index,
                                            "http://api.metacpan.org/v0/release/" + index));
    if (content)
        try
        {
            auto doc(load_json_or_throw(*content));

            json_t * version(json_object_get_or_throw(doc.get(), "version"));

            try
            {
                return std::make_shared<IDInfo>(paludis::VersionSpec(json_get_string_or_throw(version), { }));
            }
            catch (const paludis::BadVersionSpecError & e)
            {
                using namespace paludis;
                Log::get_instance()->message("cpan.bad_version", ll_qa, lc_context) << e.message();
            }
        }
        catch (const JsonException & e)
        {
            using namespace paludis;
            Log::get_instance()->message("cpan.json", ll_qa, lc_context) << e.message();
        }

    return nullptr;
}
