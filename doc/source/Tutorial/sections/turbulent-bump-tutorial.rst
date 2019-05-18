Bump in a Channel
-----------------

The simulation of turbulent flow over a two-dimensional bump in a channel is considered in this tutorial and will be performed using Caelus |version|. As with the other tutorials, setting up the directory structure, fluid properties, boundary conditions, turbulence properties etc will be shown. Further to this, visualisation of the solution will be shown to look at the velocity and pressure contours over the bump surface. These steps would be shown in sufficient details so that the user is able to reproduce the tutorial accurately.

Objectives
````````````
Some of the main objectives of this tutorial would be for the user to get familiarise with setting up the Caelus simulation for steady, turbulent, incompressible flow over a two-dimensional bump in a channel and be able to post-process the desired solution. Following would be some of the steps that would be covered.

* Background
   -  A brief description about the problem
   -  Geometry and freestream details

* Grid generation
   -  Computational domain and boundary details
   -  Computational grid generation
   -  Exporting grid to Caelus

* Problem definition
   -  Directory structure
   -  Setting up boundary conditions, physical properties and control/solver attributes

* Execution of the solver
   -  Monitoring the convergence
   -  Writing the log files

* Results
   -  Visualisation of flow near the bump


Pre-requisites
````````````````
The user should be familiar with a Linux command line environment via a terminalor caelus-console (For Windows OS). It is also assumed that Caelus is installed correctly with appropriate environment variables set.  The grid used here is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/bump.html>`__ in a Plot3D format and is exported to Caelus format using `Pointwise <http://www.pointwise.com/>`_. However, the user is free to use their choice of grid generation tool to covert the Plot3D file to Caelus format.

Background
````````````
Turbulent flow over a bump in a channel is quite similar to a flat-plate flow, except that due to the curvature effect, a pressure gradient is developed. The flow would be assumed to be steady-state and incompressible. To demonstrate the effect of curvature, the skin friction distribution along the bump will be plotted. For further information on this case, the user can refer to :ref:`turbulent-bump-verification-label`.

The bump, as shown in the schematic below in :numref:`t-bump-schematic-tutorials` has a upstream and a downstream flat-plate region that begins at x = 0 m and x = 1.5 m respectively, which gives a total length of L = 1.5 m. The flow has a unit Reynolds number of :math:`3 \times 10^6` and Air is used as a fluid with a temperature of 300 K. Based on these values, kinematic viscosity can be evaluated to :math:`\nu = 2.314537 \times 10^{-5} m^2/s`. To match the required Reynolds number, a freestream velocity of U = 69.436 m/s would be used. Note that the two-dimensional plane considered here is in :math:`x-z` directions.

.. _t-bump-schematic-tutorials:
.. figure:: sections/tutorial-figures/t-bump-schematic-tutorials.*
   :width: 500px
   :align: center

   Schematic of the 2D bump

The freestream conditions that will be used is given in the below table
(:ref:`t-bump-freestream-conditions`)

.. _t-bump-freestream-conditions:
.. table:: Freestream conditions

   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+
   | Fluid      | :math:`L~(m)`  | :math:`Re/L~(1/m)`     | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)` | :math:`T~(K)` | :math:`\nu~(m^2/s)`           |
   +============+================+========================+=====================+=====================+===============+===============================+
   | Air        | 1.5            | :math:`3 \times 10^6`  | 69.436113           | Gauge (0)           | 300           |:math:`2.314537\times10^{-5}`  |
   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+

Grid Generation
````````````````
The hexahedral grid used in this tutorial is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/bump_grids.html>`__ that contains 704 X 320 cells in :math:`x-z` directions respectively. The grid originally is in Plot3D format and is converted to Caelus compatible ``polyMesh`` format.

The computational domain is a rectangular channel encompassing the bump. :numref:`t-bump-domain-tutorials` shown below identifies the boundary conditions in a two-dimensional plane (:math:`x-z`). The bump region, highlighted in blue extends between :math:`0 \leq x \leq 1.5~m`, where the velocity at the wall is zero, wherein :math:`u,v,w=0` represented through a no-slip boundary. Upstream and downstream of the bump, a symmetry boundary at the wall is used. The inlet and outlet are placed at the end of the symmetry as depicted in the figure and the top boundary has a symmetry condition.

.. _t-bump-domain-tutorials:
.. figure:: sections/tutorial-figures/t-bump-domain-tutorials.*
   :width: 500px
   :align: center
		
   Computational domain of a 2D bump

