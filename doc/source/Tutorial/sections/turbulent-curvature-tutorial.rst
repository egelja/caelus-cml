Convex Curvature
-----------------

Turbulent flow in a constant-area duct having a convex curvature is considered in this tutorial. Caelus |version| will be used and the process of setting-up of directory structure, fluid properties, boundary conditions, turbulent properties, etc will be explained here. In addition to this, the flow within the duct will be visualised. The steps would be sufficient for the user to reproduce the tutorial accurately.

Objectives
````````````

Through this tutorial, the user will get familiarise with setting up the Caelus simulation for steady, turbulent, incompressible flow in a two-dimensional duct having a convex curvature and subsequently post-process the results. The steps that would be followed in this tutorial is outlined below

* Background
   -  A brief description about the problem
   -  Geometry and inflow details

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
   -  Visualising the flow within the convex duct

Pre-requisites
````````````````

It will be assumed that the user is comfortable and familiar with the Linux command line environment using a terminal or Caelus-console (on Windows OS). The user should also make sure that Caelus is installed correctly with appropriate environment variables set. The grid used here is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/smitscurve_val.html>`__ in Plot 3D format and is exported to Caelus using `Pointwise <http://www.pointwise.com/>`_. The user can use their choice of grid generation tool to convert Plot3D file to Caelus format.

Background
````````````
Turbulent flow in a constant-area duct with a curvature is an interesting case. Here as a result of a curvature, pressure gradients occur in the vicinity of the curvature having localised effect. The flow will be assumed as steady-state and incompressible. Non-dimensional shear-stress (skin-friction coefficient) will be used to show the influence of curvature on the flow. Validation and verification of this exercise is detailed in section :ref:`turbulent-curvature-verification-label` and the user is suggested to refer for more information.

The inlet of the duct as can be seen from the schematic below in :numref:`t-curvature-schematic-tutorials` has an inclination of :math:`\alpha = 30^\circ`. This is followed by a rapid bend at the same angle, :math:`\alpha` after a distance of about 1.4 m. The downstream extends to 1.6 meters. The inflow has a Reynolds number of :math:`2.1 \times 10^6`, with Air as the fluid. The temperature of the inflow is at 293 K and U is the inlet velocity. Based on the Reynolds number, temperature and velocity, the kinematic viscosity is evaluated to :math:`\nu = 1.519470 \times 10^{-5}~m^2/s`. The geometric-plane for this case in 2-D is :math:`x-z` plane.

.. _t-curvature-schematic-tutorials:
.. figure:: sections/tutorial-figures/t-curvature-schematic-tutorials.*
   :width: 500px
   :align: center

   Schematic representation of the 2D curvature geometry

The inflow conditions for this case is given in the below table (:ref:`t-curvature-freestream-conditions`)

.. _t-curvature-freestream-conditions:
.. table:: Freestream conditions

   +------------+--------------------------+---------------------+---------------------+---------------+-------------------------------+
   | Fluid      | :math:`Re/L~(1/m)`       | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)` | :math:`T~(K)` | :math:`\nu~(m^2/s)`           |
   +============+==========================+=====================+=====================+===============+===============================+
   | Air        | :math:`2.1 \times 10^6`  | 31.9088             | Gauge (0)           | 293           |:math:`1.5194\times10^{-6}`    |
   +------------+--------------------------+---------------------+---------------------+---------------+-------------------------------+

To achieve a inlet velocity of U = 31.9088 m/s at :math:`\alpha = 30^\circ`, the velocity components in :math:`x` and :math:`z` are resolved. These are given in the below table


.. _t-curvature-alpha-conditions:
.. table:: Velocity components in x and z directions

   ======================  ===================    =================
   :math:`\alpha`          :math:`u~(m/s)`        :math:`w~(m/s)`
   ======================  ===================    =================
   :math:`30^\circ`         27.63313              15.95443
   ======================  ===================    =================

Grid Generation
````````````````

The structured grid for this case has been obtained from from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/smitscurve_grids.html>`__ in a Plot3 format which contains 512 cells in streamwise direction and 192 in the flow normal direction. The Plot3D file has to be converted to ``polyMesh`` format.

In :numref:`t-curvature-domain-tutorials`, the computational domain can be seen which as expected follows the geometry. The velocity at the internal walls (highlighted in blue) are zero, wherein :math:`u, v, w =0` representing a no-slip boundary. The inlet and the outlet are applied to the start and end of the domain respectively.

.. _t-curvature-domain-tutorials:
.. figure:: sections/tutorial-figures/t-curvature-domain-tutorials.*
   :width: 500px
   :align: center

   Computational domain of the convex curvature

