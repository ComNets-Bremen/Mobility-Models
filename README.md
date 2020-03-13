# Mobility-Models

The [ORBIT Mobilty Model (SOLAR)](https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=1589669) and the 
[Comunity-based Mobility Model (CMM)](https://www.cl.cam.ac.uk/~cm542/papers/realman06.pdf) are synthetic mobility models 
to simulate mobility of nodes in ad-hoc networks. The code in this repository is an implementation of these
models for the [INET Framework](https://inet.omnetpp.org) of the [OMNeT++](https://omnetpp.org) simulator.

Here are the most important links to these models working.

1. Requirements - [Prerequisites](#prerequisites)

2. Install OMNeT++, INET Framework and the mobility models (SOLAR and CMM) - [Install and Build](#install-and-build)

3. Run models - [Running Models](#running-models)




## Prerequisites

These models have been tested on the following versions of INET and OMNeT++.

- OMNeT Version 5.5.1
- INET Framework version 4.1.1


## Install and Build

Follow the following procedure to install and build the models.

1. Install and build [OMNeT++](https://omnetpp.org)

2. Create a new workspace in the OMNeT++ IDE

3. Install and build the [INET Framework](https://inet.omnetpp.org) in the created workspace

4. Copy all the files of the two subfolders (`examples` and `src`) in to the freshly installed INET, maintaining 
the same folder hierarchy 

5. Rebuild INET



## Running Models

1. Use the omnetpp.ini files provided in the examples for SOLAR and CMM to run the mobility models

Here are some important information.

- To run the mobility models with random configurations, set the `isRandom` (for CMM), `isProbabilityRandom` 
(for SOLAR) and `isPositionRandom` (for SOLAR) variables to true in the `omnetpp.ini`

- To run the mobility models with pre-calculated matrices, set `address` (for CMM), `probabilityAddress` (for SOLAR)
and `positionAddress` (for SOLAR) to the locations of the CSV files of the matrices. Examples are given in the 
`cmm_parameters` and `solar_parameters` folders. The order of the file names are the selection (execution) order.



## Acknowledgements

This code uses a lightweight C++ template library called Eigen for linear algebra computations. This library, 
which is included with this code, is released by the original authors of the code under the Mozilla Public 
License ver 2.0.



## Licenses

The developed mobility models are released under GPL ver 3.0 and the Eigen library is released under the Mozilla 
Public License ver 2.0 by its authors.


## Questions or Comments

If you have any questions, comments or suggestions, please write to anyone of us listed blow using 
mobility-models@comnets.uni-bremen.de.

  - Vishnupriya Parimalam
  - Anna Förster
  - Asanga Udugama


## Model Developers

The implementation of these mobility models were designed and developed by,

  - Leonardo Sarmiento
  - Vishnupriya Parimalam


