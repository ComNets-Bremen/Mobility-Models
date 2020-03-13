package inet.mobility.SOLAR;
import inet.mobility.base.MovingMobilityBase;
simple SOLARmember extends MovingMobilityBase{
    parameters:
        double initialX @unit(m) = default(uniform(this.constraintAreaMinX, this.constraintAreaMaxX));
        double initialY @unit(m) = default(uniform(this.constraintAreaMinY, this.constraintAreaMaxY));
        double initialZ @unit(m) = default(nanToZero(uniform(this.constraintAreaMinZ, this.constraintAreaMaxZ)));
        bool initFromDisplayString = default(true);
        volatile double interspeed @unit(mps) = default(mps); // use uniform(minSpeed, maxSpeed) or another distribution
        volatile double intraspeed @unit(mps) = default(mps); // use uniform(minSpeed, maxSpeed) or another distribution
        volatile double waitTime @unit(s) = default(0s); // wait time between reaching a target and choosing a new one
        double tupdate = default(0); //Update time
        int usedRNG = default(0);
        @class(SOLARmember);
        @display("i=abstract/person;is=s");
}
