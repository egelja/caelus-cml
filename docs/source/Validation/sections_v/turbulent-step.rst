.. _turbulent-step-verification-label:

Two-dimensional Backward Facing Step
--------------------------------------

*Turbulent, incompressible flow over a two-dimensional backward facing step*

**Nomenclature**

======================  =====================================       =========================
Symbol                  Definition                                  Units (SI)
======================  =====================================       =========================
:math:`a`               Speed of sound                              :math:`m/s`
:math:`c_f`             Skin friction coefficient                   Non-dimensional
:math:`c_p`             Pressure coefficient                        Non-dimensional
:math:`H`               Step height                                 :math:`m`
:math:`I`               Turbulent intensity                         Percentage
:math:`k`               Turbulent kinetic energy                    :math:`m^2/s^2` 
:math:`M_\infty`        Freestream Mach number                      Non-dimensional
:math:`p`               Kinematic pressure                          :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_H`            Reynolds number based on step height        Non-dimensional
:math:`T`               Temperature                                 :math:`K`
:math:`u`               Local velocity in x-direction               :math:`m/s`
:math:`u_*`             Frictional velocity                         :math:`m^2/s`
:math:`U`               Freestream velocity in x-direction          :math:`m/s`
:math:`U_{ref}`         Reference velocity                          :math:`m/s`
:math:`x`               Distance in x-direction                     :math:`m`
:math:`z`               Distance in z-direction                     :math:`m`
:math:`y^+`             Wall distance                               Non-dimensional
:math:`\epsilon`        Turbulent dissipation                       :math:`m^2/s^3`
:math:`\mu`             Dynamic viscosity                           :math:`kg/m~s`
:math:`\nu`             Kinematic viscosity                         :math:`m^2/s`
:math:`\tilde{\nu}`     Turbulence field variable                   :math:`m^2/s`
:math:`\rho`            Density                                     :math:`kg/m^3`
:math:`\tau_w`          Wall shear stress                           :math:`kg/m~s^2`
:math:`\omega`          Specific dissipation rate                   :math:`1/s`
:math:`\infty`          Freestream conditions                       -
:math:`t` (subscript)   Turbulent property                          -
======================  =====================================       =========================

**Introduction**

This study investigates steady turbulent, incompressible flow over a two-dimensional backward facing step at zero angle of incidence. Unlike the previous cases, the efficacy of wall functions in separated flow is evaluated for different turbulent models. The validation of these wall functions are carried out through the comparison of skin-friction coefficient (:math:`c_f`) and pressure coefficient (:math:`c_p`) downstream of the step with those of the experimental data. In addition to Spalart–Allmaras and :math:`k - \omega~\rm{SST}`, Realizable :math:`k-\epsilon` turbulence model was also considered in this exercise.

**Problem definition**

The backward facing step configuration is obtained from the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/backstep_val.html>`__ and is a widely considered case for the purpose of verification and validation. This particular study is based on the experiments carried out by Driver and Seegmiller :cite:`Driver1985`. The schematic of the step configuration in :numref:`turbulentstepschematic` below as considered in the `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/backstep_val.html>`__.

.. _turbulentstepschematic:
.. figure:: sections_v/validation-figures/turbulent-step-schematic.*
   :width: 500px
   :align: center

   Schematic representation of the backward facing step in 2D

The height of the step (H) is chosen to be 1.0 m and is located at x = 0 m. Upstream of the step, the plate extends to 110 step heights such that a fully developed turbulent boundary layer with proper thickness exists at separation (x = 0 m). This is followed by a downstream plate which extends up to 50 step heights. The flow has a Reynolds number of 36000 based on the step height with a freestream velocity (:math:`U_\infty`) in the x-direction. The inflow is assumed to be Air as a perfect gas with a temperature of 298 K, giving the value of speed of sound (:math:`a`) at 346.212 m/s. A reference Mach number (:math:`M_\infty`) of 0.128 is used to obtain the freestream velocity, which was :math:`U_\infty` = 44.315 m/s. The kinematic viscosity was then evaluated based on the velocity and the Reynolds number. The following table summarises the freestream conditions used for the backward facing step.

