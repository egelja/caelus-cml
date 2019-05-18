.. _turbulent-airfoil-verification-label:

Two-dimensional NACA 0012 Airfoil
---------------------------------

**Nomenclature**

*Turbulent, incompressible flow over a two-dimensional NACA airfoil*

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`a`               Speed of sound                        :math:`m/s`
:math:`c_f`             Skin friction coefficient             Non-dimensional
:math:`c_p`             Pressure coefficient                  Non-dimensional
:math:`C`               Chord length                          :math:`m`
:math:`I`               Turbulent intensity                   Percentage
:math:`k`               Turbulent kinetic energy              :math:`m^2/s^2` 
:math:`M_\infty`        Freestream Mach number                Non-dimensional
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_L`            Reynolds number                       :math:`1/m`
:math:`T`               Temperature                           :math:`K`
:math:`u`               Local velocity in x-direction         :math:`m/s`
:math:`w`               Local velocity in z-direction         :math:`m/s`
:math:`U`               Freestream velocity                   :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`z`               Distance in y-direction               :math:`m`
:math:`y^+`             Wall distance                         Non-dimensional
:math:`\alpha`          Angle of attack                       Degrees
:math:`\mu`             Dynamic viscosity                     :math:`kg/m~s`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
:math:`\tilde{\nu}`     Turbulence field variable             :math:`m^2/s`
:math:`\rho`            Density                               :math:`kg/m^3`
:math:`\omega`          Specific dissipation rate             :math:`1/s`
:math:`\infty`          Freestream conditions                 -
:math:`t` (subscript)   Turbulent property                    -
======================  ==================================    =========================

**Introduction**

This case deals with the steady turbulent incompressible flow over a two-dimensional NACA 0012 airfoil. The study is conducted at two angles of attack, :math:`\alpha = 0^\circ` and :math:`\alpha = 10^\circ` respectively. The simulations are carried out over a series of four grids and compared with CFL3D data and the results are also compared with the experimental data. This exercise therefore verifies and validates the turbulence models used through the distribution of skin-friction coefficient (:math:`c_f`) and pressure coefficient (:math:`c_p`) over the airfoil surface.

**Problem definition**

This verification and validation exercise is based on the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_val.html>`__ case for a NACA 0012 airfoil and follows the same flow conditions used in the incompressible limit. However, the numerical code CFL3D uses a freestream Mach number (:math:`M_\infty`) of 0.15. In :numref:`turbulent-airfoil-schematic` the schematic of the airfoil is shown. Note that the 2D plane in :numref:`turbulent-airfoil-schematic`  is depicted in :math:`x-z` directions as the computational grid also follows the same 2D plane.

.. _turbulent-airfoil-schematic:
.. figure:: sections_v/validation-figures/turbulent-airfoil-schematic.*
   :width: 600px
   :align: center

   Schematic representation of the 2D airfoil (Not to scale)

The length of the airfoil chord is C = 1.0 m, wherein, x = 0 is at the leading edge and the Reynolds number is :math:`6 \times 10^6` and :math:`U_\infty` is the freestream velocity. For :math:`\alpha = 10^\circ`, the velocity components are evaluated in order to have a same resultant freestream velocity. The freestream flow temperature in this case is assumed to be 300 K and for Air as a perfect gas, the speed of sound (:math:`a`) can be evaluated to 347.180 m/s. Based on the freestream Mach number and speed of sound, freestream velocity can be evaluated to :math:`U_\infty` = 52.077 m/s. Using the value of velocity and Reynolds number, kinematic viscosity can be calculated. The following table summarises the freestream conditions used:

+-------+-----------------------+------------------------+----------------------------+----------------------+-------------------------------+
| Fluid | :math:`Re_L~(1/m)`    | :math:`U_\infty~(m/s)` | :math:`p_\infty~(m^2/s^2)` | :math:`T_\infty~(K)` | :math:`\nu_\infty~(m^2/s)`    |
+=======+=======================+========================+============================+======================+===============================+
| Air   | :math:`6 \times 10^6` | 52.0770                | :math:`(0)` Gauge          | 300                  | :math:`8.679514\times10^{-6}` |
+-------+-----------------------+------------------------+----------------------------+----------------------+-------------------------------+

