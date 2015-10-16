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
    cancelAndDelete(controlTimer);
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
    periodCount=0;
    errMean = 0;
    errVar = 0;

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
        EV << "Subsystem::handleMessage() collision: ";

        if (collision){
            theta=0;
            EV << "true" << endl;
        }
        else {
            theta=1;
            EV << "false" << endl;
        }
        delete pkt;
    }
}

void Subsystem::updateError(){
    EV << "Subsystem::updateError() entering function." << endl;

    error = (1-theta)*sysA*error + normal(0, sqrt(varW));
    EV << "Err: " << error << endl;

    if (ev.isGUI())
        updateDisplay();

    theta = 0;

    updateErrVar();
}

bool Subsystem::decideOnTx(){
    EV << "Subsystem::decideOnTx() entering function." << endl;

    EV << "Subsystem::decideOnTx() lambda: "<< Lambda << ", error: " << fabs(error) << endl;

    if (fabs(error)>Lambda){
        EV << "Subsystem::decideOnTx() decision: true" << endl;
        return true;
    }
    else {
        EV << "Subsystem::decideOnTx() decision: false" << endl;
        return false;
    }
}

void Subsystem::transmit(){
    EV << "Subsystem::transmit() entering function." << endl;

    int channel = intuniform(1,M);

    ErrorPkt *pkt = new ErrorPkt();
    pkt->setId(id);
    pkt->setChannel(channel);
    pkt->setError(error);

    EV << "Subsystem::transmit() channel choice: " << channel << endl;

    sendDirect(pkt, server->gate("in"));
}

void Subsystem::updateErrVar(){
    periodCount++;
    double delta = error - errMean;
    errMean += delta/((double)periodCount);
    errVar += delta*(error-errMean);
}

void Subsystem::finish(){
    recordScalar("errVar", errVar/((double)periodCount-1));
}

void Subsystem::updateDisplay(){
    char buf[30];
    sprintf(buf, "err: %.4f", error);
    getDisplayString().setTagArg("t",0,buf);
    getParentModule()->getDisplayString().setTagArg("t",0,buf);
}
