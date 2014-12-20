#include <network/Client.h>

#include <network/IOServiceManager.h>

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

namespace network
{

typedef boost::asio::ip::tcp::resolver Resolver;
typedef boost::asio::ip::tcp::resolver::query Query;

Client::Client(const std::string& address, const std::string& port)
{
    BOOST_LOG_TRIVIAL(trace) << "Connecting to " << address << ":" << port;
    Resolver resolver(IOServiceManager::get().ioService());
    Query query(address,port);
    auto endpoint = resolver.resolve(query);
    boost::asio::async_connect(socket(), endpoint,
            boost::bind(&Client::connect, this,
                boost::asio::placeholders::error));
}

Client::~Client()
{
    BOOST_LOG_TRIVIAL(trace) << "Client disconnected";
}

void Client::connect(ErrorCode error)
{
    if (!error) {
        BOOST_LOG_TRIVIAL(trace) << "Client connected";
        startRead();
    } else {
        throw boost::system::system_error(error);
    }
}

}
