#ifndef __INET_SOLARCOORDINATOR_H_
#define __INET_SOLARCOORDINATOR_H_
#include <omnetpp.h>
#include <string.h>
#include <omnetpp.h>
#include <algorithm>
#include <queue>
#include <inet/libraries/structures.h>
using namespace omnetpp;
namespace inet {
    class SOLARcoordinator : public cSimpleModule,structures::Structures{
        protected:
            virtual MatrixXd SOLARplacePositions(senviroment env, int numplaces);
            virtual VectorXd SOLARrandomSort(VectorXd vector, int size);
            virtual smatrix SOLARrandInterMatrix(int numhost, int numplaces);
            virtual int SOLARgroupUpdNode(smatrix interm, int host);
            virtual VectorXd SOLARpositionxhost(MatrixXd position,int groupxhost, double range);
            virtual sconf SOLARconfiguration(int numhost,int numplaces,senviroment enviroment);
            virtual void initialize();
            virtual void handleMessage(cMessage *msg);
            sconf solarconf;
            sproc solarproc;
            sgroup solargroup;
            senviroment solarenviroment;
            double solarhrange;
            double solartschedule;
            int solarnumhost;
            int solarnumplaces;
            const char* positionsAddress;
            const char* probabilitiesAddress;
            int numMatrix;
            bool isPositionRandom;
            bool isProbabilityRandom;
            int currentMatrix;
        public:
            sposition SOLARgetPosition(int index);
            double SOLARgetRange();
    };
} //namespace
#endif
