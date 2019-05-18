NACA Airfoil
------------

In this tutorial, the turbulent flow over a two-dimensional NACA 0012 airfoil at two angles of attack, namely :math:`0^\circ` and :math:`10^\circ` will be considered. Caelus |version| will be used and the basic steps to set-up the directory structure, fluid properties, boundary conditions, turbulence properties etc will be shown. Visualisation of pressure and velocity over the airfoil are also shown. With these, the user should be able to reproduce the tutorial accurately.

Objectives
````````````

The user will get familiar in setting up Caelus simulation for steady, turbulent, incompressible flow over a two-dimensional airfoils at different angles of attack. Alongside, the user will be able to decompose the mesh on several CPUs performing a parallel simulation. Some of the steps that would be detailed are as follows

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
   -  Visualisation of flow over the airfoil

Pre-requisites
````````````````

It is understood that the user will be familiar with the Linux command line environment via a terminal or Caelus-console (For Windows OS) and Caelus is installed corrected with appropriate environment variables set. The grid for this case is obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_val.html>`__ as a Plot3D format and is converted to Caelus using `Pointwise <http://www.pointwise.com/>`__. The user is however free to use their choice of grid generation tool to convert the original Plot3D grid to Caelus readable format.

Background
````````````

Turbulent flow over airfoils is an interesting example to highlight some of the capabilities of Caelus. Here, the flow undergoes rapid expansion due to strong surface curvatures thereby inducing pressure and velocity gradients along the surface. Depending on shape of the curvature, adverse or favourable pressure gradients can exist on either side. These can be examined through surface quantities like pressure and skin-friction distributions. The user can refer to the verification and validation of this case at :ref:`turbulent-airfoil-verification-label`.

The schematic of NACA 0012 airfoil at two angles of attack are shown in :numref:`t-airfoil-schematic-tutorials` for a two-dimensional profile. A chord length (C) of 1.0 m is considered for both and has a Reynolds number of :math:`6 \times 10^6`. The flow is assumed to be Air with a freestream temperature of 300 K. Considering these values, the freestream velocity can be evaluated to U = 52.077 m/s. Note that the geometric plane considered for two-dimensionality is in :math:`x-z` directions. 

.. _t-airfoil-schematic-tutorials:
.. figure:: sections/tutorial-figures/t-airfoil-schematic-tutorials.*
   :width: 500px
   :align: center

   Schematic representation of the airfoil
   
The freestream conditions are given in the below table

.. _t-airfoil-freestream-conditions:
.. table:: Freestream conditions

   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+
   | Fluid      | :math:`C~(m)`  | :math:`Re/L~(1/m)`     | :math:`U~(m/s)`     | :math:`p~(m^2/s^2)` | :math:`T~(K)` | :math:`\nu~(m^2/s)`           |
   +============+================+========================+=====================+=====================+===============+===============================+
   | Air        | 1.0            | :math:`6 \times 10^6`  | 52.0770             | Gauge (0)           | 300           |:math:`8.6795\times10^{-6}`    |
   +------------+----------------+------------------------+---------------------+---------------------+---------------+-------------------------------+

As noted earlier, flow at two angles of attack (:math:`\alpha`) will be considered in this tutorial. In order to obtain a free-stream velocity of 52.0770 m/s at :math:`\alpha = 0^\circ` and :math:`10^\circ`, the velocity components in :math:`x` and :math:`z` have to be resolved. The following table provides these values

.. _t-airfoil-alpha-conditions:
.. table:: Velocity components in x and z directions

   ===========================     ===================    =================
   :math:`\alpha~\rm{Degrees}`     :math:`u~(m/s)`        :math:`w~(m/s)`
   ===========================     ===================    =================
   :math:`0^\circ`                 52.0770                0.0
   :math:`10^\circ`                51.2858                9.04307
   ===========================     ===================    =================
     
Grid Generation
````````````````

