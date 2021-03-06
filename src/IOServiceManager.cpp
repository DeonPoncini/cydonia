#include <cydonia/IOServiceManager.h>
#include <zephyr/make_unique.h>

namespace cydonia
{

IOServiceManager& IOServiceManager::get()
{
    static IOServiceManager instance;
    return instance;
}

IOServiceManager::IOServiceManager() :
    mIOService(zephyr::make_unique<IOService>())
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
