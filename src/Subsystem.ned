// This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
// Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems
// 
// Contact:
// Mikhail Vilgelm
// mikhail.vilgelm@tum.de
// 
// Copyright (c) Chair of Communication Networks, Technical University of Munich

package adaptivethreshold;

//
// Represents a sub-system
//
simple Subsystem
{
    parameters:
    	double sysA;
    	double varW;
    	double controlPeriod @unit(s);
    	int M;
    	int N;
    	double Lambda;
    	bool adaptLambda = default(false);
    	int defaultM = default(5);
    	bool adaptationExperiment = default(false);
    gates:
        input in @directIn;
        //output out;
}
