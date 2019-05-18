.. _turbulent-flat-plate-verification-label:

Zero Pressure Gradient Flat Plate
---------------------------------

:index:`Turbulent`, :index:`incompressible` flow over a :index:`two-dimensional` Sharp-Leading Edge flat-plate

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`a`               Speed of sound                        :math:`m/s`
:math:`c_f`             Skin friction coefficient             Non-dimensional
:math:`k`               Turbulent kinetic energy              :math:`m^2/s^2` 
:math:`L`               Length of the plate                   :math:`m`
:math:`M_\infty`        Freestream Mach number                Non-dimensional
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_L`            Reynolds number                       :math:`1/m`
:math:`T`               Temperature                           :math:`K`
:math:`u`               Local velocity in x-direction         :math:`m/s`
:math:`U`               Freestream velocity in x-direction    :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
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

In this case, steady turbulent incompressible flow over a two-dimensional sharp-leading edge :index:`flat-plate` is considered at zero angle of incidence, which generates a turbulent boundary layer with zero-pressure gradient over the flat plate. The simulations are carried over a series of four successively refined grids and the solutions are compared with the `CFL3D <http://cfl3d.larc.nasa.gov>`__ data. This therefore serves as both grid independence study and verification of the turbulence models. The distribution of skin-friction coefficient (:math:`c_f`) along the plate is used to verify the accuracy of the models.

**Problem definition**

This exercise is based on the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__ case for a flat-plate and follows the same conditions used in the incompressible limit. However, note that `CFL3D <http://cfl3d.larc.nasa.gov>`__ uses a freestream Mach number (:math:`M_\infty`) of 0.2 as it is a compressible solver. The schematic of the geometric configuration is shown in :numref:`turbulentflatplateschematic`.

.. _turbulentflatplateschematic:
.. figure:: sections_v/validation-figures/turbulent-flat-plate-schematic.*
   :width: 500px
   :align: center

   Schematic representation of the flat plate

The length of the plate is L = 2.0 m, wherein, x = 0 is at the leading edge and the Reynolds number is :math:`5 \times 10^6` and :math:`U_\infty` is the freestream velocity in the x -direction. The inflow temperature in this case is assumed to be 300 K and for Air as a perfect gas, the speed of sound (:math:`a`) can be evaluated to 347.180 m/s. Based on the freestream Mach number and speed of sound, velocity can be calculated, which is :math:`U_\infty` = 69.436 m/s. Using velocity and Reynolds number, the :index:`kinematic viscosity` can be evaluated. The following table summarises the freestream conditions used:

.. table:: Freestream conditions

  +------------+-----------------------+------------------------+-----------------------------+-----------------------+---------------------------------+
  | Fluid      | :math:`Re_L~(1/m)`    | :math:`U_\infty~(m/s)` | :math:`p_\infty~(m^2/s^2)`  | :math:`T_\infty~(K)`  | :math:`\nu_\infty~(m^2/s)`      |
  +============+=======================+========================+=============================+=======================+=================================+
  | Air        | :math:`5 \times 10^6` | 69.436113              | :math:`(0)` Gauge           |  300                  | :math:`1.38872\times10^{-5}`    |
  +------------+-----------------------+------------------------+-----------------------------+-----------------------+---------------------------------+

Note that in an incompressible flow the :index:`density` (:math:`\rho`) does not change and is reasonable to assume a constant density throughout the calculation. In addition, temperature is not considered and therefore the viscosity can also be held constant. In Caelus, for incompressible flows, pressure and viscosity are always specified as kinematic.

*Turbulent Properties for Spalart–Allmaras model*

The turbulent inflow boundary conditions used for the Spalart–Allmaras model were calculated as :math:`\tilde{\nu}_{\infty} = 3 \cdot \nu_\infty` and subsequently turbulent eddy viscosity was evaluated. The following table provides the values of these used in the current simulations:

