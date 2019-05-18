Tee Junction
------------

This tutorial introduces the steady, laminar flow through a two-dimensional :math:`90^\circ` tee-junction. Here, we will be using Caelus |version| and some of the basic requirements to begin a Caelus simulation will be shown. These include, specifying input data defining boundary conditions, fluid properties and discretization/solver settings. At the end, visualisation is carried out to look at the pressure and velocity contours within the tee-junction. The details in running a Caelus simulation for the tee-junction will be shown in sufficient detail so that the user is able to repeat the numerical experiment.

Objectives
````````````
Through the completion of this tutorial, the user will be able to set-up the Caelus simulation for laminar, incompressible flow through a two-dimensional junction and subsequently estimate the flow-split. Following are the steps that will be carried out in this tutorial

* Background
	- A brief description about the problem
	- Geometry and flow conditions

* Grid generation
	- Computational domain and boundary details
	- Computational grid generation
	- Exporting grid to Caelus

* Problem definition
	- Directory structure
	- Setting up boundary conditions, physical properties and control/solver attributes

* Execution of the solver
	- Monitoring the convergence
	- Writing the log files
	
* Results
	- Visualising the flow inside the tee-junction
	
Pre-requisites 
````````````````
It is assumed that the user is familiar with the Linux command line environment using a terminal or Caelus-console (for Windows OS) and that Caelus is installed correctly with appropriate environment variables set. The grid used here is generated using `Pointwise <http://www.pointwise.com/>`_ and the user is free to use their choice of grid generation tool having exporting capabilities to the Caelus grid format.

Background
````````````
The flow in a tee-junction presents with a simple introduction in carrying out separated flow simulation in Caelus. Because of the presence of a side branch, the flow separates forming a recirculating region. This in turn affects the mass flow through main and side branches altering the flow splits. For more details, the user can refer to the validation example show in :ref:`tee-junction-validation-label`.

A schematic of the tee-junction geometry is shown in :numref:`tj-schematic-tutorials`. Here, :math:`L = 3.0~m` and :math:`W = 1.0~m` with a Reynolds number of :math:`Re_w = 300` based on the side branch width. The velocity (:math:`V`) is assumed to be :math:`1~m/s` in the :math:`y` direction for simplicity. With these flow parameters, the kinematic viscosity can be evaluated to :math:`\nu = 0.00333~m^2/s`.

.. _tj-schematic-tutorials:
.. figure:: sections/tutorial-figures/tj-schematic-tutorials.*
   :width: 300px
   :align: center

   Tee-junction geometry
		
In the table, the flow parameters are provided.

.. _tj-flow-conditions:
.. table:: Flow conditions

   +----------------+---------------------+-----------------------+-------------------------------+
   | :math:`Re`     | :math:`V~(m/s)`     | :math:`p~(m^2/s^2)`   | :math:`\nu~(m^2/s)`           |
   +================+=====================+=======================+===============================+
   | 300            | 1.0                 | 0 Gauge               | 0.00333                       |
   +----------------+---------------------+-----------------------+-------------------------------+
		
Grid Generation
````````````````

A hexahedral grid is generated for the tee-junction grid using `Pointwise <http://www.pointwise.com/>`_. Specific grid generation details are not discussed here, however information regarding the computational domain and boundary conditions are provided. With this, the user will be able to generate an equivalent grid using their choice of tool.

The computational domain should follow the tee-junction geometry and the details are shown in :numref:`tj-domain-tutorials`. Due to viscous nature of the flow, the velocity at the walls is zero, which should be represented through a no-slip boundary condition (:math:`u, v, w = 0`) highlighted in blue. A fully developed laminar flow with a parabolic velocity profile will also be applied as a profile boundary at the inlet. This would ensure that the velocity is fully developed before it approaches the side branch, otherwise requiring to have sufficient length in main branch for the flow to develop. As shown in the geometry, the domain will have two outlets, one at the end of the main channel and the other at the end of side branch. Also of further importance is that the exit pressures at the two outlets are set equal. 

.. _tj-domain-tutorials:
.. figure:: sections/tutorial-figures/tj-domain-tutorials.*
   :width: 300px
   :align: center
		
   Tee junction Computational Domain

The 2D structured grid is shown in :numref:`tj-grid-tutorials` for a :math:`x-y` plane. Caelus is a 3D solver and hence requires a 3D grid although the flow here is assumed to be two-dimensional. The 3D grid was obtained by extruding the 2D grid in the third (:math:`z` - direction) dimension by *one-cell* thick. The two :math:`x-y` planes obtained as a result of mesh extrusion needs boundary conditions to be specified. As the flow is assumed to be 2D, we do not need to solve the flow in :math:`z` direction and this was achieved by specifying *empty* boundary condition for each of those two planes.

