
#ifndef __mixr_xzmq_ZeroMQHandler_H__
#define __mixr_xzmq_ZeroMQHandler_H__

#include "mixr/base/network/NetHandler.hpp"

#include <map>
#include <string>

namespace mixr {
namespace base { class Boolean; class Integer; class String; }
namespace xzmq {
class ZeroMQContext;

// Mapping types used to convert the enums and 0MQ constants to strings
// and back again!
typedef std::map <std::string, int> s2i_t;
typedef std::map <int, std::string> i2s_t;

//------------------------------------------------------------------------------
// Class: ZeroMQHandler
//
// Description: Manages a ZeroMQ (0MQ) socket
//
// ZeroMQHandler defines the basic 0MQ socket interface.  It is derived from
// the base::NetHandler.  It is noted that 0MQ sockets can only talk to other
// 0MQ sockets.  Ensure any communications strategy uses 0MQ at endpoints.
//
// The derivation from NetHandler is to allow this handler to be used in place
// of other NetHandler objects but 0MQ is so dissimilar that only the
// signature type is used - little sharing of methods.
//
// The following slots are provided for socket definition:
//
//    context     - The context used to create the socket
//    socketType  - Socket type (REQ, REP, DEALER, ROUTER, PUB, SUB, XPUB,
//                  XSUB, PUSH, PULL, PAIR, or STREAM)
//    connect     - Connect a full endpoint definition string (tcp://*:12345)
//    accept      - Accept a full endpoint definition string (tcp://*:12345)
//    noWait      - Set the no-wait flag
//    <options>   - The set options will each have a slot for them
//
//------------------------------------------------------------------------------
class ZeroMQHandler final: public base::NetHandler
{
   DECLARE_SUBCLASS(ZeroMQHandler, base::NetHandler)

public:
   ZeroMQHandler();

   bool initNetwork(const bool noWaitFlag) final;
   bool isConnected() const final;
   bool closeConnection() final;
   bool sendData(const char* const packet, const int size) final;
   unsigned int recvData(char* const packet, const int maxSize) final;

   // Casting for the dereference operator much like base::String.  This is
   // useful when using a 0MQ function directly like zmq_poll.
   operator void* ()               { return socket; }
   operator const void* () const   { return socket; }

   bool setBlocked() final;
   bool setNoWait() final;

protected:
   bool setContext(ZeroMQContext* const context);
   bool setSocketType(const char* const type);
   bool setConnect(const char* const type);
   bool setAccept(const char* const type);
   bool setNoWait(const bool noWait);
   bool setLinger(const int period);
   bool setSubscribe(const char* const filter);
   bool setBackLog(const int count);
   bool setIdentity(const char* const ident);
   bool setSendBufSize(const int count);
   bool setRecvBufSize(const int count);
   bool setSendHWM(const int mark);
   bool setRecvHWM(const int mark);

private:
   void initData();
   
   // The master context is the main process context.  It will be used by
   // all subsequent handlers unless otherwise specified with the context
   // slot.
   static ZeroMQContext* masterContext;

   static s2i_t sts2i;                // ZeroMQHandler type maps
   static i2s_t sti2s;

protected:
   ZeroMQContext* context{};         // Parent context (mixr not ZeroMQ)
   int            socketType{-1};    // Socket type
   std::string    endpoint;          // Endpoint for binding
   int            linger{-1};        // Socket linger period (ms)
   std::string    subscribe;         // Message filter
   int            backLog{-1};       // Connection queue size
   std::string    identity;          // Socket identity
   int            sendBufSize{-1};   // Kernel buffer size for sending
   int            recvBufSize{-1};   // Kernel buffer size for receiving
   int            sendHWM{-1};       // High-water-mark for outbound messages
   int            recvHWM{-1};       // High-water-mark for inbound messages
   bool           noWait{};          // No wait flag from the slot
   void*          socket{};          // 0MQ socket
   bool           doBind{};          // Accept or connect!
   bool           dontWait{};        // 0MQ no wait flag
   bool           ready{};           // Initialization was successful

private:
   // slot table helper methods
   bool setSlotContext(ZeroMQContext* const);
   bool setSlotSocketType(const base::String* const);
   bool setSlotConnect(const base::String* const);
   bool setSlotAccept(const base::String* const);
   bool setSlotNoWait(const base::Boolean* const);
   bool setSlotLinger(const base::Integer* const);
   bool setSlotSubscribe(const base::String* const);
   bool setSlotBackLog(const base::Integer* const);
   bool setSlotIdentity(const base::String* const);
   bool setSlotSendBufSize(const base::Integer* const);
   bool setSlotRecvBufSize(const base::Integer* const);
   bool setSlotSendHWM(const base::Integer* const);
   bool setSlotRecvHWM(const base::Integer* const);
};

}
}

#endif
