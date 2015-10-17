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
    double sysA;
    double varW;
    double Lambda;
    int M;
    int N;
    int id;
    double controlPeriod;

    bool adaptLambda;

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

    double lambdaLookupTable();

};

#endif /* SUBSYSTEM_H_ */
