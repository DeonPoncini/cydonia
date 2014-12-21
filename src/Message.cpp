#include <cydonia/Message.h>

#include <cstring>

namespace cydonia
{

Message::Message(int type, unsigned size, int frame) :
    mMsgHeader{type,size,frame},
    mData(nullptr)
{
    encodeHeader();
}

Message::Message(Message&& other) :
    mMsgHeader(other.mMsgHeader),
    mData(other.mData)
{
    other.mData = nullptr;
    other.mMsgHeader.length = 0;
}

Message& Message::operator=(Message&& rhs)
{
    if (*this != rhs) {
        mMsgHeader = rhs.mMsgHeader;
        mData = rhs.mData;
        rhs.mData = nullptr;
        rhs.mMsgHeader.length = 0;
    }
    return *this;
}

Message::~Message()
{
    deallocate();
}

bool Message::operator==(const Message& rhs)
{
    return (mMsgHeader.type == rhs.mMsgHeader.type) &&
        (mMsgHeader.length == rhs.mMsgHeader.length) &&
        (mMsgHeader.frame == rhs.mMsgHeader.frame) &&
        (mData == rhs.mData);
}

bool Message::operator!=(const Message& rhs)
{
    return !(*this == rhs);
}

char* Message::header()
{
    return reinterpret_cast<char*>(&mMsgHeader);
}

const char* Message::header() const
{
    return reinterpret_cast<const char*>(&mMsgHeader);
}

void Message::decodeHeader()
{
    if (mMsgHeader.length > 0) {
        allocate();
        memcpy(mData,header(),HEADER_SIZE);
    }
}

void Message::encodeHeader()
{
    if (mMsgHeader.length > 0) {
        allocate();
        memcpy(mData,&mMsgHeader,HEADER_SIZE);
    }
}

char* Message::body()
{
    return mData + HEADER_SIZE;
}

const char* Message::body() const
{
    return mData + HEADER_SIZE;
}

char* Message::data()
{
    return mData;
}

const char* Message::data() const
{
    return mData;
}

void Message::setBody(const char* data)
{
    memcpy(body(),data,mMsgHeader.length);
}

void Message::allocate()
{
    mData = new char[dataSize()];
}

void Message::deallocate()
{
    if (mData != nullptr) {
        delete mData;
        mData = nullptr;
    }
}

}
