Triangular Cavity
-----------------

In this tutorial, we carry out laminar, incompressible flow inside a triangular cavity in two-dimensions using Caelus |version|. Details regarding setting up of the directories, fluid properties, boundary conditions, etc will be discussed. Subsequent to this, post-processing the velocity distribution along the center-line will be shown.

Objectives
````````````

With the completion of this tutorial, the user would be familiar with setting up a steady-state Caelus simulation for laminar, incompressible flow over lip-driven cavity. Following are the steps that would be performed

* Background
	- A brief description about the problem
	- Geometry and freestream details

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
	- Showing the flow structure inside the cavity

Pre-requisites 
````````````````
It is assumed that the user is familiar with the Linux command line environment using a terminal or Caelus-console (for Windows OS) and that Caelus is installed correctly with appropriate environment variables set. The grid used here is generated using `Pointwise <http://www.pointwise.com/>`_ and the user is free to use their choice of grid generation tool having exporting capabilities to the Caelus grid format.

Background
````````````

Flow inside lid-driven cavities is a classical case to study cases with flow recirculation. In the present case, the top wall of the cavity moves at constant velocity initiating a recirculation motion with the cavity and as a consequence, a boundary layer develops in the direction of the moving lid. The feature that is of interest is the velocity distribution along the center-line of cavity. Details regarding the validation of this case is given in :ref:`t-cavity-validation`.

The triangular cavity schematic is shown in :numref:`tcschematictutorials`. Here D represents the cavity depth which is 4 m and the top width, W = 2 m. For this configuration, the Reynolds number based on the cavity depth is 800 and the wall velocity is assumed and set to 2 m/s. This give us with a kinematic viscosity of 0.01. Note that the two-dimensional plane considered here is in :math:`x-z`.

.. _tcschematictutorials:
.. figure:: sections/tutorial-figures/tc-schematic-tutorials.*
   :width: 300px
   :align: center
  
   Schematic of a triangular cavity

The below table gives the summary of the freestream conditions used here

.. table:: Freestream conditions

   +----------------+---------------------+---------------------------+-------------------------------+
   | :math:`Re_D`   | :math:`U~(m/s)`     | :math:`p~(Pa)`            | :math:`\nu~(m^2/s)`           |
   +================+=====================+===========================+===============================+
   | 800            | 2.0                 | :math:`(0)` Gauge         | :math:`0.01`                  |
   +----------------+---------------------+---------------------------+-------------------------------+

Grid Generation
````````````````

A hybrid-grid consisting of quadrilateral and triangular cells has been generated for this cavity geometry using `Pointwise <http://www.pointwise.com/>`_. Details regarding the generation of grid is not covered in this tutorial, however details regarding computational domain and boundary conditions are provided.

The computational domain for the triangular cavity follows the cavity geometry due to internal flow configuration. This is in contrast to other flow configurations here where the flow was over the region of interest. A schematic of the domain is shown in :numref:`tc-domain-tutorials`. The velocity at the cavity walls (high lighted in blue) is zero, represented through a no-slip boundary, wherein :math:`u, v, w = 0`. Whereas the top wall has a uniform velocity in the x-direction.

.. _tc-domain-tutorials:
.. figure:: sections/tutorial-figures/tc-domain-tutorials.*
   :width: 300px
   :align: center
		
   Computational domain for a triangular cavity

The hybrid grid is shown in :numref:`tc-grid-tutorials`. As can be seen, up to a depth of D = 1.35 m, structured grids are used and after which it is filled with triangular unstructured elements. In the structured domain, 40 X 40 cells are used respectively. In the 2D domain, a total of 5538 cells are present, however the ``polyMesh`` format of Caelus should be in 3D. This was achieved by extruding the grid in the :math:`x-y` plane by *one cell* thick and subsequently specifying empty boundary conditions to the extruded planes. This should force Caelus to solve the flow the flow in 2D in the extruded direction, which is :math:`z`.

