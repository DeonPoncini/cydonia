#include <cydonia/Session.h>
#include <cydonia/SessionManager.h>

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

namespace cydonia
{

Session::Session()
{
}

Session::~Session()
{
    BOOST_LOG_TRIVIAL(trace) << "Session destroyed";
}

void Session::start()
{
    BOOST_LOG_TRIVIAL(trace) << "Starting session";
    startRead();
}

void Session::onReadError()
{
    terminateSession();
}

void Session::onWriteError()
{
    terminateSession();
}

void Session::terminateSession()
{
    SessionManager::get().remove(shared_from_this());
}

}
