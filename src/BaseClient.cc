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

#include "BaseClient.h"
#include "Server.h"

BaseClient::BaseClient() {
    // TODO Auto-generated constructor stub

}

BaseClient::~BaseClient() {
    // TODO Auto-generated destructor stub
}


void BaseClient::initialize(){
    EV << "BaseClient::initialize() entering function." << endl;
    id = getId();

    loop = NULL;

    M = (int)par("M");
    N = (int)par("N");

    adaptLambda = (bool)par("adaptLambda");
    adaptationExperiment = (bool)par("adaptationExperiment");
    defaultM = (int)par("defaultM");

    server = getSimulation()->getModuleByPath("server");

    controlTimer = new cMessage("controlTimer");
    scheduleAt(simTime()+getNextArrivalTime(), controlTimer);
}

void BaseClient::handleMessage(cMessage *msg){
    EV << "BaseClient::handleMessage() entering function." << endl;

    if (msg == controlTimer){
        processControlTimer();
        scheduleAt(simTime()+getNextArrivalTime(), controlTimer);
    }
    else if (msg->isSelfMessage()){
        processSelfMessage(msg);
    }
    else {
        processFeedback(msg);
    }
}

void BaseClient::processControlTimer(){
    if (adaptationExperiment){
        Server *srv = check_and_cast<Server*> (server);
        M = srv->getM();
    }

    loop->updateError(this);

    if (getEnvir()->isGUI())
        updateDisplay();

    if (decideOnTx()){
        transmit();
    }
}

void BaseClient::processSelfMessage(cMessage * msg){
    //nothing expected here
}

double BaseClient::getNextArrivalTime(){
    //nothing expected here
}