The ``polyMesh`` grid obtained from the conversion of Plot3D is in 3D. However, the flow over a bump is two-dimensional and is solved in a 2D plane with :math:`x-z` directions. Therefore, ideally we can have cells with *one-cell* thick in the direction (:math:`y`), normal to the 2D plane, where the flow can be assumed to be symmetry. The two :math:`x-z` planes obtained as a result of having a 3D grid require boundary conditions to be specified. As the flow is assumed to be 2D, we do not need to solve the flow in 3D and this can easily be achieved in Caelus by specifying ``empty`` boundary condition for each of the two planes. This consequently allows for the flow in :math:`y` direction to be symmetry. 

.. Note::
   A velocity value of :math:`v=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`y` direction.

.. _t-bump-grid-tutorials:
.. figure:: sections/tutorial-figures/t-bump-grid-tutorials.*
   :width: 800px
   :align: center
   
   Computational grid of a two-dimensional bump in :math:`x-z` plane

In :numref:`t-bump-grid-tutorials` above, the 2D grid is shown with a distribution of 704 X 320 in :math:`x-z` directions respectively. The inset focuses the distribution in the region between :math:`0 \leq x \leq 1.5~m`. As can be seen, the distribution of the grids in the flow normal direction is finer near the wall to capture the turbulent boundary layer more accurately and it is estimated that :math:`y^+` is less than 1 for the chosen grid and therefore, no wall-function has been used and the integration is carried out up to the wall.

Problem definition
````````````````````
This section deals with several key instructions need to set-up the turbulent flow over a bump. A full working case of this can be found in:

.. code-block:: bash
		
   /tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/

The user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**

.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned


In order to set-up the problem Caelus requires ``time``, ``constant`` and ``system`` sub-directories within the main working directory. Typically, the simulations are started at time :math:`t = 0~s`, which requires a time sub-directory to be ``0``. 

Within the ``0`` sub-directory, additional files specifying the boundary properties are present. The below table lists the necessary files required based
on the turbulence model chosen

+-------------------------------------------------+------------------------------------------------------+
| Parameter                                       | File name                                            |
+=================================================+======================================================+
| Pressure (:math:`p`)                            | ``p``                                                |
+-------------------------------------------------+------------------------------------------------------+
| Velocity (:math:`U`)                            | ``U``                                                |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent viscosity (:math:`\nu`)               | ``nut``                                              |
+-------------------------------------------------+------------------------------------------------------+
| Turbulence field variable (:math:`\tilde{\nu}`) | ``nuTilda`` (Only for SA model)                      |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent kinetic energy (:math:`k`)            | ``k`` (Only for :math:`k-\omega~\rm{SST}` model)     |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent dissipation rate (:math:`\omega`)     | ``omega`` (Only for :math:`k-\omega~\rm{SST}` model) |
+-------------------------------------------------+------------------------------------------------------+

In this tutorial, we will be considering two turbulence models namely, Spalart-Allmaras (SA) and :math:`k-\omega` - Shear Stress Transport (:math:`\rm{SST}`). The contents of the files listed above sets the dimensions, initialisation and boundary conditions to the defining problem, which also
forms three principle entries required. Firstly, we begin with looking at these files in the ``0`` sub-directory 

The user should take into account that Caelus is case sensitive and therefore the directory set-up should be identical to what is shown here.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Referring back to :numref:`t-bump-domain-tutorials`, the following are the boundary conditions that will be specified:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-bump-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-bump-SST-conditions`)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k =<<0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:
 
     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 69.436113` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-bump-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-bump-SST-conditions`)

We begin with ``p``, the pressure file using a text editor with the following content 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/p
    

From the above information, it can be seen that the file begins with a dictionary named ``FoamFile`` which contains the standard set of keywords for version, format, location, class and object names. The explanation of the principle entries are as follows

* ``dimension``
	-  is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	-  is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	-  is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches. 

Similarly, we have the file ``U`` with the following information 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/U
    

As with the pressure, the principle entries for velocity field are self-explanatory and the dimensions are typically for that of velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Since the initialisation of the flow is with a uniform freestream velocity, we should set the ``internalField`` to ``uniform (69.4361 0 0)`` representing three components of velocity. In a similar manner, ``inflow`` boundary patch has three velocity components.

In addition to ``p`` and ``U``, the turbulent properties are also needed at the boundary patches and these can be set in a similar process. We begin with the file ``nut``, which corresponds to turbulent kinematic viscosity as shown below. 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/nut
    

