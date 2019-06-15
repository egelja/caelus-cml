Laminar Flat Plate 
------------------

In this tutorial, simulation of laminar incompressible flow over a two-dimensional sharp leading-edge flat plate using Caelus |version| is introduced here. First, pre-requisites to begin a Caelus simulation is discussed followed by various dictionary entries defining fluid properties, boundary conditions, solver setting, etc that are needed. Finally, the presence of laminar boundary layer is visualised using velocity contours. Here, the basic procedures of running Caelus is shown in sufficient detail such that the user feels comfortable with the usage.

Objectives
````````````
With the completion of this tutorial, the user will be familiar with setting up Caelus simulation for steady, laminar, incompressible flow over flat-plates in two-dimensions and subsequently post-processing the results. Following are some of the steps carried out in this tutorial

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
	- Visualisation of the laminar boundary layer

Pre-requisites 
````````````````
It is assumed that the user is familiar with the Linux command line environment using a terminal or Caelus-console (for Windows OS) and that Caelus is installed correctly with appropriate environment variables set. The grid used here is generated using `Pointwise <http://www.pointwise.com/>`_ and the user is free to use their choice of grid generation tool having exporting capabilities to the Caelus grid format.

Background
````````````
The flow over a flat-plate presents an ideal case where initial steps of a Caelus simulation can be introduced to the user in easy steps. Here, laminar, incompressible flow over a sharp-leading edge plate is solved in a time-accurate approach. This results in the formation of laminar boundary layer which is then compared with the famous Blasius :cite:`Blasius1908` analytical solution in the form of a non-dimensional shear stress distribution (skin-friction coefficient). For more details, the user is suggested to refer the validation of flat-plate in section :ref:`Flat plate validation <flat-plate-validation-label>`.

The length of the flat-plate considered here is :math:`L = 0.3048~m` with a Reynolds number based on the total length of :math:`Re = 200,000`. A schematic of the geometry is shown in :numref:`fpschematictutorials`, wherein :math:`U` is the flow velocity in :math:`x` direction. An inflow temperature of :math:`T = 300~K` can assumed for the fluid *air* which corresponds to a kinematic viscosity (:math:`\nu`) of :math:`\nu = 1.5896306 \times 10^{-5}~m^2/s`. Using the values of :math:`Re`, :math:`L` and :math:`\nu`, we can evaluate the freestream velocity to :math:`U = 10.4306~m/s`.

.. _fpschematictutorials:

.. figure:: sections/tutorial-figures/fp-schematic-tutorials.*
   :width: 600px
   :align: center
   
   Schematic of the flat-plate flow

In the following table, details of the freestream conditions are provided.

.. _fp-freestream-conditions:
.. table:: Freestream conditions

  +------------+----------------+---------------+---------------------+----------------------+---------------+-------------------------------+
  | Fluid      | :math:`L~(m)`  |:math:`Re`     | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)`  | :math:`T~(K)` | :math:`\nu~(m^2/s)`           |
  +============+================+===============+=====================+======================+===============+===============================+
  | Air        | 0.3048         | 200,000       | 69.436113           | Gauge (0)            | 300           |:math:`1.58963\times10^{-5}`   |
  +------------+----------------+---------------+---------------------+----------------------+---------------+-------------------------------+

Grid Generation
`````````````````
As noted earlier, `Pointwise <http://www.pointwise.com/>`_ has been used here to generate a hexahedral grid. Specific details pertaining to its usage are not discussed here, rather a more generic discussion is given about the computational domain and boundary conditions. This would facilitate the user to obtain a Caelus compatible grid using their choice of grid generating tool.

The computational domain is a rectangular block encompassing the flat-plate. The below (:numref:`fpdomaintutorials`) shows the details of the boundaries that will be used in two-dimensions (:math:`x-y` plane). First, the flat-plate, which is our region of interest (highlighted in blue) is extended between :math:`0\leq x \leq 0.3048~m`. Because of viscous nature of the flow, the velocity at the wall is zero which can be represented through a no-slip boundary (:math:`u, v, w = 0`). Upstream of the leading edge, a slip boundary will be used to simulate a freestream flow approaching the flat-plate. However, downstream of the plate, it would be ideal to further extend up to three plate lengths (highlighted in green) with no-slip wall. This would then ensure that the boundary layer in the vicinity of the trailing edge is not influenced by outlet boundary. Since the flow is incompressible (subsonic), the disturbance caused by the pressure can propagate both upstream as well as downstream. Therefore, the placement of the inlet and outlet boundaries are to be chosen to have minimal or no effect on the solution. The inlet boundary as shown will be placed at start of the slip-wall at :math:`x = -0.06~m` and the outlet at the exit plane of the no-slip wall (green region) at :math:`x = 1.2192~m`. Both inlet and outlet boundary are between :math:`0\leq y \leq 0.15~m`. A slip-wall condition is used for the entire top boundary.

