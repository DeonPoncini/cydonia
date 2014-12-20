#include <network/Utils.h>

#include <boost/chrono.hpp>

time_t currentMs()
{
    boost::chrono::milliseconds ms =
        boost::chrono::duration_cast<boost::chrono::milliseconds>(
           boost::chrono::high_resolution_clock::now().time_since_epoch());
    return ms.count();
}

