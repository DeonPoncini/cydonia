---
Title: cydonia
Description: TCP Client and Server
Author: Deon Poncini

---
cydonia
===============

Developed by Deon Poncini <dex1337@gmail.com>

Description
-----------
Cydonia is an asynchronous TCP server that assigns a single session to each
incoming connection. Each server has a protocol that defines what messages can
be sent from the client to the server and back again.

Messages are either strings or arbritrary data buffers that can be sent from
client to server, or sent as responses from the server. Each message has a
user defined type. The job on the protocol is to provide an interface for
sending messages on the client side, and receiving these messages on the server
side. The server can then respond to these messages with a new message in
response.

The IOServiceManager must be run to initiate communication from either client
or server. If both client and server are in the same process, this only needs
to be done once.

Cydonia is built as a simplification layer on top of boost ASIO, to make the
task of running a TCP server and connecting with clients very simple.

Building
--------
Use the [scripts](http://github.com/DeonPoncini/scripts) build system to build
this library, just add cydonia to your manifest xml file with the appropriate
remote

Usage
-----
See test/main.cpp for an example implementation of how to use the server and
clients.

To build a cydonia client, the user needs to override the cydonia::Protocol
class. In the example, this is the TestProtocol class. The constructor takes a
cydonia::MessageIO reference, the cydonia client will be passed in as this
parameter. The example also defines a listener, this is the recommended way to
get your application to receive messages from the client.

The protocol class should define a set of functions that send the messages
destined for the server. The protocol implementation calls sendMessage with two
signatures, either a std::string, or a char buffer with a size. This is the raw
data that will be sent across the wire.

The protocol class will need to override the message function. This is what is
called when messages are received by the client or server. Any behavior that
the protocol should undertake is defined here, such as calling the listeners
that the message has been received.

To create and run the client, create a client with the server address and port.
Then create a protocol object and construct it with the client as its MessageIO
parameter. Start up IOServiceManager::run, which is blocking so needs to be done
in a separate thread. While the IOServiceManager is running, call any functions
on the protocol as desired and they will get sent to the server.

To build a Cydonia server the user needs to override an additional class, the
cydonia::Session class. Every time a client connects to the server, an instance
of this session will be created. The session should contain as a member variable
the protocol the server supports, this should be the same protocol as given to
the client. If any constructor parameters are required to construct the session,
they can be passed in to the constructor of the server and the session will be
initialized with these parameters. The proper way to get messages out of the
protocol to the server application is to pass a listener into the session,
then have the session listen to the protocol messages and pass this back into
the application itself.

To start a cydonia server, initialize the server templated on the user session.
Pass in the constructor the port to bind to, and then any constructor parameters
needed to construct the session. After this start up the IOServiceManager::run

To run the example application, first launch a server:
    cydonia-test server 10000
Then connect with the client:
    cydonia-test client 127.0.0.1 10000
The above sequence will launch a server bound on port 10000, then run a client
on the same machine to send messages to the server. The default test client
sends a string to the server 100 times and gets a string result from the server,
and then exits the client when all strings have been returned.
