# OpenFOAM_AAA_case_files
This repository contains the complete case configurations for the geometries analysed as part of my Master's thesis: AFN1, AAA001, AAA004, AAA013, AAA014, and AAA017. The five AAA models corresponds to geometries of patients diagnosed with abdominal aortic aneurysms, and was collected from the open dataset AAA-100 (https://zenodo.org/records/10932957).

The 0 folder contains the employed boundary conditions. The flowRateInletVelocity boundary conditions is utilized for the inlet velocity. The corresponding volumetric flow rate file is provided in the DataFiles folder. The implemented three-element Windkessel boundary condition (threeElementWindkessel) is utilized at the outlet boundaries for pressure. 

The system folder contains all simulation settings. In this file, one can find the employed discretization schemes (fvSchemes) and linear solvers (fvSolution). The mesh configurations are provided in two snappyHexMesh (SHM) files: snappyHexMeshDict and snappyHexMeshDict_layering. These utilize the mesh quality control parameters in meshQualityDict and meshQualityDict_layering, respectively. The two sets of SHM files are used in order to perform castellation and snapping in one step, and the layer addition in a final separate step. The reason for this is described in the thesis. 

The constant folder contains the definition of kinematic viscosity. This folder should also contain a triSurface folder with a corresponding STL trisurface file. It should be noted that this is an essential prerequisite for the execution of SHM. However, due to large file sizes, these folders were only possible to obtain for AAA001, AAA004, and AAA014.

The implemented boudnary condition code is included in the pimpleFoamWK3 folder. These files must be linked to the pimpleFoam solver in order to run the simulations. This procedure is detailed in the thesis. Note that the implemented threeElementWindkessel boundary condition, is largely based on code developed by M. Raza. His original implementation and documentation can be found in his project report: https://www.tfd.chalmers.se/~hani/kurser/OS_CFD_2024/MuhammadAhmadRaza/OSCFD2024_ProjectReport_MARaza.pdf

The executable files (Allrun_castellating and Allrun_addLayers) automizes the procedure required for mesh generation with snappyHexMesh in two steps: castellate and snap, and addLayers. The last executable file (Allrun_WK3) automizes the procedure required for running pimpleFoam. Note that these files must be copied into all the case folders. 
The simulations can be conducted using the following procedure:

1. Generate the mesh with snappyHexMesh. The procedure is automized by the Allrun_castellating and Allrun_addLayers executable files. Perform the following commands from terminal

   Command: ./Allrun_castellating
   Command: ./Allrun_addLayers

2. Run the simulations in parallel. The procedure is automized by the Allrun_WK3 executable files. Run the following command:

   Command: ./Allrun_WK3

Note that the number of processors must be changed in the Allrun_WK3 executable file, and in system/decomposeParDict. This can be done by utilizing the 'nano' command from terminal. 
   

