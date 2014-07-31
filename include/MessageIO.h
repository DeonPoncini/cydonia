#ifndef NETWORK_MESSAGEIO_H
#define NETWORK_MESSAGEIO_H

#include "Message.h"

#include <boost/asio.hpp>
#include <boost/chrono.hpp>

#include <deque>
#include <string>
#include <vector>
#include <ctime>

typedef boost::asio::io_service IOService;
typedef boost::asio::ip::tcp::socket Socket;
typedef boost::system::error_code ErrorCode;


namespace network
{

typedef std::shared_ptr<Message> MessagePtr;

class MessageListener
{
public:
    virtual ~MessageListener() {}
    virtual void onRecv(const Message& message) = 0;
};

class MessageIO
{
public:
    MessageIO();
    virtual ~MessageIO() {}

    void sendMessage(int type, const std::string& message, int frameId);
    void sendMessage(int type, const char* message,
            unsigned size, int frameId);

    inline Socket& socket() { return mSocket; }

    void addListener(MessageListener* listener);
    void removeListener(MessageListener* listener);

protected:
    void startRead();

    void write(ErrorCode error);

    virtual void onReadError() {}
    virtual void onWriteError() {}

private:
    void readHeader(ErrorCode error, MessagePtr message);
    void readBody(ErrorCode error, MessagePtr message);
    void onRecv(MessagePtr message);

    Socket mSocket;
    std::deque<Message> mWriteQueue;
    std::vector<MessageListener*> mListeners;
};

}

#endif
