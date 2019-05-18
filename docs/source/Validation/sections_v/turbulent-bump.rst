.. _turbulent-bump-verification-label:

Two-dimensional Bump in a Channel
---------------------------------

*Turbulent, incompressible flow over a two-dimensional bump in a channel*

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`a`               Speed of sound                        :math:`m/s`
:math:`c_f`             Skin friction coefficient             Non-dimensional
:math:`k`               Turbulent kinetic energy              :math:`m^2/s^2` 
:math:`L`               Length of the bump                    :math:`m`
:math:`M_\infty`        Freestream Mach number                Non-dimensional
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_L`            Reynolds number                       :math:`1/m`
:math:`T`               Temperature                           :math:`K`
:math:`u`               Local velocity in x-direction         :math:`m/s`
:math:`U`               Freestream velocity in x-direction    :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`z`               Distance in z-direction               :math:`m`
:math:`y^+`             Wall distance                         Non-dimensional
:math:`\mu`             Dynamic viscosity                     :math:`kg/m~s`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
:math:`\tilde{\nu}`     Turbulence field variable             :math:`m^2/s`
:math:`\rho`            Density                               :math:`kg/m^3`
:math:`\omega`          Specific dissipation rate             :math:`1/s`
:math:`\infty`          Freestream conditions                 -
:math:`t` (subscript)   Turbulent property                    -
======================  ==================================    =========================

**Introduction**

This case covers the verification of turbulent incompressible flow over a two-dimensional bump in a channel. The bump acts as a perturbation causing local changes to the velocity and pressure over the bump surface. Since the perturbation is quite small, the flow remains attached to the bump surface. The simulations are carried out over a series of four grids which are successively refined and are studied over two turbulence models, serving as a grid independence study. The distribution of skin-friction (:math:`c_f`) is then compared with that of the `CFL3D <http://cfl3d.larc.nasa.gov>`__ data.

**Problem definition**

This verification exercise is based on the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/bump.html>`__ case for a 2D bump in a channel and follows the same conditions used in the limit of incompressibility. In this case, `CFL3D <http://cfl3d.larc.nasa.gov>`__ uses a freestream Mach number (:math:`M_\infty`) of 0.2. The schematic of the geometric configuration is shown in :numref:`turbulent-bump-schematic`.

.. _turbulent-bump-schematic:
.. figure:: sections_v/validation-figures/turbulent-bump-schematic.*
   :width: 500px
   :align: center

   Schematic representation of the close-up of the bump (Not to scale)

The location of the plate upstream of the bump begins at x=0 m and ends at x = 1.5 m, giving a total plate length of L = 1.5 m. The flow has a Reynolds number of :math:`3 \times 10^6` with a freestream velocity :math:`U_\infty` in the x-direction. The temperature of the inflow is assumed to be 300 K and for Air as a perfect gas, the speed of sound (:math:`a`) can be evaluated to 347.180 m/s. With the freestream Mach number and speed of sound, the freestream velocity was calculated to :math:`U_\infty` = 69.436 m/s. Kinematic viscosity was then obtained from velocity and Reynolds number. The following table summarises the freestream conditions used for this case.

+------------+-----------------------+------------------------+-----------------------------+----------------------+---------------------------------+
| Fluid      | :math:`Re_L~(1/m)`    | :math:`U_\infty~(m/s)` | :math:`p_\infty~(m^2/s^2)`  | :math:`T_\infty~(K)` | :math:`\nu_\infty~(m^2/s)`      |
+============+=======================+========================+=============================+======================+=================================+
| Air        | :math:`3 \times 10^6` | 69.436113              | :math:`(0)` Gauge           |  300                 | :math:`2.314537\times10^{-5}`   |
+------------+-----------------------+------------------------+-----------------------------+----------------------+---------------------------------+

