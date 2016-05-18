/*
 * ControlLoop.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: ga49zav
 */

#include <ControlLoop.h>


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

    error = (1-theta)*sysA*error + callback->normal(0, sqrt(varW));
    EV << "Err: " << error << endl;

    theta = 0;

    updateErrVar();
}


