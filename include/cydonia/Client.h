#ifndef CYDONIA_CLIENT_H
#define CYDONIA_CLIENT_H

#include <boost/asio.hpp>

#include "MessageIO.h"

namespace cydonia
{

class Client : public MessageIO
{
public:
    Client(const std::string& address, const std::string& port);
    ~Client();

private:
    void connect(ErrorCode error);
};

}

#endif
