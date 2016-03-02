/*
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
*/

#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include <csimplemodule.h>
#include <omnetpp.h>
#include "ControlLoop.h"

class Subsystem: public cSimpleModule {

protected:
    /**
     * Standard functions
     */
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    void updateDisplay();

public:
    Subsystem();
    virtual ~Subsystem();

protected:
    /*
     * timer for error updates
     */
    cMessage * controlTimer;

    /*
     * access to the server module
     */
    cModule *server;

    /*
     * parameters
     */

    ControlLoop * loop;

    int id; //own id in the simulation
    int M; //number of channels - changing if (adaptationExperiment)
    int N; //number of subsystems

    /**
     * Indicates whether the use a static or adaptive scheduler
     */
    bool adaptLambda;
    /**
     * Indicates the type of experiment running:
     * if (!adaptationExperiment) static value for M is used,
     * otherwise it is changing according to pG
     */
    bool adaptationExperiment;

    /**
     * If it is non-adaptive scheduler we need to know default M value
     * Never used if it is not adaptation experiment
     */
    int defaultM;

    /**
     * Check whether transmission is to be attempted
     */
    bool decideOnTx();

    /**
     * Attempt a transmission
     */
    virtual void transmit();

    virtual void processControlTimer();
    virtual void processSelfMessage(cMessage * msg);
    virtual void processFeedback(cMessage * msg);
    virtual void collisionEvent();
    virtual void successEvent();

    /**
     * Just the lookup table for the adaptation experiment. Values are same as in Table 2 from the paper.
     */
    double lambdaLookupTable(int m, int n);

};

#endif /* SUBSYSTEM_H_ */
