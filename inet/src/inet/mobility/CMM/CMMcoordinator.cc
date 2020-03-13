#include "CMMcoordinator.h"
namespace inet {
    Define_Module(CMMcoordinator);
    MatrixXd CMMcoordinator::CMMhostPosition(MatrixXd position,sgroup group, double range){
        int numhost=group.size;
        int numplaces=group.numgroup;
        MatrixXd groupposition(numhost,2);
        for(int i=0;i<numplaces;i++){
            groupposition.col(0)=(group.content.array()==i).select(VectorXd::Constant(numhost,position(i,0)),groupposition.col(0));
            groupposition.col(1)=(group.content.array()==i).select(VectorXd::Constant(numhost,position(i,1)),groupposition.col(1));
        }
        return groupposition;
    }
    MatrixXd CMMcoordinator::CMMplacePositions(senviroment env, int numplaces){
        int numrow=std::trunc(env.xmax/cmmhrange);
        int numcol=std::trunc(env.ymax/cmmhrange);
        VectorXd Xp(numrow*numcol);
        VectorXd Yp(numrow*numcol);
        for (int i=0;i<numrow;i++){
            Xp.segment(i*numcol,numcol)=VectorXd::LinSpaced(numcol,0,(numcol-1)*cmmhrange);
            Yp.segment(i*numcol,numcol)=VectorXd::Constant(numcol,i*cmmhrange);
        }
        MatrixXd position(numplaces,2);
        for (int i=0;i<numplaces;i++){
            int randindex=uniform(0,numrow*numcol-i-2, usedRNG);
            position(i,0)=Xp(randindex);
            Xp(randindex)=Xp(numrow*numcol-i-1);
            position(i,1)=Yp(randindex);
            Yp(randindex)=Yp(numrow*numcol-i-1);
        }
        EV << "position" << endl;
        EV << position << endl;
        return position;
    }
    smatrix CMMcoordinator::CMMrandInterMatrix(int numhost){
        smatrix interm;
        MatrixXd result(numhost,numhost);
        if (isRandom){
            MatrixXd mask(numhost,numhost);
            MatrixXd matrix(numhost,numhost);
            for (int i=0;i<numhost;i++){
                for (int j=0;j<numhost;j++){
                    matrix(i,j)=uniform(0,1, usedRNG);
                }
            }
            mask=MatrixXd::Constant(numhost,numhost,1.)-(MatrixXd)VectorXd::Constant(numhost,1.).asDiagonal();
            result=MatrixXd::Constant(numhost,numhost,1.)-((matrix + matrix.transpose())*0.5).cwiseProduct(mask);
        }else{
            std::string fileType (".csv");
            std::string path (address);
            std::string file = std::to_string(currentMatrix);
            std::string newAddress=path+file+fileType;
            result=readMatrix(newAddress.c_str());
            currentMatrix=(currentMatrix+1)%numMatrix;
        }

        interm.nrow=numhost;
        interm.ncol=numhost;
        interm.content=result;
        return interm;
    }
    sgroup CMMcoordinator::CMMgroupIniVector(smatrix interm,double threshold){
        sgroup group;
        MatrixXf::Index maxIndex;
        int numhost=interm.nrow;
        VectorXd vec(numhost);
        MatrixXd hmask(numhost,numhost);
        MatrixXd mask(numhost,numhost);
        MatrixXd matrix=interm.content;
        float maxNorm;
        group.numgroup=0;
        group.content=VectorXd::Zero(numhost);
        maxNorm = matrix.colwise().sum().maxCoeff(&maxIndex);
        while (maxNorm != 0){
            vec=matrix.col(maxIndex);
            vec=(vec.array()>=threshold).select(VectorXd::Constant(numhost,0.),VectorXd::Constant(numhost,1.));
            hmask.colwise()=vec;
            mask=hmask.cwiseProduct(hmask.transpose());
            matrix=matrix.cwiseProduct(mask);
            group.content+=(VectorXd::Constant(numhost,1.)-vec)*group.numgroup;
            group.numgroup++;
            maxNorm = matrix.colwise().sum().maxCoeff(&maxIndex);
        }
        group.size=numhost;
        return group;
    }
    smatrix CMMcoordinator::CMMgroupParameters(sgroup group, smatrix interm){
        int numgroup=group.numgroup;
        int numhost=group.size;
        smatrix parameters;
        VectorXd ngroup(numgroup);  //Number of hosts in each group
        MatrixXd mgroup(numgroup,numhost); //Sum of host interactions with other hosts that belongs to each group devided by the number of hosts in each group
        VectorXd vgroup(numhost);
        MatrixXd mask(numhost,numhost);
        MatrixXd matrix=interm.content;
        for (int i=0;i<numgroup;i++){
            vgroup=(group.content.array()==i).select(VectorXd::Constant(numhost,1.),VectorXd::Constant(numhost,0.));
            mask.colwise()=vgroup;
            matrix=interm.content.cwiseProduct(mask);
            ngroup(i)=vgroup.sum();
            if (ngroup(i)==0){
                ngroup(i)=1;
            }
            mgroup.row(i)=1/ngroup(i)*matrix.colwise().sum();
        }
        parameters.content=mgroup;
        parameters.nrow=numgroup;
        parameters.ncol=numhost;
        return parameters;
    }
    sgroup CMMcoordinator::CMMgroupUpdVector(smatrix parameters){
        MatrixXf::Index maxIndex;
        int numhost=parameters.ncol;
        int numgroup=parameters.nrow;
        sgroup group;
        VectorXd vec(numhost);
        for (int i=0;i<parameters.ncol;i++){
            parameters.content.col(i).maxCoeff(&maxIndex);
            vec(i)=maxIndex;
        }
        group.size=numhost;
        group.numgroup=numgroup;
        group.content=vec;
        return group;
    }
    sconf CMMcoordinator::CMMconfiguration(int numhost,double threshold,senviroment enviroment){
        sconf cmmconf;
        cmmconf.interm=CMMrandInterMatrix(numhost);
        cmmconf.group=CMMgroupIniVector(cmmconf.interm,threshold);
        cmmconf.pposition=CMMplacePositions(enviroment,cmmconf.group.numgroup);
        return cmmconf;
    }
    sproc CMMcoordinator::CMMprocess(sconf cmmconf, sgroup group,double hrange){
        sproc proc;
        smatrix parameters;
        parameters=CMMgroupParameters(group,cmmconf.interm);
        proc.group=CMMgroupUpdVector(parameters);
        proc.hposition=CMMhostPosition(cmmconf.pposition,group,hrange);
        return proc;
    }
    void CMMcoordinator::initialize(){
        cmmnumhost=par("numhost");
        cmmthreshold=par("threshold");
        cmmhrange=par("hrange");
        cmmtupdate=par("tupdate");
        cmmtschedule=par("tschedule");
        cmmenviroment.xmax=par("xmax");
        cmmenviroment.ymax=par("ymax");
        isRandom=par("isRandom");
        if (!isRandom){
            numMatrix=par("numMatrix");
            address=par("address");
            currentMatrix=0;
        }
        usedRNG = par("usedRNG");
        cmmconf=CMMconfiguration(cmmnumhost,cmmthreshold,cmmenviroment);
        cmmgroup=cmmconf.group;
        cMessage *schedule =new cMessage("Schedule");
        schedule->setKind(SCHEDULE);
        scheduleAt(simTime()+cmmtschedule,schedule);
        cmmproc=CMMprocess(cmmconf,cmmgroup,cmmhrange);
        cmmgroup=cmmproc.group;
    }
    void CMMcoordinator::handleMessage(cMessage *msg){
        if (msg->isSelfMessage()){
            switch (msg->getKind()){
                case SCHEDULE:{
                    cmmconf=CMMconfiguration(cmmnumhost,cmmthreshold,cmmenviroment);
                    cmmgroup=cmmconf.group;
                    cMessage *schedule =new cMessage("Schedule");
                    schedule->setKind(SCHEDULE);
                    scheduleAt(simTime()+cmmtschedule,schedule);
                }break;
            }
        }
        delete msg;
    }
    sposition CMMcoordinator::CMMgetPosition(int index){
        sposition pos;
        cmmproc=CMMprocess(cmmconf,cmmgroup,cmmhrange);
        cmmgroup=cmmproc.group;
        pos.x=cmmproc.hposition(index,0)+uniform(0, cmmhrange, usedRNG);
        pos.y=cmmproc.hposition(index,1)+uniform(0, cmmhrange, usedRNG);
        return pos;
    }
} //namespace