To get a freestream velocity of :math:`U_\infty` = 52.077 m/s at :math:`\alpha = 10^\circ`, the velocity components in :math:`x` and :math:`z` are resolved. These are provided in the table below

=============================    ===================    =================
:math:`\alpha~\rm{(Degrees)}`    :math:`u~(m/s)`        :math:`w~(m/s)`
=============================    ===================    =================
:math:`0^\circ`                  52.0770                0.0
:math:`10^\circ`                 51.2858                9.04307
=============================    ===================    =================

Note that in an incompressible flow, the density (:math:`\rho`) does not vary and a constant density can be assumed throughout the calculation. Further, since temperature is not considered here, the viscosity is also held constant. In Caelus for incompressible flow simulations, pressure and viscosity are always specified as kinematic.

*Turbulent Properties for Spalart–Allmaras model*

The turbulent inflow boundary conditions used for the Spalart–Allmaras model were calculated as :math:`\tilde{\nu}_{\infty} = 3 \cdot \nu_\infty` and subsequently turbulent eddy viscosity was evaluated. The following table provides the values of these used in the current simulations:

.. _turbulent-airfoil-SA-conditions:
.. table:: Turbulent freestream conditions for SA Model

   +-------------------------------------+----------------------------------+
   | :math:`\tilde{\nu}_\infty~(m^2/s)`  | :math:`\nu_{t~\infty}~(m^2/s)`   |
   +=====================================+==================================+
   | :math:`2.603854 \times 10^{-5}`     | :math:`1.8265016 \times 10^{-6}` |
   +-------------------------------------+----------------------------------+

*Turbulent Properties for k-omega SST model*

The turbulent inflow boundary conditions used for :math:`k-\omega~\rm{SST}` were calculated as follows and is as given in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_val.html>`__

.. math::
   
   k_{\infty} = \frac{3}{2} (U_\infty I)^2

.. math::

   \omega_{\infty} = 1 \times 10^{-6} \cdot \frac{\rho_\infty a^2_\infty}{\mu_\infty} = \frac{266.7 U_\infty}{L}

.. math::

   \nu_{t~\infty} = 0.009 \times \nu_\infty

Note that the dynamic viscosity in the above equation is obtained from Sutherland formulation and density is evaluated as :math:`\rho = \mu / \nu`. In the below table, the turbulent properties used in the current simulations are provided.

.. _turbulent-airfoil-SST-conditions:
.. table:: Turbulent freestream conditions for SST Model

   +-----------------+---------------------------------+---------------------------------+---------------------------------+
   | :math:`I`       | :math:`k_{\infty}~(m^2/s^2)`    | :math:`\omega_{\infty}~(1/s)`   | :math:`\nu_{t~\infty}~(m^2/s)`  |
   +=================+=================================+=================================+=================================+
   | :math:`0.052\%` | :math:`1.0999 \times 10^{-3}`   | :math:`13887.219`               | :math:`7.811564 \times 10^{-8}` |
   +-----------------+---------------------------------+---------------------------------+---------------------------------+

**Computational Domain and Boundary Conditions**

The computational domain used for the airfoil simulations and the details of the boundaries are shown in :numref:`turbulent-airfoil-domain` for a :math:`x-z` plane. The leading edge and the trailing edge extends between :math:`0 \leq x \leq 1.0~m` and the entire airfoil has a no-slip boundary condition. The far-field domain extends by about 500 chord lengths in the radial direction and the inlet is placed for the entire boundary highlighted in green. The outlet boundary is placed at the exit plane, which is at :math:`x \approx 500~m`. 

.. _turbulent-airfoil-domain:
.. figure:: sections_v/validation-figures/turbulent-airfoil-domain.*
   :width: 800px
   :align: center

   Computational domain for a 2D airfoil (Not to scale)

*Boundary Conditions and Initialisation*

* Inlet
   - Velocity:

     - :math:`\alpha=0^\circ`: Fixed uniform velocity :math:`u = 52.0770~m/s`; :math:`v = w = 0.0~m/s` in :math:`x, y` and :math:`z` directions respectively
     - :math:`\alpha=10^\circ`: Fixed uniform velocity :math:`u = 51.2858~m/s`; :math:`v = 0.0~m/s` and :math:`w = 9.04307~m/s` in :math:`x, y` and :math:`z` directions respectively
     - Pressure: Zero gradient
     - Turbulence:

       - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
       - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
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
     - :math:`\alpha=10^\circ`: Fixed uniform velocity :math:`u = 51.2858~m/s`; :math:`v = 0.0~m/s` and :math:`w = 9.04307~m/s` in :math:`x, y` and :math:`z` directions respectively
     - Pressure: Zero Gauge pressure
     - Turbulence:

       - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
       - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

**Computational Grid**

The 3D computational grid for the NACA 0012 airfoil was obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/naca0012_val.html>`__ as a Plot3D format. Using `Pointwise <http://www.pointwise.com/>`__ it was then converted to Caelus format. As indicated earlier, the two-dimensional plane of interest in the Plot3D grid is in :math:`x-z` directions. As the flow is considered here to be two-dimensional, and simpleSolver being a 3D solver, the two :math:`x-z` planes are specified with empty boundary conditions consequently treating as symmetry flow in :math:`y` direction. To study the sensitivity of the grid, four grids were considered from the original set of five, in which the coarsest grid was excluded from this study. Details of the different grids used are given in the below table. Not that for both angles of attack, same grid is used.

