//
//This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
//Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems
//
//Contact:
//Mikhail Vilgelm
//mikhail.vilgelm@tum.de
//
//Copyright (c) Chair of Communication Networks, Technical University of Munich
//

packet ErrorPkt {
    int id;
    int channel;
    bool collision;
    double error;
}
