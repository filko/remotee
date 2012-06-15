#include "rubyforge.hh"

#include <paludis/util/log.hh>
#include <paludis/util/make_null_shared_ptr.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

std::string Rubyforge::name() const
{
    return "rubyforge";
}

std::shared_ptr<const IDInfo> Rubyforge::process(const std::shared_ptr<const paludis::PackageID> & id,
                                                 const std::string & index)
{
    paludis::Context context("When processing " + stringify(*id) + " with rubyforge index '" + index + "'");

    auto content(Cache::get_instance()->get("rubyforge", index,
                                            "https://rubygems.org/api/v1/gems/" + index + ".json"));
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
                Log::get_instance()->message("rubyforge.bad_version", ll_qa, lc_context) << e.message();
            }
        }
        catch (const JsonException & e)
        {
            using namespace paludis;
            Log::get_instance()->message("rubyforge.json", ll_qa, lc_context) << e.message();
        }

    return paludis::make_null_shared_ptr();
}
