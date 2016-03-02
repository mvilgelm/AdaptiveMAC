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
    //channelStateSignal = registerSignal("channelState");

    endRxEvent = new cMessage("end-reception");

    changeChannelState(FREE);

    //emit(channelStateSignal,IDLE);

    gate("in")->setDeliverOnReceptionStart(true);

    setSimParameters();

    currentCollisionNumFrames = 0;
    receiveCounter = 0;
    WATCH(currentCollisionNumFrames);

    //receiveBeginSignal = registerSignal("receiveBegin");
    receiveSignal = registerSignal("receive");
    //collisionSignal = registerSignal("collision");
    //collisionLengthSignal = registerSignal("collisionLength");
//
    emit(receiveSignal, 0L);
//    emit(receiveBeginSignal, 0L);

}


void WLANServer::handleMessage(cMessage *msg)
{
    if (msg==endRxEvent)
    {
        /**
         * transmission successfully finished
         */
        EV << "reception finished\n";
        changeChannelState(FREE);
        emit(channelStateSignal,IDLE);


        // update statistics
        simtime_t dt = simTime() - recvStartTime;
        if (currentCollisionNumFrames==0)
        {
            // start of reception at recvStartTime
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            // end of reception now
            emit(receiveSignal, 0l);

            sendAck();

        }
        else
        {
            // start of collision at recvStartTime
            cTimestampedValue tmp(recvStartTime, currentCollisionNumFrames);
            //emit(collisionSignal, &tmp);

            //emit(collisionLengthSignal, dt);
        }

        currentCollisionNumFrames = 0;
        receiveCounter = 0;
        //emit(receiveBeginSignal, receiveCounter);

        // update network graphics
        if (ev.isGUI())
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

        //emit(receiveBeginSignal, ++receiveCounter);

        if (channelState==FREE)
        {
            /**
             * no collision (yet)
             */
            EV << "started receiving\n";
            recvStartTime = simTime();
            changeChannelState(BUSY);
            //emit(channelStateSignal, TRANSMISSION);
            scheduleAt(endReceptionTime, endRxEvent);

            if (ev.isGUI())
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
            if (ev.isGUI())
            {
                getDisplayString().setTagArg("i2",0,"x_red");
                getDisplayString().setTagArg("t",0,"COLLISION");
                getDisplayString().setTagArg("t",2,"#800000");
                char buf[32];
                sprintf(buf, "Collision! (%ld frames)", currentCollisionNumFrames);
                bubble(buf);
            }

            cancelAndDelete(pkt);
            //delete pkRxInProgress;
        }
        changeChannelState(BUSY);
        //delete pkt;
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
    /*const char * senderName = pk->getSenderName();
    EV << "Server::sendAck --- acknowledge reception to the sender: "
            << senderName << endl;*/

    //create ack packet
    ErrorPkt *ack = new ErrorPkt(pk->getName());
    ack->setId(pk->getId());
//    ack->setPktNum(pk->getPktNum());
    ack->setBitLength(ackLenBits->longValue());

    //send direct ack (no propagation delay assumed)
    cModule *sender = cSimulation::getActiveSimulation()->getModule(pk->getId());
    simtime_t duration = ack->getBitLength() / txRate;
    sendDirect(ack, radioDelay, duration, sender->gate("in"));

    // start of sending: now
    emit(receiveSignal, 1l);
    // end of sending:
    simtime_t ackSendEndTime = simTime() + duration;
    cTimestampedValue tmp(ackSendEndTime, 0l);
    emit(receiveSignal, &tmp);
}

void WLANServer::setSimParameters(){
    txRate = par("txRate");
    radioDelay = par("radioDelay");
    ackLenBits = &par("ackLenBits");
}

void WLANServer::changeChannelState(ChannelState newState){
    channelState = newState;
}
