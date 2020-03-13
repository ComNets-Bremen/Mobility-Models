#include "SOLARmember.h"

#include "SOLARcoordinator.h"
namespace inet {
    Define_Module(SOLARmember);
    int solaride;
    sposition solarpos;
    cModule *solarparent;
    cModule *solarsimulation;
    cModule *solarsubmod;
    double solartupdate;
    simtime_t targettime;
    SOLARmember::SOLARmember() {
        nextMoveIsWait = false;
    }
    void SOLARmember::initialize(int stage) {
        solartupdate=par("tupdate");
        usedRNG = par("usedRNG");
        LineSegmentsMobilityBase::initialize(stage);
        if (stage == INITSTAGE_LOCAL) {
            stationary = (par("intraspeed").getType() == 'L' || par("intraspeed").getType() == 'D') && (double)par("intraspeed") == 0;
        }
        if (stage==1){
            solarparent = getParentModule();
            solaride = solarparent->getIndex();
            solarsimulation = solarparent->getModuleByPath("^");
            solarsubmod = solarsimulation->getSubmodule("SOLARcoordinator");
            targettime=simTime()+solartupdate;
            solarpos=setNewGroup();
        }
    }
    Coord SOLARmember::setNewPosition(){
        double range;
        SOLARcoordinator *target = check_and_cast<SOLARcoordinator *>(solarsubmod);
        range = target->SOLARgetRange();
        Coord theTarget=targetPosition;
        theTarget.x = solarpos.x+uniform(0,range, usedRNG);
        theTarget.y = solarpos.y+uniform(0,range, usedRNG);
        return theTarget;
    }
    sposition SOLARmember::setNewGroup(){
        SOLARcoordinator *target = check_and_cast<SOLARcoordinator *>(solarsubmod);
        sposition pos = target->SOLARgetPosition(solaride);
        return pos;
    }
    void SOLARmember::setTargetPosition(){
        double speed;
        if (nextMoveIsWait) {
            simtime_t waitTime = par("waitTime");
            nextChange = simTime() + waitTime;
        }
        else {
            if (simTime()>targettime){
                solartupdate=par("tupdate");
                targettime=simTime()+solartupdate;
                solarpos=setNewGroup();
                speed = par("interspeed");
            }else{
                speed = par("intraspeed");
            }
            targetPosition = setNewPosition();
            double distance = lastPosition.distance(targetPosition);
            simtime_t travelTime = distance / speed;
            nextChange = simTime() + travelTime;
        }
        nextMoveIsWait = !nextMoveIsWait;
    }
    void SOLARmember::move(){
        LineSegmentsMobilityBase::move();
        raiseErrorIfOutside();
    }
    double SOLARmember::getMaxSpeed() const{
        return NaN;
    }
} // namespace inet

