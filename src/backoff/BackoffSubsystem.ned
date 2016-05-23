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

package contentionaccess.backoff;


simple BackoffSubsystem {
    parameters:
        @signal[backoff](type="double");
        @statistic[choosenBackoff](title="choosen backoff"; source="backoff"; record=vector; intepolationmode=none);
        
        double slotLength @unit(s);
        double pktDuration;  // in slots
        double ackDuration;  // in slots
        
        int minExponent;
        int maxExponent;
        
        string exponentType;
        
        // from subsystem
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