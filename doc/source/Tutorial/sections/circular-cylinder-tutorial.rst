Circular Cylinder 
-----------------

The simulation of laminar, incompressible flow over a two-dimensional circular cylinder is shown in this tutorial. Caelus |version| will be used and the details of setting up directory structure, fluid properties, boundary conditions, etc will be shown. This tutorial introduces to the user in carrying out a time-dependent simulation of a externally separated flow. Further to this, the flow around the cylinder would be visualised using velocity and pressure contours.

Objectives
````````````
Through this tutorial, the user would be familiar in setting up a time-dependent Caelus simulation for laminar, incompressible flow in two-dimensions for external separated flows. Following will be some of the steps that will be performed.

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
	- Showing the flow structure in near and far wake

Pre-requisites 
````````````````
It is assumed that the user is familiar with the Linux command line environment using a terminal or Caelus-console (for Windows OS) and that Caelus is installed correctly with appropriate environment variables set. The grid used here is generated using `Pointwise <http://www.pointwise.com/>`_ and the user is free to use their choice of grid generation tool having exporting capabilities to the Caelus grid format.


Background
````````````
The flow over a circular cylinder is a classical configuration to study separation and its related phenomena. This provides an ideal case for the user to use Caelus for flow over bluff bodies that represents externally separated flow. It has been shown that for low Reynolds number flows (:math:`40 \leq Re \leq 150`), period vortex shedding occurs in the wake. This phenomena of vortex shedding behind bluff bodies is referred as the *Karman Vortex Street* :cite:`Roshko1954`. The frequency associated with the oscillations of vortex streets can be expressed via Strouhal number (:math:`St`) which is non-dimensional relating to the frequency of oscillations (:math:`f`) of vortex shedding, cylinder diameter (:math:`D`) and the flow velocity (:math:`U`) as

.. math::

   St = \frac{fD}{U}

For a Reynolds number based on the cylinder diameter of :math:`Re_D = 100`, the Strouhal number is about :math:`St\approx 0.16-0.17` as determined through experiments and is nearly independent of the diameter of the cylinder. More details are discussed in section :ref:`circular-cylinder-validation`

The diameter chosen for the cylinder here is :math:`D = 2~m` and is the characteristic length for the Reynolds number, which is (:math:`Re_D = 100`). The velocity is assumed to be :math:`U = 1~m/s` in the :math:`x` direction. Based on these, the kinematic velocity can be estimated as :math:`\nu = 0.02~m^2/s`. The below :numref:`cc-schematic-tutorials` shows the schematic of the cylinder in the :math:`x-y` plane.

.. _cc-schematic-tutorials:
.. figure:: sections/tutorial-figures/cc-schematic-tutorials.*
   :width: 500px
   :align: center
   
   Schematic of the circular cylinder in two-dimensions
		
In the below table a summary of the freestream conditions are given

.. _cc-freestream-conditions:
.. table:: Freestream conditions

   +----------------+---------------------+---------------------------+-------------------------------+
   | :math:`Re_D`   | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)`       | :math:`\nu~(m^2/s)`           |
   +================+=====================+===========================+===============================+
   | 100            | 1.0                 | :math:`(0)` Gauge         | :math:`0.02`                  |
   +----------------+---------------------+---------------------------+-------------------------------+

Grid Generation
````````````````
A hexahedral grid around the circular cylinder was development with a O-grid topology using  `Pointwise <http://www.pointwise.com/>`_. Specific grid generation details are omitted while proving sufficient details regarding computational domain and boundary conditions. With these details the user should be able to recreate the required grid for the two-dimensional cylinder

A rectangular computational domain in the :math:`x-y` plane has been constructed surrounding the circular cylinder as shown in :numref:`cc-domain-tutorials`. A full cylinder was considered as the vortices developed behind the cylinder are of the periodic nature. Upstream of the cylinder, the domain is extended by 5 cylindrical diameters, whereas, downstream it was extended up to 20. Since the flow here is viscous dominated, sufficient downstream length is required to capture the initial vortex separation from the surface of the cylinder and the subsequent shedding process. In the :math:`y` direction, the domain is extended up to 5 cylindrical diameters on either side. From the figure, multiple inlet boundaries to this domain can be seen, one at the far end of the upstream and the other two for the top and bottom boundaries. This type of configuration is particularly needed to appropriately model the inflow, very similar to an undisturbed flow in an experimental set-up. It should be noted here that for top and bottom boundaries, the flow is in the :math:`x` direction. Outlet boundary to the domain is placed at the downstream end which is at 20 cylindrical diameters. Since the fluid behaviour is viscous, the velocity at the wall is zero (:math:`u, v, w = 0~m/s`) represented here through a no-slip boundary condition as highlighted in blue.
		
