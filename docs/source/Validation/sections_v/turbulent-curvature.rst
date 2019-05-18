.. _turbulent-curvature-verification-label:

Two-dimensional Convex Curvature
---------------------------------

*Turbulent, incompressible flow in a two-dimensional convex curvature channel*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`a`               Speed of sound                        :math:`m/s`
:math:`c_f`             Skin friction coefficient             Non-dimensional
:math:`c_p`             Pressure coefficient                  Non-dimensional
:math:`I`               Turbulent intensity                   Percentage
:math:`k`               Turbulent kinetic energy              :math:`m^2/s^2` 
:math:`M_i`             Inlet Mach number                     Non-dimensional
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_L`            Reynolds number                       :math:`1/m`
:math:`T`               Temperature                           :math:`K`
:math:`u`               Local velocity in x-direction         :math:`m/s`
:math:`w`               Local velocity in z-direction         :math:`m/s`
:math:`U`               Inlet velocity                        :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`y^+`             Wall distance                         Non-dimensional
:math:`\alpha`          Angle of attack                       Degrees
:math:`\mu`             Dynamic viscosity                     :math:`kg/m~s`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
:math:`\tilde{\nu}`     Turbulence field variable             :math:`m^2/s`
:math:`\rho`            Density                               :math:`kg/m^3`
:math:`\omega`          Specific dissipation rate             :math:`1/s`
:math:`i`               Inlet conditions
:math:`t` (subscript)   Turbulent property
:math:`ref`             Reference pressure                    :math:`Pa`
======================  ==================================    =========================

**Introduction**

In this case, the turbulent incompressible flow in a constant-area duct having a convex curvature is investigated as a part of verification and validation. The effect of curvature on the capability of turbulence model to predict the boundary layer accurately is of primary concern in this study. Similar to previous cases, the simulations are carried out over a series of four grids that tests the sensitivity of solution as the grid is refined. As with the earlier cases, Spalart-Allmaras (SA) and :math:`k-\omega~\rm{SST}` turbulence models are tested. However due to the presence of strong curvature, a variant of Spalart–Allmaras turbulence model which accounts for rotational and curvature effects is additionally considered. This model is referred to as "Spalart–Allmaras Rotational/Curvature" (SA-RC) :cite:`Shur2000` . The results are then verified against `CFL3D <http://cfl3d.larc.nasa.gov>`__ data and validated with experimental pressure distributions.

**Problem definition**

