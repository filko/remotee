#ifndef REMOTEE_REMOTES_HH
#define REMOTEE_REMOTES_HH

#include <map>
#include <paludis/util/pimp.hh>
#include <paludis/package_id.hh>
#include <set>
#include <string>
#include <vector>

template <typename T, typename U>
struct OrderBySecondReverse
{
    bool operator()(const std::pair<T, U> & lh, const std::pair<T, U> & rh)
    {
        return lh.second > rh.second;
    }
};

typedef std::map<std::shared_ptr<const paludis::PackageID>,
    std::multiset<std::pair<std::string, paludis::VersionSpec>, OrderBySecondReverse<std::string, paludis::VersionSpec>>,
    paludis::PackageIDSetComparator> IDRemoteMap;

class Remotes
{
    paludis::Pimp<Remotes> imp_;
public:
    explicit Remotes();
    ~Remotes();

    Remotes(const Remotes &) = delete;
    Remotes & operator=(const Remotes &) = delete;

    void insert(const std::string & type, const std::string & name, const std::shared_ptr<const paludis::PackageID> & id);
    void dump();
    IDRemoteMap perform(bool only_better);
};

#endif
