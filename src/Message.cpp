#include "Message.h"

#include <cstdlib>
#include <cstring>

namespace network
{

Message::Message(int type, unsigned size, int frame) :
    mMsgHeader{type,size,frame},
    mData(nullptr)
{
    encodeHeader();
}

Message::Message(const Message& other) :
    mData(nullptr)
{
    mMsgHeader = other.mMsgHeader;
    encodeHeader();
    if (mMsgHeader.length > 0)
    {
        memcpy(mData,other.mData,other.dataSize());
    }
}

Message::~Message()
{
    deallocate();
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
    if (mMsgHeader.length > 0)
    {
        allocate();
        memcpy(mData,header(),HEADER_SIZE);
    }
}

void Message::encodeHeader()
{
    if (mMsgHeader.length > 0)
    {
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
    if (mData != nullptr)
    {
        delete mData;
        mData = nullptr;
    }
}

}