.. _fpdomaintutorials:
.. figure:: sections/tutorial-figures/fp-domain-tutorials.*
   :width: 600px
   :align: center
		
   Flat-plate computational domain

The 2D structured grid is shown in :numref:`fp-grid-tutorials`. Since Caelus is a 3D solver, it necessitates the grid to be in 3D. Therefore, the 3D grid should be obtained through extruding the 2D gird in the :math:`z` direction by a minimum of *one-cell* thick. The final 3D grid should be then exported to Caelus format (polyMesh). The two :math:`x-y` planes obtained as a result of grid extrusion need boundary conditions to be specified. As the flow over a flat-plate is generally 2D, we do not need to solve the flow in the third dimension. This can easily be achieved in Caelus by specifying ``empty`` boundary condition for each of those two planes.

.. Note::
   A velocity value of :math:`w=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`z` direction.

.. _fp-grid-tutorials:
.. figure:: sections/tutorial-figures/fp-grid-tutorials.*
   :width: 700px
   :align: center
		
   Flat-plate computational grid

The flat-plate has a total of 400 cells over the region of interest between :math:`0 \leq x \leq 0.3048~m` and 286 cells in the no-slip wall that extends for an additional 3 plate lengths (green region in the above figure). In the wall normal direction, 298 cells are used and sufficient refinement close to the wall was made to ensure that accurate boundary layer is captured.

Problem definition
````````````````````

Several important instructions would be shown here to set-up the flat-plate problem along with the detail of configuration files used. A full working case can be found in:

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D

However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**


.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned
		
In order to set-up the problem, three main sub-directories containing all the relevant information are used. Caelus requires ``time``, ``constant`` and ``system`` sub-directories. Since we begin the simulation at time :math:`t = 0~s`, the ``time`` sub-directory should be just ``0``. 

The ``0`` sub-directory is where additional two files, ``p`` and ``U`` for pressure (:math:`p`) and velocity (:math:`U`) respectively are kept. The contents of these two files sets the dimensions, initialisation and boundary conditions to the problem, which also form three principle entries required. 
		
It should be noted that Caelus is case sensitive and therefore the user should set-up the directories (if applicable), files and the contents identical to what is mentioned here.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

First let us look at setting up the boundary conditions. Referring back to :numref:`fpdomaintutorials`, following are the boundary conditions that need to be specified:

* Inlet
    - Velocity: Fixed uniform velocity :math:`u = 10.4306~m/s` in :math:`x` direction
    - Pressure: Zero gradient
    
* Slip wall
    - Velocity: slip
    - Pressure: slip

* No-slip wall
    - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
    - Pressure: Zero gradient

* Outlet
    - Velocity: Zero gradient velocity
    - Pressure: Fixed uniform gauge pressure :math:`p = 0`
    
* Initialisation
    - Velocity: Fixed uniform velocity :math:`u = 10.4306~m/s` in :math:`x` direction
    - Pressure: Zero Gauge pressure
    
Now let us look at the contents and significance of each file in these sub-directories beginning with the pressure (:math:`p`) file.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/0/p
    

The above file begins with a dictionary named ``FoamFile`` which contains standard set of keywords for version, format, location, class and object names. The following are the principle entries required for this case.

* ``dimension``
	- is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	- is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0``

* ``boundaryField``
	- is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.
	
Similarly, the file ``U``, shown below sets the boundary conditions for velocity.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/0/U
    

The principle entries for velocity field are self explanatory and the dimension are typical for velocity with units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Since we initialise the flow with a uniform freestream velocity, we set the ``internalField`` to ``uniform (10.43064759 0 0)`` representing three components of velocity. In a similar manner, ``inflow``, ``wall`` and ``downstream`` boundary patches have three velocity components. 

At this stage it is important to ensure that the boundary conditions (``inflow``, ``outflow``, ``top``, etc) specified in the above files should be the grid boundary patches (surfaces) generated by the grid generation tool and their names are identical. Further, the two boundaries in :math:`x-y` plane obtained due to grid extrusion have been named as ``symm-left`` and ``symm-right`` with specifying ``empty`` boundary conditions forcing Caelus to assume the flow to be in two-dimensions. This completes the setting up of boundary conditions.

*Grid file and Physical Properties*

The flat-plate grid files that is generated in the Caelus format resides in the ``constant/polyMesh`` sub-directory. It contains information relating to the points, faces, cells, neighbours and owners of the mesh.

In addition, the physical properties are specified in various different files present in the directory ``constant``. In the ``transportProperties`` file, transport model and kinematic viscosity are specified. The contents of this file are as follows

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/constant/transportProperties
    

As the flow is Newtonian, the ``transportModel`` is specified with ``Newtonian`` keyword and the value of kinematic viscosity (``nu``) is given which has the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The next file is the ``turbulenceProperties`` file, where the *type of simulation* is specified.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/constant/turbulenceProperties
    

Since the flow here is laminar, the ``simulationType`` would be ``laminar``. Similarly, in the ``RASProperties`` file, ``RASModel`` is set to ``laminar`` as shown below. 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/constant/RASProperties

*Controls and Solver Attributes*

The files required to control the simulation and specifying the type of discretization method along with the linear solver settings are present in the ``system`` directory. 

The ``controlDict`` file is shown below:

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/system/controlDict
    

Here, the application ``simpleSolver`` refers to the SIMPLE solver that will be used. We also begin the simulation at :math:`t = 0~s`, which logically explains the need for ``0`` directory where the data files are read at the beginning of the run. Therefore, we need to set the keyword ``startFrom`` to ``startTime``, where ``startTime`` would be ``0``. Since ``simpleSolver`` is a steady-state solver, the keyword ``endTime`` corresponds to the total number of iterations.The interval at which output files are written is controlled by ``writeControl`` and ``writeInterval`` keywords.

The discretization schemes and parameters are specified through the ``fvSchemes`` file, shown below  

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/system/fvSchemes
    

Here, the discretization schemes for finite volume discretization of time-derivative, gradient, divergence and Laplacian terms are specified. 

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/laminar/ACCM_flatPlate2D/system/fvSolution
    

Different linear solvers are used here to solve pressure and velocity. The ``GAMG`` preconditioner is applied to improve convergence of the ``p`` solver. Also, by setting the keyword ``consistent`` to ``true``, ``SIMPLEC`` solver is used and therefore a relaxation factor of ``1.0`` is applied for ``p``. 

The set-up of the directory structure along with the relevant files are completed. Let us verify again by typing the following command (on Linux and Caelus-console prompt) and the directory tree should be identical to the one shown below

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
Before execution of the solver, renumbering of the grid or mesh needs to be performed as well as checking the quality of the grid. Renumbering the grid-cell list is vital to reduce the matrix bandwidth while the quality check gives us the mesh statistics. Renumbering and mesh quality can be determined by executing the following from the top directory 

.. code-block:: bash
	
   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

It is suggested for the user to take note of the bandwidth before and after the mesh renumbering. When the ``checkMesh`` is performed, the following information is given as an output 

.. literalinclude:: sections/tutorial-figures/fp_checkmesh.txt

The mesh non-orthogonality as reported above is 0 and therefore no non-orthogonal corrections are needed in this case. In the case of mesh non-orthogonality being high, certain number of corrections are to be accounted for which can be set in the ``fvSolution`` file with the keyword ``nNonOrthogonalCorrectors``. The next step is to execute the solver and monitoring the progress of the solution. The solver is always executed from the top directory which is ``ACCM_flatPlate2D`` in our case as below 

.. code-block:: bash

   caelus run -l my-flat-plate.log -- simpleSolver 

Now the simulation begins and the output of the solution process is written to the log file, ``my-flat-plate.log``. In another the terminal window the solver process a ca nbe watched through plotting of the residuals

.. code-block:: bash
	
   caelus logs -w my-flat-plate.log
		
Now the convergence of pressure can be seen with respect to number of iterations along with other convergence properties.

.. _fp-lam-convergence-tutorials:
.. figure:: sections/tutorial-figures/fp-lam-convergence-tutorials.*
   :width: 500px
   :align: center
		
   Convergence of pressure with respect to number of iterations.
	
Results
````````
A brief qualitative data of the simulated flat-plate results are given here. Since the aim here is to obtained the steady solution, the results therefore represent the final steady state condition. In :numref:`fpvelocitytutorials`, the contours of velocity magnitude are shown which highlights the development of the boundary layer. Since the Reynolds number of the flow is high, thickness of the boundary layer is quite thin in comparison to the length of the plate.


.. _fpvelocitytutorials:
.. figure:: sections/tutorial-figures/fp-velocity-tutorials.*
   :width: 700px
   :align: center
		
   Contour of velocity magnitude over the flat-plate
