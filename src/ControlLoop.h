/*
 * ControlLoop.h
 *
 *  Created on: Mar 2, 2016
 *      Author: ga49zav
 */

#ifndef CONTROLLOOP_H_
#define CONTROLLOOP_H_

#include <omnetpp.h>

class ControlLoop {
public:
    ControlLoop();
    // parameters
    double sysA; //A_i
    double varW; //noise variance w_i
    double Lambda; //transmission threshold - if static
    double controlPeriod;

    /*
     * error state
     */
    long double error;
    int theta; //was transmission successful or not?

    /*
     * For mean and error variance tracking
     */
    double errVar;
    int periodCount;
    double errMean;

    /**
     * Update the error status based on dynamics
     */
    void updateError();

    /**
     * Online error variance calculation
     */
    void updateErrVar();
};

#endif /* CONTROLLOOP_H_ */