The structured grid used for this tutorial can cells obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_grids.html>`__ in Plot3D format that contains 512 around the airfoil and 256 cells in the flow normal direction. This should be then converted to ``polyMesh`` format.

The computational domain for the NACA 0012 airfoil is shown in :numref:`t-airfoil-domain-tutorials` along with the boundary conditions. A large domain exists around the airfoil (highlighted in blue) extending 500 chord lengths in the radial direction and the inlet condition is given  for the entire boundary highlighted in green, whereas the outlet is placed at the exit plane which is about :math:`x \approx 500~m`. The velocity on the airfoil surface is zero, wherein :math:`u, v, w = 0` represented through a no-slip boundary.

.. _t-airfoil-domain-tutorials:
.. figure:: sections/tutorial-figures/t-airfoil-domain-tutorials.*
   :width: 600px
   :align: center
      
   Computational domain of a 2D airfoil
   
The ``polyMesh`` grid is in three-dimensions, however the flow over airfoils can be assumed to be 2D at low angles of attack and is solved here in :math:`x-z` directions. Therefore, a *one-cell* thick grid normal to the 2D flow plane is sufficient, where the flow can be assumed to be symmetry. The two :math:`x-z` planes that are prevalent require boundary conditions to be specified. Since the flow is assumed to be 2D, we do not need to solve the flow in the third-dimension and this can be easily achieved in Caelus by specifying empty boundary conditions for each of the two planes. Consequently, the flow will be treated as symmetry in :math:`y` direction. 

.. Note::
   A velocity value of :math:`v=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`y` direction.

.. _t-airfoil-grid-tutorials:
.. figure:: sections/tutorial-figures/t-airfoil-grid-tutorials.*
   :width: 800px
   :align: center
   
   Computational grid of a 2D airfoil in :math:`x-z` plane

The 2D airfoil grid in :math:`x-z` plane is shown in :numref:`t-airfoil-grid-tutorials` which has a distribution of 512 X 256 cells. The grid in the vicinity of airfoil is shown as an inset and a very fine distribution can be noted very close to the wall. It was estimated that :math:`y^+` is less than 1 to capture the turbulent boundary layer accurately and no wall-function is used.

Problem definition
````````````````````

In this section, various steps needed to set-up the turbulent flow over an airfoil will be shown. A full working case of this can be found in:

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D
   
However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**


.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned


The problem requires ``time``, ``constant`` and ``system`` sub-directories within the main working directory. Here, the simulation will be started at time t = 0 s, which requires a time sub-directory named ``0``. 
      
The ``0`` sub-directory has files in which boundary properties are specified. In the below table, the list of necessary files are provided based on the turbulence model chosen 

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
| Turbulent kinetic energy (:math:`k`)            | ``k`` (Only for :math:`k-\omega~\rm{SST}`  model)    |
+-------------------------------------------------+------------------------------------------------------+
| Turbulent dissipation rate (:math:`\omega`)     | ``omega`` (Only for :math:`k-\omega~\rm{SST}` model) |
+-------------------------------------------------+------------------------------------------------------+

We will consider two turbulence models in this tutorial, namely Spalart-Allmaras (SA) and :math:`k-\omega` - Shear Stress Transport (:math:`\rm{SST}`). The contents of the files listed above sets the dimensions, initialisation and boundary conditions to the defining problem, which also forms three principle entries required. 

The user should note that Caelus is case sensitive and therefore the directory and file set-up should be identical to what is shown here.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Referring back to :numref:`t-airfoil-domain-tutorials`, the following are the boundary conditions that will be specified:

* Inlet
   - Velocity:

     - :math:`\alpha=0^\circ`: Fixed uniform velocity :math:`u = 52.0770~m/s`; :math:`v = w = 0.0~m/s` in :math:`x, y` and :math:`z` directions respectively
     - :math:`\alpha=10^\circ`: Fixed uniform velocity :math:`u = 51.2858~m/s`; :math:`v = 0.0~m/s` and :math:`w = 9.04307~m/s` in :math:`x, y` and :math:`z` directions respectively

   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-airfoil-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-airfoil-SST-conditions`)
          
* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed unifSpalart–Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k = <<0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)
        
* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )
        
* Initialisation
   - Velocity:

     - :math:`\alpha=0^\circ`: Fixed uniform velocity :math:`u = 52.0770~m/s`; :math:`v = w = 0.0~m/s` in :math:`x, y` and :math:`z` directions respectively
     - :math:`\alpha=10^\circ`: Fixed uniform velocity :math:`u = 51.2858~m/s`; :math:`v = 0.0~m/s` and :math:`w = 9.04307~m/s` in :math:`x, y` and :math:`z` directions

   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-airfoil-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-airfoil-SST-conditions`)

 
