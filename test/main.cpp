#include <network/Client.h>
#include <network/Protocol.h>
#include <network/Server.h>

#include <iostream>
#include <string>

class TestProtocol : public network::Protocol
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void onPongRecv() = 0;
    };

    TestProtocol(network::MessageIO& messageIO, Listener* l) :
        network::Protocol(messageIO),
        mListener(l)
    {
    }

    virtual ~TestProtocol() {}

    void sendPing()
    {
        sendMessage(PING,"Ping",-1);
    }

    void sendPong()
    {
        sendMessage(PONG,"Pong",-1);
    }

private:
    enum MessageType
    {
        PING = 0,
        PONG
    };

    virtual void message(const network::Message& message) override
    {
        switch (message.type()) {
            case MessageType::PING:
                std::cout << "Ping received" << std::endl;
                sendPong();
                break;
            case MessageType::PONG:
                std::cout << "Pong received" << std::endl;
                if (mListener != nullptr) {
                    mListener->onPongRecv();
                }
                break;
        }
    }

    Listener* mListener;
};

class TestProtocolListener : public TestProtocol::Listener
{
public:
    virtual ~TestProtocolListener() {}
    virtual void onPongRecv() override
    {
        network::IOServiceManager::get().stop();
    }
};

class TestSession : public network::Session
{
public:
    TestSession() :
        mProtocol(*this, nullptr)

    {
    }

private:
    TestProtocol mProtocol;
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
        TestProtocolListener listener;
        TestProtocol protocol{client, &listener};
        protocol.sendPing();
        network::IOServiceManager::get().run();
    } else {
        usage();
        return -1;
    }

    return 0;
}
