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

#include <BackoffSubsystem.h>

#include "ErrorPkt_m.h"

BackoffSubsystem::BackoffSubsystem() {
    // TODO Auto-generated constructor stub

    backOffTimer = new cMessage("backOffTimer");

}

BackoffSubsystem::~BackoffSubsystem() {
    // TODO Auto-generated destructor stub
}

void BackoffSubsystem::processSelfMessage(cMessage * msg){
    if (msg == backOffTimer){
        //resent data
        transmit();
    }

}

void BackoffSubsystem::collisionEvent(){
    // keep theta 0
    loop->theta = 0;
    scheduleAt(simTime()+getBackOffTime(), backOffTimer);

}

simtime_t BackoffSubsystem::getBackOffTime(){
    return 0;
}

void BackoffSubsystem::transmit(){
    //EV << "Subsystem::transmit() entering function." << endl;

    int channel = intuniform(1,M);

    ErrorPkt *pkt = new ErrorPkt();
    pkt->setId(id);
    pkt->setChannel(channel);
    pkt->setError(loop->error);

    EV << "Subsystem::transmit() channel choice: " << channel << endl;

    sendDirect(pkt, server->gate("in"));
}
