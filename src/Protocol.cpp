#include <network/Protocol.h>

#include <sstream>

namespace network
{

Protocol::Protocol(MessageIO& messageIO) :
    mMessageIO(messageIO),
    mReplyThread(&Protocol::replyThread, this)
{
    mMessageIO.addListener(this);
}

Protocol::~Protocol()
{
    mMessageIO.removeListener(this);
    mMessageLoop.terminate();
    if (mReplyThread.joinable()) {
        mReplyThread.join();
    }
}

void Protocol::onRecv(Message&& message)
{
    mMessageLoop.write(std::move(message));
}

void Protocol::sendMessage(int type, const std::string& message, int frameId)
{
    mMessageIO.sendMessage(type,message,frameId);
}

void Protocol::sendMessage(int type, const char* message, unsigned size,
        int frameId)
{
    mMessageIO.sendMessage(type,message,size,frameId);
}

void Protocol::replyThread()
{
    while(!mMessageLoop.shouldExit()) {
        while (mMessageLoop.more()) {
            message(mMessageLoop.read());
        }
    }
}

}