======================  ==================================   ===============================  =============== ============
Grid                    Cells over airfoil                   Cells in normal direction        Total           :math:`y^+`
======================  ==================================   ===============================  =============== ============
Grid-2                  128                                  64                               14,336          0.465
Grid-3                  256                                  128                              57,344          0.209
Grid-4                  512                                  256                              229,376         0.098
Grid-5                  1024                                 512                              917,504         0.047
======================  ==================================   ===============================  =============== ============

The below :numref:`turbulent-airfoil-grid` shows the 2D grid in :math:`x-z` plane for Grid-3 and the refinement around the airfoil is shown in the inset. Sufficient refinement can be seen in the wall normal direction and all the grid have a :math:`y^+ < 1` and no wall function is used for the airfoil surface throughout the current verification and validation cases.

.. _turbulent-airfoil-grid:
.. figure:: sections_v/validation-figures/turbulent-airfoil-grid.*
   :width: 800px
   :align: center

   Airfoil grid (Grid-3) shown in 2D

**Results and Discussion**

The solution to the turbulent flow over the NACA 0012 airfoil was obtained using Caelus |version|. SimpleSolver was used and the solutions were run sufficiently long until the residuals for pressure, velocity and turbulence quantities were less than :math:`1 \times 10^{-6}`. The finite volume discretization of the gradient of pressure and velocity was carried out using the linear approach. Where as the divergence of velocity and mass flux was carried out through the linear upwind method. However, for the divergence of the turbulent quantities, upwind approach was utilised and linear approach for the divergence of the Reynolds stress terms. For the discretization of the Laplacian terms, again linear corrected method was used. For some grids having greater than 50 degree non-orthogonal angle, linear limited with a value of 0.5 was used for the Laplacian of the turbulent stress terms.

The verification results are shown firstly for both angles of attack and is followed by the experimental validation data.

*Verification results: Spalart–Allmaras*

The following :numref:`turbulent-airfoil-caelus-sacc-0` and :numref:`turbulent-airfoil-caelus-sacc-10` shows the skin-friction distribution over the upper surface for :math:`\alpha=0^\circ` and :math:`\alpha=10^\circ` from Caelus for different grids. In both cases, Grid-4 and Grid-5 essentially produces the same solution suggesting a grid-independence solution is obtained.

.. _turbulent-airfoil-caelus-sacc-0:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-SACC-0.*
   :width: 600px
   :align: center
   
   Skin-friction distribution obtained from Caelus simulations using SA turbulence model for :math:`\alpha=0^\circ`
   
.. _turbulent-airfoil-caelus-sacc-10:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-SACC-10.*
   :width: 600px
   :align: center

   Skin-friction distribution obtained from Caelus simulations using SA turbulence model for :math:`\alpha=10^\circ`

