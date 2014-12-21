#ifndef CYDONIA_SESSION_H
#define CYDONIA_SESSION_H

#include "MessageIO.h"

#include <memory>
#include <boost/asio.hpp>

namespace cydonia
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
