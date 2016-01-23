/*
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
*/

#ifndef SERVER_H_
#define SERVER_H_

#include <csimplemodule.h>
#include <omnetpp.h>

class Server: public cSimpleModule {

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    /*
     * parameters
     */
    cMessage * checkQTimer;
    double controlPeriod; //length of the control period
    int M; //number of channels

    cQueue * incomings;

    /*
     * statistics
     */
    long successCount;
    long collisionCount;
    long accessCount;
    long periodCount;


    /**
     * Process the queue with incoming transmissions
     */
    void processQ();

public:
    /**
     * for M lookups (from the subsystems' side) in case of adaptation
     */
    int getM();

    double pG; //probability of the "good" channel state

    bool adaptationExperiment; //defines whether the adaptation experiment is running or not


public:
    Server();
    virtual ~Server();
};

#endif /* SERVER_H_ */
