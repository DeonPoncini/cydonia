#ifndef NETWORK_IOSERVICEMANAGER_H
#define NETWORK_IOSERVICEMANAGER_H

#include <memory>
#include <boost/asio.hpp>

namespace network
{

typedef boost::asio::io_service IOService;
typedef std::shared_ptr<IOService> IOServicePtr;

class IOServiceManager
{
public:
    static IOServiceManager& get();

    void run();
    void stop();

    inline IOService& ioService() { return *mIOService; }

private:
    IOServiceManager();

    IOServicePtr mIOService;
};

}

#endif