.. _turbulent-flat-plate-SA-conditions:
.. table:: Turbulence freestream conditions for SA model

   +------------------------------------+----------------------------------+
   | :math:`\tilde{\nu}_\infty~(m^2/s)` | :math:`\nu_{t~\infty}~(m^2/s)`   |
   +====================================+==================================+
   | :math:`4.166166 \times 10^{-5}`    | :math:`2.9224023 \times 10^{-6}` |
   +------------------------------------+----------------------------------+

*Turbulent Properties for k-omega SST model*

The turbulent inflow boundary conditions used for :math:`k-\omega~\rm{SST}` were calculated as follows and is as given in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__

.. math::
   
   k_{\infty} = 9 \times 10^{-9} \cdot a^2_\infty = \frac{1.125 U_\infty^2}{Re_L}

.. math::

   \omega_{\infty} = 1 \times 10^{-6} \cdot \frac{\rho_\infty a^2_\infty}{\mu_\infty} = \frac{125 U_\infty}{L}

.. math::

   \nu_{t~\infty} = 0.009 \times \nu_\infty

Note that the :index:`dynamic viscosity` for the above equation is obtained from Sutherland formulation and density is calculated as :math:`\rho = \mu / \nu`. The below table provides the turbulent properties used in the current simulations

.. _turbulent-flat-plate-SST-conditions:
.. table:: Turbulence freestream conditions for :math:`k-\omega~\rm{SST}` model

   +------------------------------------+----------------------------------+---------------------------------+
   | :math:`k_{\infty}~(m^2/s^2)`       | :math:`\omega_{\infty}~(1/s)`    | :math:`\nu_{t~\infty}~(m^2/s)`  |
   +====================================+==================================+=================================+
   | :math:`1.0848 \times 10^{-3}`      | :math:`8679.5135`                | :math:`1.24985 \times 10^{-7}`  |
   +------------------------------------+----------------------------------+---------------------------------+

**Computational Domain and Boundary Conditions**

The computational domain is a rectangular block encompassing the flat-plate. :numref:`turbulent-flat-plate-domain` below shows the details of the boundaries used in two-dimensions (:math:`x-y` plane). As can be seen, the region of interest (highlighted in blue) extends between :math:`0\leq x \leq 2.0~m` and has a no-slip boundary condition. Upstream of the leading edge, a symmetry boundary is used to simulate a freestream flow approaching the flat-plate. The inlet boundary as shown in :numref:`turbulent-flat-plate-domain` is placed at start of the symmetry at :math:`x = -0.3333~m` and the outlet at the exit plane of the no-slip wall (blue region) at :math:`x = 2.0~m`.  A symmetry plane condition is used for the entire top boundary.

.. _turbulent-flat-plate-domain:
.. figure:: sections_v/validation-figures/turbulent-flat-plate-domain.*
   :width: 500px
   :align: center

   Flat-plate computational domain 

*Boundary Conditions and Initialisation*

Following are the boundary condition details used for the computational domain:

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart-Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\textrm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart-Allmaras (Fixed uniform values of :math:`\nu_{t}=0` and :math:`\tilde{\nu}=0`)
     - :math:`k-\omega~\textrm{SST}` (Fixed uniform values of :math:`k = 0` and :math:`\nu_t=0`; :math:`\omega` = omegaWallFunction)

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart-Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\textrm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 69.436113~m/s` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart-Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\textrm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)


**Computational Grid**

The 3D structured grid was obtained from `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__ as a Plot3D and was converted to Caelus format using `Pointwise <http://www.pointwise.com/>`__. It should be noted that the flow normal direction in the Plot3D grids is :math:`z` and the two-dimensional plane of interest is in :math:`x-z` directions. Since the flow-field of interest is two-dimensional, and :index:`simpleSolver` being a 3D solver, the two :math:`x-z` planes are specified with empty boundary conditions. As mentioned earlier, a series of four grids were considered from the original set of five, excluding the coarsest. Details of the different grids used are given below.

