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

#ifndef BASECLIENT_H_
#define BASECLIENT_H_


#include "Subsystem.h"

class BaseClient: public Subsystem {
public:
    BaseClient();
    virtual ~BaseClient();

protected:
    /**
     * Standard functions
     */
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);


protected:
    /**
     * Attempt a transmission
     */
    //virtual void transmit();

    virtual void processControlTimer();
    virtual void processSelfMessage(cMessage * msg);
    //virtual void processFeedback(cMessage * msg);
    //virtual void collisionEvent();
    //virtual void successEvent();

    /**
     * to enable custom arrivals
     */
    virtual double getNextArrivalTime();
};

#endif /* BASECLIENT_H_ */
