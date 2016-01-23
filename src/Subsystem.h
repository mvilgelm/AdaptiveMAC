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

private:
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
    double sysA; //A_i
    double varW; //noise variance w_i
    double Lambda; //transmission threshold - if static
    int M; //number of channels - changing if (adaptationExperiment)
    int N; //number of subsystems
    int id; //own id in the simulation
    double controlPeriod;

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
     * Update the error status based on dynamics
     */
    void updateError();

    /**
     * Check whether transmission is to be attempted
     */
    bool decideOnTx();

    /**
     * Attempt a transmission
     */
    void transmit();

    /*
     * error state
     */
    long double error;
    int theta; //was transmission successful or not?

    /**
     * Online error variance calculation
     */
    void updateErrVar();

    /*
     * For mean and error variance tracking
     */
    double errVar;
    int periodCount;
    double errMean;

    /**
     * Just the lookup table for the adaptation experiment. Values are same as in Table 2 from the paper.
     */
    double lambdaLookupTable(int m, int n);

};

#endif /* SUBSYSTEM_H_ */
