Turbulent Flat Plate
--------------------

This tutorial considers the simulation of :index:`turbulent` incompressible flow over a two-dimensional sharp leading-edge flat plate using Caelus |version|. Some basic steps to start a Caelus simulation for a turbulent flow environment will be shown such as specifying input data to define the boundary conditions, fluid properties, turbulence parameters and discretization/solver settings. Subsequently, the velocity contour over the plate will be visualised to identify the developed boundary layer. It will be further shown in sufficient detail to carry out Caelus simulation so that the user is able to reproduce accurately.

Objectives
````````````
Through this tutorials the user will be familiarised with setting up the Caelus simulation for steady, turbulent, incompressible flow over a sharp leading-edge flat-plate in two-dimensions. Further, the user will also be able to visualise the boundary layer. The following steps are carried out in this tutorial

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
	- Visualisation of turbulent boundary layer

Pre-requisites
````````````````
It is assumed that the user is familiar with the Linux command line environment using a terminal or Caelus-console (for Windows OS) and Caelus is installed correctly with appropriate environment variables set. The grid used here is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__ in a Plot3D format and is exported to Caelus format using `Pointwise <http://www.pointwise.com/>`_. However, the user is free to use their choice of grid generation tool to covert the Plot3D file to Caelus format.

Background
````````````
Turbulent flow over a flat-plate configuration presents an ideal case to introduce the user with the turbulent simulation using Caelus. Here, the steady-state solution to the incompressible flow over the plate will be obtained, which results in a turbulent boundary layer. The shear stress distribution along the length of the wall and the velocity profile across the wall would be used to infer the development of the turbulent boundary layer. The user can look at the validation section for more details at :ref:`turbulent-flat-plate-verification-label`.

The flat-plate length considered for this tutorial is L = 2.0 m and with a unit Reynolds number of :math:`5 \times 10^6`. Air is used as a fluid and a temperature of T = 300 K is assumed. Based on the Reynolds number and temperature, kinematic viscosity evaluates to :math:`\nu = 1.38872\times10^{-5}~(m^2/s)`. A freestream velocity of :math:`U = 69.436113~m/s` is used. In :numref:`tfpschematictutorials`, a schematic of the flat-plate is shown. Note that the 2D plane of interest is in :math:`x-z` directions.

.. _tfpschematictutorials:
.. figure:: sections/tutorial-figures/t-fp-schematic-tutorials.*
   :width: 500px
   :align: center

   Schematic of the flat-plate flow

The freestream conditions that would be used is given in the below table

.. _t-fp-freestream-conditions:
.. table:: Freestream conditions

   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+
   | Fluid      | :math:`L~(m)`  | :math:`Re/L~(1/m)`     | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)` | :math:`T~(K)` | :math:`\nu~(m^2/s)`           |
   +============+================+========================+=====================+=====================+===============+===============================+
   | Air        | 0.3048         | :math:`5 \times 10^6`  | 69.436113           | Gauge (0)           | 300           |:math:`1.38872\times10^{-5}`   |
   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+

Grid Generation
````````````````

The hexahedral grid used in this tutorial is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate_grids.html>`__ that has 137 X 97 cells in :math:`x-z` directions respectively. The original 3D grid is in Plot3D and is then converted to Caelus compatible ``polyMesh`` format.

The computational domain is a rectangular block that encompasses the flat-plate. In :numref:`t-fp-domain-tutorials` below, the details of the boundaries in 2D (:math:`x-z` plane) that will be used is shown. The region of interest, which is highlighted in blue extends between :math:`0 \leq x \leq 2.0~m`, where the leading-edge is at :math:`x=0`. Due to the viscous nature of the flow, the velocity at the wall is zero which is represented through a no-slip boundary wherein :math:`u,v,w = 0`. Upstream of the leading edge, a symmetry boundary at the wall will be used. The inlet boundary is placed at the start of the symmetry boundary and the outlet is placed at the exit of the flat-plate the no-slip wall. The entire top boundary will be again modelled as a symmetry plane.