First, the pressure file named ``p`` has the following contents

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/p
    

In the information shown above, it can be seen that the file begins with a dictionary named ``FoamFile`` which contains the standard set of keywords for version, format, location, class and object names. The explanation of the principle entries are as follows

* ``dimension``
	-  is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]`` 

* ``internalField``
	-  is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	-  is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches. 

Similarly, the file ``U`` is defined as follows,

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/U
    

The principle entries for velocity field are self-explanatory and the dimensions are typically for that of velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). The user should note that appropriate entry has to be enabled for both ``internalField`` and ``inlet`` ``boundaryField`` depending on which angle of attack (AOA/:math:`\alpha`) is being simulated. Here, both initialisation and inlet have a uniform flow velocity specified with three velocity components. For example at :math:`\alpha = 10^\circ`, we specify ``(51.2858 0 9.04307)`` for :math:`u, v, w` components respectively. Similarly, the wall boundary patch have three velocity components.

The turbulent properties are also required to be specified at the boundary patches and these can be done similar to ``p`` and ``U``. The file ``nut``, defines turbulent kinematic viscosity and its boundary conditions as follows. 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/nut
    

As noted above, the turbulent viscosity is specified as kinematic and therefore the units are in :math:`m^2/s` (``[0 2 -1 0 0 0 0]``). The turbulent viscosity value at freestream, specified at inlet patch is calculated as detailed in :ref:`turbulent-airfoil-SA-conditions` and :ref:`turbulent-airfoil-SST-conditions` for SA and SST models respectively and is specified accordingly. The same value also goes for ``internalField``. Note that a ``fixedValue`` of ``0`` is used for the ``wall`` which suggests that on the wall, it is only the molecular (laminar) viscosity that prevails.

The next turbulent property to set is the ``nuTilda`` which is a turbulent field variable, specified to only SA model and has the same units (``[0 2 -1 0 0 0 0]``) as kinematic turbulent viscosity. Details pertaining to this are given in :ref:`turbulent-airfoil-SA-conditions`. The following are the contents of the file ``nuTilda`` and the entries specified for the ``boundaryField`` are identical to that of turbulent kinematic viscosity explained above.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/nuTilda
    

We can now proceed to the files ``k`` and ``omega``, specific to only :math:`k-\omega~\rm{SST}` model. :math:`k-\omega~\rm{SST}` is a turbulence model which solves for turbulent kinetic energy and the specific rate of dissipation using two partial differential equations. Caelus therefore requires information about these to be specified at the boundary patches when this model is chosen. Starting with the file ``k``, 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/k
    

The unit of kinetic energy is :math:`m^2/s^2` and this is set in ``dimensions`` as ``[0 2 -2 0 0 0 0]``. As with other turbulent quantities discussed above, the value of :math:`k` (refer :ref:`turbulent-airfoil-SST-conditions`) needs to be specified for ``internalField``, ``inlet`` and ``wall``. Please note that for wall ``boundaryField`` with no wall-function, a small, non-zero ``fixedValue`` is required.

We now proceed to the file ``omega`` and the value for this is evaluated as detailed in :ref:`turbulent-airfoil-SST-conditions`

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/0/omega
    

The unit of specific rate of dissipation for :math:`\omega` is :math:`1/s` which is set in ``dimensions`` as ``[0 0 -1 0 0 0 0]``. The ``internalField`` and ``inlet`` gets a ``fixedValue``. Note that for wall ``boundaryField``, we specify ``omegaWallFunction`` and this is a model requirement and sets omega to the correct value near wall based on the :math:`y^+`. In conjunction, the value that goes with ``omegaWallFunction`` can be anything and for simplicity its set to ``1``.

