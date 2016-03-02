/*
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
*/

#include <src/Subsystem.h>
#include "ErrorPkt_m.h"
#include "Server.h"

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

    loop = new ControlLoop();

    loop->error = 0;
    loop->varW = (double)par("varW");
    loop->sysA = (double)par("sysA");
    loop->Lambda = (double)par("Lambda");
    loop->controlPeriod = (double)par("controlPeriod");

    loop->theta = 0;
    loop->periodCount=0;
    loop->errMean = 0;
    loop->errVar = 0;


    M = (int)par("M");
    N = (int)par("N");


    adaptLambda = (bool)par("adaptLambda");
    adaptationExperiment = (bool)par("adaptationExperiment");
    defaultM = (int)par("defaultM");

    server = simulation.getModuleByPath("server");
    //if (!server) error("server not found");
    controlTimer = new cMessage("controlTimer");
    scheduleAt(simTime()+loop->controlPeriod, controlTimer);
}

void Subsystem::handleMessage(cMessage *msg){
    EV << "Subsystem::handleMessage() entering function." << endl;

    if (msg == controlTimer){
        processControlTimer();
        scheduleAt(simTime()+loop->controlPeriod, controlTimer);
    }
    else if (msg->isSelfMessage()){
        processSelfMessage(msg);
    }
    else {
        processFeedback(msg);
    }
}

bool Subsystem::decideOnTx(){
    //EV << "Subsystem::decideOnTx() entering function." << endl;
    //EV << "Subsystem::decideOnTx() lambda: "<< Lambda << ", error: " << fabs(error) << endl;

    if (!adaptationExperiment) {

        if (fabs(loop->error)>loop->Lambda){
            //EV << "Subsystem::decideOnTx() decision: true" << endl;
            return true;
        }
        else {
            //EV << "Subsystem::decideOnTx() decision: false" << endl;
            return false;
        }
    }
    else {
        if (adaptLambda) {
            if (fabs(loop->error)>lambdaLookupTable(M,N)){
                //EV << "Subsystem::decideOnTx() decision: true" << endl;
                return true;
            }
            else {
                //EV << "Subsystem::decideOnTx() decision: false" << endl;
                return false;
            }
        }
        else {
            if (fabs(loop->error)>lambdaLookupTable(defaultM,N)){
                //EV << "Subsystem::decideOnTx() decision: true" << endl;
                return true;
            }
            else {
                //EV << "Subsystem::decideOnTx() decision: false" << endl;
                return false;
            }
        }
    }
}

void Subsystem::transmit(){
    //EV << "Subsystem::transmit() entering function." << endl;

    int channel = intuniform(1,M);

    ErrorPkt *pkt = new ErrorPkt();
    pkt->setId(id);
    pkt->setChannel(channel);
    pkt->setError(loop->error);

    EV << "Subsystem::transmit() channel choice: " << channel << endl;

    sendDirect(pkt, server->gate("in"));
}

void Subsystem::finish(){
    recordScalar("errVar", loop->errVar/((double)loop->periodCount-1));
}

void Subsystem::updateDisplay(){
    char buf[30];
    sprintf(buf, "err: %.4F", loop->error);
    getDisplayString().setTagArg("t",0,buf);
    getParentModule()->getDisplayString().setTagArg("t",0,buf);
}

double Subsystem::lambdaLookupTable(int m, int n){
    switch (m) {
    case 5: {
        switch (n) {
        case 4:
            return 1.0;
        case 6:
            return 1.5;
        case 8:
            return 2.0;
        case 10:
            return 2.4;
        case 12:
            return 3.5;
        case 14:
            return 5.2;
        case 16:
            return 8.1;
        }
    }
    case 10: {
        switch (n) {
        case 4:
            return 0.6;
        case 6:
            return 0.8;
        case 8:
            return 1.0;
        case 10:
            return 1.2;
        case 12:
            return 1.4;
        case 14:
            return 1.6;
        case 16:
            return 1.8;
        }
    }
    }
    EV << "didn't find any matching value" << endl;
}

void Subsystem::processFeedback(cMessage * msg){

    ErrorPkt *pkt = check_and_cast<ErrorPkt*> (msg);
    bool collision = pkt->getCollision();
    EV << "Subsystem::handleMessage() collision: ";

    if (collision){
        collisionEvent();
    }
    else {
        successEvent();
    }
    delete pkt;

}

void Subsystem::collisionEvent(){
    loop->theta=0;
    EV << "true" << endl;
}

void Subsystem::successEvent(){
    loop->theta=1;
    EV << "false" << endl;
}

void Subsystem::processControlTimer(){
    if (adaptationExperiment){
        Server *srv = check_and_cast<Server*> (server);
        M = srv->getM();
    }

    loop->updateError();

    if (ev.isGUI())
        updateDisplay();

    if (decideOnTx()){
        transmit();
    }
}

void Subsystem::processSelfMessage(cMessage * msg){
    //nothing expected here
}


