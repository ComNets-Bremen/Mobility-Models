#ifndef __INET_CMMCOORDINATOR_H_
#define __INET_CMMCOORDINATOR_H_
#include <omnetpp.h>
#include <string.h>
#include <omnetpp.h>
#include <algorithm>
#include <queue>
#include <inet/libraries/structures.h>
using namespace omnetpp;
namespace inet {
    class CMMcoordinator : public cSimpleModule, structures::Structures{
        protected:
            virtual MatrixXd CMMhostPosition(MatrixXd position,sgroup group, double range);
            virtual MatrixXd CMMplacePositions(senviroment env, int numplaces); //asignComunitiesToGrids
            virtual smatrix CMMrandInterMatrix(int numhost); //setInteractionMatrx
            virtual sgroup CMMgroupIniVector(smatrix interm,double threshold); //createComunities
            virtual smatrix CMMgroupParameters(sgroup group, smatrix interm); //calculateSocialAtractivity
            virtual sgroup CMMgroupUpdVector(smatrix parameters);
            virtual sconf CMMconfiguration(int numhost,double threshold,senviroment enviroment);
            virtual sproc CMMprocess(sconf cmmconf, sgroup group,double hrange);
            virtual void initialize();
            virtual void handleMessage(cMessage *msg);
            sconf cmmconf;
            sproc cmmproc;
            sgroup cmmgroup;
            senviroment cmmenviroment;
            double cmmhrange;
            double cmmtupdate;
            double cmmtschedule;
            double cmmthreshold;
            int cmmnumhost;
            const char *address;
            int numMatrix;
            bool isRandom;
            int currentMatrix;
        public:
            sposition CMMgetPosition(int index);
    };
} //namespace
#endif
