#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////
// MockChannel.h - echo messages from input queue to output queue  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013       //
/////////////////////////////////////////////////////////////////////

namespace mockChannel {

  using namespace System;

  typedef String Message;

  public ref class IChannel abstract 
  {
  public: 
    static IChannel^ CreateChannel();
    virtual Message^ getMessage()=0;
    virtual void postMessage(Message^ msg)=0;
    virtual void shutDown()=0;
  };
}
#endif
