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

#include <WLANServer.h>
#include "WLANServer.h"
#include "ErrorPkt_m.h"

Define_Module(WLANServer);


WLANServer::WLANServer()
{
    endRxEvent = NULL;
    pkRxInProgress = NULL;
}

WLANServer::~WLANServer()
{

    cancelAndDelete(endRxEvent);
}

void WLANServer::initialize()
{

    endRxEvent = new cMessage("end-reception");

    changeChannelState(FREE);

    gate("in")->setDeliverOnReceptionStart(true);

    setSimParameters();

    currentCollisionNumFrames = 0;
    receiveCounter = 0;
    WATCH(currentCollisionNumFrames);

    _receiveSignal = registerSignal("receive");

}


void WLANServer::handleMessage(cMessage *msg)
{
    if (msg==endRxEvent)
    {
        /**
         * transmission finished
         */
        EV << "reception finished\n";
        changeChannelState(FREE);

        // update statistics
        simtime_t dt = simTime() - _recvStartTime;
        if (currentCollisionNumFrames==0)
        {
            cTimestampedValue tmp(_recvStartTime, 1l);

            sendAck();

        }
        else
        {
            // start of collision at recvStartTime
            cTimestampedValue tmp(_recvStartTime, currentCollisionNumFrames);

        }

        currentCollisionNumFrames = 0;
        receiveCounter = 0;

        // update network graphics
        if (getEnvir()->isGUI())
        {
            getDisplayString().setTagArg("i2",0,"x_off");
            getDisplayString().setTagArg("t",0,"");
        }

        cancelAndDelete(pkRxInProgress);

    }
    else
    {
        /**
         * received a new packet
         */
        cPacket *pkt = check_and_cast<cPacket *>(msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();

        if (channelState==FREE)
        {
            /**
             * no collision (yet)
             */
            EV << "started receiving\n";
            _recvStartTime = simTime();
            changeChannelState(BUSY);

            scheduleAt(endReceptionTime, endRxEvent);

            if (getEnvir()->isGUI())
            {
                getDisplayString().setTagArg("i2",0,"x_yellow");
                getDisplayString().setTagArg("t",0,"RECEIVE");
                getDisplayString().setTagArg("t",2,"#808000");
            }
            pkRxInProgress = pkt;
        }
        else
        {
            /**
             * collision occured
             */
            EV << "another frame arrived while receiving -- collision!\n";

            if (currentCollisionNumFrames==0)
                currentCollisionNumFrames = 2;
            else
                currentCollisionNumFrames++;

            if (endReceptionTime > endRxEvent->getArrivalTime())
            {
                cancelEvent(endRxEvent);
                scheduleAt(endReceptionTime, endRxEvent);
            }

            // update network graphics
            if (getEnvir()->isGUI())
            {
                getDisplayString().setTagArg("i2",0,"x_red");
                getDisplayString().setTagArg("t",0,"COLLISION");
                getDisplayString().setTagArg("t",2,"#800000");
                char buf[32];
                sprintf(buf, "Collision! (%ld frames)", currentCollisionNumFrames);
                bubble(buf);
            }

            cancelAndDelete(pkt);
        }
        changeChannelState(BUSY);
    }
}

void WLANServer::finish()
{
}

void WLANServer::sendAck()
{
    if (pkRxInProgress==NULL)
        error("no packet is being received...");

    ErrorPkt *pk = check_and_cast<ErrorPkt*> (pkRxInProgress);

    //create ack packet
    ErrorPkt *ack = new ErrorPkt(pk->getName());
    ack->setId(pk->getId());
    ack->setBitLength(ackLenBits);

    //send direct ack
    cModule *sender = cSimulation::getActiveSimulation()->getModule(pk->getId());
    simtime_t duration = ack->getBitLength() / TX_RATE;
    sendDirect(ack, 0.0, duration, sender->gate("in"));

//    // start of sending: now
//    // //emit(_receiveSignal, 1l);
//    // end of sending:
//    simtime_t ackSendEndTime = simTime() + duration;
//    cTimestampedValue tmp(ackSendEndTime, 0l);
//    // //emit(_receiveSignal, &tmp);
}

void WLANServer::setSimParameters(){

    // TODO uncomment and implement as parameters
    // TX_RATE = par("txRate");
    // ackLenBits = &par("ackLenBits");
    TX_RATE = 80000; // bps
    ackLenBits = 80;
}

void WLANServer::changeChannelState(ChannelState newState){
    channelState = newState;
}