This verification and validation exercise is based on the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/smitscurve_val.html>`__ case and follows the same conditions used in the incompressible limit. As CFL3D is a compressible CFD code, the original simulation used a inlet Mach number (:math:`M_i`) of 0.093. A schematic of the geometric configuration is shown below and is depicted in :math:`x-z` plane as the computational grid also follows the same 2D plane of flow.

.. _turbulent-curvature-schematic:
.. figure:: sections_v/validation-figures/turbulent-curvature-schematic.*
   :width: 500px
   :align: center

   Schematic representation of the 2D curvature geometry (Not to scale)

As can be seen above, the 2D duct has a rapid bend of :math:`\alpha = 30^\circ` after about a distance of 1.4 m and the downstream extends up to 1.6 m in length. The cross-section of the duct is 0.127 m and the inner radius and outer radius of curvature are 0.127 m and 0.254 m respectively. The flow has a Reynolds number of :math:`2.1 \times 10^6` and U is the inlet velocity. To achieve a desired inlet velocity at an angle of 30 degrees, the velocity components are evaluated. The inlet temperature for this case is T = 293 K and for Air as a perfect gas, the speed of sound (:math:`a`) can be evaluated to 343.106 m/s. Based on the inlet Mach number and speed of sound, the inlet velocity can be calculated to U = 31.908 m/s. Using velocity and Reynolds number kinematic viscosity can be calculated. The table below summarises the inlet conditions used:

+------------+-------------------------+------------------+----------------------+--------------------+---------------------------------+
| Fluid      | :math:`Re_L~(1/m)`      | :math:`U~(m/s)`  | :math:`p~(m^2/s^2)`  | :math:`T~~(K)`     | :math:`\nu_i~(m^2/s)`           |
+============+=========================+==================+======================+====================+=================================+
| Air        | :math:`2.1 \times 10^6` | 31.9088          | :math:`(0)` Gauge    |  293               | :math:`1.519470\times10^{-5}`   |
+------------+-------------------------+------------------+----------------------+--------------------+---------------------------------+

In order to achieve a inlet velocity of U = 31.9088 m/s at :math:`\alpha=30^\circ`, the velocity components in :math:`x` and :math:`z` are resolved. These are given below

=============================    ===================    =================
:math:`\alpha~\rm{(Degrees)}`    :math:`u~(m/s)`        :math:`w~(m/s)`
=============================    ===================    =================
:math:`30^\circ`                 27.63389               15.95443
=============================    ===================    =================

It should be noted that in an incompressible flow, the density (:math:`\rho`) does not vary and a constant density assumption is valid throughout the calculation. Further, the temperature field is not solved and therefore its influence on viscosity can be neglected and a constant viscosity can be used. In Caelus for incompressible flow simulations, pressure and viscosity are always specified as kinematic.

*Turbulent Properties for Spalart–Allmaras and Spalart–Allmaras Curvature Correction model*

The inflow conditions used for turbulent properties in Spalart-Allamaras model was calculated as :math:`\tilde{\nu}_{i} = 3 \cdot \nu_i` and subsequently turbulent eddy viscosity was evaluated. The following table provides the values of these used in the current simulations:

.. _turbulent-curvature-SA-conditions:
.. table:: Turbulent freestream conditions for SA Model

   +------------------------------------+----------------------------------+
   | :math:`\tilde{\nu}_i~(m^2/s)`      | :math:`\nu_{t~i}~(m^2/s)`        |
   +====================================+==================================+
   | :math:`4.558411 \times 10^{-5}`    | :math:`3.197543 \times 10^{-6}`  |
   +------------------------------------+----------------------------------+

*Turbulent Properties for k-omega SST model*

The turbulent inflow boundary conditions used for :math:`k-\omega~\rm{SST}` were calculated as follows and is as given in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_val.html>`__

.. math::
   
   k_{i} = \frac{3}{2} (U_i I)^2

.. math::

   \omega_{i} = 1 \times 10^{-6} \cdot \frac{\rho_i a^2_i}{\mu_i}

.. math::

   \nu_{t~i} = 0.009 \times \nu_i

Note that the dynamic viscosity in the above equation is obtained from Sutherland formulation and density is evaluated as :math:`\rho = \mu / \nu`. In the below table, the turbulent properties used in the current simulations are provided

.. _turbulent-curvature-SST-conditions:
.. table:: Turbulent freestream conditions for SST Model

   +----------------+----------------------------------+----------------------------------+---------------------------------+
   |:math:`I`       | :math:`k_{i}~(m^2/s^2)`          | :math:`\omega_{i}~(1/s)`         | :math:`\nu_{t~i}~(m^2/s)`       |
   +================+==================================+==================================+=================================+
   |:math:`0.083\%` | :math:`1.0521 \times 10^{-3}`    | :math:`7747.333`                 | :math:`1.36756 \times 10^{-7}`  |
   +----------------+----------------------------------+----------------------------------+---------------------------------+

**Computational Domain and Boundary Conditions**

The computational domain for the duct is quite simple and follows the geometry as is shown in :numref:`turbulent-curvature-domain`. The walls are modelled as no-slip boundary and is highlighted in blue and the outlet is placed at the end of the duct.

.. _turbulent-curvature-domain:
.. figure:: sections_v/validation-figures/turbulent-curvature-domain.*
   :width: 800px
   :align: center

   Computational domain for a 2D convex curvature (Not to scale)

