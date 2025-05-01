# OpenFOAM_AAA_case_files
This repository contains information on boundary conditions, system settings, transport properties, and mesh configurations used in the models analyzed as part of my Master's thesis. It includes nearly all components necessary to reproduce the simulations. However, due to the large file size, the STL surfaces used in the snappyHexMesh process are not included in the constant/triSurface directory.

The implemented boundary condition code is included in the pimpleFoamWK3 folder. In order to link these to the pimpleFoam solver, one can follow the procedure outlined in the thesis. Note that the implemented threeElementWindkessel boundary condition, is largely based on code developed by M. Raza. His original implementation and documentation can be found in his project report: https://www.tfd.chalmers.se/~hani/kurser/OS_CFD_2024/MuhammadAhmadRaza/OSCFD2024_ProjectReport_MARaza.pdf

In order to reproduce the simulations, one must first create a triSurface folder in the constant dictionary. The STL files for each respective geoemtry must thereafter be included in constant/triSurface. When this is done, the following procedure can be used to run the simulations: 

1. Generate the mesh with snappyHexMesh. The procedure is automized by the Allrun_castellating and Allrun_addLayers executable files. Perform the following commands from terminal

   Command: ./Allrun_castellating
   Command: ./Allrun_addLayers

2. Run the simulations in parallel. The procedure is automized by the Allrun_WK3 executable files. Run teh following command:

   Command: ./Allrun_WK3

This step requires that the threeElementWindkessel boundary condition is linked to the pimpleFoam solver. The executable files must be adjusted according to the number of processors utilized for the simulations. 
   

