.. _ras-equations label:

*****************************************
Reynolds Averaged Navier-Stokes Equations
*****************************************

The general form of the incompressible Navier-Stokes equations in absence
of body forces is given by the following expression:

.. math::
   \partial_t u_i + \partial_j(u_i u_j) = -\frac{1}{\rho} \partial_i p + \partial_j \nu \partial_j u_i.

The momentum equation is complemented by continuity equation, which for incompressible flow is the divergence free condition for velocity:

.. math::
   \partial_i u_i = 0

The Reynolds averaging procedure decomposes instantaneous variables into
time averaged and fluctuating components according the expression

.. math::
   u_i(x_i,t) = \overline{u}_i(x_i) + u_i'(x_i,t).

Instantaneous quantities are averaged over a sufficiently long time such that
the time scale of the fluctuating part of the field is much smaller
than the averaging interval :math:`T`

.. math::
   \overline{u}_i = \frac{1}{T} \int u_i(x_i,t) dt.

This definition allows for time variability in the averaged
momentum and continuity equations although the structures that have 
characteristic time scales comparable to or larger than 
the filter width :math:`T` could be resolved.

An important property of the fluctuating portion of the flow fields in
Reynolds averaged simulations is that they have zero mean value by the
definition:

.. math::
   \frac{1}{T} \int u_i'(x_i,t) dt = 0.

Keeping in mind this property, the Reynolds averaging procedure is 
applied to the Navier-Stokes equations to obtain Reynolds Averaged 
Navier-Stokes (RANS) equations:

.. math::
   \partial_i \overline{u}_i = 0

.. math::
   \partial_t \overline{u}_i + \partial_j (\overline{u}_i \overline{u}_j) = \frac{1}{\rho} \partial_i \overline{p} + \partial_j(\nu \partial_j \overline{u}_i ) - \partial_j \overline{u_i' u_j'}

The new term in momentum equations, :math:`-\partial_j\overline{u_i' u_j'}`, 
appears due to the non-linear nature of the momentum equation. This term is
called the Reynolds stress tensor and it represents the influence of 
turbulence on the mean flow. The Reynolds stress tensor contains 6 unique,
unknown components. The unknowns must be modelled and are determined by 
relating them to time averaged quantities. The most common approach used to 
close the Reynolds stresses is the Boussinesq hypothesis:

.. math::
   -\overline{u_i' u_j'} = 2 \nu_{t} \left( \overline{S}_{ij} - \frac{2}{3} k \delta_{ij}\right),

where the term :math:`\overline{S}_{ij}` is the symmetric part of the
time averaged stress tensor

.. math::
   \overline{S}_{ij} = \frac{1}{2}(\partial_j \overline{u}_i + \partial_i \overline{u}_j),

and :math:`k` is turbulent kinetic energy 

.. math::
   k = \frac{1}{2} \sum_i \overline{(u_i')^2}.

The Boussinesq hypothesis is also called eddy viscosity approximation 
and most RANS turbulence models use it in this linear form. The main 
difference between various turbulence models employing the Boussinesq 
hypothesis is how the turbulent viscosity :math:`\nu_t` is determined.

The following sections describes the transport equations
of the RANS turbulence models included in Caelus |version|. Note, the 
over-bar used to denote time averaged quantities, will be omitted for 
the clarity throughout this section.

