.. _k-omega-SST label:

:math:`k`--:math:`\omega` SST Model
-----------------------------------

**Model description**

The :math:`k`--:math:`\omega` shear stress transport (SST) turbulence model was introduce by Menter :cite:`Menter1994` as an improvement on the original two equation :math:`k`--:math:`\omega` model of Wilcox :cite:`Wilcox1988`. The SST formulation uses the :math:`k`--:math:`\omega` formulation in the inner parts of the boundary layer and it switches to a :math:`k`--:math:`\varepsilon` behaviour in the free-stream portions of the flow domain. Thus, the :math:`k`--:math:`\omega` SST turbulence model does not require a damping function close to the wall enabling its use with near-wall resolved meshes without modification. At the same time, the :math:`k`--:math:`\omega` SST models reverts to a :math:`k`--:math:`\varepsilon` model behaviour away from the walls thus removing the sensitivity of the original :math:`k`--:math:`\omega` model to the
:math:`\omega` free-stream boundary value.

The implementation of the :math:`k`--:math:`\omega` SST model in Caelus |version| is that of Kuntz et al :cite:`Menter2003`. The transport equations for turbulence kinetic energy and specific dissipation are:

.. math::
   \partial_t k + \partial_j (u_j k) = \frac{P}{\rho} - \beta^* \omega k + \partial_j[(\nu + \sigma_k \nu_t) \partial_j k]

.. math::
   \partial_t \omega + \partial_j(u_j \omega) = \frac{\gamma P}{\rho \nu_t} - \beta \omega^2 + \partial_j[(\nu + \nu_t) \partial_j \omega] + 2(1-F_1) \frac{\sigma_{\omega2}}{\omega} \partial_j k \partial_j \omega

It should be noted that this set of transport equations only
applicable for incompressible flows as density was assumed constant. Terms 
appearing in the :math:`k`--:math:`\omega` SST model have the following definitions:

**Model definitions**

======================  ==================================    =========================
Symbol                  Definition                            Formula
======================  ==================================    ========================= 
:math:`\nu_t`           Turbulent viscosity                   :math:`\frac{a_1 k}{max (a_1 \omega, S F_2)}`
:math:`S_{ij}`          Symmetric part of stress tensor       :math:`\frac{1}{2}(\partial_j u_i + \partial_i u_j)`
:math:`S`               Strain invariant                      :math:`S=\sqrt{S_{ij}S_{ij}}`
:math:`F_1`             Blending function                     :math:`tanh(arg_1^4)`
:math:`arg_1`           Argument for :math:`F_1` function     :math:`min\left[max \left(\frac{\sqrt{k}}{\beta^* \omega d},\frac{500 \nu}{d^2 \omega}\right), \frac{4 \sigma_{\omega 2} k}{CD_{k \omega} d^2}\right]`
:math:`CD_{k \omega}`           Function                              :math:`max\left(2 \sigma_{\omega 2}\frac{1}{\omega} \frac{\partial k}{\partial x_j} \frac{\partial \omega}{\partial x_j}, 10^{-10}\right)`
:math:`F_2`             Blending function                     :math:`tanh(arg_2^2)`
:math:`arg_2`           Argument for :math:`F_2` function     :math:`max \left(2 \frac{\sqrt{k}}{\beta^* \omega d},\frac{500 \nu}{d^2 \omega}\right)`
======================  ==================================    =========================

This variant of the :math:`k`--:math:`\omega` SST turbulence model uses a production limiter in both :math:`k` and :math:`\omega` equations,

.. math::
   P = min(P, 10 \beta^* \omega k).

**Model constants**

:math:`\gamma_1 = \frac{5}{9}`

:math:`\gamma2 = 0.44`   

:math:`\sigma_{k 1} = 0.85`

:math:`\sigma_{\omega 1} = 0.5`

:math:`\beta_1 = 0.075`

:math:`\sigma_{k 2} = 1.0` 

:math:`\sigma_{\omega 2} = 0.856`

:math:`\beta_2 = 0.0828`  

:math:`\beta^* = 0.09` 

:math:`\kappa = 0.41`

:math:`a_1 = 0.31`

**Boundary conditions**

As described above, the :math:`k`--:math:`\omega` SST turbulence model does not require
damping functions near the wall. Thus it can be used for both resolved and 
unresolved near-wall meshes. In the case of meshes resolved near the wall, 
i.e. :math:`y^+ \sim 1`, the following set of boundary conditions 
are recommended:

:math:`k_{wall} = 0`

:math:`\omega_{wall} =` omegaWallFunction

:math:`\nu_t|_{wall} = 0`

For meshes that are not wall-resolved, i.e., meshes that have the first
cell height next to the wall :math:`y^+ \gg 1`, wall functions will be needed.
In such cases, a wall function for :math:`\nu_t` based on the velocity is
recommended.

:math:`k_{wall} =` kqRWallFunction

:math:`\omega_{wall} =` omegaWallFunction

:math:`\nu_t|_{wall} =` nutUSpaldingWallFunction or alternatively nutUWallFunction if :math:`y^+ \gt 30`

Inlet values of can be determined as follows:

:math:`k_{inlet} = \frac{3}{2} (U_{inlet} I)^2`,

:math:`\omega = C_{\mu}^{-1/4} \frac{\sqrt{k}}{l}`,

:math:`\nu_t` should be set to "calculated" or computed from the turbulent viscosity relation.

:math:`I = \frac{|u_i'|}{|u_i|}` is the turbulence intensity

and :math:`l` is a turbulence length scale.

On outlets, zero gradient is an acceptable choice.

