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

    virtual void onRecv(const Message& message) override;

protected:
    void sendMessage(int type, const std::string& message, int frameId);
    void sendMessage(int type, const char* message,
            unsigned size, int frameId);

private:
    MessageIO& mMessageIO;
    std::vector<Listener*> mListeners;
    zephyr::QueueLoop<Message> mMessageLoop;
    std::thread mReplyThread;

    void replyThread();
    virtual void message(const Message& message) = 0;
};

}

#endif