Here, the turbulent viscosity is specified as kinematic and therefore the units are :math:`m^2/s` (``[0 2 -1 0 0 0 0]``). The value of turbulence viscosity at freestream, specified at inflow patch is calculated as detailed in :ref:`turbulent-bump-SA-conditions` and :ref:`turbulent-bump-SST-conditions` for SST models respectively and is specified accordingly. The same value also goes for ``internalField``. Note that a ``fixedValue`` of ``0`` is used for the ``wall`` which suggests that on the wall, it is only the molecular (laminar) viscosity that prevails.

The next variable is the ``nuTilda`` which is a turbulence field variable, specific to only SA model and has the same units (``[0 2 -1 0 0 0 0]``) as kinematic turbulent viscosity. The details of which are given in :ref:`turbulent-bump-SA-conditions`. The following contents given in the file ``nuTilda`` and the entries specified for the ``boundaryField`` are identical to that of turbulent kinematic viscosity explained above.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/nuTilda
    

We now proceed to files ``k`` and ``omega``, specific to only :math:`k-\omega~\rm{SST}` model. As we know, :math:`k-\omega~\rm{SST}` is a turbulence model which solves for the turbulent kinetic energy and the specific rate of dissipation using two partial differential equations. Caelus therefore requires information about these to be specified at the boundary patches when this model is chosen as shown below.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/k
    

The unit of kinetic energy is :math:`m^2/s^2` and this is set in ``dimensions`` as ``[0 2 -2 0 0 0 0]``. As with other turbulent quantities discussed above, the value of :math:`k` (refer :ref:`turbulent-bump-SST-conditions`) needs to be specified for ``internalField``, ``inflow`` and ``wall``. Please note that for wall ``boundaryField`` with no wall-function, a small, non-zero ``fixedValue`` is required.

We now evaluate the value for :math:`\omega` in the ``omega`` file as shown below and as detailed in :ref:`turbulent-flat-plate-SST-conditions`.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/0/omega
    

The unit of specific rate of dissipation for :math:`\omega` is :math:`1/s` which is set in ``dimensions`` as ``[0 0 -1 0 0 0 0]``. The ``internalField`` and ``inflow`` gets a ``fixedValue``. Note that for wall ``boundaryField``, we specify ``omegaWallFunction`` and this is a model requirement and sets omega to the correct value near wall based on the :math:`y^+`. In conjunction, the value that goes with ``omegaWallFunction`` can be anything and for simplicity its set to ``1``.

Before proceeding further, it is important to ensure that the boundary conditions (``inflow``, ``outflow``, ``top``, etc) added in the above files should be the grid boundary patches (surfaces) generated by the grid generation tool and their names are identical. Additionally, the two boundaries :math:`x-z` plane named here as ``left`` and ``right`` have ``empty`` boundary conditions which forces Caelus to assume the flow to be in two-dimensions. With this, the setting up of the boundary conditions are complete.

*Grid file and Physical Properties*

The grid file for the turbulent-bump need to be placed in ``constant/polyMesh`` sub-directory. In addition to this, the physical properties are specified in various different files present in the ``constant`` directory. The three files that are listed in addition to the ``polyMesh`` sub-directory set the physical properties. The first one, ``RASProperties`` in which the Reynolds-Average-Stress (RAS) is specified, is shown below. Please note that depending on the turbulence model you wish to run with, the line that corresponds to that specific model should be enabled.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/constant/RASProperties
    

Next, kinematic viscosity is specified in the ``transportProperties`` file, as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/constant/transportProperties
    
As the viscous behaviour is Newtonian, the ``transportModel`` is given using the keyword ``Newtonian`` and the value of molecular (laminar) kinematic viscosity (``nu``) is given having the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, which sets the ``simulationType`` to ``RASModel``. Both SA and :math:`k-\omega~\rm{SST}` are classified as Reynolds Average Stress (RAS) models.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/constant/turbulenceProperties
    

*Controls and Solver Attributes*

This section details the files require to control the simulation and the specifying discretization methods in addition to the linear solver settings. These files are placed in the ``system`` directory. 

The ``controlDict`` file contains the following details

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/system/controlDict 
   

Referring to the above information, some explanation is needed. Here, ``simpleSolver`` is used and the simulation begins at ``t = 0 s``. This now explains the logical need for having a ``0`` directory where the data files are read at the beginning of the run, which is ``t = 0 s`` in this case. Therefore, the keyword ``startFrom`` is set to ``startTime``, where ``startTime`` would be ``0``. The simulation would be carried out as steady-state and therefore we require to specify the total number of iterations as a keyword for ``endTime``. Through the ``writeControl`` and ``writeInterval`` keywords, the solution intervals at which they are saved can be specified. Also note that a function object to obtain the force over the wall for every 50 iterations is included. In order to obtain this, a freestream density (`rhoInf`) need to be specified.

