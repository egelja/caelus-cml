.. _flat-plate-validation-label:

Flat-plate
----------

*Laminar, incompressible flow over a two-dimensional Sharp-Leading Edge flat-plate*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`c_f`             Skin friction coefficient             Non-dimensional
:math:`e` (subscript)   Boundary layer edge conditions        -
:math:`L`               Length of the plate                   :math:`m`
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re`              Reynolds number                       Non-dimensional
:math:`T`               Temperature                           :math:`K`
:math:`u`               Local velocity in x-direction         :math:`m/s`
:math:`U`               Freestream velocity in x-direction    :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
:math:`\eta`            Blasius parameter                     Non-dimensional
======================  ==================================    =========================

**Introduction**

In this validation case, steady, incompressible, laminar flow over a two-dimensional sharp-leading edge flat-plate at zero angle of incidence is investigated. The flow generates a laminar boundary layer and the computational results are compared with the Blasius solution for the incompressible flow. Validation of the flow over a flat-plate forms the basis of these validation efforts as it is perhaps the most well understood configuration for CFD code-validation.

Blasius, in his work :cite:`Blasius1908` obtained the solution to the Boundary Layer Equations using a transformation technique. Here, equations of continuity and momentum in two-dimensional form are converted into a single ordinary differential equation (ODE). The solution to this ODE can be numerically obtained and is regarded as the exact solution to the boundary layer equations. It is only valid for steady, incompressible, laminar flow over a flat-plate. One of the highlights of Blasius solution is the analytical expression for the skin friction coefficient (:math:`c_f`) distribution along the flat-plate given by

.. math::

   c_f \approx \frac{0.644}{\sqrt{Re_{x}}}
  
where :math:`Re_{x}` is the local Reynolds number defined as

.. math::

   Re_x = \frac{Ux}{\nu}\rm{,}
    
:math:`U` is the freestream velocity, :math:`x` is the distance starting from the leading edge and :math:`\nu` is the kinematic viscosity.

**Problem definition**

This exercise is based on the validation work carried out by the NASA `NPARC alliance <http://www.grc.nasa.gov/WWW/wind/valid/archive.html>`_ for a flow over a `flat-plate <http://www.grc.nasa.gov/WWW/wind/valid/fplam/fplam.html>`_ using the same conditions in the incompressible limit. A schematic of the geometric configuration is shown in :numref:`flat-plate-schematic`.

.. _flat-plate-schematic:
.. figure:: sections_v/validation-figures/flat-plate-schematic.*
   :width: 500px
   :align: center

   Schematic representation of the flat plate

The length of the plate is L = 0.3048 m wherein, x = 0 is the leading edge, the Reynolds number of the flow based on the length of the plate is 200,000 and U is the velocity in the x-direction. Assuming the inlet flow is at a temperature of 300 K, the kinematic viscosity can be determined from dynamic viscosity and density of the fluid. The value is given in the table below. The value of dynamic viscosity is obtained from the Sutherland viscosity formulation. Using the Reynolds number, plate length and kinematic viscosity, the freestream velocity evaluates to U = 10.4306 m/s. The following table summarises the freestream conditions used:

+------------+----------------+----------------+----------------------+--------------------+---------------------------------+
| Fluid      | :math:`Re`     | :math:`U~(m/s)`| :math:`p~(m^2/s^2)`  | :math:`T~(K)`      | :math:`\nu~(m^2/s)`             |
+============+================+================+======================+====================+=================================+
| Air        | 200,000        | 10.43064       | :math:`(0)` Gauge    |  300               | :math:`1.58963\times10^{-5}`    |
+------------+----------------+----------------+----------------------+--------------------+---------------------------------+

As we have assumed the flow incompressible, the density (:math:`\rho`)  remains constant. In addition, since the fluid temperature is not considered, the viscosity remains constant. For incompressible flows, the kinematic forms of pressure and viscosity are always used in Caelus.

**Computational Domain and Boundary Conditions**

The computational domain is a rectangular block encompassing the flat-plate. :numref:`flat-plate-domain` shows the details of the boundaries used in two-dimensions (:math:`x-y` plane). As indicated in blue, the region of interest extends between :math:`0\leq x \leq 0.3048~m` and has a no-slip boundary condition. Upstream of the leading edge, a slip boundary is used to simulate freestream uniform flow approaching the flat-plate. However, downstream of the plate, there is additional no-slip wall a further three plate lengths (highlighted in green). This ensures that the boundary layer in the vicinity of the trailing edge is not influenced by the outlet boundary. Since the flow is subsonic, disturbances cause the pressure to propagate both upstream and downstream. Therefore, placement of the inlet and outlet boundaries were chosen to have minimal effect on the solution. The inlet boundary as shown in the figure below is placed at start of the slip-wall (:math:`x = -0.06~m`) and the outlet at the end of the second no-slip wall (:math:`x = 1.2192~m`). Both inlet and outlet boundaries are between :math:`0\leq y \leq 0.15~m`. A slip-wall condition is used for the entire top boundary.

