#include "SessionManager.h"

#include "Session.h"

namespace network
{

SessionManager& SessionManager::get()
{
    static SessionManager instance;
    return instance;
}

void SessionManager::remove(SessionPtr session)
{
    mSessions.erase(std::remove(mSessions.begin(),mSessions.end(),session),
            mSessions.end());
}

}
