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

#ifndef BACKOFFSUBSYSTEM_H_
#define BACKOFFSUBSYSTEM_H_

#include <Subsystem.h>

class BackoffSubsystem: public Subsystem {
public:
    BackoffSubsystem();
    virtual ~BackoffSubsystem();

protected:
    virtual void initialize();

    /**
     * Back-off parameters
     */
    cMessage * _backOffTimer;

    double SLOT_LENGTH;
    int MIN_EXPONENT;
    int MAX_EXPONENT;

    std::string _exponentType;

    cMessage * _waitForAckTimer;

    // current exponent
    int _boExponent;


    virtual void collisionEvent();
    virtual void successEvent();
    virtual void processSelfMessage(cMessage * msg);
    virtual void processControlTimer();

    simtime_t getBackOffTime();

    /**
     * Attempt a transmission
     */
    virtual void transmit();
private:
    simsignal_t backoffSignal;

};

#endif /* BACKOFFSUBSYSTEM_H_ */