The grid that is used, ``polyMesh`` format is in three-dimensions. However it is assumed that the flow through the duct can be modelled as 2D and is the 2D plane considered for this case is :math:`x-z` directions. As with all the previous cases, one-cell thick normal to the 2D plane is sufficient, assuming the flow to be symmetry. The two :math:`x-z` planes that are present in the grid require boundary conditions to be specified. An empty boundary condition can be used in Caelus for the two planes that forces the solver not to solve in the third-dimension, essentially treating the flow as symmetry in :math:`y` direction.

.. Note::
   A velocity value of :math:`v=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`y` direction.

.. _t-curvature-grid-tutorials:
.. figure:: sections/tutorial-figures/t-curvature-grid-tutorials.*
   :width: 800px
   :align: center
   
   Convex curvature grid in two-dimensions in :math:`x-z` plane

The 2D grid in :math:`x-z` plane is shown in :numref:`t-curvature-grid-tutorials` having a distribution of 512 X 192 cells. The inset in the figure highlights the region vicinity of the curvature and very fine distribution of cells can be seen close to the wall. It is estimated that :math:`y^+` is less than 1 in order to capture turbulent boundary layer accurately and thus no wall-function is used.

Problem definition
````````````````````

This section details the various steps required to set-up the turbulent flow inside a convex duct. A full working case of this can be found in:

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D

**Directory Structure**

.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned


A few more sub-directories are needed in the top-level directory to set-up the case. Caelus requires ``time``, ``constant`` and ``system`` sub-directories within the main ``my-turbulent-curvature`` working directory. Since we start the simulation at time, t =0 s, a ``time`` sub-directory named ``0`` is required. A

The ``0`` sub-directory cotains the files in which boundary properties are specified. In the below table, the list of necessary files are provided based on the turbulence model chosen 

+-------------------------------------------------+------------------------------------------------------+
| Parameter                                       | File name                                            |
+=================================================+======================================================+
| Pressure (:math:`p`)                            | ``p``                                                |
+-------------------------------------------------+------------------------------------------------------+
| Velocity (:math:`U`)                            | ``U``                                                |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent viscosity (:math:`\nu`)               | ``nut``                                              |
+-------------------------------------------------+------------------------------------------------------+
| Turbulence field variable (:math:`\tilde{\nu}`) | ``nuTilda`` (Only for SA & SA-CC model)              |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent kinetic energy (:math:`k`)            | ``k`` (Only for :math:`k-\omega~\rm{SST}` model)     |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent dissipation rate (:math:`\omega`)     | ``omega`` (Only for :math:`k-\omega~\rm{SST}` model) |
+-------------------------------------------------+------------------------------------------------------+

We consider simulating this case with three turbulence models, namely Spalart-Allmaras (SA), Spalart–Allmaras Rotational/Curvature (SA-RC) and :math:`k-\omega` - Shear Stress Transport (:math:`\rm{SST}`). The contents that are entered in these files set the dimensions, initialisation and boundary conditions to the defining problem, which also form three principle entries required.

Caelus is case sensitive and therefore the user should carefully set-up the case as shown here if applicable.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Referring back to figure :ref:`t-curvature-domain-tutorials`, following are the boundary conditions that will be specified:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 27.63313~m/s`; :math:`v = 0.0~m/s` and :math:`w = 15.95443~m/s` in :math:`x, y` and :math:`z` directions respectively
   - Pressure: Zero gradient
   - Turbulence:

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t~i}` and :math:`\tilde{\nu}_{i}` as given in :ref:`turbulent-curvature-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{i}`, :math:`\omega_{i}` and :math:`\nu_{t~i}` as given in :ref:`turbulent-curvature-SST-conditions`)

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu} =0`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k = <<0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)
    
* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - SA & SA-RC (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 27.63389~m/s`; :math:`v = 0.0~m/s` and :math:`w = 15.95443~m/s` in :math:`x, y` and :math:`z` directions respectively
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t~i}` and :math:`\tilde{\nu}_{i}` as given in :ref:`turbulent-curvature-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{i}`, :math:`\omega_{i}` and :math:`\nu_{t~i}` as given in the above table) as given in :ref:`turbulent-curvature-SST-conditions`)

First, we begin with the pressure file, ``p`` and using a text editor with the following content.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/p
  

As noted above, the file begins with a dictionary named ``FoamFile`` containing the essential set of keywords for version, format, location, class and object names. The explanation of the principle entries are as follows

* ``dimension``
	-  is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	-  is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	-  is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches.

Similarly, the contents for the file ``U`` is as follows

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/U
  

The principle entries for velocity field are self-explanatory and the dimensions are typically for that of velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Here, both initialisation and inlet have a uniform flow velocity specified with three velocity components. For example at :math:`\alpha = 30^\circ`, we specify ``(27.63313 0 15.954)`` for :math:`u, v, w` components respectively. Similarly, the top and wall boundary patch have three velocity components.

