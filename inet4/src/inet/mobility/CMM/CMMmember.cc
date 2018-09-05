#include "CMMmember.h"
#include "CMMcoordinator.h"
namespace inet {
    Define_Module(CMMmember);
    int cmmide;
    sposition cmmpos;
    cModule *cmmparent;
    cModule *cmmsimulation;
    cModule *cmmsubmod;
    CMMmember::CMMmember() {
        nextMoveIsWait = false;
    }
    void CMMmember::initialize(int stage) {
        LineSegmentsMobilityBase::initialize(stage);
        if (stage == INITSTAGE_LOCAL) {
            stationary = (par("speed").getType() == 'L' || par("speed").getType() == 'D') && (double)par("speed") == 0;
        }
        if (stage==1){
            cmmparent = getParentModule();
            cmmide = cmmparent->getIndex();
            cmmsimulation = cmmparent->getModuleByPath("^");
            cmmsubmod = cmmsimulation->getSubmodule("CMMcoordinator");
        }
    }
    Coord CMMmember::setNewPosition(){
        CMMcoordinator *target = check_and_cast<CMMcoordinator *>(cmmsubmod);
        cmmpos = target->CMMgetPosition(cmmide);
        Coord theTarget=targetPosition;
        theTarget.x = cmmpos.x;
        theTarget.y = cmmpos.y;
        return theTarget;
    }
    void CMMmember::setTargetPosition(){
        if (nextMoveIsWait) {
            simtime_t waitTime = par("waitTime");
            nextChange = simTime() + waitTime;
        }
        else {
            targetPosition = setNewPosition();
            double speed = par("speed");
            double distance = lastPosition.distance(targetPosition);
            simtime_t travelTime = distance / speed;
            nextChange = simTime() + travelTime;
        }
        nextMoveIsWait = !nextMoveIsWait;
    }
    void CMMmember::move(){
        LineSegmentsMobilityBase::move();
        raiseErrorIfOutside();
    }
    double CMMmember::getMaxSpeed() const{
        return NaN;
    }
} // namespace inet

