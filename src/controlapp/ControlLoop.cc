/*
 * ControlLoop.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: ga49zav
 */

#include <ControlLoop.h>

//#include <>


ControlLoop::ControlLoop(){

}


void ControlLoop::updateErrVar(){
    //online variance calculation algorithm
    //storing all the values becomes a burden on the filesystem...

    periodCount++;
    double delta = error - errMean;
    errMean += delta/((double)periodCount);
    errVar += delta*(error-errMean);
}

void ControlLoop::updateError(cComponent * callback){
    EV << "Subsystem::updateError() entering function." << endl;
    try {
        error = (1-theta)*sysA*error + callback->normal(0, sqrt(varW));
    }
    catch (...) {
        throw cRuntimeError("Cannot perform the error update, overflow probable");
    }

    if ((error > 10000000) || (error < -10000000))
        throw cRuntimeError("Overflow probable: error is too large");

    EV << "Err: " << error << endl;

    theta = 0;

    updateErrVar();
}


