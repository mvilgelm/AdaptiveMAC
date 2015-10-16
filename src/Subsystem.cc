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

#include <src/Subsystem.h>
#include "ErrorPkt_m.h"

Define_Module(Subsystem);

Subsystem::Subsystem() {
    // TODO Auto-generated constructor stub

}

Subsystem::~Subsystem() {
    // TODO Auto-generated destructor stub
}

void Subsystem::initialize(){
    EV << "Subsystem::initialize() entering function." << endl;
    id = getId();
    error = 0;
    varW = (double)par("varW");
    sysA = (double)par("sysA");
    Lambda = (double)par("Lambda");
    M = (int)par("M");
    controlPeriod = (double)par("controlPeriod");

    theta = 0;

    server = simulation.getModuleByPath("server");
    //if (!server) error("server not found");
    controlTimer = new cMessage("controlTimer");
    scheduleAt(simTime()+controlPeriod, controlTimer);
}

void Subsystem::handleMessage(cMessage *msg){
    EV << "Subsystem::handleMessage() entering function." << endl;

    if (msg == controlTimer){
        updateError();
        if (decideOnTx()){
            transmit();
        }
        scheduleAt(simTime()+controlPeriod, controlTimer);
    }
    else {
        ErrorPkt *pkt = check_and_cast<ErrorPkt*> (msg);
        bool collision = pkt->getCollision();
        if (collision)
            theta=0;
        else
            theta=1;
        delete pkt;
    }
}

void Subsystem::updateError(){
    EV << "Subsystem::updateError() entering function." << endl;

    error = (1-theta)*sysA*error + normal(0, sqrt(varW));
    EV << "Err: " << error << endl;
}

bool Subsystem::decideOnTx(){
    EV << "Subsystem::decideOnTx() entering function." << endl;

    if (abs(error)>Lambda)
        return true;
    else
        return false;
}

void Subsystem::transmit(){
    EV << "Subsystem::transmit() entering function." << endl;

    int channel = intuniform(1,M);

    ErrorPkt *pkt = new ErrorPkt();
    pkt->setId(id);
    pkt->setChannel(channel);
    pkt->setError(error);

    sendDirect(pkt, server->gate("in"));
}
