Backward Facing Step
---------------------

This tutorial focuses on the turbulent flow over a two-dimensional backward facing step using Caelus |version|. In particular this tutorial emphasises the use of wall functions for a high :math:`y^+` grid. The basic steps to set-up the directory structure, fluid properties, boundary conditions, turbulence properties etc will be discussed. Flow visualisation such as pressure and velocity contours within the separated region will be shown. With these steps in place, the user should be able to reproduce the tutorial accurately.

Objectives
````````````

Through this tutorial, the user will get familiar in setting up Caelus simulation for steady, turbulent, incompressible flow over a two-dimensional backward facing step with wall functions. Further, the velocity and pressure contours within the separated region will be highlighted. Following would be the steps that would be carried out.

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
   -  Visualisation of flow over within the separated region

Pre-requisites
````````````````

By now the user should be familiar with the Linux command line environment via a terminalor caelus-consolu (for Windows OS). The grid for this case can be obtained from the full working case directory shown below and was generated using `Pointwise <http://www.pointwise.com/>`__ and appropriately converting it to Caelus format.

Background
````````````

Turbulent flow over a backward facing step is a classical configuration to examine steady separated flows. Here due to the presence of the step, the flow undergoes separation and the subsequent shear layer reattaches downstream forming a recirculation region. The thickness of the boundary layer at the lip of the step and the flow Reynolds number are important parameters that determines the length of separated region. A decrease in pressure or favourable pressure gradient in the immediate vicinity of the step is a classical behaviour that contributes to the increase in drag. The user is suggested to refer to the verification and validation of this case at :ref:`turbulent-step-verification-label` for more detailed analysis.

The schematic of the backward facing step is shown in :numref:`t-step-schematic-tutorials` in two-dimensions. A step height (H) of 1.0 m is used with a flow Reynolds number of 36000, which is based on the step height. Air is considered as a fluid with a freestream temperature of 298 K and the freestream velocity corresponds to U = 44.315 m/s. The user should note that the two-dimensional geometric plane considered is in :math:`x-z` directions.

.. _t-step-schematic-tutorials:
.. figure:: sections/tutorial-figures/t-step-schematic-tutorials.*
   :width: 500px
   :align: center

   Schematic representation of the backward facing step

Freestream conditions are detailed in the below table

.. _t-step-freestream-conditions:
.. table:: Freestream conditions

  +------------+-----------------------+------------------+----------------------+--------------------+---------------------------------+
  | Fluid      | :math:`Re_H`          | :math:`U~(m/s)`  | :math:`p~(m^2/s^2)`  | :math:`T~(K)`      | :math:`\nu_\infty~(m^2/s)`      |
  +============+=======================+==================+======================+====================+=================================+
  | Air        | :math:`36000`         | 44.31525         | :math:`(0)` Gauge    |  298.330           | :math:`1.230979\times10^{-3}`   |
  +------------+-----------------------+------------------+----------------------+--------------------+---------------------------------+

Grid Generation
````````````````

A fully structured gird is developed for this geometry and is converted to ``polyMesh`` format. The grid in ``polyMesh`` format can be found in the full working directory of this case at

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/constant/polyMesh

The computational domain for the step is shown in :numref:`t-step-domain-tutorials` which also highlights the boundary conditions used. The upstream flat plate extends for up to 110 step heights so that a fully turbulent boundary layer is ensured prior to the step at which the flow separates. Downstream of the step, the plate extends to 50 step heights giving sufficient length for the flow to reattach. The inlet and the outlet is placed as indicated in the :numref:`t-step-domain-tutorials` and closely flows the experimental set-up. The velocity on the step surfaces is zero, wherein :math:`u,v,w = 0` represented through a no-slip boundary.


.. _t-step-domain-tutorials:
.. figure:: sections/tutorial-figures/t-step-domain-tutorials.*
   :width: 500px
   :align: center
      
   Computational domain of a 2D backward facing step

The grid in ``polyMesh`` is in three-dimensions, although the flow over the step is two-dimensional in nature. A *one-cell* thick grid normal to the flow plane (:math:`x-z`) is therefore sufficient for Caelus to consider symmetry flow in the :math:`y` direction. The two resulting planes that are in (:math:`x-z`) direction need boundary conditions to be specified. Since the flow is assumed to be 2D, we do not need to solve the flow in the third-dimension and this can be easily achieved in Caelus by specifying empty boundary conditions for each of the two planes. This consequently treats the flow in :math`y` direction as symmetry.

.. Note::
   A velocity value of :math:`v=0` needs to be specified at appropriate boundaries although no flow is solved in the :math:`y` direction.

