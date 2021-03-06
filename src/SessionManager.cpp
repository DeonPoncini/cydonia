#include <cydonia/SessionManager.h>

#include <cydonia/Session.h>

namespace cydonia
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
