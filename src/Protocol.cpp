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

void Protocol::addListener(Listener* l)
{
    mListeners.push_back(l);
}

void Protocol::removeListener(Listener* l)
{
    mListeners.erase(std::remove(std::begin(mListeners),
                std::end(mListeners), l), std::end(mListeners));
}

void Protocol::onRecv(const Message& message)
{
    mMessageLoop.write(message);
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
            auto m = mMessageLoop.read();
            message(m);
        }
    }
}

}