.. _t-step-grid-tutorials:
.. figure:: sections/tutorial-figures/t-step-grid-tutorials.*
   :width: 800px
   :align: center
   
   Computational grid of a 2D backward facing step in :math:`x-z` plane

The 2D step grid in :math:`x-z` plane is shown in figure %s:numref:`t-step-grid-tutorials` which has a total of 189 cells in the streamwise direction and 64 cells in the normal direction, with 20 cells representing the step height. As noted earlier, the grid is developed to use with wall functions and hence the :math:`y^+ \approx 30` in this case.

Problem definition
````````````````````

This section details various steps needed to set-up the turbulent flow over a step. A full working case of this can be found in:

.. code-block:: bash

   /tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D

However,the user is free to start the case setup from scratch consistent with the directory stucture discussed below. 

**Directory Structure**

.. Note::
   All commands shown here are entered in a terminal window, unless otherwise mentioned

In order to set-up the problem Caelus requires ``time``, ``constant`` and ``system`` sub-directories within the main working directory. Here, the simulation will be started at time t = 0 s, which requires a time sub-directory named ``0``. 

The ``0`` sub-directory contains few additional files in which boundary conditions are specified. In the below table, the list of necessary files are provided based on the turbulence model chosen.

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
| Turbulent dissipation (:math:`\epsilon`)        | ``epsilon`` (Only for  R :math:`k-\epsilon` model)   |
+-------------------------------------------------+------------------------------------------------------+

In this tutorial we consider three turbulence models. They are Spalart-Allmaras (SA), :math:`k-\omega` - Shear Stress Transport (:math:`\rm{SST}`) and Realizable :math:`k-epsilon` models. The content of the files listed above sets the dimensions, initialisation and boundary conditions to the defining problem, which also forms three principle entries required. 

The user should note that Caelus is case sensitive and therefore the directory and file set-up should be identical to what is shown here.

**Boundary Conditions and Solver Attributes**

*Boundary Conditions*

