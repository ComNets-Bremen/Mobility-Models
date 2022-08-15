#ifndef __INET_SOLARMEMBER_H
#define __INET_SOLARMEMBER_H
#include <inet/libraries/structures.h>
#include "inet/common/INETDefs.h"
#include "inet/mobility/base/LineSegmentsMobilityBase.h"
using namespace omnetpp;
namespace inet {
    class INET_API SOLARmember : public LineSegmentsMobilityBase{
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
        virtual sposition setNewGroup();
        //virtual void handleMessage(cMessage *msg);
        int solaride;
        sposition solarpos;
        cModule *solarparent;
        cModule *solarsimulation;
        cModule *solarsubmod;
        double solartupdate;
        simtime_t targettime;
        int usedRNG;
      public:
        SOLARmember();
        virtual double getMaxSpeed() const override;
    };
} // namespace inet
#endif // ifndef __INET_RANDOMWPMOBILITY_H

