#ifndef NETWORK_SESSIONMANAGER_H
#define NETWORK_SESSIONMANAGER_H

#include <memory>
#include <boost/asio.hpp>
#include <vector>

namespace network
{

class Session;
typedef std::shared_ptr<Session> SessionPtr;

class SessionManager
{
public:
    static SessionManager& get();

    template <typename T, typename... Args>
    SessionPtr create(Args&&... args);
    void remove(SessionPtr session);

private:
    SessionManager() {}
    std::vector<SessionPtr> mSessions;
};

template <typename T, typename... Args>
SessionPtr SessionManager::create(Args&&... args)
{
    auto session = std::make_shared<T>(std::forward<Args>(args)...);
    mSessions.push_back(session);
    return session;
}

}

#endif