Referring back to :numref:`t-step-domain-tutorials`, the following are the boundary conditions that will be specified:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 44.31525~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-step-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-step-SA-conditions`)
     - Realizable :math:`k-\epsilon` (Fixed uniform value of :math:`k_{\infty}`, :math:`\epsilon_{\infty}` and :math:`\nu_{t_\infty}` as given in :ref:`turbulent-step-RKE-conditions`)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras:

       - :math:`\nu_t`: type nutUWallFunction with a uniform value of :math:`\nu_t=0` 
       - :math:`\tilde{\nu}`: type fixedValue with a value of :math:`\tilde{\nu}=0`

     - :math:`k-\omega~\rm{SST}`:

       - :math:`k`: type kqRWallFunction with a uniform value of :math:`k_{\infty}`
       - :math:`\omega`: type omegaWallFunction with a uniform value of :math:`\omega_{\infty}`
       - :math:`\nu_t`: type nutUWallFunction with a uniform value of :math:`\nu_t=0`

     - Realizable :math:`k-\epsilon`:

       - :math:`k`: type kqRWallFunction with a uniform value of :math:`k_{\infty}`
       - :math:`\epsilon`: type epsilonWallFunction with a uniform value of :math:`\epsilon=0`
       - :math:`\nu_t`: type nutUWallFunction with a uniform value of :math:`\nu_t=0`

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )
     - Realizable :math:`k-\epsilon` (Zero gradient :math:`k` and :math:`\epsilon`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 44.31525` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in :ref:`turbulent-step-SA-conditions`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-step-SA-conditions`)
     - Realizable :math:`k-\epsilon` (Fixed uniform values of :math:`k_{\infty}`, :math:`\epsilon_{\infty}` and :math:`\nu_{t~\infty}` as given in :ref:`turbulent-step-RKE-conditions`)

First, the file ``p``, shown below, contains initial and boundary conditions for pressure.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/p
    

As can be noted from the above file, it begins with a dictionary named ``FoamFile`` which contains the standard set of keywords for version, format, location, class and object names. The explanation of the principle entries are as follows

* ``dimension``
	-  is used to specify the physical dimensions of the pressure field. Here, pressure is defined in terms of kinematic pressure with the units (:math:`m^2/s^2`) written as  ``[0 2 -2 0 0 0 0]``

* ``internalField``
	-  is used to specify the initial conditions. It can be either uniform or non-uniform. Since we have a 0 initial uniform gauge pressure, the entry is ``uniform 0;``

* ``boundaryField``
	-  is used to specify the boundary conditions. In this case its the boundary conditions for pressure at all the boundary patches. 

In the similar way, the file ``U`` is defined for velocity

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/U
    

The principle entries for velocity field are self-explanatory and the dimensions are typically for that of velocity with the units :math:`m/s` (``[0 1 -1 0 0 0 0]``). Here, both initialisation and inlet have a uniform flow velocity specified with three velocity components. Therefore these should be set to ``uniform (44.31525 0 0);`` while the wall is given a no-slip condition.

The turbulent properties are also required to be specified at the boundary patches and these can be done similar to ``p`` and ``U``. We have the file ``nut``, which defines turbulent kinematic viscosity in the domain as follows

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/nut
    

As noted above, the turbulent viscosity is specified as kinematic and therefore the units are in :math:`m^2/s` (``[0 2 -1 0 0 0 0]``). The turbulent viscosity value at freestream, specified at inlet patch is calculated as detailed in :ref:`turbulent-step-SA-conditions`, :ref:`turbulent-step-SST-conditions` and :ref:`turbulent-step-RKE-conditions` for SA, SST and RKE models respectively and is specified accordingly. The same value also goes for ``internalField``.

The next turbulent property set is the ``nuTilda`` which is a turbulent field variable, specified to only SA model and has the same units (``[0 2 -1 0 0 0 0]``) as kinematic turbulent viscosity. Details pertaining to this are given in :ref:`turbulent-step-SA-conditions`. The following is the file ``nuTilda`` with the entries specified for the ``boundaryField`` are identical to that of turbulent kinematic viscosity explained above.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/nuTilda
    

We can now proceed to the file ``k`` which is specific to both :math:`k-omega~\rm{SST}` and Realizable :math:`k-epsilon` models and represents turbulent kinetic energy.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/k
    

The unit of kinetic energy is :math:`m^2/s^2` and this is set in ``dimensions`` as ``[0 2 -2 0 0 0 0]``. The value of :math:`k` (refer :ref:`turbulent-step-SST-conditions` and :ref:`turbulent-step-RKE-conditions`) needs to be specified for ``internalField``, ``inlet`` and ``wall``. Please note that for wall ``kqRWallFunction`` with values of freestream are required.

We now proceed to the file ``omega``, specific to only :math:`k-\omega~\rm{SST}` model and the value for this is evaluated as detailed in :ref:`turbulent-step-SST-conditions`

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/omega
    

The unit of specific rate of dissipation for :math:`\omega` is :math:`1/s` which is set in ``dimensions`` as ``[0 0 -1 0 0 0 0]``. The ``internalField`` and ``inlet`` gets a ``fixedValue``. Note that for wall ``boundaryField``, we specify ``omegaWallFunction`` required for high :math:`y^+`.

The final file in this class is the ``epsilon``, specific to only Realizable :math:`k-\epsilon` model and the value for this is evaluated as detailed in :ref:`turbulent-step-RKE-conditions`

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/0/epsilon
    

The unit of turbulent dissipation for :math:`\epsilon` is :math:`m^2/s^3` which is set in ``dimensions`` as ``[0 2 -3 0 0 0 0]``. The ``internalField`` and ``inlet`` gets a ``fixedValue`` similar to :math:`\omega`. Note that for wall ``boundaryField``, we specify ``epsilonWallFunction`` required for high :math:`y^+`.

Before proceeding to the next step, it is vital to ensure that the boundary conditions (``inlet``, ``outlet``, ``wall``, etc) added in the above files should be the grid boundary patches (surfaces) generated by grid generation tool and their names are identical. Additionally, the two boundaries :math:`x-z` plane named here as ``symm-left`` and ``symm-right`` have ``empty`` boundary conditions which forces Caelus to assume the flow to be in two-dimensions. With this, the setting up of the boundary conditions are complete.

*Grid file and Physical Properties*

The files associated with the grid need to be placed in ``polyMesh`` sub-directory, which resides in the ``constant`` directory. We use identical grid for all the three turbulence simulation. In addition, the physical properties are specified in various different files present in the ``constant`` directory. 

There are three files listed in addition to the ``polyMesh`` sub-directory. The first one, ``RASProperties`` in which the Reynolds-Average-Stress (RAS) model is specified, which is shown below. Please note that depending on the turbulence model you wish to run with, the line that corresponds to that specific model should be enabled.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/constant/RASProperties
    

Second from the list is the ``transportProperties`` file, where laminar kinematic viscosity is specified as shown below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/constant/transportProperties
    

The viscous behaviour is modelled as Newtonian and hence the keyword ``Newtonian`` is used for the ``transportModel`` and the molecular (laminar) kinematic viscosity (:math:`nu`) is given having the units :math:`m^2/s` (``[0 2 -1 0 0 0 0]``).

The final file in this class is the ``turbulenceProperties`` file, which sets the ``simulationType`` to ``RASModel``. Here, SA, :math:`k-\omega~\rm{SST}` and Realizable :math:`k-\epsilon` are classified as Reynolds Average Stress (RAS) models.

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/constant/turbulenceProperties
    

*Controls and Solver Attributes*

This section details the files required to control the simulation, discretization methods and linear solvers. These files are to be placed in the ``system`` directory. First, navigate to the ``system`` directory
The following is the ``controlDict`` file, 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/system/controlDict
    

Here, ``simpleSolver`` is used and the simulation begins at ``t = 0 s``. This now explains the logical need for having a ``0`` directory where the data files are read at the beginning of the run, which is ``t = 0 s`` in this case. Therefore, the keyword ``startFrom`` is set to ``startTime``, where ``startTime`` is set to ``0``. The simulation would be carried out as steady-state and therefore we require to specify the total number of iterations as a keyword for ``endTime``. Via the ``writeControl`` and ``writeInterval`` keywords, the solution intervals at which they are saved can be specified. Also note that a function object to obtain the force over the wall (step surface) for every 100 iterations is included. In order to obtain this, a freestream density (``rhoInf``) need to be specified.

The discretization schemes for the finite volume discretization that will be used should be set through the ``fvSchemes`` file show below 

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/system/fvSchemes
    

The linear solver controls and tolerances are set in ``fvSolution`` as given below

.. literalinclude:: ../../../tutorials/incompressible/simpleSolver/ras/ACCM_backwardFacingStep2D/system/fvSolution
    

The user should now take a note that in the ``fvSolution`` file, different linear solvers are used to solve for velocity, pressure and turbulence quantities. We also set the ``nNonOrthogonalCorrectors`` to 1 for this case. To ensure the stability of the solution, the relaxation is set to primary and turbulent variables. The ``relTol`` is not set to ``0`` unlike a time-accurate set-up as we are solving for a steady-state solution and a very low (:math:`\approx 0`) tolerance at every iteration is unnecessary. Since the entire system of equations converge to a global set ``tolerance`` the convergence would occur as the solution progresses to a steady state.

With this, the set-up of the directory structure with all the relevant files are complete. This can be verified again by issuing the following command and the directory tree should appear identical to the one shown below

.. code-block:: bash
	
   cd my-turbulent-step/
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
       ├── fvSchemes
       └── fvSolution

Execution of the solver
````````````````````````

Prior to the execution of the solver, it is important to renumber and to carry out a quality check on the grid/mesh. Renumbering reduces the bandwidth whereas the quality check shows the mesh statistics. These two can be performed by executing the following commands from the top working directory

.. code-block:: bash
	
   caelus run -- renumberMesh -overwrite
   caelus run -- checkMesh

When the ``renumberMesh`` is performed, the user should take note of the bandwidth before and after the mesh renumbering. In a similar manner, when the ``checkMesh`` is performed, the mesh statistics are shown as below

.. literalinclude:: sections/tutorial-figures/t_step_checkmesh.txt

At this stage, the solver can be executed from the top directory using the following command

.. code-block:: bash

    caelus run -l my-turbulent-step.log -- simpleSolver 


The log file can be further processed to look at the convergence history and this can be done as follows

.. code-block:: bash

   caelus logs -w my-turbulent-step.log

The ``caelus-plotResiduals -l`` allows you to look at the convergence of various variables with respect to the number of iterations carried out and is shown in :numref:`t-step-convergence-tutorials`. 


.. _t-step-convergence-tutorials:
.. figure:: sections/tutorial-figures/t-step-convergence-tutorials.*
   :width: 400px
   :align: center
		
   Convergence of pressure with respect to iterations

Results
````````

The flow within the separated region is visualised here through the velocity and pressure contours. In :numref:`t-step-velocitypressure-tutorials` velocity magnitude and pressure contours are shown for SA model. Immediate downstream of the step a decrease in pressure is seen followed by an increases as the shear layer reattaches to the step. This is consistent with the formation of a low velocity recirculating region behind the step.


.. _t-step-velocitypressure-tutorials:
.. figure:: sections/tutorial-figures/t-step-velocitypressure-tutorials.*
   :width: 800px
   :align: center
    
   Velocity magnitude and pressure contours over the backward facing step



















