*Boundary Conditions and Initialisation*

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 27.63389~m/s`; :math:`v = 0.0~m/s` and :math:`w = 15.95443~m/s` in :math:`x, y` and :math:`z` directions respectively
   - Pressure: Zero gradient
   - Turbulence:

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t~i}` and :math:`\tilde{\nu}_{i}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{i}`, :math:`\omega_{i}` and :math:`\nu_{t~i}` as given in the above table)

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu} =0`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k = 0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)

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

     - SA & SA-RC (Fixed uniform values of :math:`\nu_{t~i}` and :math:`\tilde{\nu}_{i}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{i}`, :math:`\omega_{i}` and :math:`\nu_{t~i}` as given in the above table)

**Computational Grid**

The computational grid in 3D for the convex curvature duct was obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/smitscurve_val.html>`__ as a Plot3D format. The same was used in Caelus by converting it in the required format with the use of `Pointwise <http://www.pointwise.com/>`_. Since the flow field is assumed to the two-dimensional, the 2D computational plane of interest is in :math:`x-z` directions. SimpleSolver is a 3D solver, therefore the two additional :math:`x-z` planes are specified with empty boundary conditions. To look at the effect of grid sensitivity, four grids were considered from the original set of five, while the coarsest grid was not included in this study. The table below gives the details of different grids used.

======================  ==================================   ===============================  =================
Grid                    Cells in streamwise direction        Cells in normal direction        Total
======================  ==================================   ===============================  =================
Grid-2                  128                                  48                               6144
Grid-3                  256                                  96                               98304
Grid-4                  512                                  192                              98,304
Grid-5                  1024                                 384                              393,216
======================  ==================================   ===============================  =================

The 2D convex curvature grid is shown in :numref:`turbulent-curvature-grid` below, for Grid-3 in :math:`x-z` plane and the inset shows the grids in the vicinity of the strong curvature. Grids are refined close to the wall in order to capture the turbulent boundary layer and all grids have a :math:`y^+ < 1` and no wall function is used through the validation and verification of this configuration.

.. _turbulent-curvature-grid:
.. figure:: sections_v/validation-figures/turbulent-curvature-grid.*
   :width: 800px
   :align: center

   Convex curvature grid (Grid-3) shown in 2D (Inset shows the close-up of the curvature region)

**Results and Discussion**

The turbulent flow inside the convex curvature duct was simulated using Caelus |version| through the use of simpleSolver. The solutions were run until the residuals for pressure, velocity and turbulent quantities were less than :math:`1 \times 10^{-6}`.  The finite volume discretization of the gradient of pressure and velocity was carried out using the linear approach. Where as the divergence of velocity and mass flux was carried out through the linear upwind method. However, for the divergence of the turbulent quantities, upwind approach was utilised and linear approach for the divergence of the Reynolds stress terms. For the discretization of the Laplacian terms, again linear corrected method was used. For some grids having greater than 50 degree non-orthogonal angle, linear limited with a value of 0.5 was used for the Laplacian of the turbulent stress terms.

The verification results of the turbulence model are discussed first, which is then followed by the experimental validation.

*Verification results Spalart–Allmaras (SA)*

In :numref:`turbulent-curvature-caelus-sa`, the skin-friction distribution obtained over the lower wall of the duct is shown from Caelus for different grids. As can be seen, there is very little difference in skin-friction variation among the different grids. The oscillatory behaviour noted at Grid-5 very close to the corner is also apparent in CFL3D data as will be see in :numref:`turbulent-curvature-caelus-sa`.

.. _turbulent-curvature-caelus-sa:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-SA.*
   :width: 600px
   :align: center
   
   Skin-friction distribution obtained from Caelus simulations using SA turbulence model on the lower surface of the duct

The skin-friction coefficient comparison with CFL3D is shown in :numref:`turbulent-curvature-caelus-cfl3d-sa`. It should be noted that the available solution from CFL3D was for Grid-4 and hence to be consistent, Grid-4 solution from Caelus is used for comparison. In the vicinity of strong curvature region, Caelus compares very well with CFL3D, however both upstream and downstream, there seems to be some difference in the solution.

.. _turbulent-curvature-caelus-cfl3d-sa:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-CFL3D-SA.*
   :width: 600px
   :align: center
   
   Skin-friction comparison between Caelus and CFL3D using SA turbulence model on the lower surface of the duct

*Verification results Spalart–Allmaras Rotational/Curvature (SA-RC)*

The following :numref:`turbulent-curvature-caelus-sarc` shows the grid sensitivity and verification with CFL3D data respectively. The solution that have been used for verification is obtained from Grid-4 and the trends are similar to what is noted for the SA model.

.. _turbulent-curvature-caelus-sarc:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-SARC.*
   :width: 600px
   :align: center
   
   Skin-friction distribution obtained from Caelus simulations using SA-RC turbulence model on the lower surface of the duct