The turbulent properties are also required to be specified at the boundary patches and these can be done similar to ``p`` and ``U``. First, we start with opening the file ``nut``, which is turbulent kinematic viscosity having the following details

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/nut
  

In the above file, the turbulent viscosity is specified as kinematic and therefore the units are in :math:`m^2/s` (``[0 2 -1 0 0 0 0]``). The turbulent viscosity value at inlet, specified at inlet patch is calculated as detailed in :ref:`turbulent-curvature-SA-conditions` and :ref:`turbulent-curvature-SST-conditions` for SST models respectively and is specified accordingly. The same value also goes for ``internalField``. Note that a ``fixedValue`` of ``0`` is used for the ``wall`` which suggests that on the wall, it is only the molecular (laminar) viscosity that prevails.

The next property to set is the ``nuTilda``, which is the turbulent field variable, specified to only SA and SA-RC models and has the same units (``[0 2 -1 0 0 0 0]``) as kinematic turbulent viscosity. For more information about these, the user can look at :ref:`turbulent-curvature-SA-conditions`. Following are the contents of the file ``nuTilda`` and the entries specified for the ``boundaryField`` are identical to that of turbulent kinematic viscosity explained above.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/nuTilda
   

We can now set the properties of ``k`` and ``omega``, specific to only :math:`k-\omega~\rm{SST}` model. :math:`k-\omega~\rm{SST}` is a turbulence model which solves for turbulent kinetic energy and the specific rate of dissipation using two partial differential equations. Caelus therefore requires information about these to be specified at the boundary patches when this model is chosen. Starting with the file ``k``, 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/k
   

The unit of kinetic energy is :math:`m^2/s^2` and this is set in ``dimensions`` as ``[0 2 -2 0 0 0 0]``. As with other turbulent quantities discussed above, the value of :math:`k` (refer :ref:`turbulent-curvature-SST-conditions`) needs to be specified for ``internalField``, ``inlet`` and ``wall``. Please note that for wall ``boundaryField`` with no wall-function, a small, non-zero ``fixedValue`` is required.

Similarly in the file ``omega``, the value is evaluated as detailed in :ref:`turbulent-curvature-SST-conditions`

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/0/omega
   

The unit of specific rate of dissipation for :math:`\omega` is :math:`1/s` which is set in ``dimensions`` as ``[0 0 -1 0 0 0 0]``. The ``internalField`` and ``inlet`` gets a ``fixedValue``. Note that for wall ``boundaryField``, we specify ``omegaWallFunction`` and this is a model requirement and sets omega to the correct value near wall based on the :math:`y^+`. In conjunction, the value that goes with ``omegaWallFunction`` can be anything and for simplicity its set to ``1``.

At this stage it is important to ensure that the boundary conditions (``inlet``, ``outlet``, ``wall``, etc) added in the above files should be the grid boundary patches (surfaces) generated by grid generation tool and their names are identical. In addition, the two boundaries :math:`x-z` plane named here as ``left`` and ``right`` have ``empty`` boundary conditions which forces Caelus to assume the flow to be in two-dimensions. With this, the setting up of the boundary conditions are complete.

*Grid file and Physical Properties*

The grid file for the convex curvature has to be placed in ``polyMesh`` sub-directory, which is in the ``constant`` directory. Further to is, the physical properties should be specified in various different files present in the ``constant`` directory. 

.. code-block:: bash

   polyMesh  RASProperties  transportProperties  turbulenceProperties

As noted above, the three files are listed in the ``constant`` sub-directory. The first one is the ``RASProperties`` where, Reynolds-Average-Stress (RAS) model is specified as shown below. Please note that depending on the turbulence model you wish to run with, the line that corresponds to that specific model should be enabled.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/constant/RASProperties
  

Second from the list is the ``transportProperties`` file, where kinematic viscosity is specified as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/constant/transportProperties
  

The viscous behaviour is modelled as Newtonian and hence the keyword ``Newtonian`` is used for the ``transportModel`` and the molecular (laminar) kinematic viscosity (:math:`nu`) is given having the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, which sets the ``simulationType`` to ``RASModel``. Both SA and :math:`k-\omega~\rm{SST}` are classified as Reynolds Average Stress (RAS) models.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/constant/turbulenceProperties
  

*Controls and Solver Attributes*

This section will provide details and settings required to control the simulation, specifying discretisation methods and linear solver settings. These files should be saved in the ``system`` directory.

The ``controlDict`` file contains the following details

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/system/controlDict
  

