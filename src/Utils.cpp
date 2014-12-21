#include <network/Utils.h>

#include <chrono>

time_t currentMs()
{
    std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
           std::chrono::high_resolution_clock::now().time_since_epoch());
    return ms.count();
}

