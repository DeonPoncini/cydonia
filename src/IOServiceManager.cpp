#include <network/IOServiceManager.h>

#include <boost/make_shared.hpp>

namespace network
{

IOServiceManager& IOServiceManager::get()
{
    static IOServiceManager instance;
    return instance;
}

IOServiceManager::IOServiceManager() :
    mIOService(boost::make_shared<IOService>())
{
}

void IOServiceManager::run()
{
    mIOService->run();
}

void IOServiceManager::stop()
{
    mIOService->stop();
}

}
