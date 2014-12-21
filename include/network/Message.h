#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <cstddef>
#include <ctime>

namespace network
{

class Message
{
private:
    struct header_t
    {
        int type;
        unsigned length;
        int frame;
    };

public:
    static const int HEADER_SIZE = sizeof(header_t);

    Message(int type = 0, unsigned size = 0,int frame = -1);
    Message(const Message& other) = delete;
    Message& operator=(const Message&) = delete;
    Message(Message&& other);
    Message& operator=(Message&& rhs);
    ~Message();

    bool operator==(const Message& rhs);
    bool operator!=(const Message& rhs);

    char* header();
    const char* header() const;
    void decodeHeader();

    inline int type() const { return mMsgHeader.type; }
    inline unsigned size() const { return mMsgHeader.length; }
    inline int id() const { return mMsgHeader.frame; }
    inline unsigned dataSize() const { return HEADER_SIZE + size(); }

    char* body();
    const char* body() const;
    void setBody(const char* data);

    char* data();
    const char* data() const;

private:
    void allocate();
    void deallocate();
    void encodeHeader();

    header_t mMsgHeader;
    char* mData;
};

}

#endif