.. Note::
   A velocity value of :math:`w=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`z` direction.

.. _tj-grid-tutorials:
.. figure:: sections/tutorial-figures/tj-grid-tutorials.*
   :width: 500px
   :align: center
		
   Tee junction Structured Grid
		
A coarse grid with a total of 2025 cells is made for the tee-junction of which, 90 cells are distributed along the height of the main channel, and 45 along the length of the side branch. The distribution is such that a dimensional length, :math:`L = 1~m` has a total of 45 cells and this gives a distribution of :math:`(2/3)45 = 30` cells for the :math:`(2/3) L` segment of the main channel. The width :math:`W` consists of 15 cells.

Problem definition
````````````````````
We begin with instructions to set-up the tee-junction problem and subsequently configuring the required input files. A full working case can be found in the following location

.. code-block:: bash
		
   /tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction

However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**

.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned

For setting up the problem the following directories are needed:``time``, ``constant`` and ``system``, where relevant files are placed. In this case, the ``time`` directory would be named ``0`` as we begin the simulation at time :math:`t = 0~s`. 

In the ``0`` sub-directory, two additional files ``p`` and ``U`` for pressure (:math:`p`) and velocity (:math:`U`) are present. The input contents of these two files set the dimensions, initial and boundary conditions to the problem. These three forms the principle entries required. 

It should be noted that Caelus is case sensitive and therefore the user should set-up the directories (if applicable), files and the contents identical to what is mentioned here.


**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Referring to :numref:`tj-domain-tutorials`, the following boundary conditions will be applied:

* Inlet
    - Velocity: Parabolic velocity profile; average velocity of :math:`v = 1.0~m/s` in :math:`y` direction
    - Pressure: Zero gradient
    
* No-slip wall
    - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
    - Pressure: Zero gradient

* Outlet-1
    - Velocity: Zero gradient velocity
    - Pressure: Fixed uniform gauge pressure :math:`p = 0`
    
* Outlet-2
    - Velocity: Zero gradient velocity
    - Pressure: Fixed uniform gauge pressure :math:`p = 0`
    
* Initialisation
    - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
    - Pressure: Zero Gauge pressure

The first quantity to define would be the pressure (:math:`p`) and this is done by in file ``p``, which can be opened using a text editor.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/0/p
    
		
As we can see, the above file begins with a dictionary named ``FoamFile`` which contains standard set of keywords such as version, format, location, class and object names. This is followed by the principle elements

* ``dimension``
	- is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]``

* ``internalField``
	- is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	- is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.
	
In a similar manner, the input data for the velocity file is shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/0/U
   
		
As noted above, the principle entries for the velocity filed are self explanatory with the typical dimensional units of :math:`m/s` (``[0 1 -1 0 0 0 0]``). The initialisation of the flow is done at :math:`0~m/s` which is set using ``internalField`` to ``uniform (0 0 0);`` which represents three components of velocity.

As discussed previously, a parabolic velocity profile is set for the ``inlet``. This is done through an external library to Caelus called as `groovyBC <http://openfoamwiki.net/index.php/Contrib/groovyBC>`_ which allows to specify boundary conditions in terms of an expression. In this case an expression for a parabolic velocity profile in the :math:`y` direction is obtained by setting the following expression

.. code-block:: bash
		
   "Vmax=1.0;xp=pts().x;minX=min(xp);maxX=max(xp);para=-(maxX-pos().x)*(pos().x-minX)/(0.25*pow(maxX-minX,2))*normal();"

and the velocity at the centerline is uniform at :math:`1~m/s` represented through ``uniform (0 1 0);`` 

The boundary conditions (``inlet``, ``outlet``, ``wall``, etc) specified above should be the grid boundary patches (surfaces) generated by the grid-generation tool. It should be ensured by the user that their names are identically matched. In addition, the two boundaries in :math:`x-y` plane obtained due to grid extrusion are named as ``symm-left-right`` with applying ``empty`` boundary conditions enforcing the flow to be in two-dimensions. It should however be noted that the two planes are grouped together and the ``empty`` patch is applied. This is a capability of Caelus, where similar boundaries can be grouped together and is also used for the ``wall`` boundary, where multiple walls are present in tee-junction.

*Grid file and Physical Properties*

The grid that has been generated for Caelus format is placed in the ``polyMesh`` sub-directory of ``constant``. Additionally, the physical properties are specified in three different files, placed in the ``constant`` sub-directory. The first file ``transportProperties``, contains the detail about the transport model for the viscosity and kinematic viscosity. The contents are as follows

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/constant/transportProperties
    
		
We use ``Newtonian;`` keyword as the flow is solved under Newtonian assumption, and a kinematic viscosity (:math:`nu`) with the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``) is specified.

The next file in the ``constant`` sub-directory is the ``turbulenceProperties``. Here, the *type of simulation* through the keyword ``simulationType`` is set to be ``laminar;`` as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/constant/turbulenceProperties

