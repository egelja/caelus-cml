.. _Spalart-Allmaras label:

Spalart-Allmaras Turbulence  Model
-----------------------------------
 
**Model Description**

The Spalart-Allmaras model uses one equation to compute the turbulent 
viscosity. The Spalart-Allmaras model used in Caelus |version| corresponds
to the version of Shur et al. :cite:`Shur2000` with curvature correction. 
The transport equation in this version is the same as the original 
Spalart-Allmaras model :cite:`Spalart1994` but with rotation and 
curvature effects accounted for. This modification is accomplished 
by introducing the :math:`f_{r1}` function into the transport equation 
for the viscosity-like variable

.. math::
   \partial_t \tilde{\nu} + u_j \partial_j \tilde{\nu} = c_{b1}f_{r1}\tilde{S}\tilde{\nu} - c_{w1}f_{w1}\left(\frac{\tilde{\nu}}{d}\right)^2 + \frac{1}{\sigma}\left[\partial_j(\nu+\tilde{\nu})\partial_j \tilde{\nu} + c_{b2}\partial_i \tilde{\nu} \partial_i \tilde{\nu} \right]

Terms appearing in the Spalart-Allmaras turbulence model have the following
definitions:

**Model definitions**

======================  ==================================    =========================
Symbol                  Definition                            Formula
======================  ==================================    ========================= 
:math:`\tilde{\nu}`     Viscosity-like variable               
:math:`\nu_t`           Turbulent eddy viscosity              :math:`\tilde{\nu} f_{v1}`
:math:`f_{v1}`          Viscous Damping Function              :math:`\frac{\chi^3}{\chi^3 + c_{v1}}`
:math:`\chi`            Viscosity ratio                       :math:`\frac{\tilde{\nu}}{\nu}`
:math:`\nu`             Kinematic Viscosity                             
:math:`\tilde{S}`       Modified vorticity                    :math:`\Omega + \frac{\tilde{\nu}}{\kappa^2 d^2}f_{v2}`
:math:`\Omega`          Vorticity magnitude                   :math:`\Omega = \sqrt{2W_{ij}W_{ij}}`
:math:`W_{ij}`          Vorticity                             :math:`\frac{1}{2}(\partial_j u_i - \partial_i u_j)`        
:math:`d`               Wall distance                         
:math:`f_{v2}`          Function                              :math:`1-\frac{\chi}{1+\chi f_{v1}}`
:math:`f_{w}`           Function                              :math:`g\left[\frac{1+c_{w3}^6}{g^5+c_{w3}^6}\right]^{1/6}`
:math:`g`               Function                              :math:`r+c_{w2}(r^6-r)`
:math:`r`               Variable                              :math:`r=min\left[\frac{\tilde{\nu}}{\tilde{S} \kappa^2 d^2},10\right]`
======================  ==================================    =========================

It should be noted that the Caelus |version| implementation of the
Spalart-Allmaras turbulence model omits the rotational and turbulence
tripping terms as the flow is assumed fully turbulent.

**Model constants**

:math:`c_{b1} = 0.1355`

:math:`\sigma = \frac{2}{3}`   

:math:`c_{b2} = 0.622`

:math:`c_{w2} = 0.3`

:math:`c_{w3} = 2.0`

:math:`c_{v1} = 7.1` 

:math:`c_{w1} = \frac{c_{b1}}{\kappa^2} + \frac{1+c_{b2}}{\sigma}`

:math:`\kappa = 0.41`

The Spalart-Allmaras model neglects the contribution of the turbulent kinetic
energy to the eddy viscosity. This is deemed acceptable for many 
incompressible and low Mach number compressible flows.

**Boundary conditions**

The Spalart-Allmaras turbulence model was originally developed for external
aerodynamics of streamlined bodies. Typically, mesh resolutions used for 
external aerodynamics have :math:`y^+` values close to 1. Therefore, 
the Spalart-Allmaras model is often referred to as a low Reynolds number
model when it comes to near wall treatment. In the case of meshes resolved 
near the wall, i.e. :math:`y^+ \sim 1`, the following set of boundary conditions 
are recommended:

:math:`\tilde{\nu}|_{wall} = 0`

:math:`\nu_t|_{wall} = 0`

However, in Caelus |version| it is possible to use the Spalart-Allmaras on
meshes that are not wall-resolved, i.e., meshes that have the first
cell height next to the wall :math:`y^+ \gg 1`. In such cases, a wall
function for :math:`\nu_t` based on the velocity is recommended.

:math:`\nu_t|_{wall} =` nutUWallFunction (:math:`y^+ > 30`) 

Inlet values of :math:`\tilde{\nu}` at the far field boundary for
external flows is suggested as follows:

:math:`\tilde{\nu}|_{inlet} = 3 \nu` to :math:`5 \nu`.

:math:`\nu_t` should be set to "calculated".

It is also possible to specify :math:`\tilde{\nu}` from the turbulence
intensity and length scale as follows:

:math:`\tilde{\nu}|_{inlet} = \sqrt{\frac{3}{2}} |u_i| I l`

where :math:`I = \frac{|u_i'|}{|u_i|}` is turbulence intensity and

:math:`l` is a turbulence length scale (measure of largest eddies in the flow field).

In the case of internal flows, depending on the level of
turbulence intensity, :math:`\tilde{\nu}` can be determined from the
turbulence viscosity ratio

:math:`\tilde{\nu} = 10\nu` or higher and

:math:`\nu_t` should be set to "calculated".

At the outlet, zero gradient an acceptable choice.
