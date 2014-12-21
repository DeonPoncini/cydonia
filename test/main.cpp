#include <cydonia/Client.h>
#include <cydonia/Protocol.h>
#include <cydonia/Server.h>

#include <iostream>
#include <string>

const auto PING_COUNT = 100;

class TestProtocol : public cydonia::Protocol
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void onPongRecv() = 0;
    };

    TestProtocol(cydonia::MessageIO& messageIO, Listener* l) :
        cydonia::Protocol(messageIO),
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

    virtual void message(cydonia::Message&& message) override
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
        mRecCount++;
        if (mRecCount == PING_COUNT) {
            cydonia::IOServiceManager::get().stop();
        }
    }
private:
    int mRecCount = 0;
};

class TestSession : public cydonia::Session
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
    std::cout << "Usage: cydonia-test server <port> | client <ip>"
        << " <port>" << std::endl;
}

void iothread()
{
    cydonia::IOServiceManager::get().run();
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
        cydonia::Server<TestSession> server(port);
        std::thread t(iothread);
        if (t.joinable()) {
            t.join();
        }
    } else if (action == "client") {
        std::string ip(argv[2]);
        std::string port(argv[3]);
        cydonia::Client client(ip, port);
        TestProtocolListener listener;
        TestProtocol protocol{client, &listener};
        std::thread t(iothread);
        for (auto i = 0; i < PING_COUNT; i++) {
            protocol.sendPing();
        }
        if (t.joinable()) {
            t.join();
        }
    } else {
        usage();
        return -1;
    }

    return 0;
}
