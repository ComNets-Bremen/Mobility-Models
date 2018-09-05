#ifndef INET_MOBILITY_BASE_STRUCTURES_H_
#define INET_MOBILITY_BASE_STRUCTURES_H_
#include <inet/libraries/Eigen/Dense>
#include <iostream>
#include <fstream>
#include <string>

using namespace Eigen;
using namespace std;

#define SCHEDULE    0
#define UPDATE      1
#define MAXBUFSIZE  ((int) 1e6)

typedef struct sgroup {
    int size;
    int numgroup;
    VectorXd content;
} sgroup;
typedef struct smatrix {
    int nrow;
    int ncol;
    MatrixXd content;
} smatrix;
typedef struct sconf {
    smatrix interm;
    sgroup group;
    MatrixXd pposition;

} sconf;
typedef struct senviroment {
    double xmax;
    double ymax;
} senviroment;
typedef struct sproc {
    sgroup group;
    MatrixXd hposition;
} sproc;
typedef struct sposition {
    double x;
    double y;
} sposition;

namespace structures {
    class Structures {
    public:
        Structures();
        virtual ~Structures();
        MatrixXd readMatrix(const char *filename);
    };
}
#endif

