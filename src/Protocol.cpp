#include <network/Protocol.h>

#include <sstream>

namespace MessageTypes
{
    enum _MessageTypes
    {
        PING = 0,
        PONG
    };
};

namespace network
{

Protocol::Protocol(network::MessageIO& messageIO) :
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

void Protocol::sendPing()
{
    mMessageIO.sendMessage(MessageTypes::PING,"Ping",-1);
}

void Protocol::sendPong()
{
    mMessageIO.sendMessage(MessageTypes::PONG,"Pong",-1);
}

void Protocol::onRecv(const Message& message)
{
    mMessageLoop.write(message);
}

void Protocol::replyThread()
{
    while(!mMessageLoop.shouldExit()) {
        while (mMessageLoop.more()) {
            auto message = mMessageLoop.read();
            switch (message.type()) {
                case MessageTypes::PING:
                    std::cout << "Ping received" << std::endl;
                    sendPong();
                    break;
                case MessageTypes::PONG:
                    std::cout << "Pong received" << std::endl;
                    break;
            }
        }
    }
}

}
