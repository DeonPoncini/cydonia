#ifndef CYDONIA_MESSAGE_H
#define CYDONIA_MESSAGE_H

namespace cydonia
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

    Message(int type = 0, unsigned size = 0, int frame = -1);
    Message(const Message& other) = delete;
    Message& operator=(const Message&) = delete;
    Message(Message&& other);
    Message& operator=(Message&& rhs);
    ~Message();

    bool operator==(const Message& rhs);
    bool operator!=(const Message& rhs);

    void setBody(const char* data);

    char* data();
    const char* data() const;

    char* header();
    const char* header() const;

    char* body();
    const char* body() const;

    inline unsigned dataSize() const { return HEADER_SIZE + size(); }

    inline int type() const { return mMsgHeader.type; }
    inline unsigned size() const { return mMsgHeader.length; }
    inline int id() const { return mMsgHeader.frame; }

    void decodeHeader();

private:
    void allocate();
    void deallocate();
    void encodeHeader();

    header_t mMsgHeader;
    char* mData;
};

}

#endif