.. _cc-domain-tutorials:
.. figure:: sections/tutorial-figures/cc-domain-tutorials.*
   :width: 600px
   :align: center
		
   Circular cylinder computational domain
		
The hexahedral grid around the cylinder is shown in :numref:`cc-grid-tutorials` for a :math:`x-y` plane. Caelus is a 3D solver and requires the grid to be in 3D. This was obtained by extruding the grid in the :math:`x-y` plane by *one cell* thick and subsequently specifying *empty* boundary conditions to the extruded planes. This enforces that Caelus to solve the flow in two dimensions assuming symmetry in the :math:`z` direction as is required in this case due to the two-dimensionality of the flow. 

.. Note::
   A velocity value of :math:`w=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`z` direction.

.. _cc-grid-tutorials:
.. figure:: sections/tutorial-figures/cc-grid-tutorials.*
   :width: 600px
   :align: center

   O-grid around the cylinder and structured gird representation

The 2D domain consisted of 9260 cells in total. An O-grid topology is constructed around the cylinder and extended to a maximum of 1 cylindrical diameter in the radial direction with a distribution of 10 cells. Around the cylinder, 84 cells are used giving 21 cells per each O-grid block. Upstream of the O-grid in the :math:`x` direction, 31 cells were used and 100 in the downstream. The region of interest is about 10 diameters downstream, where the grids are refined. In the :math:`y` direction, both positive and negative axes, 21 cells are used beyond the O-grid region.

Problem definition
````````````````````
We first begin with instructions to set-up the circular cylinder case in addition to the configuration files that are needed. A full working case can be found in the following directory:

.. code-block:: bash

   /tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/

However, the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**

.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned

For setting up this problem, Caelus requires ``time``, ``constant`` and ``system`` sub-directories within the main working directory. Since we start the simulation at time, t = 0 s, a ``time`` sub-directory named ``0`` is required. 

The ``0`` sub-directory contains files, ``p`` and ``U``, which describe the dimensions, initialisation and boundary conditions of pressure (:math:`p`) and velocity (:math:`U`) respectively. 
		
It is to be noted that Caelus is case sensitive and therefore the user should set-up the directories (if applicable), files and the contents identical to what is mentioned here.

*Boundary Conditions*

We now begin with setting up the boundary conditions. Referring back to :numref:`cc-domain-tutorials`, the following are the boundary conditions that need to be specified:

* Inlet
    - Velocity: Fixed uniform velocity :math:`u = 1.0~m/s` in :math:`x` direction
    - Pressure: Zero gradient

* No-slip wall
    - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
    - Pressure: Zero gradient

* Outlet
    - Velocity: Zero gradient velocity
    - Pressure: Fixed uniform gauge pressure :math:`p = 0`
    
* Initialisation
    - Velocity: Fixed uniform velocity :math:`u = 1.0~m/s` in :math:`x` direction
    - Pressure: Zero Gauge pressure

Beginning with the pressure (:math:`p`), the dictionary begins with ``FoamFile`` containing standard set of keywords for version, format, location, class and object names as shown below.

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/0/p
    

The following provides the explanation to the principle entries

* ``dimension``
	- is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	- is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0``

* ``boundaryField``
	- is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.

In a similar manner, the file ``U`` contains the following entries

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/0/U
    

The principle entries for velocity field are self explanatory and the dimension are typical for velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Since we initialise the flow with a uniform freestream velocity, we set the ``internalField`` to ``uniform (1.0 0 0)`` representing three components of velocity. Similarly, ``inlets`` and ``wall`` boundary patches have three velocity components.

Before proceeding further, it is important to ensure that the boundary conditions (``inlet``, ``outlet``, ``wall``, etc) specified in the above files should be the grid boundary patches (surfaces) generated by the grid generation tool and their names are identical. Further, the two boundaries in :math:`x-y` plane obtained due to grid extrusion have been named as ``symm-left`` and ``symm-right`` with specifying ``empty`` boundary conditions forcing Caelus to assume the flow to be in two-dimensions. This completes the setting up of boundary conditions.

*Grid file and Physical Properties*

The circular cylinder grid file is placed in the ``constant/polyMesh`` sub-directory. Additionally, the physical properties are specified in different files present in the ``constant`` directory. 

The first file is ``transportProperties``, where the transport model and the kinematic viscosity are specified. The contents of this file are as follows

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/constant/transportProperties
    

Since the flow is Newtonian,  the ``transportModel`` is specified with ``Newtonian`` keyword and the value of kinematic viscosity (``nu``) is given which has the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final in this sun-directory is the ``turbulenceProperties`` file, where the *type of simulation* is specified as

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/constant/turbulenceProperties
    

As the flow here is laminar, the ``simulationType`` would be ``laminar``.

*Controls and Solver Attributes*

This section details the files required to control the simulation,  specifying the type of discretization method and linear solver settings. These files can be found in the ``system`` directory. 

The first file, ``controlDict`` is shown below

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/system/controlDict
    

In this file, the application ``pimpleSolver`` refers to the PIMPLE solver that is used in this tutorial. We also begin the simulation at t = 0 s, which logically explains the need for ``0`` directory where the data files are read at the beginning of the run. Therefore, we need to set the keyword ``startFrom`` to ``startTime``, where ``startTime`` would be ``0``. The simulation is run for 360 seconds specifying through the keywords ``stopAt`` and ``endTime``. Since PIMPLE solver is time-accurate, we also need to set the time-step via ``deltaT``. The results are written at every 0.01 seconds via ``writeControl`` and ``writeInterval`` keywords.

The discretization schemes and its parameters are specified in the ``fvSchemes`` file which is shown below

.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/system/fvSchemes
    

In the ``fvSolution`` file, linear solver controls and tolerances are set as shown in the below file

.. _fp-fvSolution:
.. literalinclude:: ../../../tutorials/incompressible/pimpleSolver/laminar/ACCM_circularCylinder/system/fvSolution
    

Note that different linear solvers are used here to solve for pressure and velocity. Also, ``nNonOrthogonalCorrectors`` is set to 1, since there is some degree of non-orthogonality in the grid.

At this stage, the directory structure should be identical to the one shown below. This can be done by using the ``tree`` command on Linux OS.

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
   │   │   ├── points
   │   │   └── sets
   │   ├── transportProperties
   │   └── turbulenceProperties
   └── system
       ├── controlDict
       ├── fvSchemes
       └── fvSolution

Execution of the solver
````````````````````````

Prior to solver execution, renumbering of the grid or mesh needs to be performed as well as checking the quality of the grid. Renumbering the grid-cell list is vital to reduce the matrix bandwidth while the quality check gives us the mesh statistics. Renumbering and mesh quality can be determined by executing the following from the top directory 

.. code-block:: bash
	
   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

The user should take note of the bandwidth before and after the mesh renumbering. When the ``checkMesh`` is performed, the mesh statistics are shown as below

.. literalinclude:: sections/tutorial-figures/cc_checkmesh.txt

In the next step, execution of the solver can be performed while monitoring the progress of the solution. The solver is always executed from the top directory which is ``ACCM_circularCylinder`` in this case.

.. code-block:: bash
	
   caelus run -l my-circular-cylinder.log -- pimpleSolver 

The output of the solution process is saved in the log file, ``my-circular-cylinder.log``. In a separate terminal window the convergence history can be monitored using

.. code-block:: bash
	
   caelus logs -w my-circular-cylinder.log

With the above, the convergence of pressure along with other variables can be seen with respect to time. The same is shown in the :numref:`cc-convergence-tutorials` and due to the periodic nature of vortex shedding, oscillatory convergence of pressure is seen. 


.. _cc-convergence-tutorials:
.. figure:: sections/tutorial-figures/cc-convergence-tutorials.*
  :width: 500px
  :align: center
		
  Convergence of pressure with respect to time

Results
````````

In this section, some qualitative results obtained as a result of steady vortex shedding in the wake of the cylinder is shown. :numref:`cc-velocitypressure-tutorials` shows the velocity magnitude and pressure contour for the flow over the cylinder. The formation of vortex shedding is clearly visible from the velocity contour and the pressure variation due to oscillating vortex in the wake. The vortex break-up that occurs in the near wake of the cylinder, travels several diameters downstream eventually diffusing into the flow.

.. _cc-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/cc-velocitypressure-tutorials.*
   :width: 700px
   :align: center
		
   Velocity magnitude and pressure contour plots for the flow over the 2D cylinder
