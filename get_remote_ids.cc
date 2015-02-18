#include "get_remote_ids.hh"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <paludis/environment.hh>
#include <paludis/util/visitor.hh>
#include <paludis/util/indirect_iterator-impl.hh>
#include <vector>

using namespace paludis;

namespace
{

struct RemoteVisitor
{
    RemoteIDs & remotes;
    Environment * env_;

    RemoteVisitor(RemoteIDs & r, Environment * env)
        : remotes(r),
          env_(env)
    {
    }

    void visit(const PlainTextSpecTree::NodeType<PlainTextDepSpec>::Type & node)
    {
        std::vector<std::string> out;
        auto in(node.spec()->text());
        boost::split(out, in, boost::is_any_of(":"));
        if (2 == out.size())
            remotes.push_back({out[0], out[1]});
    }

    void visit(const PlainTextSpecTree::NodeType<PlainTextLabelDepSpec>::Type & )
    {
    }

    void visit(const PlainTextSpecTree::NodeType<AllDepSpec>::Type & node)
    {
        std::for_each(indirect_iterator(node.begin()), indirect_iterator(node.end()), accept_visitor(*this));
    }

    void visit(const PlainTextSpecTree::NodeType<ConditionalDepSpec>::Type & node)
    {
        if (node.spec()->condition_met(env_, 0))
            std::for_each(indirect_iterator(node.begin()), indirect_iterator(node.end()), accept_visitor(*this));
    }
};

struct MetadataVisitor
{
    RemoteIDs remotes;
    Environment * env_;

    MetadataVisitor(Environment * env)
    : env_(env)
    {
    }

    void visit(const MetadataSpecTreeKey<PlainTextSpecTree> & key)
    {
        RemoteVisitor rv(remotes, env_);
        key.parse_value()->top()->accept(rv);
    }

    template <typename T>
    void visit(const T & )
    {
    }
};

}

RemoteIDs get_remote_ids(const MetadataKey & mk, Environment * env)
{
    MetadataVisitor visitor(env);
    mk.accept(visitor);
    return visitor.remotes;
}
