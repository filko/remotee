#include "vim.hh"

#include <paludis/util/log.hh>
#include <paludis/util/make_null_shared_ptr.hh>
#include <paludis/util/options.hh>
#include <sstream>
#include <stdexcept>

#include "cache.hh"
#include "xml.hh"

std::string Vim::name() const
{
    return "vim";
}

std::shared_ptr<const IDInfo> Vim::process(const std::shared_ptr<const paludis::PackageID> & id, const std::string & index)
{
    paludis::Context context("When processing " + stringify(*id) + " with vim index '" + index + "'");

    auto content(Cache::get_instance()->get("vim", index,
                                            "http://www.vim.org/scripts/script.php?script_id=" + index));
    if (content)
    {
        auto doc(load_html_or_throw(*content));
        auto ctx(xml_get_context_or_throw(doc));
        auto obj(xml_eval_expression_or_throw(ctx, "//td[3 and @class = 'rowodd' or @class = 'roweven']/b"));

        paludis::VersionSpec best("0", { });
        xmlNodeSetPtr nodes(obj->nodesetval);
        for (int i(0), i_end((nodes) ? nodes->nodeNr : 0);
             i < i_end; ++i)
        {
            auto type(nodes->nodeTab[i]->type);
            if (type == XML_ELEMENT_NODE || type == XML_ATTRIBUTE_NODE)
            {
                xmlNodePtr cur(nodes->nodeTab[i]);
                UniqueXmlString key(xmlNodeGetContent(cur));
                try
                {
                    best = std::max(best, paludis::VersionSpec((const char*)key.get(), { }));
                }
                catch (const paludis::BadVersionSpecError & e)
                {
                    using namespace paludis;
                    Log::get_instance()->message("vim.bad_version", ll_qa, lc_context) << e.message();
                }
            }
        }

        if (paludis::VersionSpec("0", { }) < best)
            return std::make_shared<IDInfo>(best);
    }

    return paludis::make_null_shared_ptr();
}
