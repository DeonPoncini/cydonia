#include <network/MessageIO.h>
#include <network/IOServiceManager.h>
#include <zephyr/Timestamp.h>

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

namespace network
{

MessageIO::MessageIO() :
    mSocket(IOServiceManager::get().ioService())
{
}

void MessageIO::addListener(MessageListener* listener)
{
    mListeners.push_back(listener);
}

void MessageIO::removeListener(MessageListener* listener)
{
    mListeners.erase(std::remove(mListeners.begin(),mListeners.end(),
                listener),mListeners.end());
}

void MessageIO::sendMessage(int type, const std::string& message, int frameId)
{
    BOOST_LOG_TRIVIAL(trace) << "Send message: " << message;
    sendMessage(type, message.c_str(),message.size()+1,frameId);
}

void MessageIO::sendMessage(int type, const char* message,
        unsigned size, int frameId)
{
    // make a message object
    if (frameId == -1) {
        frameId = zephyr::timestamp::ms();
    }
    Message msg(type, size, frameId);
    msg.setBody(message);

    bool alreadyWriting = !mWriteQueue.empty();
    mWriteQueue.emplace_back(std::move(msg));

    if (!alreadyWriting) {
        boost::asio::async_write(mSocket,
                boost::asio::buffer(mWriteQueue.front().data(),
                    mWriteQueue.front().dataSize()),
                boost::bind(&MessageIO::write, this,
                    boost::asio::placeholders::error));
    }
}

void MessageIO::startRead()
{
    boost::asio::ip::tcp::no_delay nodelay(true);
    mSocket.set_option(nodelay);
    auto message = std::make_shared<Message>();
    boost::asio::async_read(mSocket,
        boost::asio::buffer(message->header(),Message::HEADER_SIZE),
        boost::bind(&MessageIO::readHeader, this,
            boost::asio::placeholders::error, message));
}

void MessageIO::write(ErrorCode error)
{
    if (!error) {
        mWriteQueue.pop_front();
        if (!mWriteQueue.empty()) {
            boost::asio::async_write(mSocket,
                    boost::asio::buffer(mWriteQueue.front().data(),
                        mWriteQueue.front().dataSize()),
                    boost::bind(&MessageIO::write, this,
                        boost::asio::placeholders::error));
        }
    } else {
        BOOST_LOG_TRIVIAL(trace) << "Error writing message: " << error.message();
        onWriteError();
    }
}

void MessageIO::readHeader(ErrorCode error, MessagePtr message)
{
    if (!error) {
        message->decodeHeader();
        boost::asio::async_read(mSocket,
                boost::asio::buffer(message->body(),message->size()),
                boost::bind(&MessageIO::readBody, this,
                    boost::asio::placeholders::error, message));
    } else {
        BOOST_LOG_TRIVIAL(trace) << "Header read error: " << error.message();
        onReadError();
    }
}

void MessageIO::readBody(ErrorCode error, MessagePtr message)
{
    if (!error) {
        BOOST_LOG_TRIVIAL(trace) << "Receive message: " << message->body();
        // let the handling happen
        onRecv(message);

        // read the next message
        auto nextMessage = std::make_shared<Message>();
        boost::asio::async_read(mSocket,
                boost::asio::buffer(nextMessage->header(),
                    Message::HEADER_SIZE),
                boost::bind(&MessageIO::readHeader, this,
                    boost::asio::placeholders::error, nextMessage));
    } else {
        BOOST_LOG_TRIVIAL(trace) << "Body error: " << error.message();
        onReadError();
    }
}

void MessageIO::onRecv(MessagePtr message)
{
    // dispatch
    for(auto& l : mListeners) {
        l->onRecv(std::move(*message));
    }
}

}
