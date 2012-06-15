#ifndef REMOTEE_CACHE_HH
#define REMOTEE_CACHE_HH 1

#include <functional>
#include <memory>
#include <paludis/util/pimp.hh>
#include <paludis/util/singleton.hh>

class Cache
    : public paludis::Singleton<Cache>
{
    friend class paludis::Singleton<Cache>;
    paludis::Pimp<Cache> imp_;

    Cache();
    ~Cache();
public:
    void init(const std::string & path, int expire);

    Cache(const Cache &) = delete;
    Cache & operator=(const Cache &) = delete;

    std::shared_ptr<const std::string> get(const std::string & module, const std::string & entry,
                                           const std::string & url) const;

    void attach_uncached_observer(const std::function<void()> & f);
    void release_uncached_observer();
};

#endif