Similarly, in the ``RASProperties`` file, ``RASModel`` is set to ``laminar``.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/constant/RASProperties
    

*Controls and Solver Attributes*

The necessary files to control the simulation and specify solver attributes such as discretization method, linear solver settings can be found in the ``system`` directory. The ``controlDict`` file contains information regarding the simulation as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/system/controlDict
    

Since groovyBC is used, few relevant libraries are imported by calling the following soon at the end of the file.

.. code-block:: bash
	
   libs 
   (
   "libsimpleSwakFunctionObjects.so"
   "libswakFunctionObjects.so"
   "libgroovyBC.so"
   );

Next, the application ``simpleSolver;`` referring to the SIMPLE solver is used in this simulation. As we begin the simulation at :math:`t = 0~s`, we need the boundary condition files to be present in the ``0`` directory, which has been formerly done. The keywords, ``startTime`` to ``startTime`` is used, where ``startTime`` is set to a value ``0``. Since ``simpleSolver`` is a steady-state solver, the keyword ``endTime`` corresponds to the total number of iterations.The interval at which output files are written is controlled by ``writeControl`` and ``writeInterval`` keywords.

The schemes for finite volume discretization are specified through ``fvSchemes`` file with the contents as follows

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/system/fvSchemes
    

As apparent from the above file, discretization schemes are set for time-derivative, gradient, divergence and Laplacian terms.

In the final file, ``fvSolution``, linear solver settings are made as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_teeJunction/system/fvSolution
    

In the above file, different linear solvers can be seen to be used to solve pressure and velocity fields. By setting the keyword ``consistent`` to ``true``, ``SIMPLEC`` solver is used and therefore a relaxation factor of ``1.0`` is applied for ``p``. Further, the grid used here is perfectly orthogonal and therefore the orthogonal correction specified via ``nNonOrthogonalCorrectors`` is set to 0.

With these, the set-up of the relevant directories and files are completed. Let us view the directory structure to ensure all are present. The tree should be identical to the one shown below

.. code-block:: bash
	
   tree
   .
   ├── 0
   │   ├── p
   │   └── U
   ├── constant
   │   ├── polyMesh
   │   │   ├── boundary
   │   │   ├── faces
   │   │   ├── neighbour
   │   │   ├── owner
   │   │   └── points
   │   ├── RASProperties
   │   ├── transportProperties
   │   └── turbulenceProperties
   └── system
       ├── controlDict
       ├── fvSchemes
       └── fvSolution

Execution of the solver
````````````````````````
The execution of the solver involves few different steps. The first of which is to renumber the grid or mesh followed by checking the mesh quality. Renumbering reduces the matrix bandwidth while quality check shows the mesh statistics. These can be performed as follows

.. code-block:: bash
	
   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh
		
During the process of renumbering, grid-cell bandwidth information before and after ``renumberMesh`` is shown and the user can take a note of this. The mesh statistics are as shown below after invoking ``checkMesh``

.. literalinclude:: sections/tutorial-figures/tj_checkmesh.txt
	
From the above information, the mesh non-orthogonality is very small and therefore no non-orthogonal corrections are required for the solver to be carried out and we set ``nNonOrthogonalCorrectors`` to ``0`` in the ``fvSolution`` file. In the next step, we will execute the solver and monitor the progress of the simulation. The solver should be executed from the top level directory 

.. code-block:: bash
	
  caelus run -l my-tee-junction.log -- simpleSolver 
		

The progress of the simulation is written to the log file ``my-tee-junction.log``, which can further be processed to get the convergence history. In a separate terminal window use

.. code-block:: bash
	
   caelus logs -w my-tee-junction.log 
		
The plot indicates the convergence history for pressure and other variables with respect to number of iterations. The same for pressure is shown in :numref:`tj-convergence-tutorials`. 

.. _tj-convergence-tutorials:
.. figure:: sections/tutorial-figures/tj-convergence-tutorials.*
   :width: 500px
   :align: center
		
   Convergence of pressure with respect to number of iterations.

Results
````````

The solution obtained for the tee-junction at steady state is shown here using qualitative contour plots. In :numref:`tj-velocitypressure-tutorials`, velocity magnitude and pressure contour plots are shown. In addition, streamlines superimposed on the velocity magnitude is given. The change in the flow pattern due to the presence of side branch is quite evident from the velocity magnitude contour. The streamlines particularly facilitate to visualise the flow separation phenomenon which is occurring in this case, just before the flow entering the side branch. Also to note is the velocity profile at the inlet, which is fully developed as expected.

.. _tj-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/tj-velocitypressure-tutorials.*
   :width: 700px
   :align: center
		
   Velocity magnitude and pressure contour plots within the tee-junction






		