Since the flow is incompressible, the density (:math:`\rho`) does not change and therefore, a constant density is assumed throughout the calculation. Further, the temperature is not considered and hence it does not have any influence on viscosity, which is therefore kept constant. Note that in Caelus, pressure and viscosity are always specified as kinematic for a incompressible flow simulation.

*Turbulent Properties for Spalart–Allmaras model*

The turbulent inflow boundary conditions used for the Spalart–Allmaras model were calculated as :math:`\tilde{\nu}_{\infty} = 3 \cdot \nu_\infty` and subsequently turbulent eddy viscosity was evaluated. The following table provides the values of these used in the current simulations:

.. _turbulent-bump-SA-conditions:
.. table:: Turbulence freestream conditions for SA model

   +------------------------------------+----------------------------------+
   | :math:`\tilde{\nu}_\infty~(m^2/s)` | :math:`\nu_{t~\infty}~(m^2/s)`   |
   +====================================+==================================+
   | :math:`6.943611 \times 10^{-5}`    | :math:`4.8706713 \times 10^{-6}` |
   +------------------------------------+----------------------------------+

*Turbulent Properties for k-omega SST model*

The turbulent inflow boundary conditions used for :math:`k-\omega~\rm{SST}` were calculated as follows and is as given in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__

.. math::
   
   k_{\infty} = 9 \times 10^{-9} \cdot a^2_\infty = \frac{0.675 U_\infty}{Re_L}

.. math::

   \omega_{\infty} = 1 \times 10^{-6} \cdot \frac{\rho_\infty a^2_\infty}{\mu_\infty} = \frac{50 U_\infty}{L}

.. math::

   \nu_{t~\infty} = 0.009 \times \nu_\infty

Note that the dynamic viscosity for the above equation is obtained from Sutherland formulation and density is calculated as :math:`\rho = \mu / \nu`. The below table provides the turbulent properties used in the current simulations:

.. _turbulent-bump-SST-conditions:
.. table:: Turbulence freestream conditions for :math:`k-\omega~\rm{SST}` model

   +------------------------------------+----------------------------------+---------------------------------+
   | :math:`k_{\infty}~(m^2/s^2)`       | :math:`\omega_{\infty}~(1/s)`    | :math:`\nu_{t~\infty}~(m^2/s)`  |
   +====================================+==================================+=================================+
   | :math:`1.0848 \times 10^{-3}`      | :math:`5207.6475`                | :math:`2.08310 \times 10^{-7}`  |
   +------------------------------------+----------------------------------+---------------------------------+

**Computational Domain and Boundary Conditions**

The computational domain consists of a rectangular channel encompassing the bump. In :numref:`turbulent-bump-domain` , the details of the boundaries used in two-dimensions (:math:`x-y` plane) are shown. The region of interest, which is the bump extends between :math:`0\leq x \leq 1.5~m` and has a no-slip boundary condition. Upstream and downstream of the bump, the symmetry boundary extends about 17 bump lengths. The inlet boundary is placed at the start of the symmetry at :math:`x = -25.0~m` and the outlet is placed at :math:`x = 26.5~m`. For the entire top boundary, symmetry plane condition is used.

.. _turbulent-bump-domain:
.. figure:: sections_v/validation-figures/turbulent-bump-domain.*
   :width: 500px
   :align: center

   Computational domain for a 2D bump (Not to scale)

*Boundary Conditions and Initialisation*

Following are the boundary condition details used for the computational domain:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k = 0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

**Computational Grid**

The 3D computational grid was obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/bump.html>`__ as a Plot3D and was converted to Caelus format using `Pointwise <http://www.pointwise.com/>`__. In the Plot3D computational grid, the flow normal direction is :math:`z` and thus the two-dimensional plane of interest is in :math:`x-z` directions. Further, since the flow-field is of two-dimensional, and the simpleSolver being a 3D solver, the two :math:`x-z` planes are specified with empty boundary conditions. A series of four grids were considered from the original set of five, excluding the coarsest grid  and the following table give its details.

