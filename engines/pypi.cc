#include "pypi.hh"

#include <iostream>
#include <paludis/util/log.hh>
#include <paludis/util/make_null_shared_ptr.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "json.hh"

std::string Pypi::name() const
{
    return "pypi";
}

std::shared_ptr<const IDInfo> Pypi::process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index)
{
    paludis::Context context("When processing " + stringify(*id) + " with pypi index '" + index + "'");

    auto content(Cache::get_instance()->get("pypi", index,
                                            "http://pypi.python.org/pypi/" + index + "/json"));
    if (content)
        try
        {
            auto doc(load_json_or_throw(*content));

            json_t * info(json_object_get_or_throw(doc.get(), "info"));
            json_t * version(json_object_get_or_throw(info, "version"));

            try
            {
                return std::make_shared<IDInfo>(paludis::VersionSpec(json_get_string_or_throw(version), { }));
            }
            catch (const paludis::BadVersionSpecError & e)
            {
                using namespace paludis;
                Log::get_instance()->message("pypi.bad_version", ll_qa, lc_context) << e.message();
            }
        }
        catch (const JsonException & e)
        {
            using namespace paludis;
            Log::get_instance()->message("pypi.json", ll_qa, lc_context) << e.message();
        }

    return paludis::make_null_shared_ptr();
}
