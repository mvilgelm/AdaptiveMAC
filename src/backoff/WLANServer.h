//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef WLANSERVER_H_
#define WLANSERVER_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Aloha server; see NED file for more info.
 */
class WLANServer : public cSimpleModule {
  private:
    // state variables, event pointers

    typedef enum ChannelState {
        BUSY,
        FREE,
    } ChannelState;

    ChannelState channelState;
    cMessage *endRxEvent;

    long currentCollisionNumFrames;
    long receiveCounter;

    simtime_t _recvStartTime;
    enum {IDLE=0, TRANSMISSION=1, COLLISION=2};
    simsignal_t channelStateSignal;

    //packet waiting to be acknowledged
    cPacket *pkRxInProgress;
    void sendAck();

    // statistics
    simsignal_t _receiveBeginSignal;
    simsignal_t _receiveSignal;
    simsignal_t _collisionLengthSignal;
    simsignal_t _collisionSignal;

    double TX_RATE;
    int ackLenBits;


  public:
    WLANServer();
    virtual ~WLANServer();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    void setSimParameters();
    void changeChannelState(ChannelState newState);
};

#endif /* WLANSERVER_H_ */
