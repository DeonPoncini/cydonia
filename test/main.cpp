#include <network/Client.h>
#include <network/Protocol.h>
#include <network/Server.h>

#include <iostream>
#include <string>

class TestSession : public network::Session
{
public:
    TestSession() :
        mProtocol(*this)

    {
    }

private:
    network::Protocol mProtocol;
};

void usage()
{
    std::cout << "Usage: network-test server <port> | client <ip>"
        << " <port>" << std::endl;
}

int main(int argc, char* argv[])
{
    // validate input
    if (argc < 2) {
        usage();
        return -1;
    }

    std::string action(argv[1]);

    if (action == "server" && argc != 3) {
        usage();
        return -1;
    }

    if (action == "client" && argc != 4) {
        usage();
        return -1;
    }

    if (action == "server") {
        auto port = atoi(argv[2]);
        network::Server<TestSession> server(port);
        network::IOServiceManager::get().run();
    } else if (action == "client") {
        std::string ip(argv[2]);
        std::string port(argv[3]);
        network::Client client(ip, port);
        network::Protocol protocol(client);
        protocol.sendPing();
        network::IOServiceManager::get().run();
    } else {
        usage();
        return -1;
    }

    return 0;
}