The discretization schemes for the finite volume discretization that will be used should be set through the ``fvSchemes`` file show below 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/system/fvSchemes 
    

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/system/fvSolution 
    

The user should note that in the ``fvSolution`` file, different linear solvers are used to solve for velocity, pressure and turbulence quantities. We also set the ``nNonOrthogonalCorrectors`` to 1 for this case. To ensure the stability of the solution, the relaxation is set to primary and turbulent variables. The ``relTol`` is set to ``0`` unlike a time-accurate set-up as we are solving for a steady-state solution and a very low (:math:`\approx 0`) tolerance at every iteration is unnecessary. Since the entire system of equations converge to a global set ``tolerance`` the convergence would occur as the solution progresses to a steady state.

With this, the set-up of the directory structure with all the relevant files are complete and the directory tree should appear identical to the one shown below 

.. code-block:: bash
	
   .
   ├── 0
   │   ├── epsilon
   │   ├── k
   │   ├── nut
   │   ├── nuTilda
   │   ├── omega
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
       ├── DecomposeParDict	
       ├── fvSchemes	
       └── fvSolution

Execution of the solver
````````````````````````
It is always important to renumber and to do a quality check on the grid/mesh before executing the solver. Renumbering reduces the matrix bandwidth whereas the quality check shows the mesh statistics. These two can be performed by executing the following commands from the top working directory

.. code-block:: bash

   caelus run -- renumberMesh -overwrite 
   caelus run -- checkMesh

At this stage, it is suggested that the user should take note of the bandwidth before and after the mesh renumbering. When the ``checkMesh`` is performed, the mesh statistics are shown as below 


.. literalinclude:: sections/tutorial-figures/cc_checkmesh.txt

The output of the ``checkMesh`` indicates that the mesh check has failed through the final message``Failed 1 mesh checks.`` and this is because of the high aspect ratio meshes present immediate to the wall due to very low (:math:`<< 1~y^+`). However, Caelus will solve on this mesh.

In this tutorial, it will be shown further to utilise the multi-core capability of CPUs thus performing a parallel computation for large grids, such as the one considered here. At first the grid has to be decomposed into smaller pieces that can be solved by each single CPU core. The number of 	decomposition should be equal to the number of CPU core available for parallel computing. The decomposition should be carried out through a file ``decomposeParDict`` present in the ``system`` sub-directory with the following content,

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_bump2D/system/decomposeParDict
    

In the above file, the keyword ``numberOfSubdomains`` defines the number of decomposed sub-domains. In this case, the grid is partitioned into 4 sub-domains. We use ``simple`` as the method of decomposition and ``n`` is used to specify the number of decomposition that should be carried out in x, y and z directions respectively. In this case ``(4 1 1)`` performs 4 decompositions in x direction and 1 in both y and z directions. The execution to decompose the grid is carried out again from the top directory as follows

.. code-block:: bash

   caelus run -- decomposePar

Now the decomposition should begin and the details of which are displayed in the terminal window. Subsequently, 4 processor directories will be generated as shown below

.. code-block:: bash

    0  constant  processor0  processor1  processor2  processor3  system

The solver can now be executed for parallel computation in the host machine from the top directory using the following command

.. code-block:: bash

   caelus run -p -l my-turbulent-bump.log -- simpleSolver 

Note that here it is assumed that the parallel computing is available in the host machine. With the execution of the above commands, the simulation begins and the progress is written to the specified log file (``my-turbulent-bump.log``).

The log file can be further processed to look at the convergence history and this can be done as follows

.. code-block:: bash

   caelus logs -w my-turbulent-bump.log


This allows you to look at the convergence of the variables with respect to the number of iterations carried out and the :numref:`t-bump-convergence-tutorials` indicates the same for pressure.

.. _t-bump-convergence-tutorials:
.. figure:: sections/tutorial-figures/t-bump-convergence-tutorials.*
   :width: 400px
   :align: center
		
   Convergence of pressure with respect to iterations

Results
````````

The flow visualisation over the bump is shown here through the contours of velocity and pressure for SA model. In :numref:`t-bump-velocitypressure-tutorials` the variation of velocity and pressure can be seen as the bump is approached. As expected due to the curvature, flow accelerates while pressure reduces over the bump.

.. _t-bump-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/t-bump-velocitypressure-tutorials.*
   :width: 750px
   :align: center
		
   Contours of velocity and pressure over the bump surface

















































