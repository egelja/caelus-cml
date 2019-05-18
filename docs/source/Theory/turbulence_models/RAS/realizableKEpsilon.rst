.. _Realizable-k-varepsilon label:

Realizable :math:`k`--:math:`\varepsilon` Turbulence  Model
-----------------------------------------------------------

**Model description**

The realizable :math:`k`--:math:`\varepsilon` turbulence model is an improvement
on the standard :math:`k`--:math:`\varepsilon` model as it is modified to
guarantee realizability of normal Reynolds stresses :cite:`Shih1995`. 
In the standard :math:`k`--:math:`\varepsilon` turbulence model it
is possible, under certain conditions, to have negative normal Reynolds
stresses, i.e. non-realizable. The realizable :math:`k`--:math:`\varepsilon` turbulence 
model resolves this problem by introducing a non-constant definition of 
:math:`C_{\mu}` and by modifying the :math:`\varepsilon` equation. The transport 
equations for the realizable :math:`k`--:math:`\varepsilon` turbulence model are as 
follows:

.. math::
   \partial_t k + \partial_j (u_j k) = \partial_j\left[\mu + \frac{\mu_t}{\sigma_k}\partial_j k\right] + G_k -\varepsilon

.. math::
   \partial_t \varepsilon + \partial_j (u_j \varepsilon) = \partial_j\left[\mu + \frac{\mu_t}{\sigma_{\varepsilon}}\partial_j \varepsilon \right] + C_1 S \varepsilon - C_2 \frac{\varepsilon^2}{k+ \sqrt{\nu\varepsilon}}


The terms appearing in the  realizable :math:`k`--:math:`\varepsilon` turbulence model have the following definitions:

**Model definitions**

===============================  ==================================    =========================
Symbol                           Definition                            Formula
===============================  ==================================    ========================= 
:math:`\nu_t`                    Turbulent viscosity                   :math:`C_{\mu}\frac{k^2}{\varepsilon}`                             
:math:`C_1`                      Model parameter                       :math:`max \left(0.43,\frac{\eta}{\eta+5}\right)`
:math:`\eta`                     Strain magnitude over time scale      :math:`S\frac{k}{\varepsilon}`
:math:`S`                        Strain magnitude                      :math:`S = \sqrt{2S_{ij}S_{ij}}`
:math:`S_{ij}`                   Strain tensor                         :math:`\frac{1}{2}(\partial_j u_i + \partial_i u_j)`
:math:`C_{\mu}`                  Model parameter                       :math:`\frac{1}{A_0 + A_S \frac{k U^*}{\varepsilon}}`
:math:`U^*`                      Velocity                              :math:`\sqrt{S_{ij}S_{ij}+\tilde{\Omega_{ij}}\tilde{\Omega_{ij}}}`
:math:`\overline{\Omega}_{ij}`   Rate of rotation tensor               :math:`\frac{1}{2}(\partial_j u_i - \partial_i u_j)`        
:math:`\omega_k`                 Angular velocity                          
:math:`\Omega_{ij}`              Rotation tensor                       :math:`\overline{\Omega}_{ij} - \varepsilon_{ijk} \omega_k`
:math:`\tilde{\Omega}_{ij}`      Modified rotation tensor              :math:`\Omega_{ij} - 2 \varepsilon_{ijk} \omega_k`
:math:`A_0`                      Model constant                        :math:`4.04`
:math:`A_S`                      Function                              :math:`\sqrt{6} cos(\phi)`
:math:`\phi`                     Function                              :math:`\frac{1}{3} cos^{-1} (\sqrt{6} W)`
:math:`W`                        Variable                              :math:`\frac{S_ijS_{jk}S{ki}}{S^3}`
===============================  ==================================    =========================

**Model constants**

:math:`C_{1\varepsilon} = 1.44`

:math:`C_2 = 1.9`   

:math:`\sigma_k = 1.0`

:math:`\sigma_{\varepsilon} = 1.2`

**Boundary conditions**

The realizable :math:`k`--:math:`\varepsilon` was developed as a high-Reynolds 
number model. In other words, the wall treatment is such that wall functions 
for :math:`\nu_t`, :math:`k`, and :math:`\varepsilon` fields must be always 
used. It is recommended that velocity based wall functions for the 
:math:`\nu_t` field is used. On the inlet, the following relations can 
be used to determine the values of :math:`k` and :math:`\varepsilon` fields:

:math:`k|_{inlet} = \frac{3}{2} (U_{inlet} I)^2`,

:math:`\varepsilon = C_{\mu}^{3/4} \frac{k^{3/2}}{l}`,

:math:`\nu_t` should be set to "calculated" or computed from the viscosity 
ratio.

:math:`I = \frac{|u_i'|}{|u_i|}` is the turbulence intensity

and :math:`l` is a turbulence length scale.

On outlets, zero gradient is an acceptable choice.

