#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

#include "MessageIO.h"

#include <memory>
#include <boost/asio.hpp>

namespace network
{

class Session : public MessageIO,
    public std::enable_shared_from_this<Session>
{
public:
    Session();
    virtual ~Session();

    void start();

protected:
    virtual void onReadError();
    virtual void onWriteError();

private:
    void terminateSession();
};

}

#endif
