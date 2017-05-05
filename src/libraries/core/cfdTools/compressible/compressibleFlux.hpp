/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Description
    Compressible flux for compressible pressure equation. 

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

surfaceVectorField const ULeft = 
    fvc::interpolate(U, left, "reconstruct(T)")();

surfaceVectorField const URight = 
    fvc::interpolate(U, right, "reconstruct(T)")();

surfaceScalarField const Uf = mag(0.5*(ULeft + URight))();

volScalarField const c = sqrt(1/psi)();

surfaceScalarField const cLeft = 
    fvc::interpolate(c, left, "reconstruct(T)")();

surfaceScalarField const cRight = 
    fvc::interpolate(c, right, "reconstruct(T)")();

surfaceScalarField const cf = (0.5*(cLeft + cRight))();

surfaceScalarField const Mf = (Uf/cf)();

surfaceScalarField MfSqr = sqr(Mf)();

surfaceScalarField const fM = 
    (min(scalar(1.0), Mf*sqrt(scalar(8)+pow((scalar(1)-MfSqr),2))/(1+MfSqr)))();

surfaceScalarField const psiLeft = 
    fvc::interpolate(psi, left, "reconstruct(T)")();

surfaceScalarField const psiRight = 
    fvc::interpolate(psi, right, "reconstruct(T)")();

phil = 0.5*
(
    (0.5*(ULeft+URight) & mesh.Sf())*0.5*(psiRight+psiLeft)
    -
    mag(0.5*(ULeft+URight) & mesh.Sf())*(psiRight-psiLeft)
);

phis = 0.25*(psiRight+psiLeft)*((ULeft+URight) & mesh.Sf());

phid = fM*phis + (scalar(1)-fM)*phil; 


