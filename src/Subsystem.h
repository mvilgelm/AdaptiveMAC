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
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

public:
    Subsystem();
    virtual ~Subsystem();

private:
    cMessage * controlTimer;
    cModule *server;
    double sysA;
    double varW;
    double Lambda;
    int M;
    int id;
    double controlPeriod;

    void updateError();
    bool decideOnTx();
    void transmit();

    //error
    double error;
    int theta;


};

#endif /* SUBSYSTEM_H_ */
