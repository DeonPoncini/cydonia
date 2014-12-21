#ifndef CYDONIA_PROTOCOL_H
#define CYDONIA_PROTOCOL_H

#include "MessageIO.h"

#include <zephyr/QueueLoop.h>

#include <thread>
#include <vector>

namespace cydonia
{

class Protocol : public MessageListener
{
public:
    Protocol(MessageIO& messageIO);
    virtual ~Protocol();

    virtual void onRecv(Message&& message) override;

protected:
    void sendMessage(int type, const std::string& message, int frameId);
    void sendMessage(int type, const char* message,
            unsigned size, int frameId);

private:
    MessageIO& mMessageIO;
    zephyr::QueueLoop<Message> mMessageLoop;
    std::thread mReplyThread;

    void replyThread();
    virtual void message(Message&& message) = 0;
};

}

#endif
