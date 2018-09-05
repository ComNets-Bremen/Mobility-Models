#ifndef __INET_CMMMEMBER_H
#define __INET_CMMMEMBER_H
#include <inet/libraries/structures.h>
#include "inet/common/INETDefs.h"
#include "inet/mobility/base/LineSegmentsMobilityBase.h"
using namespace omnetpp;
namespace inet {
    class INET_API CMMmember : public LineSegmentsMobilityBase{
      protected:
        bool nextMoveIsWait;
      protected:
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        /** @brief Initializes mobility model parameters.*/
        virtual void initialize(int stage) override;
        /** @brief Overridden from LineSegmentsMobilityBase.*/
        virtual void setTargetPosition() override;
        /** @brief Overridden from LineSegmentsMobilityBase.*/
        virtual void move() override;
        virtual Coord setNewPosition();
        int cmmide;
        sposition cmmpos;
        cModule *cmmparent;
        cModule *cmmsimulation;
        cModule *cmmsubmod;
      public:
        CMMmember();
        virtual double getMaxSpeed() const override;
    };
} // namespace inet
#endif // ifndef __INET_RANDOMWPMOBILITY_H

