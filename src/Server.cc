/*
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
*/

#include <src/Server.h>
#include <vector>

Define_Module(Server);

#include "ErrorPkt_m.h"

Server::Server() {
    //do stuff in initialize

}

Server::~Server() {
    //cleanup
    cancelAndDelete(checkQTimer);
    incomings->clear();
    delete incomings;
}

void Server::initialize(){
    EV << "Server::initialize() entering function" << endl;

    resetStatistic();
    setParameters();

    incomings = new cQueue();
    checkQTimer = new cMessage("checkQTimer");
    //initial offset for the control period: not to intercept with the incomings
    scheduleAt(simTime()+controlPeriod+controlPeriod/4, checkQTimer);
}

void Server::handleMessage(cMessage *msg){
    EV << "Server::handleMessage() entering function" << endl;

    if (msg->isSelfMessage()){
        //self-timer for checking the queue
        if (adaptationExperiment){
            if (bernoulli(pG))
                M = 10;
            else
                M = 5;
        }

        periodCount++;
        processQ();
        scheduleAt(simTime()+controlPeriod, checkQTimer);
    }
    else {
        //message from control loop, inqueue
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

    //record for statistics
    int attempts = incomings->getLength();
    accessCount += attempts;

    //go through list of messages and choose collided channels
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

    //go through list of messages and send replies
    while (!tempQ.isEmpty()){
        ErrorPkt * pkt = check_and_cast<ErrorPkt*> (tempQ.pop());
        int chnl = pkt->getChannel();
        it = collisions->find(chnl);

        cModule* subSys = getSimulation()->getModule(pkt->getId());

        if ( it == collisions->end()) {
            //no collision, send positive feedback
            pkt->setCollision(false);
            successCount++;
            sendDirect(pkt, subSys->gate("in"));
        }
        else {
            //collision, send negative feedback
            collisionCount++;
            pkt->setCollision(true);
            sendDirect(pkt, subSys->gate("in"));
        }
    }

    //cleanup
    incomings->clear();
    tempQ.clear();

}

void Server::finish(){
    recordScalar("Throughput", (float)successCount/((float)periodCount*(float)M));
    recordScalar("Access", (float)accessCount/((float)periodCount*(float)M));
    recordScalar("Collisions", (float)collisionCount/((float)periodCount*(float)M));
}

int Server::getM(){
    return M;
}

void Server::setParameters(){
    //probability of good channel
    pG = (double)par("pG");
    adaptationExperiment = (bool)par("adaptationExperiment");

    //parameters
    controlPeriod = (double)par("controlPeriod");
    M = (int)par("M");
}

void Server::resetStatistic(){
    successCount = 0;
    periodCount = 0;
    accessCount = 0;
    collisionCount = 0;

}