.. _flat-plate-domain:
.. figure:: sections_v/validation-figures/flat-plate-domain.*
   :width: 500px
   :align: center

   Flat plate computational domain
    
*Boundary Conditions and Initialisation*

The following are the boundary condition details used for the computational domain:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 10.4306~m/s` in :math:`x` direction
   - Pressure: Zero gradient
    
* Slip wall
   - Velocity: Slip
   - Pressure: Slip

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
    
* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 10.4306~m/s` in :math:`x` direction
   - Pressure: Zero Gauge pressure

**Computational Grid**

A 2D structured grid was generated using `Pointwise <http://www.pointwise.com/>`__ in the :math:`x-y` plane. Since Caelus is a 3D computational framework, it necessitates the grid to also be 3D. Therefore, a 3D grid was obtained using `Pointwise <http://www.pointwise.com/>`_ by extruding the 2D grid in the positive :math:`z` direction by *one cell*. The final 3D grid was then exported to the Caelus format (polyMesh). The two :math:`x-y` planes obtained as a result of grid extrusion need boundary conditions to be specified. As the flow over a flat-plate is generally 2D, we do not need to solve the flow in the third dimension. This is achieved in Caelus by specifying *empty* boundary condition for each plane. Although, no flow is computed in the :math:`z` direction, a velocity of :math:`w = 0` has to be specified for the velocity boundary condition as indicated above.

.. _flat-plate-grid:
.. figure:: sections_v/validation-figures/flat-plate-grid.*
   :width: 800px
   :align: center

   Structured grid for a flat plate domain

:numref:`flat-plate-grid` shows the 2D grid in the :math:`x-y` plane. As can be seen, the grid is refined perpendicular to the wall in order to capture resolve the viscous effects. To ensure that the gradients in boundary layer are well resolved, about 50 grid nodes are placed between the wall and the boundary layer edge. Grid refinement is also added at the leading edge so that the growth of the boundary layer is also well resolved. In this particular case, 400 cells were used in the stream-wise (:math:`x`) direction (:math:`x \leq 0 \leq 0.3048~m`) and 600 in the wall normal (:math:`y`) direction. For no-slip wall beyond :math:`x > 0.3048`, a similar distribution is used.

**Results and Discussion**

A time-dependent solution to the two-dimensional flat-plate was obtained using Caelus |version|. The SLIM transient solver was used here and the flow was simulated sufficiently long (several plate length flow times) such that steady flow was established. For the discretization of time-dependent terms, the first-order Euler scheme was used. A Gauss linear discretization was used for the pressure and velocity gradients. A linear upwind discretization was for the divergence of velocity and mass flux. A linear corrected scheme was used for Laplacian discretization while cell-to-face centre interpolation used linear interpolation. 

In :numref:`cf-flat-plate-laminar`, the skin-friction distribution along the flat-plate obtained from the CFD simulation is compared with that of the Blasius analytical solution. Here, the distance :math:`x` is normalised with the length of the plate (:math:`L`). Excellent agreement is observed along the entire length of the flat-plate.

.. _cf-flat-plate-laminar:
.. figure:: sections_v/validation-figures/cf-flat-plate-laminar.*
   :width: 600px
   :align: center

   Skin-friction comparison between SLIM and Blasius solutions
    
At the exit plane of the flat-plate at :math:`x = 0.3048~m`, velocity data was extracted across the boundary layer and compared with the Blasius analytical solution. This is shown in :numref:`velocity-profile-laminar` where the velocity profile is plotted using similarity variables from the Blasius solution. Here, :math:`\eta` is the non-dimensional distance from the wall to the boundary layer edge and :math:`U_e` is the velocity at the boundary layer edge. Similar to skin-friction, the velocity profile also exhibits excellent agreement with the Blasius solution.

.. _velocity-profile-laminar:
.. figure:: sections_v/validation-figures/velocity-profile-laminar.*
   :width: 600px
   :align: center

   Non-dimensional velocity profile comparison between SLIM and Blasius solutions
    
**Conclusions**

The steady, incompressible, laminar flow over a two-dimensional flat-plate was simulated using Caelus |version| utilising the SLIM solver. The results were validated against the Blasius analytical solutions resulting in excellent agreement.
