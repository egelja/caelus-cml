.. _spherical-cavity-validation:

Spherical Cavity
----------------

*Natural Convection in a Spherical Cavity*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`C_p`             Specific heat at constant pressure    :math:`J/kg \cdot K`
:math:`D`               Diameter of the sphere                :math:`m`
:math:`g`               Gravitational constant                :math:`9.80~m/s^2`
:math:`k`               Thermal conductivity                  :math:`W/m \cdot K`
:math:`Nu`              Nusselt number                        Non-dimensional
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Pr`              Prandtl number                        Non-dimensional
:math:`r`               Radius of the sphere                  :math:`m`
:math:`Ra`              Rayleigh number                  	  Non-dimensional
:math:`T`               Temperature                           :math:`K`
:math:`u`               velocity in x-direction               :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`z`               Distance in z-direction               :math:`m`
:math:`\beta`           Coefficient of thermal expansion      :math:`1/K`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
:math:`\rho`            Density                               :math:`kg/m^3`
======================  ==================================    =========================

In this study, laminar flow inside a heated spherical cavity is investigated. A temperature gradient was applied to the surface of the cavity inducing fluid motion due to buoyancy effects. Here, a Rayleigh number (:math:`Ra`) of 2000 and a Prandtl number (:math:`Pr`) of 0.7 is used to set appropriate thermal conditions to the problem.

Analytical solutions to the heated spherical cavity were investigated by McBain and Stephens :cite:`McBain2000` at various Rayleigh numbers up to :math:`Ra = 10000`. However, it was noted that asymptotic heat transfer rate predictions as a function of Nusselt number (:math:`Nu`) deviated largely with increase in Rayleigh number. Therefore a value of :math:`Ra = 2000` was chosen to avoid these deviations. At this value, the analytical heat transfer rate compares very well with the asymptotic solution. Isotherms from the analytical solution were used for comparision with numerical results.

**Problem Definition**

The schematic of the spherical cavity is depicted :numref:`sphere-schematic` and as can be seen, only half of the sphere is considered here with an :math:`x-y` symmetry plane. The sphere is located at :math:`x = 0, y = 0, z = 0` with a radius :math:`r = 0.5~m`. 

.. _sphere-schematic:
.. figure:: sections_v/validation-figures/sphere-schematic.*
   :width: 500px
   :align: center
   
   Schematic representation of the sliced sphere
   
As indicated, a non-uniform temperature profile was used as a thermal boundary condition on the spherical wall. The temperature (:math:`T`) was specified as a function of :math:`x`:

.. math::
   
   T = x

with, 

.. math::
   
   T = -0.5~K \quad \text{at} \quad x = -0.5~m
   
.. math::

   T = 0.5~K \quad \text{at} \quad x = 0.5~m

The energy equation in a non-dimensional form :cite:`McBain2000` for a steady state can be expressed as:

.. math::

   Gr~Pr~u \cdot T = \nabla^2 T

where, :math:`Gr` is the Grashof number. Since, :math:`Ra = Gr~Pr` the above equation can be rewritten along with expanding the Laplacian term (:math:`\nabla^2`) as

.. math::
   
   Ra~u \cdot T = \nabla~[\alpha \nabla T]
   
   \text{or}
   
   u \cdot T = \frac{1}{Ra}~\alpha~\nabla~[ \nabla T]
   
where :math:`\alpha` is the thermometric conductivity. It is reasonable to assume that (:math:`1/Ra`) is approximately equal to the thermometric conductivity (:math:`\alpha`), which is given as

.. math::

   \alpha = \frac{k}{\rho C_p} = \frac{\nu}{Pr}
   
where, :math:`k`, :math:`\rho`, :math:`C_p` and :math:`\nu` are the thermal conductivity, density, specific heat capacity and kinematic viscosity of the fluid respectively. Using the above relation with a value of :math:`Ra = 2000` and :math:`Pr = 0.7`, the kinematic viscosity was calculated to be :math:`\nu = 3.4 \times 10^{-4}~m^2/s`. The coefficient of thermal expansion (:math:`\beta`) needed to model the Boussinesq buoyancy term was evaluated from the following relation

.. math::

   Gr = \frac{g~\beta~\Delta T~D^3}{\nu^2}

where, :math:`g` is the acceleration due to gravity and :math:`D` is the diameter of the sphere. :math:`\beta` was calculated to :math:`3.567\times10^{-5}~1/K`. In the following table, a summary of the properties are given. Note that gravity acts in :math:`-y` direction.

+----------------+----------------+----------------+-----------------------+-----------------------------+-----------------------------+
| :math:`Ra`     | :math:`Pr`     | :math:`T~(K)`  | :math:`p~(m^2/s^2)`   | :math:`\nu~(m^2/s)`         | :math:`\beta~(1/K)`         |
+================+================+================+=======================+=============================+=============================+
| :math:`2000`   | :math:`0.7`    | :math:`T = x`  | :math:`(0)` Gauge     | :math:`3.4 \times 10^{-4}`  | :math:`3.567\times10^{-5}`  |
+----------------+----------------+----------------+-----------------------+-----------------------------+-----------------------------+

Although the temperature is calculated in this simulation, a constant viscosity is used. Since the temperature gradient is very small (:math:`\mathcal{O}(1)`), effect of temperature on the viscosity would be insignificant. The kinematic definition of pressure is used here.

**Computational Domain and Boundary Conditions**

The computational domain was a half sphere with an :math:`x-y` plane of symmetry at :math:`z = 0~m`. The surface temperature was prescribed as discussed above. The initialisation of the fluid temperature within the sphere follows that of the surface temperature (:math:`T = x`) and is depicted in :numref:`sphere-domain` at the symmetry plane. Note that this figure also aids in providing a clarity of understanding for the temperature variation over the spherical surface.

.. _sphere-domain:
.. figure:: sections_v/validation-figures/sphere-domain.*
   :width: 400px
   :align: center
   
   Computational domain and temperature boundary condition

*Boundary Conditions and Initialisation*

* Wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Uniform zero Buoyant Pressure
   - Temperature: Linear unction of :math:`x` (:math:`T = x`)

* Symmetry Plane
   - Velocity: Symmetry
   - Pressure: Symmetry
   - Temperature: Symmetry
	
* Initialisation
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Uniform zero Buoyant Pressure
   - Temperature: Linear function of :math:`x` (:math:`T = x`)
	
**Computational Grid**

The computational grid for the half sphere was generated using `Pointwise <http://www.pointwise.com/>`_. A fully structured grid was constructured with a total of 18564 cells. As seen in :numref:`sphere-grid`, an O-H topology used where an H-block is centred within 5 O-blocks.

.. _sphere-grid:
.. figure:: sections_v/validation-figures/sphere-grid.*
   :width: 700px
   :align: center
   
   O-grid distribution on the wall and plane of symmetry
   
**Results and Discussion**

The steady solution to the natural convection in a buoyant sphere was obtained using Caelus |version| with the SLIM solver that includes the a buoyancy source term based on the Boussinesq assumption. Since SLIM is inherently time-accurate, the simulation was run sufficiently long such that a steady state was achieved. In :numref:`sphere-isotherms`, compares the isotherms obtained with SLIM and the analytical isotherms obtained with a first order approximation. Close agreement was observed.

 .. _sphere-isotherms:
 .. figure:: sections_v/validation-figures/sphere-isotherms.*
    :width: 700px
    :align: center
	
    Comparison of temperature isotherms between computational and analytical data


**Conclusions**

A validation study of a buoyant flow inside a spherically heated cavity was conducted using Caelus |version|. The isotherms obtained from the CFD results were compared with the first order analytical solution and excellent agreement was observed.



