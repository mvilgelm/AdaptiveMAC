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
// Server represent a controller entity (in an emulative fashion)
//

simple Server
{
    parameters:
        double controlPeriod @unit(s);
        bool adaptationExperiment = default(false);
        double pG = default(0.0);
        int M;
    gates:
        input in @directIn;
        //output out;
}
