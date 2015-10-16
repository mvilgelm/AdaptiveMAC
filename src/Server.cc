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

#include <src/Server.h>
#include <vector>

Define_Module(Server);

#include "ErrorPkt_m.h"

Server::Server() {
    // TODO Auto-generated constructor stub

}

Server::~Server() {
    cancelAndDelete(checkQTimer);
    incomings->clear();
    delete incomings;
}

void Server::initialize(){
    EV << "Server::initialize() entering function" << endl;

    successCount = 0;
    periodCount = 0;
    accessCount = 0;

    checkQTimer = new cMessage("checkQTimer");

    controlPeriod = (double)par("controlPeriod");
    M = (int)par("M");

    incomings = new cQueue();

    //initial offset for the control period
    scheduleAt(simTime()+controlPeriod+controlPeriod/4, checkQTimer);
}

void Server::handleMessage(cMessage *msg){
    EV << "Server::handleMessage() entering function" << endl;

    if (msg->isSelfMessage()){
        periodCount++;
        processQ();
        scheduleAt(simTime()+controlPeriod, checkQTimer);
    }
    else {
        incomings->insert(msg);
    }
}

void Server::processQ(){
    EV << "Server::processQ() entering function" << endl;

    std::set<int>* noCollisions = new std::set<int>();
    std::set<int>* collisions = new std::set<int>();

    std::set<int>::iterator it;

    cQueue tempQ = cQueue();


    std::vector<double> errorVector = std::vector<double>();

    int attempts = incomings->getLength();
    accessCount += attempts;

    while (!incomings->isEmpty()){
        ErrorPkt * pkt = check_and_cast<ErrorPkt*> (incomings->pop());

        int chnl = pkt->getChannel();

        EV << "Server::processQ() received channel: " << chnl << endl;

        it = noCollisions->find(chnl);

        if ( it != noCollisions->end()) {
            collisions->insert(chnl);
        }

        noCollisions->insert(chnl);

        errorVector.push_back(pkt->getError());

        tempQ.insert(pkt);
    }

    while (!tempQ.isEmpty()){
        ErrorPkt * pkt = check_and_cast<ErrorPkt*> (tempQ.pop());
        int chnl = pkt->getChannel();
        it = collisions->find(chnl);

        cModule* subSys = simulation.getModule(pkt->getId());

        if ( it == collisions->end()) {
            //no collision, send positive feedback
            pkt->setCollision(false);
            successCount++;
            sendDirect(pkt, subSys->gate("in"));
        }
        else {
            //collision, send negative feedback
            pkt->setCollision(true);
            sendDirect(pkt, subSys->gate("in"));
        }
    }

    incomings->clear();
    tempQ.clear();

}

void Server::finish(){
    recordScalar("Throughput", (float)successCount/((float)periodCount*(float)M));
    recordScalar("Access", (float)accessCount/((float)periodCount*(float)M));
}