With reference to the above files, some explanation is required. In this case, ``simpleSolver`` solver is used and the simulation begins at ``t = 0 s``. This now explains the logical need for having a ``0`` directory where the data files are read at the beginning of the run, which is ``t = 0 s`` for this simulation. Therefore, the keyword ``startFrom`` is set to ``startTime``, where ``startTime`` is set to ``0``. The simulation would be carried out as steady-state and therefore we require to specify the total number of iterations as a keyword for ``endTime``. Via the ``writeControl`` and ``writeInterval`` keywords, the solution intervals at which they are saved can be specified. Also note that a function object to obtain the force over the wall for every 50 iterations is included. In order to obtain this, a inlet/inflow density (`rhoInf`) need to be specified.

The discretization schemes for the finite volume discretization that will be used should be set through the ``fvSchemes`` file show below and the contents should be copied

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/system/fvSchemes 
  

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/system/fvSolution
   

In the ``fvSolution`` file, different linear solvers are used to solve for velocity, pressure and turbulence quantities and this has to be noted by the user. We also set the ``nNonOrthogonalCorrectors`` to 1 for this case. To ensure the stability of the solution, the relaxation is set to primary and turbulent variables. The ``relTol`` is not set to ``0`` unlike a time-accurate set-up as we are solving for a steady-state solution and a very low (:math:`\approx 0`) tolerance at every iteration is unnecessary. Since the entire system of equations converge to a global set ``tolerance`` the convergence would occur as the solution progresses to a steady state.

The set-up of the directory structure with all the relevant files are now complete. This can be verified again as follows and the directory tree should appear identical to the one shown below

.. code-block:: bash
	
   cd my-turbulent-curvature/
   tree
   .
   ├── 0
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
       ├── decomposeParDict
       ├── fvSchemes
       └── fvSolution


Execution of the solver
````````````````````````

Renumbering and checking the grid/mesh quality is important before the solver is executed. Renumbering reduces the matrix bandwidth whereas the quality check shows the mesh statistics. These two can be performed by executing the following commands from the top working directory

.. code-block:: bash

   cd my-turbulent-curvature/
	
   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

With the execution of ``renumberMesh -overwrite``, the user should note the reduction in bandwidth after renumbering occurs. Similarly, when the ``checkMesh`` is performed, the mesh statistics are shown as below

.. literalinclude:: sections/tutorial-figures/t_curvature_checkmesh.txt

Apparent from the above output, the ``checkMesh`` indicates that the mesh check has failed reporting in the final message as ``Failed 1 mesh checks``. This is because of the high aspect ratio meshes present immediate to the wall with very low (:math:`<< y^+`) values. Nevertheless, this is just a warning and Caelus will solve on this mesh.

We can utilise the multi-core capability of CPUs for performing a parallel computation for large grids, such as the one considered for this tutorial. Before this can be done, the grid has to be decomposed into smaller domains that can be solved by each single CPU core. The number of decomposition should be equal to the number of CPU core available for parallel computing. The decomposition should be carried out through a file ``decomposeParDict`` present in the ``system`` sub-directory which is shown below.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_convexCurvature2D/system/decomposeParDict
   

In the above file, the the keyword ``numberOfSubdomains`` defines the number of decomposed sub-domains needed and ``4`` is used which partitions the grid into 4 sub-domains. We use ``scotch`` as the method of decomposition which automatically divides the grid. The execution to decompose the grid is carried out again from the top directory as follows

.. code-block:: bash
  
   caelus run -- decomposePar

Now the decomposition should start and the details of which are displayed in the terminal window. Subsequently, 4 processor directories will be generated as we have requested for 4 divisions of grid as shown below

.. code-block:: bash
   
   0  constant  processor0  processor1  processor2  processor3  system

The solver can now be executed for parallel computation from the top directory using the following command

.. code-block:: bash

    caelus run -p -l my-turbulent-curvature.log -- simpleSolver 

Note that here it is assumed that the parallel computing is available in the host machine. With the execution of the above commands, the simulation begins and the progress of the solution is written to the specified log file (``my-turbulent-curvature.log``).

The log file can be further processed to look at the convergence history and this can be done as follows

.. code-block:: bash

   caelus logs -w my-turbulent-curvature.log

The above statement allows you to look at the convergence of variables with respect to the number of iterations carried out as shown in :numref:`t-curvature-convergence-tutorials` for pressure.

.. _t-curvature-convergence-tutorials:
.. figure:: sections/tutorial-figures/t-curvature-convergence-tutorials.*
   :width: 400px
   :align: center
		
   Convergence of pressure with respect to iterations

Results
````````

The flow visualisation of velocity and pressure within the convex duct is presented here. In :numref:`t-curvature-velocitypressure-tutorials` velocity magnitude and pressure are shown for SA model. Due to the convex bend, the thinning of the turbulent boundary layer occurs on the lower surface and the pressure decreases. Whereas the trends are opposite on the upper surface.


.. _t-curvature-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/t-curvature-velocitypressure-tutorials.*
   :width: 800px
   :align: center
		
   Velocity magnitude and pressure contours within the convex duct






