======================  ==================================   ===============================  =============== ============
Grid                    Cells in :math:`x`-direction         Cells in :math:`z`-direction     Total           :math:`y^+`
======================  ==================================   ===============================  =============== ============
Grid-2                  176                                  80                               14,080          0.236
Grid-3                  352                                  160                              56,320          0.118 
Grid-4                  704                                  320                              225,280         0.059
Grid-5                  1408                                 640                              901,120         0.03
======================  ==================================   ===============================  =============== ============

The 2D grid in :math:`x-z` plane is shown in :numref:`turbulent-bump-grid` for Grid-3. As can be noted, the grid is sufficiently refined close to the wall in the normal direction. In addition, the grids are refined in the vicinity of the bump, including both upstream and downstream which can be seen in the inset. All grids have a :math:`y^+ < 1` and no wall function is used for the wall boundary in the current verification cases.

.. _turbulent-bump-grid:
.. figure:: sections_v/validation-figures/turbulent-bump-grid.*
   :width: 800px
   :align: center

   Bump grid (Grid-3) in 2D

**Results and Discussion**

The steady-state solution of the turbulent flow over a two-dimensional bump was obtained using Caelus |version|. The simpleSolver was used for the calculations and was run for a sufficient length until the residuals for pressure, velocity and turbulent quantities were less than :math:`1 \times 10^{-6}`. The finite volume discretization of the gradient of pressure and velocity was carried out using the linear approach. Where as the divergence of velocity and mass flux was carried out through the linear upwind method. However, for the divergence of the turbulent quantities, upwind approach was utilised and linear approach for the divergence of the Reynolds stress terms. For the discretization of the Laplacian terms, again linear corrected method was used. For some grids having greater than 50 degree non-orthogonal angle, linear limited with a value of 0.5 was used for the Laplacian of the turbulent stress terms.

*Spalart–Allmaras*

The skin-friction distributions over the 2D bump obtained from Caelus for different grids are shown in :numref:`cf-turbulent-bump-caelus-sacc`. There is very little difference in the skin-friction beyond Grid-2 suggesting that a grid-independence solution is achieved.

.. _cf-turbulent-bump-caelus-sacc:
.. figure:: sections_v/validation-figures/cf-turbulent-bump-Caelus-SACC.*
   :width: 600px
   :align: center

   Skin-friction distribution for various grids obtained from Caelus simulation using Spalart–Allmaras turbulence model

In :numref:`cf-turbulent-bump-caelus-cfl3d-sacc` , the comparison between Caelus and CFL3D is made for Grid-5 and as can be seen, a very good agreement is obtained over the entire region of the bump.

.. _cf-turbulent-bump-caelus-cfl3d-sacc:
.. figure:: sections_v/validation-figures/cf-turbulent-bump-Caelus-CFL3D-SACC.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using Spalart–Allmaras turbulence model

*k-Omega SST*

The skin-friction distribution variation for different grids obtained from :math:`k-\omega~\rm{SST}` model is shown in :numref:`cf-turbulent-bump-caelus-sst`.

.. _cf-turbulent-bump-caelus-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-bump-Caelus-SST.*
   :width: 600px
   :align: center

   Skin-friction distribution for various grids obtained from Caelus simulation using :math:`k-\omega~\rm{SST}` turbulence model

In :numref:`cf-turbulent-bump-caelus-cfl3d-sst` , the skin-friction comparison between Caelus and CFL3D is made for Grid-5 and is shown.

.. _cf-turbulent-bump-caelus-cfl3d-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-bump-Caelus-CFL3D-SST.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using :math:`k-\omega~\rm{SST}` turbulence model

**Conclusions**

The steady turbulent flow simulation over a two-dimensional bump was carried out using Caelus |version| employing simpleSolver. The solutions were obtained with two turbulence models, implemented in-house and the results were verified against CFL3D data. The comparison was found to be in good agreement with CFL3D suggesting that the turbulence model implementation is accurate in Caelus.
