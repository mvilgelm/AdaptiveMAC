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

    MIN_EXPONENT = (int) par("minExponent");
    MAX_EXPONENT = (int) par("maxExponent");

    _errorDependentExponent = (bool) par("errorDependentExponent");

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
    if (!_errorDependentExponent) {
        // regular exponent
        simtime_t t = this->intuniform(1, pow(2, _boExponent)) * SLOT_LENGTH;

        if (_boExponent < MAX_EXPONENT)
            _boExponent++;

        return t;
    }
    else {
        // error dependent exponent
        double error = this->loop->error;

        int errorCls = MAX_EXPONENT - floor((log(error)/log(2)));

        if (errorCls < 0)
            errorCls = 0;

        _boExponent = MIN_EXPONENT + errorCls;

        simtime_t t = this->intuniform(1, pow(2, _boExponent)) * SLOT_LENGTH;

        return t;
    }
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

    pkt->setDuration((double) par("pktDuration") * SLOT_LENGTH);

    EV << "Subsystem::transmit() error: "<< loop->error << ", channel choice: " << channel << endl;

    scheduleAt(simTime()+ ((double) par("pktDuration") + (double) par("ackDuration")) * SLOT_LENGTH + 0.000000001, _waitForAckTimer); // TODO timing

    sendDirect(pkt, 0.0, pkt->getDuration(), server->gate("in"));
}