.. table:: Freestream conditions

   +------------+-----------------------+-------------------------+-----------------------------+-----------------------+---------------------------------+
   | Fluid      | :math:`Re_H`          | :math:`U_\infty~(m/s)`  | :math:`p_\infty~(m^2/s^2)`  | :math:`T_\infty~(K)`  | :math:`\nu_\infty~(m^2/s)`      |
   +============+=======================+=========================+=============================+=======================+=================================+
   | Air        | :math:`36000`         | 44.31525                | :math:`(0)` Gauge           |  298.330              | :math:`1.230979\times10^{-3}`   |
   +------------+-----------------------+-------------------------+-----------------------------+-----------------------+---------------------------------+

As with all the previous cases, the flow is incompressible and therefore the density (:math:`\rho`) does not change throughout the simulation. Further, the temperature is not accounted and has no influence on viscosity and is also held constant. However note that in Caelus, pressure and viscosity for incompressible flow are always specified as kinematic.

*Turbulent Properties for Spalart–Allmaras model*

The turbulent boundary conditions at the freestream used for Spalart-Allmaras model were calculated according to :math:`\tilde{\nu}_{\infty} = 3 \cdot \nu_\infty` and turbulent eddy viscosity was subsequently evaluated. In the following table, the values used in the current simulation are provided:

.. _turbulent-step-SA-conditions:
.. table:: Turbulence freestream conditions for SA model

   +------------------------------------+----------------------------------+
   | :math:`\tilde{\nu}_\infty~(m^2/s)` | :math:`\nu_{t~\infty}~(m^2/s)`   |
   +====================================+==================================+
   | :math:`3.692937 \times 10^{-3}`    | :math:`2.590450 \times 10^{-4}`  |
   +------------------------------------+----------------------------------+

*Turbulent Properties for k-omega SST model*

The turbulent inflow boundary conditions used for :math:`k-\omega~\rm{SST}` were calculated as follows and is as given in `Turbulence Modeling Resource <http://turbmodels.larc.nasa.gov/flatplate.html>`__

.. math::
   
   k_{\infty} = \frac{3}{2} (U_\infty I)^2

.. math::

   \omega_{\infty} = 1 \times 10^{-6} \cdot \frac{\rho_\infty a^2_\infty}{\mu_\infty}

.. math::

   \nu_{t~\infty} = 0.009 \times \nu_\infty

The dynamic viscosity in the above equation is obtained from Sutherland formulation and the density is calculated through :math:`\mu/\nu`. In the table below, the turbulent properties used for the current :math:`k-\omega~\rm{SST}` simulation are provided

.. _turbulent-step-SST-conditions:
.. table:: Turbulent freestream conditions for :math:`k-\omega~\rm{SST}` Model

   +----------------+----------------------------------+----------------------------------+---------------------------------+
   |:math:`I`       | :math:`k_{\infty}~(m^2/s^2)`     | :math:`\omega_{\infty}~(1/s)`    | :math:`\nu_{t~~\infty}~(m^2/s)` |
   +================+==================================+==================================+=================================+
   |:math:`0.061\%` | :math:`1.0961 \times 10^{-3}`    | :math:`97.37245`                 | :math:`1.10787 \times 10^{-5}`  |
   +----------------+----------------------------------+----------------------------------+---------------------------------+

*Turbulent Properties for Realizable k-epsilon model*

The turbulent inflow properties used for Realizable :math:`k-\epsilon` model were evaluated as follows

.. math::
   
   k_{\infty} = \frac{3}{2} (U_\infty I)^2

.. math::
   
   \epsilon_{\infty} = \frac{0.1643~k_{\infty}^{3/2}}{\lambda}

.. math::

  \nu_{t\infty} = \frac{C_\mu~k^2}{\epsilon_{\infty}}

where, :math:`\lambda` is the turbulent length scale and is evaluated at 0.22 of the boundary layer thickness at separation (1.5H) and :math:`C_\mu` is a model constant with a value 0.09. The following table provides the evaluated turbulent properties. Note that the turbulent intensity was assumed to be 1 % for this particular model.