.. _t-fp-domain-tutorials:
.. figure:: sections/tutorial-figures/t-fp-domain-tutorials.*
   :width: 500px
   :align: center
		
   Flat-plate computational domain

The ``polyMesh`` grid as noted earlier is in 3D. However, since the flow over a flat-plate is two-dimensional, the 2D plane that is considered here is in :math:`x-z` directions. It would therefore be ideal to have *one-cell* thick in the direction (:math:`y`), normal to the 2D plane of interest, where the flow is considered symmetry. The two :math:`x-z` planes obtained as a result of having 3D grid need boundary conditions to be specified. Since the flow is 2D, we do not need to solve for flow in 3D. This can easily be achieved in Caelus by specifying *empty* boundary condition for each of the two planes. As a consequence, the flow in :math:`y` direction would be symmetry.

.. Note::
   A velocity value of :math:`v=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`y` direction.

.. _t-fp-grid-tutorials:
.. figure:: sections/tutorial-figures/t-fp-grid-tutorials.*
   :width: 800px
   :align: center
   
   Flat-plate computational grid in :math:`x-z` plane

In :numref:`t-fp-grid-tutorials`, the 2D grid is shown which has 137 X 97 cells in :math:`x-z` directions respectively. To capture the turbulent boundary layer accurately, the grids are refined close to the wall and :math:`y^+` is estimated to be less than 1. Due to this, no wall-functions would be used to estimate the velocity gradients near the wall and integration is carried up to the wall.

Problem definition
````````````````````

In this section, several key instructions would be provided to set-up the turbulent flat-plate problem along with details of file configuration. A full working case can be found in:

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D

However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**

.. note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned

For setting up the problem, we need to further have few more sub-directories where relevant files can be created. Caelus requires ``time``, ``constant`` and ``system`` sub-directories. Since we will begin the simulation at time :math:`t = 0~s`, the `time` sub-directory should be just ``0``. 

In the ``0`` sub-directory, additional files are required for specifying the boundary properties. The following table lists the necessary files required based on the turbulence model chosen.

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

As can be noted from the above table, we will be considering two turbulence models namely, Spalart-Allmaras (SA) and :math:`k-\omega` - Shear Stress Transport (:math:`\rm{SST}`) in the current exercise. These files set the dimensions, initialisation and boundary conditions to the problem, which also forms the three principle entries required. 

The user should take into account that Caelus is case sensitive and therefore where applicable, the directory set-up should be identical to what is shown here.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Initially, let us set-up the boundary conditions. Referring back to Fig. %s:num:`t-fp-domain-tutorials`, the following are the boundary conditions that will be specified:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-flat-plate-SA-conditions`)
     - :math:`k-\omega~\textrm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-flat-plate-SST-conditions`)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
     - :math:`k-\omega~\textrm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\textrm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 69.436113` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-flat-plate-SA-conditions`)
     - :math:`k-\omega~\textrm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-flat-plate-SST-conditions`)

Starting with the pressure, let us open ``p`` using a text editor, which has the following contents.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/p
    

As can be seen, the above file begins with a dictionary named ``FoamFile`` which contains the standard set of keywords for version, format, location, class and object names. 

* ``dimension``
	- is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	- is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	- is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.

In a similar approach, let us open the file ``U``.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/U
    

As detailed above, the principle entries for velocity field are self explanatory and the dimensions are typically for that of velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Since we initialise the flow with a uniform freestream velocity, we set the ``internalField`` to ``uniform (69.4361 0 0)`` which represents three components of velocity. Similarly, ``inflow`` boundary patch has three velocity components.

Similarly, the turbulent properties needed at the boundaries can be set. We begin with opening the file ``nut``, which is the turbulent kinematic viscosity and is shown below.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/nut
    

Here, the turbulent viscosity is specified as kinematic and therefore the units are :math:`m^2/s` (``[0 2 -1 0 0 0 0]`` ). The value of turbulence viscosity at freestream, specified at inflow patch is calculated as detailed in :ref:`turbulent-flat-plate-SA-conditions` and :ref:`turbulent-flat-plate-SST-conditions` for SST models respectively and is specified accordingly. The same value also goes for ``internalField``. Note that a ``fixedValue`` of ``0`` is used for the ``wall`` which suggests that on the wall, it is only the molecular (laminar) viscosity that prevails.

We shall now look at ``nuTilda`` which is a turbulence field variable, specific to the SA model and has same units (``[0 2 -1 0 0 0 0]``) as kinematic turbulent viscosity. The details of which are given in :ref:`turbulent-flat-plate-SA-conditions`. In the file ``nuTilda``, the entries specified for the ``boundaryField`` are identical to that of turbulent kinematic viscosity explained above.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/nuTilda
    

We now proceed to files ``k`` and ``omega``, specific to only :math:`k-\omega~\rm{SST}` model. As we know, :math:`k-\omega~\rm{SST}` is a turbulence model which solves for the turbulent kinetic energy and the specific rate of dissipation using two partial differential equations. Caelus therefore requires information about these to be specified when this model is used. Firstly, the file ``k`` with the following  contents is needed.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/k
    

The unit of kinetic energy is :math:`m^2/s^2` and this is set in ``dimensions`` as ``[0 2 -2 0 0 0 0]``. As with other turbulent quantities discussed above, the value of :math:`k` (refer :ref:`turbulent-flat-plate-SST-conditions` needs to be specified for ``internalField``, ``inflow`` and ``wall``. Please note that for wall ``boundaryField`` with no wall-function, a small, non-zero ``fixedValue`` is required.

Next, the value for :math:`\omega` is evaluated in ``omega`` file as shown below and as detailed in :ref:`turbulent-flat-plate-SST-conditions`.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/0/omega
    

The unit of specific rate of dissipation for :math:`\omega` is :math:`1/s` which is set in ``dimensions`` as ``[0 0 -1 0 0 0 0]``. The ``internalField`` and ``inflow`` gets a ``fixedValue``. Note that for wall ``boundaryField``, we specify ``omegaWallFunction`` and this is a model requirement and sets omega to the correct value near wall based on the :math:`y^+`. In conjunction, the value that goes with ``omegaWallFunction`` can be anything and for simplicity its set to ``1``.

Before setting up other parameters, it is important to ensure that the boundary conditions (``inflow``, ``outflow``, ``top``, etc) specified in the above files should be the grid boundary patches (surfaces) generated by the grid generation tools and their names are identical. Further, the two boundaries in :math:`x-z` plane named here as ``left`` and ``right`` have ``empty`` boundary conditions which forces Caelus to assume the flow to be in 2D. With this, the setting up of boundary conditions are completed.

*Grid file and Physical Properties*

The turbulent flat-plate grid files is placed in the ``constant/polyMesh`` sub-directory. Additionally, the physical properties are specified in various different files present in the directory ``constant``. 

As you can see in the ``constant`` directory, three files are listed in addition to the ``polyMesh`` sub-directory. In the first file, ``RASProperties``, the Reynolds-Average-Stress (RAS) model is specified as below. Note that depending on the turbulence model you wish to run with, the line that corresponds to that specific model should be enabled

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/constant/RASProperties
    

Next, we look at the ``transportProperties`` file, where transport model and kinematic viscosity is specified. 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/constant/transportProperties
    

As the viscous behaviour is Newtonian, the ``transportModel`` is given using the keyword ``Newtonian`` and the value of molecular (laminar) kinematic viscosity (``nu``) is given having the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, which sets the ``simulationType`` to ``RASModel``. Both SA and :math:`k-\omega~\rm{SST}` are classified as Reynolds Average Stress (RAS) models.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/constant/turbulenceProperties
    

*Controls and Solver Attributes*

In this section, the files required to control the simulation and specifying the type of discretization method along with the linear solver settings are provided. These are placed in the ``system`` directory. 

First, we begin with the ``controlDict`` file as below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/system/controlDict
    

As can be noted in the above file, ``simpleSolver`` solver is used and the simulation begins at ``t = 0 s``. This logically explains the need for ``0`` directory where the data files are read at the beginning of the run. Therefore, the keyword ``startFrom`` is set to ``startTime``, where ``startTime`` would be ``0``. Since the simulation is steady-state we specify the total number of iterations as a keyword for ``endTime``. Via the ``writeControl`` and ``writeInternal`` keywords, the interval at which the solutions are saved can be specified. Also included is the function object to obtain the force over the wall every ``50`` iterations. Note that for obtaining the force, the freestream density (``rhoInf``) is required and is specified with the value.

The discretization schemes for the finite volume discretization that will be used is set through the ``fvSchemes`` file shown below 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/system/fvSchemes
    

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_flatPlate2D/system/fvSolution
    

Here, different linear solvers are used to solve velocity, pressure and turbulence quantities. We also set the ``nNonOrthogonalCorrectors`` to 1 in this case. Further, relaxation is set on the primary and turbulent variables so that the solution is more stable. Furthermore, the ``relTol`` is not set to ``0`` unlike a time-accurate set-up. This is because we are solving for a steady-state solution and a very low (:math:`\approx 0`) tolerance at every iteration is not required as the entire system of equations converges to the global ``tolerance`` set as the simulation progresses to steady state.

Now the set-up of the directory structure with all the relevant files the directory tree should appear identical to the one shown below

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
       ├── fvSchemes
       └── fvSolution

Execution of the solver
````````````````````````
Prior to execution of solver, renumbering of the grid/mesh needs to be performed in addition to checking the quality of the grid/mesh. Renumbering the grid-cell list is vital to reduce the matrix bandwidth while quality check gives us the mesh statistics. Renumbering and mesh quality can be determined by executing the following from the top directory.

.. code-block:: bash

   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh


At this stage, it is suggested that the user should take note of the matrix bandwidth before and after the mesh renumbering. When the ``checkMesh`` is performed, the mesh statistics are shown as below


.. literalinclude:: sections/tutorial-figures/t_fp_checkmesh.txt

In the above terminal output, we get ``Failed 1 mesh checks.`` and this is because of the high aspect ratio meshes present immediate to the wall due to very low (:math:`<< 1~y^+`). However, Caelus can solve on this mesh. The next step is to execute the solver and monitoring the progress of the solution. The solver is always executed from the top directory. 

.. code-block:: bash
		
   caelus run -l my-turbulent-flat-plate.log -- simpleSolver 

With the execution of the above command, the simulation begins and the progress of the solution is written to the specified log file (``my-turbulent-flat-plate.log``). The log file can be further processed to look at the convergence history and this can be done as follows

.. code-block:: bash

   caelus logs -w my-turbulent-flat-plate.log

This allows you to look at the convergence of different variables with respect to the number of iterations carried out. In Fig. %s:num:`tfpconvergencetutorials` pressure convergence is shown.

.. _tfpconvergencetutorials:
.. figure:: sections/tutorial-figures/t-fp-convergence-tutorials.*
   :width: 400px
   :align: center

   Convergence of pressure with respect to iterations

Results
````````

The turbulent flow over the flat plate is shown here through velocity magnitude contours for SA model. In Fig. %s:num:`tfpvelocitytutorials` the boundary layer over the entire flat-plate and in the region up to :math:`x=0.10~m` is emphasised. The growth of the boundary layer can be seen very clearly. Since the Reynolds number of the flow is reasonably high, the turbulent boundary layer seems thin in comparison to the length of the plate.


.. _tfpvelocitytutorials:
.. figure:: sections/tutorial-figures/t-fp-velocity-tutorials.*
   :width: 700px
   :align: center

   Contour of velocity magnitude over the flat-plate























































 





