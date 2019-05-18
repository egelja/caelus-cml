.. _t-cavity-validation:

Triangular Cavity
-----------------

*Laminar, incompressible flow inside a lid driven Triangular Cavity*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`D`               Depth of the cavity                   :math:`m`
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_D`            Reynolds number based on depth		  Non-dimensional
:math:`U`               Freestream velocity in x-direction    :math:`m/s`
:math:`W`               Width		                          :math:`m`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
======================  ==================================    =========================

This validation study concerns the laminar, incompressible flow inside a lid driven triangular cavity. Here, the top wall of the cavity moves at a constant velocity initiating a recirculating motion within the cavity. Due to the viscous nature of the flow, a boundary layer develops in the direction of the moving lid, while flow reversal occurs due to the recirculating flow. The flow feature of interest is the velocity distribution along the centre-line of the cavity.

Benchmark experiments on this configuration has been reported in Jyotsna and Venka :cite:`Jyotsna1995` for a Reynolds number of 800. The main objective of this validation case was to compare the :math:`x` velocity distribution against experimental data.

**Problem Definition**

A schematic of the triangular cavity is presented in :numref:`t-cavity-schematic` where depth of the cavity D = 4 m and the width W = 2 m. The Reynolds number based on the cavity depth is 800 and the wall velocity is U = 2 m/s. Using the Reynolds number, U, and D, kinematic viscosity was calculated to be 0.01 :math:`m^2/s`. 

.. _t-cavity-schematic:
.. figure:: sections_v/validation-figures/t-cavity-schematic.*
   :width: 300px
   :align: center
   
   Schematic showing the Triangular Cavity

The table below summaries the flow properties

+----------------+---------------+---------------+----------------+----------------------+-----------------------+
| :math:`Re_D`   | :math:`D~(m)` | :math:`W~(m)` |:math:`U~(m/s)` | :math:`p~(m^2/s^2)`  | :math:`\nu~(m^2/s)`   |
+================+===============+===============+================+======================+=======================+
| 800            | 4.0           | 2.0           | 2.0            | :math:`(0)` Gauge    | :math:`0.01`          |
+----------------+---------------+---------------+----------------+----------------------+-----------------------+

The flow in this case is assumed to be incompressible and hence the density remained constant throughout. Further, the temperature field is not accounted into the calculation, and therefore the viscosity of the flow can also remain constant. Since viscosity is constant, it becomes more convenient to specify it as kinematic viscosity. It should be noted that in Caelus for incompressible flows, both pressure and viscosity are always specified as kinematic.

**Computational Domain and Boundary Conditions**

The computational domain is the triangular cavity shown in :numref:`t-cavity-domain`. Highlighted in blue, the side walls of the cavity have a no-slip boundary condition while the top wall, highlighted in green, has a uniform velocity in the :math:`x` direction.

.. _t-cavity-domain:
.. figure:: sections_v/validation-figures/t-cavity-domain.*
   :width: 300px
   :align: center
   
   Computational domain of a Triangular Cavity
   
*Boundary Conditions and Initialisation*

* Moving wall
   - Velocity: Fixed uniform velocity :math:`u = 2.0~m/s` in :math:`x` direction
   - Pressure: Zero gradient

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
	
* Initialisation
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero Gauge pressure
	
**Computational Grid**

The 2D grid in :math:`x-y` plane is shown in :numref:`t-cavity-grid`. A hybrid grid is employed for this case with a total of 5538 cells. Up to a depth of D = 1.35 m a structured grid is used while below that value an unstructured triangular grid is used. An unstructured grid is used in the bottom portion because it resulted in lower skewness in this vicinity. For the structured region, 40 cells are distributed across the width of the cavity and 40 along the depth. The cavity walls in the unstructured region have 100 cells along each. The interface of the two regions is *node matched* and has 40 cells across the width. The grid close to the cavity lid was refined to better capture the shear layer.

.. _t-cavity-grid:
.. figure:: sections_v/validation-figures/t-cavity-grid.*
   :width: 600px
   :align: center
   
   Hybrid grid for a Triangular Cavity
  
The flow characteristics in the cavity can be assumed to be two dimensional and here it has been solved with the same assumption. Since Caelus is a 3D computational framework, it necessitates the grid to also be 3D. Therefore, a 3D grid was obtained using `Pointwise <http://www.pointwise.com/>`_ by extruding the 2D grid in the positive :math:`z` direction by *one cell*. The final 3D grid was then exported to the Caelus format (polyMesh). The two :math:`x-y` planes obtained as a result of grid extrusion need boundary conditions to be specified. As the flow over a flat-plate is generally 2D, we do not need to solve the flow in the third dimension. This is achieved in Caelus by specifying *empty* boundary condition for each plane. Although, no flow is computed in the :math:`z` direction, a velocity of :math:`w = 0` has to be specified for the velocity boundary condition as indicated above.   

**Results and Discussion**

A steady solution to the cavity was obtained using Caelus |version| with the SLIM solver. While a time-dependent approach was used, the solution was simulated sufficiently long so that steady flow was achieved. To determine when this occured the velocity distribution along the cavity centre-line was monitored with respect to time. The simulations was stopped when no appreciable changes were observed.

In :numref:`t-cavity-velocity`, the :math:`x` velocity distribution along the cavity centre-line is compared with that of the benchmark experimental data. The :math:`y` distance is normalised with the cavity depth (:math:`D`) which gives :math:`y/d = 0` at the cavity lid and :math:`y/d = -1` at the bottom vertex. Similarly, the :math:`u` velocity is normalised with the velocity of the cavity lid (:math:`u_L`).

.. _t-cavity-velocity:
.. figure:: sections_v/validation-figures/t-cavity-velocity.*
   :width: 600px
   :align: center
   
   Comparison of experimental and computational :math:`x` velocity distribution along the cavity's centre-line

As seen in :numref:`t-cavity-velocity` above, the comparison the experiment is excellent.

**Conclusions**

A steady, incompressible flow past a two-dimensional triangular cavity was simulated using Caelus |version| on a hybrid grid. The velocity distribution along the centre-line of the cavity was compared with the benchmark experimental data, it was found that the SLIM results compared very favorably.