.. Note::
   A velocity value of :math:`w=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`z` direction.

.. _tc-grid-tutorials:
.. figure:: sections/tutorial-figures/tc-grid-tutorials.*
   :width: 400px
   :align: center
   
   Hybrid grid representation for a triangular cavity

Problem definition
````````````````````

This section provides the case set-up procedures along with the configuration files that are needed. A full working case of this problem is given in the following location:

.. code-block:: bash

   /tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/

However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**


.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned

Caelus requires ``time``, ``constant`` and ``system`` sub-directories within the main ``my-triangular-cavity`` working directory. Since we start the simulation at time, t = 0 s, a ``time`` sub-directory named ``0`` is required. 

The ``0`` sub-directory contains the pressure, ``p`` and velocity ``U`` files. The contents of these files set the dimensions, initialisation and boundary conditions to the case, which form the three principle entries required. 

If applicable, the user should take precautions in setting the directories and files as Caelus is case sensitive. These have to be identical to the names mentioned here.

*Boundary Conditions*

Next we start with setting-up of the boundary conditions. Referring back to :numref:`tc-domain-tutorials`, the following are the boundary conditions that need to be specified:

* Moving wall
    - Velocity: Fixed uniform velocity :math:`u = 2.0~m/s` in :math:`x` direction
    - Pressure: Zero gradient

* No-slip wall
    - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
    - Pressure: Zero gradient
    
* Initialisation
    - Velocity: Fixed uniform velocity :math:`u = 0~m/s` in :math:`x, y, z` directions
    - Pressure: Zero Gauge pressure

The file ``p`` for pressure contains the following information 

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/0/p
    

As noted from the above file, the dictionary begins with ``FoamFile`` containing standard set of keywords for version, format, location, class and object names. The following provides the explanation to the principle entries

* ``dimension``
	- is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	- is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0``

* ``boundaryField``
	- is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.

Similarly, the contents for the file ``U`` is as shown below

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/0/U
    

The principle entries for velocity field are self explanatory and the dimension are typical for velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Here, since the top wall moves with a velocity, we set a ``uniform (2.0 0 0)`` for ``moving-wall`` boundary patch. Similarly, the cavity walls (``fixed-walls``) have ``uniform (1.0 0 0)``.

At this stage, the user should ensure that the boundary conditions (``fixed-walls``, ``moving-wall`` and ``symm``) specified in the above files should be the grid boundary patches (surfaces) generated by the grid generation tool and their names are identical. Further, the two boundaries in :math:`x-y` plane obtained due to grid extrusion have been combined and named as ``symm`` with specifying ``empty`` boundary conditions forcing Caelus to assume the flow to be in two-dimensions. With this, the setting up of boundary conditions are completed.

*Grid file and Physical Properties*

The triangular cavity grid is placed in ``constant/polyMesh`` sub-directory. In addition, the physical properties are specified in different files, all present in the ``constant`` directory. 

The transport model and the kinematic viscosity are specified in the file ``transportProperties``. The contents of this file are as follows

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/constant/transportProperties
    

Since the viscous behaviour is modelled as Newtonian, the ``transportModel`` is specified with the keyword ``Newtonian`` and the value of kinematic viscosity is set with has the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, in which the *type of simulation* is specified as

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/constant/turbulenceProperties
    

The flow being laminar, the ``simulationType`` is set to ``laminar``.

*Controls and Solver Attributes*

In this section, the files required to control the simulation, setting the discretization parameters and linear solver settings are discussed can be found in the ``system`` directory.

The ``controlDict`` file is shown below.

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/system/controlDict
    

In the ``controlDict`` file, ``pimpleSolver`` refers to the application, PIMPLE solver that is used here. The simulation is also started at t = 0 s and this logically explains the need for ``0`` directory where the data files are read at the beginning of the run. Therefore, the keyword ``startFrom`` to ``startTime``, where ``startTime`` would be ``0`` is needed. The simulation is run for 200 seconds specifying through the keywords ``stopAt`` and ``endTime``. Since PIMPLE solver is time-accurate, we also need to set the time-step via ``deltaT``. The results are written at every 0.01 seconds via ``writeControl`` and ``writeInterval`` keywords.

The discretization schemes and its parameters are specified in the ``fvSchemes`` file which is shown below

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/system/fvSchemes
    

In the following file, which is the ``fvSolution`` the linear solver controls and tolerances are set as shown below

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_triangularCavity/system/fvSolution
    

It should be noted that different linear solvers are used to solve for pressure a velocity. Since we have used hybrid grids, ``nNonOrthogonalCorrectors`` is set to ``1`` as there would be some degree of non-orthogonality present due to triangular nature of the geometry.

This completes the set-up of directory structure along with all the necessary files. This can be verified by using the following commands and the directory tree should be identical to the one shown below

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
   │   ├── transportProperties
   │   └── turbulenceProperties
   └── system
       ├── controlDict
       ├── fvSchemes
       ├── fvSolution


Execution of the solver
````````````````````````

Renumbering and checking the mesh quality is needed before the solver is executed. Renumbering the grid-cell list is vital to reduce the matrix bandwidth while the quality check gives us the mesh statistics. Renumbering and mesh quality can be determined by executing the following from the top directory 

.. code-block:: bash

   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

The user should take note of the bandwidth before and after the mesh renumbering. When the ``checkMesh`` is performed, the mesh statistics are shown as below

.. literalinclude:: sections/tutorial-figures/tc_checkmesh.txt

Solver can now be executed and the progress of the solution can be monitored. The solver is always executed from the top directory which is ``my-triangular-cavity`` in this case.

.. code-block:: bash
	
   caelus run -l my-triangular-cavity.log pimpleSolver 

The output of the solver progress is saved in the log file, ``my-triangular-cavity.log``. The log file (``my-triangular-cavity.log``) can be monitored to look at the convergence history. In a separate terminal window use

.. code-block:: bash

   caelus logs -w my-triangular-cavity.log


The convergence of the pressure can now be seen with respect to time.

.. _tc-convergence-tutorials:
.. figure:: sections/tutorial-figures/tc-convergence-tutorials.*
   :width: 500px
   :align: center

   Convergence of pressure with respect to time

Results
````````

The flow within the cavity is shown here at steady state condition. :numref:`tc-velocitypressure-tutorials` presents the velocity and pressure contour plots. In addition, the streamlines indicate the multiple vortices formed within the cavity.


.. _tc-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/tc-velocitypressure-tutorials.*
   :width: 700px
   :align: center
		
   Velocity magnitude and pressure contour plots within the triangular cavity










