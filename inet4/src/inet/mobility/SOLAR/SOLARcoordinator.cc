#include "SOLARcoordinator.h"
namespace inet {
    Define_Module(SOLARcoordinator);
    MatrixXd SOLARcoordinator::SOLARplacePositions(senviroment env, int numplaces){
        MatrixXd position(numplaces,2);
        if (isPositionRandom){
            int numrow=std::trunc(env.xmax/solarhrange);
            int numcol=std::trunc(env.ymax/solarhrange);
            VectorXd Xp(numrow*numcol);
            VectorXd Yp(numrow*numcol);
            for (int i=0;i<numrow;i++){
                Xp.segment(i*numcol,numcol)=VectorXd::LinSpaced(numcol,0,(numcol-1)*solarhrange);
                Yp.segment(i*numcol,numcol)=VectorXd::Constant(numcol,i*solarhrange);
            }
            for (int i=0;i<numplaces;i++){
                int randindex=uniform(0,numrow*numcol-i-2);
                position(i,0)=Xp(randindex);
                Xp(randindex)=Xp(numrow*numcol-i-1);
                position(i,1)=Yp(randindex);
                Yp(randindex)=Yp(numrow*numcol-i-1);
            }
        }else{
            std::string fileType (".csv");
            std::string path (positionsAddress);
            std::string file = std::to_string(currentMatrix);
            std::string newAddress=path+file+fileType;
            position=readMatrix(newAddress.c_str()).transpose()-MatrixXd::Constant(numplaces,2,solarhrange/2);
            currentMatrix=(currentMatrix+1)%numMatrix;
        }
        EV << "position" << endl;
        EV << position << endl;
        return position;
    }
    VectorXd SOLARcoordinator::SOLARrandomSort(VectorXd vector, int size){
        for (int i=0;i<size-3;i++){
            int randindex=uniform(0,size-2-i);
            double aux=vector(randindex);
            vector(randindex)=vector(size-1-i);
            vector(size-1-i)=aux;
        }
        return vector;
    }
    smatrix SOLARcoordinator::SOLARrandInterMatrix(int numhost, int numplaces){
        MatrixXd matrix(numplaces,numhost);
        MatrixXd result(numplaces,numhost);
        smatrix interm;
        if (isProbabilityRandom){
            MatrixXd mask(numplaces,numhost);
            VectorXd sumvec(numhost);
            VectorXd nosortedplaces(numplaces);
            VectorXd sortedplaces(numplaces);
            nosortedplaces=VectorXd::LinSpaced(numplaces,0,numplaces-1);
            matrix=MatrixXd::Zero(numplaces,numhost);
            for (int i=0;i<numhost;i++){
                sortedplaces=SOLARrandomSort(nosortedplaces,numplaces);
                int placesxhost=uniform(1,numplaces);
                for (int j=0;j<placesxhost;j++){
                    int randomplace=sortedplaces(j);
                    matrix(randomplace,i)=uniform(0,1);
                }
            }
            sumvec=matrix.colwise().sum();
            mask.rowwise()=sumvec.cwiseInverse().transpose();
            matrix=matrix.cwiseProduct(mask);
        }else{
            std::string fileType (".csv");
            std::string path (probabilitiesAddress);
            std::string file = std::to_string(currentMatrix);
            std::string newAddress=path+file+fileType;
            matrix=readMatrix(newAddress.c_str());
            currentMatrix=(currentMatrix+1)%numMatrix;
        }
        result.row(0)=matrix.row(0);
        for(int i=1;i<numplaces;i++){
            result.row(i)=result.row(i-1)+matrix.row(i);
        }
        interm.nrow=numplaces;
        interm.ncol=numhost;
        interm.content=result;
        return interm;
    }
    int SOLARcoordinator::SOLARgroupUpdNode(smatrix interm, int host){
        int groupxhost;
        int numplaces=interm.nrow;
        VectorXd vector(numplaces);
        VectorXd mask(numplaces);
        double rscalar;
        rscalar=uniform(0,1);
        mask=VectorXd::Constant(numplaces,rscalar);
        vector=interm.content.col(host)-mask;
        vector=(vector.array()<=0).select(VectorXd::Constant(numplaces,1.),VectorXd::Constant(numplaces,0.));
        groupxhost=vector.sum();
        return groupxhost;
    }
    VectorXd SOLARcoordinator::SOLARpositionxhost(MatrixXd position,int groupxhost, double range){
        VectorXd positionxhost(2);
            positionxhost(0)=position(groupxhost,0);
            positionxhost(1)=position(groupxhost,1);
        return positionxhost;
    }
    sconf SOLARcoordinator::SOLARconfiguration(int numhost,int numplaces,senviroment enviroment){
        sconf solarconf;
        solarconf.interm=SOLARrandInterMatrix(numhost,numplaces);
        solarconf.pposition=SOLARplacePositions(enviroment,numplaces);
        return solarconf;
    }
    void SOLARcoordinator::initialize(){
        solarnumhost=par("numhost");
        solarnumplaces=par("numplaces");
        solarhrange=par("hrange");
        solartschedule=par("tschedule");
        solarenviroment.xmax=par("xmax");
        solarenviroment.ymax=par("ymax");
        isPositionRandom=par("isPositionRandom");
        isProbabilityRandom=par("isProbabilityRandom");
        if (!isPositionRandom || !isProbabilityRandom){
            numMatrix=par("numMatrix");
            currentMatrix=0;
            if (!isPositionRandom){
                positionsAddress=par("positionsAddress");
            }
            if (!isProbabilityRandom){
                probabilitiesAddress=par("probabilitiesAddress");
            }
        }
        solarconf=SOLARconfiguration(solarnumhost,solarnumplaces,solarenviroment);
        solargroup=solarconf.group;
        cMessage *schedule =new cMessage("Schedule");
        schedule->setKind(SCHEDULE);
        scheduleAt(simTime()+solartschedule,schedule);
    }
    void SOLARcoordinator::handleMessage(cMessage *msg){
        if (msg->isSelfMessage()){
            switch (msg->getKind()){
                case SCHEDULE:{
                    solarconf=SOLARconfiguration(solarnumhost,solarnumplaces,solarenviroment);
                    solargroup=solarconf.group;
                    cMessage *schedule =new cMessage("Schedule");
                    schedule->setKind(SCHEDULE);
                    scheduleAt(simTime()+solartschedule,schedule);
                }break;
            }
        }
        delete msg;
    }
    sposition SOLARcoordinator::SOLARgetPosition(int index){
        sposition pos;
        int groupxhost;
        VectorXd positionxhost(2);
        groupxhost=SOLARgroupUpdNode(solarconf.interm,index);
        positionxhost=SOLARpositionxhost(solarconf.pposition,groupxhost,solarhrange);
        pos.x=positionxhost(0);
        pos.y=positionxhost(1);
        return pos;
    }
    double SOLARcoordinator::SOLARgetRange(){
        return solarhrange;
    }
} //namespace