In :numref:`turbulent-airfoil-caelus-cfl3d-sacc-0` and `Fig. #turbulent-airfoil-caelus-cfl3d-sacc-10` , the skin-friction is compared with CFL3D on Grid-4. As can be seen, a very good agreement between the two codes can be seen.

.. _turbulent-airfoil-caelus-cfl3d-sacc-0:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-CFL3D-SACC-0.*
   :width: 600px
   :align: center
   
   Skin-friction comparison between Caelus and CFL3D using SA turbulence model for :math:`\alpha=0^\circ`
   
.. _turbulent-airfoil-caelus-cfl3d-sacc-10:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-CFL3D-SACC-10.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using SA turbulence model for :math:`\alpha=10^\circ`

*Verification results: k-omega SST*

The skin-friction distribution obtained from using :math:`k-\omega~\rm{SST}` turbulence model for :math:`\alpha=0^\circ` and :math:`\alpha=10^\circ` is shown below for different grids. The grid-sensitivity behaviour is very similar to the Spalart–Allmaras turbulence case and no change is seen between Grid-4 and Grid-5.

.. _turbulent-airfoil-Caelus-SST-0:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-SST-0.*
   :width: 600px
   :align: center
   
   Skin-friction distribution obtained from Caelus simulations using :math:`k-\omega~\rm{SST}` turbulence model for :math:`\alpha=0^\circ`
   
.. _turbulent-airfoil-Caelus-SST-10:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-SST-10.*
   :width: 600px
   :align: center

   Skin-friction distribution obtained from Caelus simulations using :math:`k-\omega~\rm{SST}` turbulence model for :math:`\alpha=10^\circ`

The comparison of the skin-friction with CFL3D using :math:`k-\omega~\rm{SST}` is shown in :numref:`turbulent-airfoil-caelus-cfl3d-sst-0` and :numref:`turbulent-airfoil-caelus-cfl3d-sst-10` for both angle of attacks and similar to the previous case, a very good agreement between the two can be seen.

.. _turbulent-airfoil-caelus-cfl3d-sst-0:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-CFL3D-SST-0.*
   :width: 600px
   :align: center
   
   Skin-friction comparison between Caelus and CFL3D using :math:`k-\omega~\rm{SST}` turbulence model for :math:`\alpha=0^\circ`
   
.. _turbulent-airfoil-caelus-cfl3d-sst-10:
.. figure:: sections_v/validation-figures/cf-turbulent-airfoil-Caelus-CFL3D-SST-10.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using :math:`k-\omega~\rm{SST}` turbulence model for :math:`\alpha=10^\circ`

*Experimental validation*

Here, the Caelus data is compared with the pressure-coefficient (:math:`c_p`) obtained experimentally by Gregory, N. and O'Reilly, C. L :cite:`Gregory1970` for both angles of attack over the upper surface. In addition, the data obtained from CFL3D is also included for verification. There is a very good agreement with the current Caelus and experiments which indicates that the correct turbulence equations are being solved in both Spalart–Allmaras and :math:`k-\omega~\rm{SST}` models.

.. _turbulent-airfoil-Caelus-CFL3D-SA-SST-0-10:
.. figure:: sections_v/validation-figures/p-turbulent-airfoil-Caelus-CFL3D-SA-SST-0.*
   :width: 600px
   :align: center
   
   Pressure comparison between Caelus, experiments and CFL3D for :math:`\alpha=0^\circ`
   
.. figure:: sections_v/validation-figures/p-turbulent-airfoil-Caelus-CFL3D-SA-SST-10.*
   :width: 600px
   :align: center

   Pressure comparison between Caelus, experiments and CFL3D for :math:`\alpha=10^\circ`

**Conclusions**

Verification and validation over a two-dimensional NACA 0012 airfoil for turbulent inflow conditions were carried out using Caelus |version| employing simpleSolver. Two turbulence models that are implemented in-house were used and the solutions were verified with CFL3D data and subsequently validated with the experimental pressure coefficient values. The results were found to be in very good agreement suggesting that the turbulence modelling implementation is appropriate and solves accurately.