.. _turbulent-step-RKE-conditions:
.. table:: Turbulent freestream conditions for Realizable :math:`k-\epsilon` Model

   +----------------+----------------------------------+---------------------------------------+---------------------------------+
   |:math:`I`       | :math:`k_{\infty}~(m^2/s^2)`     | :math:`\epsilon_{\infty}~(m^2/s^3)`   | :math:`\nu_{t\infty}~(m^2/s)`   |
   +================+==================================+=======================================+=================================+
   |:math:`1\%`     | :math:`294.57 \times 10^{-3}`    | :math:`0.079598`                      | :math:`98.11 \times 10^{-3}`    |
   +----------------+----------------------------------+---------------------------------------+---------------------------------+

**Computational Domain and Boundary Conditions**

The computational domain simply follows the step geometry for the entire bottom region. In :numref:`turbulent-step-domain` below, the boundary details in two-dimensions (:math:`x-z` plane) are shown. The walls of the upstream plate, step and the downstream plate that extend between :math:`-110 \leq x \leq 50~m` are modelled as no-slip wall boundary condition. Similarly, the top plate is also modelled as a no-slip wall. Upstream of the leading edge, that is, :math:`x \leq 110` symmetry boundary extends for a length of 20 step heights and the inlet boundary is placed at the start of the symmetry. The outlet is placed at the end of the downstream plate, which is at :math:`x = 50~m`.

.. _turbulent-step-domain:
.. figure:: sections_v/validation-figures/turbulent-step-domain.*
   :width: 600px
   :align: center

   Computational domain for a 2D step (Not to scale)

*Boundary Conditions and Initialisation*

* Inlet
   - Velocity: Fixed uniform velocity :math:`u = 44.31525~m/s` in :math:`x` direction
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)
     - Realizable :math:`k-\epsilon` (Fixed uniform value of :math:`k_{\infty}`, :math:`\epsilon_{\infty}` and :math:`\nu_{t_\infty}` as given in the above table)

* Symmetry
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Turbulence: Symmetry

* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient
   - Turbulence:

     - Spalart–Allmaras:

       - :math:`\nu_t`: type nutUWallFunction with an initial value of :math:`\nu_t=0`
       - :math:`\tilde{\nu}`: type fixedValue with a value of :math:`\tilde{\nu}=0`

     - :math:`k-\omega~\rm{SST}`:

       - :math:`k`: type kqRWallFunction with an initial value of :math:`k_{\infty}`
       - :math:`\omega`: type omegaWallFunction with an initial value of :math:`\omega_{\infty}`
       - :math:`\nu_t`: type nutUWallFunction with an initial value of :math:`\nu_t=0`

     - Realizable :math:`k-\epsilon`:

       - :math:`k`: type kqRWallFunction with an initial value of :math:`k_{\infty}`
       - :math:`\epsilon`: type epsilonWallFunction with an initial value of :math:`\epsilon=0`
       - :math:`\nu_t`: type nutUWallFunction with an initial value of :math:`\nu_t=0`

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`
   - Turbulence:

     - Spalart–Allmaras (Calculated :math:`\nu_{t}=0` and Zero gradient :math:`\tilde{\nu}`)
     - :math:`k-\omega~\rm{SST}` (Zero gradient :math:`k` and :math:`\omega`; Calculated :math:`\nu_t=0`; )
     - Realizable :math:`k-\epsilon` (Zero gradient :math:`k` and :math:`\epsilon`; Calculated :math:`\nu_t=0`; )

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 44.31525~m/s` in :math:`x` direction
   - Pressure: Zero Gauge pressure
   - Turbulence:

     - Spalart–Allmaras (Fixed uniform values of :math:`\nu_{t~\infty}` and :math:`\tilde{\nu}_{\infty}` as given in the above table)
     - :math:`k-\omega~\rm{SST}` (Fixed uniform values of :math:`k_{\infty}`, :math:`\omega_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)
     - Realizable :math:`k-\epsilon` (Fixed uniform values of :math:`k_{\infty}`, :math:`\epsilon_{\infty}` and :math:`\nu_{t~\infty}` as given in the above table)

**Computational Grid**

The 3D computational grid was generated using `Pointwise <http://www.pointwise.com/>`__. and was converted to Caelus format. Note that the plane of interest is in :math:`x-z` directions. Further, since the flow field is assumed to be two-dimensional, the two :math:`x-z` obtained as a result of a 3D grid are specified with empty boundary condition. This forces the 3D solver, simpleSolver to treat the flow in :math:`y` direction as symmetry. The following figure shows the 2D grid. As noted earlier, the purpose of this exercise is to validate the wall functions and hence the grid is designed for a :math:`y^+ = 30`. In order to design such a gird, the first cell height (:math:`\Delta z`) from wall in the wall normal (:math:`z`) direction was obtained from the following set of equations

.. math::
   
   \Delta z = \frac{y^+~\nu}{u_*}

where, :math:`u_*` is the frictional velocity given by

.. math::
  
   u_* = \sqrt{\frac{\tau_w}{\rho}}

In the above equation, :math:`\tau_w` is the shear-stress at the wall and was be estimated using the skin-friction (:math:`c_f`) relation, given as

.. math::
 
   \tau_w = c_f\frac{1}{2} \rho u_\infty^2

where, :math:`c_f` was obtained for the flat-plate as given in Schlichting :cite:`Schlichting1979` and is shown below

.. math::
   
   c_f = [2~\textrm{log}(Re_x) - 0.65]^{-2.3}

where, :math:`Re_x` is the Reynolds number based on the length of the boundary layer. In this case, it is the length developed over the upstream plate.

The 2D grid of a backward facing step in :math:`x-z` is shown in :numref:`turbulent-step-grid` for a :math:`y^+ \approx 30`. In the upstream region of the step, there are 60 cells in the streamwise and 64 in the wall normal directions respectively. Downstream of the step, there are 129 cells in the streamwise and a total of 84 cells in the normal direction. Out of 84 cells, 20 cells represent the height of the step.

.. _turbulent-step-grid:
.. figure:: sections_v/validation-figures/turbulent-step-grid.*
   :width: 800px
   :align: center

   Backward facing step grid shown in 2D for :math:`y^+ \approx 30` (Inset shows the vicinity of the step region)


**Results and Discussion**

The steady-state solution of turbulent flow over a two-dimensional backward facing step was obtained using Caelus |version|. The simpleSolver was used and the simulation was run sufficiently long until the residuals for pressure, velocity and turbulent quantities were less than :math:`1 \times 10^{-6}`. The finite volume discretization of the gradient of pressure and velocity was carried out using linear approach. Where as the divergence of velocity and mass flux was carried out through  the linear upwind method. However, for the divergence of the turbulent quantities, upwind approach was utilised. For the discretization of the Laplacian terms, again linear method was used.

*Experimental validation of skin-friction coefficient*

In this section, the validation carried out for Caelus based on skin-friction and pressure obtained experimentally by Driver and Seegmiller :cite:`Driver1985` is presented. The results obtained from CFL3D :cite:`CFL3D` are additionally shown and should be considered only as a reference and not as a benchmark for verification. This is because all the CFL3D results have been obtained without the use of wall functions and on a grid having :math:`y^+ \approx 1`. In :numref:`turbulent-stp-caelus-sa`, skin-friction distribution obtained from Caelus using SA turbulence model is compared with the experiments. Upstream of the step, the agreement is good, however, downstream post-reattachment the skin-friction under predicts the experimental data. In both these regions of the flow, Caelus results are nearly identical to that of CFL3D suggesting that the wall-function is capturing the flow characteristics accurately. Within the separated region, there is a large discrepancy and this is due to the inherent low :math:`y^+` mesh in that region, where typically a wall function becomes invalid.

.. _turbulent-stp-caelus-sa:
.. figure:: sections_v/validation-figures/cf-turbulent-step-Caelus-SA.*
   :width: 600px
   :align: center

   Skin-friction distribution obtained from Caelus simulation using SA turbulence model

Figure :numref:`turbulent-stp-caelus-sst` gives the comparison of skin-friction obtained from :math:`k-\omega~\rm{SST}` turbulence model. The result is very similar to the one obtained from SA model. In this case, the skin-friction upstream of the step is slightly under predicted, whereas, post reattachment, it seems to be closer to experiments. In contrast with the SA result, the skin-friction is now closer to the experimental data within the separated region, particularly in the region closer to the reattachment location.

.. _turbulent-stp-caelus-sst:
.. figure:: sections_v/validation-figures/cf-turbulent-step-Caelus-SST.*
   :width: 600px
   :align: center

   Skin-friction distribution obtained from Caelus simulation using :math:`k-\omega~\rm{SST}` turbulence model

In :numref:`turbulent-stp-caelus-rke`, the comparison is shown for Realizable :math:`k-\epsilon` turbulence model. Note that CFL3D data was not available for this turbulence model to use as a reference. Again, similar skin-friction behaviour can be noted both upstream and downstream of the step with reasonable agreement with the experimental data. Within the separated region, there is a large difference and this could be due to the presence of low :math:`y^+` mesh as discussed earlier.

.. _turbulent-stp-caelus-rke:
.. figure:: sections_v/validation-figures/cf-turbulent-step-Caelus-RKE.*
   :width: 600px
   :align: center

   Skin-friction distribution obtained from Caelus simulation using Realizable :math:`k-\epsilon` turbulence model

One of the key feature of modelling the backward facing step is the accurate prediction of reattachment location downstream of the step. This was determined through the location at which the reversal of skin-friction occurs over the downstream surface for each of the turbulence model considered here. In the below table, the normalised reattachment distances obtained from Caelus simulation are compared with the experimental data. Out of the three models considered here, Realizable :math:`k-\epsilon` prediction is in very good agreement with the experimentally obtained value.

.. _turbulent-step-reattachment-distances:
.. table:: Comparison of reattachment distance with the experiment data

   +------------------------------+-------------------------------------+
   |Type                          | Reattachment location (:math:`x/H`) |
   +==============================+=====================================+
   |Experimental                  | :math:`6.26 \pm 0.10`               |
   +------------------------------+-------------------------------------+
   |SA                            | :math:`5.55`                        |
   +------------------------------+-------------------------------------+
   |:math:`k-\omega~\rm{SST}`     | :math:`6.08`                        |
   +------------------------------+-------------------------------------+
   |Realizable :math:`k-\epsilon` | :math:`6.27`                        |
   +------------------------------+-------------------------------------+


*Experimental validation of pressure coefficient*

:numref:`turbulent-step-caelus-cfl3d-expt-p` gives the pressure-coefficient (:math:`c_p`) comparison among three Caelus simulations and the experimental data. The inclusion of CFL3D data is again only for reference and not as a benchmark comparison. All the simulations essentially produce the same trend and is consistent with the skin-friction coefficient distribution. The pressure prediction in both :math:`k-\omega~\rm{SST}` and Realizable :math:`k-\epsilon` are very close to each other over the entire region shown in the figure and is also in fair agreement with the experimental data. However, SA seems to show some significant deviation particularly in the region of pressure minima.

.. _turbulent-step-caelus-cfl3d-expt-p:
.. figure:: sections_v/validation-figures/p-turbulent-step-Caelus-CFL3D-Expt.*
   :width: 600px
   :align: center

   Pressure-coefficient comparison between Caelus and experiments over the backward facing step

**Conclusions**

A detailed validation of the turbulent flow over a backward facing step was carried out using Caelus |version| for a  simpleSolver. In particular the focus was to validate wall functions for grids having :math:`y^+ \approx 30`. The solutions from three turbulence models were compared with both skin-friction and pressure coefficients obtained experimentally over the surface of the model. Overall, a good agreement was noted. With respect to the reattachment location, the simulated result with Realizable :math:`k-\epsilon` turbulence model agreed very close to the experimental data. Considering both the skin-friction and pressure data, the implementation of the turbulence model is correct with providing accurate solutions.






































