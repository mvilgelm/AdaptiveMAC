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

#include <BackoffSubsystem.h>

#include "ErrorPkt_m.h"

Define_Module(BackoffSubsystem);

BackoffSubsystem::BackoffSubsystem() {
    // TODO Auto-generated constructor stub

    _backOffTimer = new cMessage("backOffTimer");
    _waitForAckTimer = new cMessage("waitForAckTimer");

}

BackoffSubsystem::~BackoffSubsystem() {

    cancelAndDelete(_waitForAckTimer);
    cancelAndDelete(_backOffTimer);
}

void BackoffSubsystem::initialize(){
    EV << "BackoffSubsystem::transmit() entering function." << endl;

    Subsystem::initialize();
    SLOT_LENGTH = (double) par("slotLength");

    // TODO make them parameters
    MIN_EXPONENT = 2;
    MAX_EXPONENT = 8;

    _boExponent = MIN_EXPONENT;
}

void BackoffSubsystem::processSelfMessage(cMessage * msg){

    if (msg == _backOffTimer){
        EV << "BackoffSubsystem::processSelfMessage\tBack-off expired, resending..." << endl;
        //resent data
        transmit();
    }
    else if (msg == _waitForAckTimer) {
        EV << "BackoffSubsystem::processSelfMessage\tNo ack received, assuming collisions" << endl;
        // no ack arrived, go for back-off
        collisionEvent();

    }

}

void BackoffSubsystem::collisionEvent(){
    EV << "BackoffSubsystem::collisionEvent() entering function." << endl;
    // keep theta 0
    loop->theta = 0;
    scheduleAt(simTime()+getBackOffTime()-0.000000001, _backOffTimer);

}

void BackoffSubsystem::successEvent(){
    loop->theta=1;
    EV << "false" << endl;
    if (_waitForAckTimer->isScheduled())
        cancelEvent(_waitForAckTimer);

    if (_backOffTimer->isScheduled())
        cancelEvent(_backOffTimer);

    _boExponent = MIN_EXPONENT;
}

simtime_t BackoffSubsystem::getBackOffTime(){
    simtime_t t = this->intuniform(1, pow(2, _boExponent)) * SLOT_LENGTH;

    if (_boExponent < MAX_EXPONENT)
        _boExponent++;

    return t;
}

void BackoffSubsystem::processControlTimer(){

    if (_waitForAckTimer->isScheduled())
        cancelEvent(_waitForAckTimer);

    if (_backOffTimer->isScheduled())
        cancelEvent(_backOffTimer);

    _boExponent = MIN_EXPONENT;

    Subsystem::processControlTimer();
}

void BackoffSubsystem::transmit(){
    EV << "BackoffSubsystem::transmit() entering function." << endl;

    int channel = intuniform(1,M);

    ErrorPkt *pkt = new ErrorPkt();
    pkt->setId(id);
    pkt->setChannel(channel);
    pkt->setError(loop->error);

    //TODO duration should be a parameter
    pkt->setDuration(0.009);

    EV << "Subsystem::transmit() error: "<< loop->error << ", channel choice: " << channel << endl;

    scheduleAt(simTime()+0.01 + 0.000000001, _waitForAckTimer); // TODO timing

    sendDirect(pkt, 0.0, pkt->getDuration(), server->gate("in"));
}
