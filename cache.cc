#include "cache.hh"

#include <paludis/util/fs_path.hh>
#include <paludis/util/fs_stat.hh>
#include <paludis/util/options.hh>
#include <paludis/util/pimp-impl.hh>
#include <paludis/util/safe_ifstream.hh>
#include <paludis/util/safe_ofstream.hh>
#include <paludis/util/stringify.hh>
#include <paludis/util/timestamp.hh>
#include <stdexcept>
#include <string>

#include "curl.hh"

using namespace paludis;

namespace paludis
{

template <>
struct Imp<Cache>
{
    mutable Curl curl_;
    std::shared_ptr<const FSPath> path_;
    std::function<void()> observer_;
    int expire_;
};

}

Cache::Cache() = default;
Cache::~Cache() = default;

void Cache::init(const std::string & path, int expire)
{
    imp_->path_ = std::make_shared<FSPath>(path);
    imp_->expire_ = expire;

    if (! imp_->path_->stat().is_directory())
        throw std::runtime_error("Cache directory " + stringify(*imp_->path_) + " doesn't exist!");
}

std::shared_ptr<const std::string> Cache::get(const std::string & module, const std::string & entry,
                                              const std::string & url) const
{
    FSPath module_dir(*imp_->path_ / module);
    module_dir.mkdir(0755, { fspmkdo_ok_if_exists });
    FSPath entry_path(module_dir / entry);
    FSStat entry_stat(entry_path.stat());
    if (! entry_stat.exists() ||
        (entry_stat.mtim().seconds() + imp_->expire_ < Timestamp::now().seconds()))
    {
        imp_->curl_.url(url);
        imp_->curl_.perform();
        if (imp_->curl_.ok()
            // && 200 == imp_->curl_.http_code()
            )
        {
            SafeOFStream os(entry_path, -1, true);
            os << imp_->curl_.content();

            if (imp_->observer_)
                imp_->observer_();

            return std::make_shared<std::string>(imp_->curl_.content());
        }
        else
        {
            entry_path.unlink();
            return nullptr;
        }
    }
    else
    {
        SafeIFStream is(entry_path);
        std::ostringstream os;
        os << is.rdbuf();
        return std::make_shared<std::string>(os.str());
    }
}

void Cache::attach_uncached_observer(const std::function<void()> & f)
{
    imp_->observer_ = f;
}

void Cache::release_uncached_observer()
{
    imp_->observer_ = nullptr;
}
