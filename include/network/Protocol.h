#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "MessageIO.h"

#include <zephyr/QueueLoop.h>

#include <thread>
#include <vector>

namespace network
{

class Connection;

class Protocol : public MessageListener
{
public:
    Protocol(MessageIO& messageIO);
    virtual ~Protocol();

    virtual void onRecv(const Message& message) override;

protected:
    void sendMessage(int type, const std::string& message, int frameId);
    void sendMessage(int type, const char* message,
            unsigned size, int frameId);

private:
    MessageIO& mMessageIO;
    zephyr::QueueLoop<Message> mMessageLoop;
    std::thread mReplyThread;

    void replyThread();
    virtual void message(const Message& message) = 0;
};

}

#endif
