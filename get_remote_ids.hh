#ifndef REMOTEE_GET_REMOTE_IDS_HH
#define REMOTEE_GET_REMOTE_IDS_HH 1

#include <list>
#include <paludis/environment-fwd.hh>
#include <paludis/metadata_key.hh>
#include <string>

struct RemoteID
{
    std::string type, name;
};

typedef std::list<RemoteID> RemoteIDs;

RemoteIDs get_remote_ids(const paludis::MetadataKey & mk, paludis::Environment * env);

#endif
