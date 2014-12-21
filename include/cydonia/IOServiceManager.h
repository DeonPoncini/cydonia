#ifndef CYDONIA_IOSERVICEMANAGER_H
#define CYDONIA_IOSERVICEMANAGER_H

#include <memory>
#include <boost/asio.hpp>

namespace cydonia
{

typedef boost::asio::io_service IOService;
typedef std::unique_ptr<IOService> IOServicePtr;

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
