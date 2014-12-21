#include <network/IOServiceManager.h>

namespace network
{

IOServiceManager& IOServiceManager::get()
{
    static IOServiceManager instance;
    return instance;
}

IOServiceManager::IOServiceManager() :
    mIOService(std::make_shared<IOService>())
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
