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
    class Listener
    {
    public:
        virtual ~Listener() {}
    };
    Protocol(MessageIO& messageIO);
    virtual ~Protocol();

    void addListener(Listener* l);
    void removeListener(Listener* l);

    void sendPing();
    void sendPong();

    virtual void onRecv(const Message& message) override;

private:
    MessageIO& mMessageIO;
    std::vector<Listener*> mListeners;
    zephyr::QueueLoop<network::Message> mMessageLoop;
    std::thread mReplyThread;

    void replyThread();
};

}

#endif
