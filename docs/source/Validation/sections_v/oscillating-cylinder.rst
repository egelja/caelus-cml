.. _circular-cylinder-validation:

Circular Cylinder 
-----------------

*Laminar, incompressible flow over a circular cylinder*

**Nomenclature**

======================  ==================================    =========================
Symbol                  Definition                            Units (SI)
======================  ==================================    =========================
:math:`D`               Diameter of the cylinder              :math:`m`
:math:`f`               Frequency                             :math:`hz`
:math:`p`               Kinematic pressure                    :math:`Pa/\rho~(m^2/s^2)`
:math:`Re_D`            Reynolds number based on diameter	  Non-dimensional
:math:`St`              Strouhal number                       Non-dimensional
:math:`U`               Freestream velocity in x-direction    :math:`m/s`
:math:`x`               Distance in x-direction               :math:`m`
:math:`y`               Distance in y-direction               :math:`m`
:math:`\nu`             Kinematic viscosity                   :math:`m^2/s`
======================  ==================================    =========================

In this validation study, laminar incompressible flow over a 2D circular cylinder is investigated at a Reynolds number of 100. This classical configuration represents flow over a bluff body dominated by a wake region. For flows having a low Reynolds number (:math:`40 \leq Re_D \leq 150`), periodic vortex shedding occurs in the wake. The phenomenon of vortex shedding behind bluff bodies is referred to as the *Karman Vortex Street* :cite:`Roshko1954` and provides an transient case for CFD code validation.

In his work, Roshko :cite:`Roshko1954` experimentally studied wake development behind two-dimensional circular cylinders from Reynolds number ranging from 40 to 10000. For Reynolds numbers of 40 to 150, the so called the *stable range* [Roshko1954]_, regular vortex streets are formed with no evidence of turbulence motion in the wake. Therefore, at a Reynolds number of 100, the vortex shedding exhibits smooth, coherent structures making it ideally suited for validating laminar CFD calculations. The frequency associated with the oscillations of the vortex streets can be characterized by the Strouhal Number (:math:`St`). The Strouhal Number is a non-dimensional number defined as

.. math::

   St = \frac{fD}{U}
	
where, :math:`f` is the frequency of oscillations of vortex shedding, :math:`D` is the diameter of the cylinder and :math:`U` is the freestream velocity of the flow. Experimentally [Roshko1954]_, it has been determined that for a Reynolds number based on the diameter of the cylinder of 100, the Strouhal number :math:`St \approx 0.16 - 0.17`. One of the main objectives in this study was to compare the :math:`St` for the CFD calculation to the experimental data of Roshko [Roshko1954]_. Provided the cylinder has a sufficient span length, the flow characteristics can be assumed to be two-dimensional as the experiments suggest.

**Problem Definition**

:numref:`cylinder-schematic` shows the schematic of the two-dimensional circular cylinder. Here, the diameter D = 2 m and is the characteristic length for the Reynolds number, which is 100. For simplicity, the freestream velocity was taken to be U = 1 m/s in the x direction. Using these values the kinematic viscosity was calculated to be 0.02 :math:`m^2/s`.

.. _cylinder-schematic:
.. figure:: sections_v/validation-figures/cylinder-schematic.*
   :width: 600px
   :align: center

   Schematic representation of a circular cylinder

In the table below, a summary of the freestream conditions are provided

+----------------+---------------+-------------------+----------------------+---------------------+
| :math:`Re_D`   | :math:`D`     | :math:`U~(m/s)`   | :math:`p~(m^2/s^2)`  | :math:`\nu~(m^2/s)` |
+================+===============+===================+======================+=====================+
| 100            | 2.0           | 1.0               | :math:`(0)` Gauge    | 0.02                |
+----------------+---------------+-------------------+----------------------+---------------------+

Here, the flow is assumed to be incompressible and therefore the density is constant. Further, no temperature is evaluated in this calculation and hence the viscosity also remains constant. For incompressible flows, the kinematic forms of pressure and viscosity are always used in Caelus.

**Computational Domain and Boundary Conditions**