Before proceeding to the next step, it is vital to ensure that the boundary conditions (``inlet``, ``outlet``, ``wall``, etc) added in the above files should be the grid boundary patches (surfaces) generated by grid generation tool and their names are identical. Additionally, the two boundaries :math:`x-z` plane named here as ``left`` and ``right`` have ``empty`` boundary conditions which forces Caelus to assume the flow to be in two-dimensions. With this, the setting up of the boundary conditions are complete.

*Grid file and Physical Properties*

The grid files associated with the NACA airfoil need to be placed in ``polyMesh`` sub-directory, which resides in the ``constant`` directory. Note that for both angles of attack, the identical grid is used. This is because the flow incidence angle is relative to the fixed airfoil and the equivalent velocity components can be easily specified thus simulating the airfoil at the required angle of attack. In addition, the physical properties are specified in various different files present in the ``constant`` directory. 

.. code-block:: bash
	
   
   polyMesh  RASProperties  transportProperties  turbulenceProperties

As seen above, the three files are listed in addition to the ``polyMesh`` sub-directory. The first one, ``RASProperties`` in which the Reynolds-Average-Stress (RAS) model is specified, which is shown below. Please note that depending on the turbulence model you wish to run with, the line that corresponds to that specific model should be enabled.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/constant/RASProperties
    

Second from the list is the ``transportProperties`` file, where kinematic viscosity is specified as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/constant/transportProperties
    

The viscous behaviour is modelled as Newtonian and hence the keyword ``Newtonian`` is used for the ``transportModel`` and the molecular (laminar) kinematic viscosity (:math:`nu`) is given having the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, which sets the ``simulationType`` to ``RASModel``. Both SA and :math:`k-\omega~\rm{SST}` are classified as Reynolds Average Stress (RAS) models. 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/constant/turbulenceProperties
    

*Controls and Solver Attributes*

In this section, the files required to control the simulation, specifying discretisation methods and linear solver settings are given. These files are to be placed in the ``system`` directory. First, navigate to the ``system`` directory. 
The ``controlDict`` file contains the following details

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/system/controlDict 
    

Referring to the above file, some explanation is required. Here, ``simpleSolver`` is used and the simulation begins at ``t = 0 s``. This now explains the logical need for having a ``0`` directory where the data files are read at the beginning of the run, which is ``t = 0 s`` in this case. Therefore, the keyword ``startFrom`` is set to ``startTime``, where ``startTime`` is set to ``0``. The simulation would be carried out as steady-state and therefore we require to specify the total number of iterations as a keyword for ``endTime``. Via the ``writeControl`` and ``writeInterval`` keywords, the solution intervals at which they are saved can be specified. Also note that a function object to obtain the force over the wall (airfoil surface) for every 50 iterations is included. In order to obtain this, a freestream density (``rhoInf``) need to be specified.

The discretization schemes for the finite volume discretization that will be used should be set through the ``fvSchemes`` file shown below.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/system/fvSchemes 
    

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/system/fvSolution
    

The user should now take a note that in the ``fvSolution`` file, different linear solvers are used to solve for velocity, pressure and turbulence quantities We also set the ``nNonOrthogonalCorrectors`` to 1 for this case. To ensure the stability of the solution, the relaxation is set to primary and turbulent variables. The ``relTol`` is not set to ``0`` unlike a time-accurate set-up as we are solving for a steady-state solution and a very low (:math:`\approx 0`) tolerance at every iteration is unnecessary. Since the entire system of equations converge to a global set ``tolerance`` the convergence would occur as the solution progresses to a steady state.

With this, the set-up of the directory structure with all the relevant files are complete. This can be verified again by issuing the following command and the directory tree should appear identical to the one shown below 

	
.. code-block:: bash
	
   cd my-turbulent-airfoil/
   tree
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
       ├── decomposePar
       ├── fvSchemes
       └── fvSolution

Execution of the solver
````````````````````````

