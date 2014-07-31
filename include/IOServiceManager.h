#ifndef NETWORK_IOSERVICEMANAGER_H
#define NETWORK_IOSERVICEMANAGER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

namespace network
{

typedef boost::asio::io_service IOService;
typedef boost::shared_ptr<IOService> IOServicePtr;

class IOServiceManager
{
public:
    static IOServiceManager& get();

    void run();
    void stop();

    inline IOService& ioService() { return *mIOService; }
    inline IOServicePtr ioServicePtr() { return mIOService; }

private:
    IOServiceManager();

    IOServicePtr mIOService;
};

}

#endif