.. _turbulent-curvature-Caelus-CFL3D-SARC:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-CFL3D-SARC.*
   :width: 600px
   :align: center
   
   Skin-friction comparison between Caelus and CFL3D using SA-RC turbulence model on the lower surface of the duct

*Verification results k-Omega SST*

In :numref:`turbulent-curvature-caelus-sst`, the skin-friction sensitivity is shown over the lower wall obtained using Caelus with the :math:`k-\omega~\rm{SST}` model. After Grid-3, not much difference in values can be noted. With Grid-5, however some oscillations can be see upstream and in the vicinity of the curvature.

.. _turbulent-curvature-caelus-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-SST.*
   :width: 600px
   :align: center
   
   Skin-friction distribution obtained from Caelus simulations using :math:`k-\omega~\rm{SST}` turbulence model on the lower surface of the duct

The skin-friction comparison between Caelus and CFL3D is shown in :numref:`turbulent-curvature-caelus-cfl3d-sst`. A very good agreement between the two is obtained.

.. _turbulent-curvature-caelus-cfl3d-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-CFL3D-SST.*
   :width: 600px
   :align: center
   
   Skin-friction comparison between Caelus and CFL3D using :math:`k-\omega~\rm{SST}` turbulence model on the lower surface of the duct

*Experimental validation* 

This section details the experimental validation carried out for Caelus and both skin-friction and pressure coefficients obtained experimentally by Smits, A. J et al. :cite:`Smits1979` are compared. Further, CFL3D is also included. In :numref:`turbulent-curvature-caelus-cfl3d-expt`, skin-friction distribution obtained from Caelus using different turbulence models is compared with the experiments. Both SA-RC and :math:`k-\omega~\rm{SST}` has a fair agreement with experiments down stream of the curvature, more so with the SA-RC model. However upstream they all seem to predict nearly the same values.

.. _turbulent-curvature-caelus-cfl3d-expt:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-CFL3D-Expt.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus, experiments and CFL3D on the lower surface of the duct.

:numref:`turbulent-curvature-caelus-cfl3d-expt-p` shows the comparison of pressure-coefficient (:math:`c_p`) distribution with experiments and CFL3D data on the lower surface. Firstly, the solutions obtained from Caelus with the three turbulence models essentially produces the same values and matches exactly with the CFL3D data. In comparison with experiments, the agreement is very good in the upstream, vicinity of the curvature and downstream and, identical to CFL3D's behaviour. Note that for obtaining the pressure-coefficient (:math:`c_p`) values, a reference pressure (:math:`p_{ref}`) is needed. However, this is not specified in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/smitscurve_val.html>`__ for this case and hence a value of 145 Pa has been used.

.. _turbulent-curvature-caelus-cfl3d-expt-p:
.. figure:: sections_v/validation-figures/p-turbulent-curvature-Caelus-CFL3D-Expt.*
   :width: 600px
   :align: center

   Pressure coefficient comparison between Caelus, experiments and CFL3D on the lower surface of the duct.

Experimental skin-friction data :cite:`Smits1979` is also available over the upper surface of the duct and is used to compare the Caelus results. :numref:`turbulent-curvature-caelus-cfl3d-expt-upper` shows the comparison. Similar to the behaviour noted for the lower surface, the SA-RC model tends to be closer to the experimental data. In general, all the three turbulence model have similar trends and agrees very closely with the CFL3D data.

.. _turbulent-curvature-caelus-cfl3d-expt-upper:
.. figure:: sections_v/validation-figures/cf-turbulent-curvature-Caelus-CFL3D-Expt-upper.*
   :width: 600px
   :align: center

   Skin-friction coefficient comparison between Caelus, experiments and CFL3D on the upper surface of the duct.

**Conclusions**

A detailed verification and validation of a turbulent flow in a convex curvature duct were carried out using Caelus |version| and simpleSolver. Here, three turbulence models used and the solutions were verified against CFL3D data. As a part of validation, Caelus results were compared with the experimental data obtained on both lower and upper surfaces. The comparison was good with both CFL3D as well as with experiments. This suggests that the implementation of the turbulence models is correct and is being solved accurately.