======================  ==================================   ===============================  =============== ============
Grid                    Cells in :math:`x`-direction         Cells in :math:`z`-direction     Total           :math:`y^+`
======================  ==================================   ===============================  =============== ============
Grid-2                  68                                   48                               3264            0.405
Grid-3                  136                                  96                               13,056          0.203
Grid-4                  272                                  192                              52,224          0.101
Grid-5                  544                                  384                              208,896         0.05
======================  ==================================   ===============================  =============== ============

In :numref:`turbulent-flat-plate-grid`, the 2D grid in the :math:`x-z` plane is shown for Grid-4. As can be seen, the grid is refined close to the wall in order to capture the turbulent boundary layer accurately. All grids have :math:`y^+ < 1` and no wall function is used for the wall boundary in the current verification cases.

.. _turbulent-flat-plate-grid:
.. figure:: sections_v/validation-figures/turbulent-flat-plate-grid.*
   :width: 600px
   :align: center

   Flat-plate grid (Grid-4) in 2D


**Results and Discussion**

The steady-state solution of the turbulent flow over a flat plate was obtained using :index:`Caelus` |version|. The  simpleSolver was used here and the solution was simulated for a sufficient length until the residuals for pressure, velocity and turbulent quantities were less than :math:`1 \times 10^{-6}`. The finite volume discretization of the gradient of pressure and velocity was carried out using the linear approach. Where as the divergence of velocity and mass flux was carried out through the linear upwind method. However, for the divergence of the turbulent quantities, upwind approach was utilised and linear approach for the divergence of the Reynolds stress terms. For the discretization of the Laplacian terms, again linear corrected method was used. For some grids having greater than 50 degree non-orthogonal angle, linear limited with a value of 0.5 was used for the Laplacian of the turbulent stress terms.

*Spalart–Allmaras*

In :numref:`cf-turb-flat-plate-caelus-sacc`, the :index:`skin-friction` distribution along the flat-plate obtained from Caelus for different grids is shown. As can be seen, all grids produce the same skin-friction values suggesting a grid-independent solution is achieved.

.. _cf-turb-flat-plate-caelus-sacc:
.. figure:: sections_v/validation-figures/cf-turbulent-flat-plate-Caelus-SACC.*
   :width: 600px
   :align: center

   Skin-friction distribution for various grids obtained from Caelus simulation using Spalart–Allmaras turbulence model

In :numref:`cf-turb-flat-plate-caelus-cfl3d-sacc`, the skin-friction distribution obtained from Caelus on Grid-5 is compared with CFL3D of the same grid. An excellent agreement is obtained all along the plate.

.. _cf-turb-flat-plate-caelus-cfl3d-sacc:
.. figure:: sections_v/validation-figures/cf-turbulent-flat-plate-Caelus-CFL3D-SACC.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using Spalart–Allmaras turbulence model

*k-Omega SST*

The skin-friction distribution for various grids obtained from :math:`k-\omega~\rm{SST}` model is shown in :numref:`cf-turb-flat-plate-caelus-sst`.

.. _cf-turb-flat-plate-caelus-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-flat-plate-Caelus-SST.*
   :width: 600px
   :align: center

   Skin-friction distribution for various grids obtained from Caelus simulation using :math:`k-\omega~\rm{SST}` turbulence model

The skin-friction comparison between Caelus and CFL3D for Grid-5 is shown in :numref:`cf-turb-flat-plate-caelus-cfl3d-sst`.

.. _cf-turb-flat-plate-caelus-cfl3d-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-flat-plate-Caelus-CFL3D-SST.*
   :width: 600px
   :align: center

   Skin-friction comparison between Caelus and CFL3D using :math:`k-\omega~\rm{SST}` turbulence model

**Conclusions**

The steady turbulent flow over a two-dimensional flat-plate was simulated using Caelus |version| utilising the simpleSolver. The simulations were carried out with two turbulence models and the obtained solutions were verified against CFL3D data. The results were found to be in good agreement with CFL3D and suggesting the turbulence implementation in Caelus is accurate.