Before executing the solver, it is important to renumber and to carry out a quality check on the grid/mesh. Renumbering reduces the bandwidth whereas the quality check shows the mesh statistics. These two can be performed by executing the following commands from the top working directory

.. code-block:: bash

   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

When the ``renumberMesh`` is performed, the user should take note of the bandwidth before and after the mesh renumbering. In a similar manner, when the ``checkMesh`` is performed, the mesh statistics are shown as below 

.. literalinclude:: sections/tutorial-figures/t_airfoil_checkmesh.txt

As can be noted above, the output of the ``checkMesh`` indicates that the mesh check has failed reporting in the final message as ``Failed 1 mesh checks``. This is because of the high aspect ratio meshes present immediate to the wall with very low (:math:`<< y^+`) values. Nevertheless, this is just a warning and Caelus will solve on this mesh.

As with the previous tutorial, it will be shown here to utilise the multi-core capability of CPUs for performing a parallel computation using MPI technique for large grids, such as the one considered here. Before this can be done, the grid has to be decomposed into smaller domains that can be solved by each single CPU core. The number of decomposition should be equal to the number of CPU core available for parallel computing. The decomposition is carried out through a file ``decomposeParDict`` present in the ``system`` sub-directory as shown below.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_airFoil2D/system/decomposeParDict
    

In the above file, the the keyword ``numberOfSubdomains`` defines the number of decomposed sub-domains needed and ``4`` is used which partitions the grid into 4 sub-domains. We use ``scotch`` as the method of decomposition which automatically divides the gird. The execution to decompose the grid is carried out again from the top directory as follows

.. code-block:: bash

   caelus run -- decomposePar

Now the decomposition should begin and the details of which are displayed in the terminal window. Subsequently, 4 processor directories will be generated as we have requested for 4 divisions of grid as shown below

.. code-block:: bash

   0  constant  processor0  processor1  processor2  processor3  system

The solver can now be executed for parallel computation in the host machine from the top directory using the following command

.. code-block:: bash

   caelus run -p -l my-turbulent-airfoil.log -- simpleSolver 

Note that here it is assumed that the parallel computing is available in the host machine. With the execution of the above commands, the simulation begins and the progress of the solution is written to the specified log file (``my-turbulent-airfoil.log``).

The log file can be further processed to look at the convergence history and this can be done as follows

.. code-block:: bash

   caelus logs -w my-turbulent-airfoil.log

The above command  allows you to look at the convergence of different variables with respect to the number of iterations carried out. The convergence of pressure is shown in :numref:`t-airfoil-convergence-tutorials`.

.. _t-airfoil-convergence-tutorials:
.. figure:: sections/tutorial-figures/t-airfoil-convergence-tutorials.*
   :width: 400px
   :align: center
		
   Convergence of pressure with respect to iterations

Results
````````

The flow over the airfoil at both :math:`0^\circ` and :math:`10^\circ` degree angle of attack are presented here. In :numref:`t-airfoil-velocitypressure-tutorials-0`, velocity magnitude and pressure contours can be seen for :math:`\alpha = 0^\circ` angle of attack. These result are for the SA model. The suction and the pressure surfaces essentially produce the same flow due to :math:`0^\circ` angle of incidence and thus contributes to zero lift. In contrast, at :math:`0^\circ` angle of incidence in :numref:`t-airfoil-velocitypressure-tutorials-10`, a low pressure region exists on the upper surface and consequently the velocity increases thus generating some lift.

.. _t-airfoil-velocitypressure-tutorials-0:
.. figure:: sections/tutorial-figures/t-airfoil-velocitypressure-tutorials-0.*
   :width: 800px
   :align: center
		
   Velocity magnitude and pressure contours for :math:`\alpha = 0^\circ` angle of attack

.. _t-airfoil-velocitypressure-tutorials-10:
.. figure:: sections/tutorial-figures/t-airfoil-velocitypressure-tutorials-10.*
   :width: 800px
   :align: center
		
   Velocity magnitude and pressure contours for :math:`\alpha = 10^\circ` angle of attack















