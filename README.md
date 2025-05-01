# OpenFOAM_AAA_case_files
This repository containes the complete case configurations of the geometries analysed as part of my Master's thesis: AFN1, AAA001, AAA004, AAA013, AAA014, and AAA017. 
The 0 folder contains the boundary conditions employed, including the implemented three Element Windkessel boundary condition (threeElementWindkessel).
The system folder contains files that together constitutes the complete solver settings and mesh configurations. This includes the blockMeshDict and snappyHexMeshDict files that are required in order to generate the mesh. 
In the constant folder, the kinematic viscosity is defined (transportProperties). Note that this folder should also contain the trisurface STL file that is required for the execution of snappyHexMesh. However, these files are excluded due to large file sizes. A prerequisite for the execution of snappyHexMesh is therefore to create a triSurface folder within the constant folder, and subsequently include the STL files here. 

The implemented boudnary condition code is included in the pimpleFoamWK3 folder. These files must be linked to the pimpleFoam solver in order to run the simulations. This procedure is detailed in the thesis. Note that the implemented threeElementWindkessel boundary condition, is largely based on code developed by M. Raza. His original implementation and documentation can be found in his project report: https://www.tfd.chalmers.se/~hani/kurser/OS_CFD_2024/MuhammadAhmadRaza/OSCFD2024_ProjectReport_MARaza.pdf

The executable files (Allrun_castellating and Allrun_addLayers) automizes the procedure required for mesh generation with snappyHexMesh in two steps: castellate and snap, and addLayers. The last executable file (Allrun_WK3) automizes the procedure required for running pimpleFoam. Note that these files must be copied into all the case folders. After the STL files are provided in the triSurface folder, and the threeElementWindkessel is linked to the solver, the simulations can be conducted using teh following procedure:

1. Generate the mesh with snappyHexMesh. The procedure is automized by the Allrun_castellating and Allrun_addLayers executable files. Perform the following commands from terminal

   Command: ./Allrun_castellating
   Command: ./Allrun_addLayers

2. Run the simulations in parallel. The procedure is automized by the Allrun_WK3 executable files. Run the following command:

   Command: ./Allrun_WK3

Note that the number of processors must be changed in the Allrun_WK3 executable file, and in system/decomposeParDict. This can be done by utilizing the 'nano' command from terminal. 
   

