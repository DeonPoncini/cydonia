#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "Session.h"
#include "SessionManager.h"
#include "IOServiceManager.h"

#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

namespace network
{

typedef std::shared_ptr<Session> SessionPtr;
typedef boost::asio::ip::tcp::acceptor Acceptor;
typedef boost::system::error_code ErrorCode;
typedef boost::asio::ip::tcp::endpoint Endpoint;
typedef boost::asio::ip::tcp TCP;

template <typename T, typename... Args>
class Server
{
public:
    Server(int port, Args&&... args);
    ~Server();

private:
    void server(int port);
    void accept(SessionPtr session, ErrorCode error);

    Acceptor mAcceptor;
    std::function<SessionPtr(void)> mMakeSession;
};


template <typename T, typename... Args>
Server<T,Args...>::Server(int port, Args&&... args) :
    mAcceptor(IOServiceManager::get().ioService(),
            Endpoint(TCP::v4(),port))
{
    BOOST_LOG_TRIVIAL(trace) << "Listening on port: " << port;

    mMakeSession = [&] {
        return SessionManager::get().create<T,Args...>(
                std::forward<Args>(args)...); };

    auto newSession = mMakeSession();
    mAcceptor.async_accept(newSession->socket(),
        boost::bind(&Server::accept,this, newSession,
            boost::asio::placeholders::error));
}

template <typename T, typename... Args>
Server<T,Args...>::~Server()
{
    BOOST_LOG_TRIVIAL(trace) << "Server shutting down";
}

template <typename T, typename... Args>
void Server<T,Args...>::accept(SessionPtr session, ErrorCode error)
{
    BOOST_LOG_TRIVIAL(trace) << "Accepted connection";
    if (!error)
    {
        session->start();
        auto newSession = mMakeSession();
        mAcceptor.async_accept(newSession->socket(),
                boost::bind(&Server::accept, this, newSession,
                    boost::asio::placeholders::error));
    }
    else
    {
        BOOST_LOG_TRIVIAL(trace) << "Connection error " << error;
        SessionManager::get().remove(session);
    }
}

}

#endif