A rectangular computational domain in the :math:`x-y` plane was constructed surrounding the circular cylinder as shown in :numref:`cylinder-domain`. A full cylinder must be used due to the oscillatory nature of the shed vortices. Rhe domain extends by 5 diameters of cylinder and 20 diameters downstream. Since the flow here is viscous dominated, sufficient downstream length is required to capture the vortex separation from the surface of the cylinder and the subsequently shedding in the wake. In the :math:`y` direction, the domain extends 5 diameters on either side. From the figure, multiple inlet boundaries to this domain can be seen, one at the upstream boundary and the other two for the top and bottom boundaries. This type of configuration is needed to appropriately model the inflow, similar to an undisturbed flow in an experimental set-up. It is noted that for top and bottom boundaries, the flow is in the :math:`x` direction. The outlet is located at the downstream boundary. The cylindrical wall is a no-slip boundary condition.

.. _cylinder-domain:
.. figure:: sections_v/validation-figures/cylinder-domain.*
   :width: 600px
   :align: center

   Computational domain of a circular cylinder

*Boundary Conditions and Initialisation*

Following are the details of the boundary conditions used:

* Inlet-1
   - Velocity: Fixed uniform velocity :math:`u = 1.0~m/s` in :math:`x` direction
   - Pressure: Zero gradient
	
* Inlet-2
   - Velocity: Fixed uniform velocity :math:`u = 1.0~m/s` in :math:`x` direction
   - Pressure; Zero gradient
	
* No-slip wall
   - Velocity: Fixed uniform velocity :math:`u, v, w = 0`
   - Pressure: Zero gradient

* Outlet
   - Velocity: Zero gradient velocity
   - Pressure: Fixed uniform gauge pressure :math:`p = 0`

* Initialisation
   - Velocity: Fixed uniform velocity :math:`u = 1.0~m/s` in :math:`x` direction
   - Pressure: Zero Gauge pressure

**Computational Grid**

The computational grid in 2D was generated using `Pointwise <http://www.pointwise.com/>`_ in the :math:`x-y` plane. Since Caelus is a 3D computational framework, it necessitates the grid to also be 3D. Therefore, a 3D grid was obtained using `Pointwise <http://www.pointwise.com/>`_ by extruding the 2D grid in the positive :math:`z` direction by *one cell*. The final 3D grid was then exported to the Caelus format (polyMesh). The two :math:`x-y` planes obtained as a result of grid extrusion need boundary conditions to be specified. As the flow over a flat-plate is generally 2D, we do not need to solve the flow in the third dimension. This is achieved in Caelus by specifying *empty* boundary condition for each plane. Although, no flow is computed in the :math:`z` direction, a velocity of :math:`w = 0` has to be specified for the velocity boundary condition as indicated above. 

.. _cylinder-grid:
.. figure:: sections_v/validation-figures/cylinder-grid.*
   :width: 800px
   :align: center

   O-grid around the cylinder and structured gird representation

The 2D domain consisted of 9260 cells. An O-grid topology was constructed around the cylinder (see the right figure) with 10 cells in the radial direction and 84 cells in the circumferential direction. 31 cells were used upstream of the O-grid, in the :math:`x` direction while 100 cells were used downstream. The region of interest is about 10 diameters downstream, where the grids are refined. In the :math:`y` direction, 21 cells were used above and below the O-grid region.

**Results and Discussion**

A time-dependent simulation was carried out using the Caelus |version| with the SLIM solver. To capture the transient start-up process, the calculation was started from time t = 0 s and was simulated up to t = 360 s, while lift and drag forces over the cylindrical surface were monitored at a frequency of 2 Hz. It was found that the on-set of vortex shedding occurred after about t = 90 s which was then followed by a steady shedding process. A Fast Fourier transformation (FFT) was carried out on the lift force data and the peak frequency of vortex shedding occurred at :math:`f = 0.0888` Hz. Based on this value, it takes about 7.8 cycles for the shedding to start.

Using the peak frequency value of :math:`f = 0.0888` Hz, :math:`St` was evaluated. The table below compares the computed value from SLIM with that of the experiment. The agreement is good given that experimental uncertainty can be relatively high at low Reynolds numbers.

+--------------------------+----------------------+------------------+
|                          | Frequency (Hz)       | Strouhal Number  |
+==========================+======================+==================+
| Experimental             | 0.0835               | :math:`0.167`    |
+--------------------------+----------------------+------------------+
| SLIM                     | 0.0888               | :math:`0.177`    |
+--------------------------+----------------------+------------------+

**Conclusions**

The transiet, incompressible, two-dimensional flow over a circular cylinder was simulated using Caelus |version| to estimate the peak frequency of vortex shedding. The value was compared to well known experimental data resulting in good agreement.
