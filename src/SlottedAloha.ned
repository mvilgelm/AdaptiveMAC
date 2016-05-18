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
// Networked Control System closed over the multi-channel slotted ALOHA network
//
network SlottedAloha
{
    parameters:
        int N; 
        int M;
        double controlPeriod @unit(s);
    submodules:
        subSys[N]: Subsystem{
            controlPeriod = controlPeriod;
            M=M;
        }
        server: Server{
            controlPeriod = controlPeriod;
            M=M;            
        }
    connections allowunconnected:
	//wireless
}
