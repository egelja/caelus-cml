.. _tee-junction-validation-label:

Tee Junction
------------

*Laminar, incompressible flow in a 90 degree tee junction*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`L`               Length of the branch                  :math:`m`
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_w`            Reynolds number based on width        Non-dimensional
:math:`V`               Freestream velocity in y-direction    :math:`m/s`
:math:`W`               Width of the branch                   :math:`m`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
======================  ==================================    =========================

**Introduction**

In this validation case, laminar, incompressible flow through a two-dimensional :math:`90^\circ` tee junction was investigated. Due to the presence of the side branch, the flow separates and forms a recirculating region. The recirculating regions influences the mass flow through the main and side branches. The numerically computed mass-flow ratio was calculated and compared with experiment.

A comprehensive study of flow through planar branches has been carried out by Hayes et al. :cite:`Hayes1989` due to its prevelance in the bio-mechanical industry. Of which, the :math:`90^\circ` right-angled tee junction is considered here for the purpose of validation.

**Problem definition**

The following figure shows the schematic of the tee-junction. Here, L = 3.0 m, W = 1.0 m respectively, the Reynolds number based on the width is 300, and V is the velocity in the y-direction. For simplicity, we have assumed the velocity, V = 1 m/s. Using these values the resulting kinematic viscosity was 0.00333 :math:`m^2/s`.

.. _tee-junction-schematic:
.. figure:: sections_v/validation-figures/tee-junction-schematic.*
   :width: 300px
   :align: center

   Tee-junction Schematic
    
The summary of the flow properties and geometric details are given in the following table.

+----------------+------------+------------+----------------+---------------------+--------------------+
| :math:`Re_w`   | :math:`L`  | :math:`W`  | :math:`V~(m/s)`| :math:`p~(m^2/s^2)` | :math:`\nu~(m^2/s)`|
+================+============+============+================+=====================+====================+
| 300            |  3.0       | 1.0        | 1.0            | :math:`(0)` Gauge   | 0.00333            |
+----------------+------------+------------+----------------+---------------------+--------------------+
    
As we have assumed the flow incompressible, the density (:math:`\rho`) remains constant. In addition, since the fluid temperature is not considered, the viscosity remains constant. For incompressible flows, the kinematic forms of pressure and viscosity are always used in Caelus.

**Computational Domain and Boundary Conditions**

Since this is an internal flow problem, the computational domain is contained within tee-junction geometry. The details are shown in :numref:`tee-junction-domain`. As indicated, all tee-junction walls have a no-slip boundary condition which has been highlighted in blue. At the inlet, a fully developed laminar flow parabolic profile is applied, otherwise a much longer main branch would be required for the flow to develop. The domain has two outlets, one at the end of the main channel and the other at the end of side branch. Note the exit pressures at the two outlets are equal. 

.. _tee-junction-domain:
.. figure:: sections_v/validation-figures/tee-junction-domain.*
   :width: 300px
   :align: center
   
   Computational domain representing tee-junction


*Boundary Conditions and Initialisation*

The following are the boundary condition details used for the computational domain:

* Inlet
   - Velocity: Parabolic velocity profile; centerline velocity of :math:`v = 1.0~m/s` in :math:`y` direction
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
    
**Computational Grid**

The 2D structured grid is shown in :numref:`tee-junction-grid`. Since Caelus is a 3D computational framework, it necessitates the grid to also be 3D. Therefore, a 3D grid was obtained by extruding the 2D grid in the positive :math:`z` direction by *one cell*. The final 3D grid was then exported to the Caelus format (polyMesh). The two :math:`x-y` planes obtained as a result of grid extrusion need boundary conditions to be specified. As the flow over a flat-plate is generally 2D, we do not need to solve the flow in the third dimension. This is achieved in Caelus by specifying *empty* boundary condition for each plane. Although, no flow is computed in the :math:`z` direction, a velocity of :math:`w = 0` has to be specified for the velocity boundary condition as indicated above.

.. _tee-junction-grid:
.. figure:: sections_v/validation-figures/tee-junction-grid.*
   :width: 500px
   :align: center

   Structured grid for tee-junction domain
   
A total of 2025 cells comprise the tee-junction of which, 90 cells are distributed along the height of the main channel, and 45 along the length of the side branch. The distribution is such that a dimensional length of :math:`L = 1~m` has a total of 45 cells, giving a distribution of :math:`(2/3)*45 = 30` cells for the :math:`(2/3) L` segment of the main channel. The width, :math:`W`, consists of 15 cells.

**Results and Discussion**

A time-dependent solution to the two-dimensional flat-plate was obtained using Caelus |version|. The SLIM transient solver was used here and the flow was simulated sufficiently long such that steady separated flow was established. To ensure this, shear-stress distribution was monitored on the lower wall of the side branch. The simulation was stopped once the separation and reattachment locations no longer varied with time.

Mass flow was calculated at the inlet and at the main outlet (outlet-1) and the mass-flow ratio was subsequently calculated. The below table compares the SLIM result with the experimental value. As can be noted, the agreement between the two is excellent.

+----------------+------------------+------------------+-------------------------+
|                | Experimental     | SLIM             | Percentage Difference   |
+================+==================+==================+=========================+
| Flow Split     |  :math:`0.887`   | :math:`0.886`    | :math:`0.112~\%`        |
+----------------+------------------+------------------+-------------------------+

**Conclusions**

The steady, incompressible, two-dimensional laminar flow in a right-angled :math:`90^\circ` tee-junction was simulated using Caelus |version| with the SLIM solver and validated against experimental data resulting in excellent agreement.
