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
    Creation of Riemann solver and selection

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/


    roeFluxVLimiter    rfvLmtr(p,U,T,thermo);
    RiemannSolver*     rfvPtr = &rfvLmtr;

    roeFluxBJLimiter   rfbjLmtr(p,U,T,thermo);
    RiemannSolver*     rfbjPtr = &rfbjLmtr;
    
    llfFluxVLimiter    llfvLmtr(p,U,T,thermo);
    RiemannSolver*     lfvPtr = &llfvLmtr;

    llfFluxBJLimiter   llfbjLmtr(p,U,T,thermo);
    RiemannSolver*     lfbjPtr = &llfbjLmtr;

    hllcFluxVLimiter   hllcfvLmtr(p,U,T,thermo);
    RiemannSolver*     hfvPtr = &hllcfvLmtr;

    hllcFluxBJLimiter  hllcfbjLmtr(p,U,T,thermo);
    RiemannSolver*     hfbjPtr = &hllcfbjLmtr;

    ktpFluxVLimiter    ktpfvLmtr(p,U,T,thermo);
    RiemannSolver*     kfvPtr = &ktpfvLmtr;

    ktpFluxBJLimiter   ktpfbjLmtr(p,U,T,thermo);
    RiemannSolver*     kfbjPtr = &ktpfbjLmtr;

    dwFluxVLimiter     dwfvLmtr(p,U,T,thermo);
    RiemannSolver*     dwvPtr = &dwfvLmtr;

    dwFluxBJLimiter    dwfbjLmtr(p,U,T,thermo);
    RiemannSolver*     dwbjPtr = &dwfbjLmtr;


    dbRegistrator<RiemannSolver*> roeFluxVLimiter
    (
        IOobject
        (
            "roeFluxVLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rfvPtr
    );

    dbRegistrator<RiemannSolver*> roeFluxBJLimiter
    (
        IOobject
        (
            "roeFluxBJLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rfbjPtr
    );

    dbRegistrator<RiemannSolver*> llfFluxVLimiter
    (
        IOobject
        (
            "llfFluxVLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        lfvPtr
    );

    dbRegistrator<RiemannSolver*> llfFluxBJLimiter
    (
        IOobject
        (
            "llfFluxBJLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        lfbjPtr
    );

    dbRegistrator<RiemannSolver*> hllcFluxVLimiter
    (
        IOobject
        (
            "hllcFluxVLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        hfvPtr
    );

    dbRegistrator<RiemannSolver*> hllcFluxBJLimiter
    (
        IOobject
        (
            "hllcFluxBJLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        hfbjPtr
    );

    dbRegistrator<RiemannSolver*> ktpFluxVLimiter
    (
        IOobject
        (
            "ktpFluxVLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        kfvPtr
    );

    dbRegistrator<RiemannSolver*> ktpFluxBJLimiter
    (
        IOobject
        (
            "ktpFluxBJLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        kfbjPtr
    );

    dbRegistrator<RiemannSolver*> dwFluxVLimiter
    (
        IOobject
        (
            "dwFluxVLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        dwvPtr
    );

    dbRegistrator<RiemannSolver*> dwFluxBJLimiter
    (
        IOobject
        (
            "dwFluxBJLimiter",
            runTime.constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        dwbjPtr
    );

    Info << "Available numeric fluxes : roeFlux, llfFlux, hllcFlux, ktpFlux, dwFlux" << endl;
    Info << "Available limiters: VLimiter, BJLimiter" << endl;


    word const fluxType
    (
        IOdictionary
        (
            IOobject
            (
                "fvSchemes",
                U.time().system(),
                U.db(),
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("Flux")
    );

    word const limiterType
    (
        IOdictionary
        (
            IOobject
            (
                "fvSchemes",
                U.time().system(),
                U.db(),
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("Limiter")
    );

    word const numericFluxScheme = fluxType + limiterType;
    Info << "Numeric flux scheme " << numericFluxScheme << " selected" << endl;

    RiemannSolver* const& rs = 
        mesh.lookupObject<dbRegistrator<RiemannSolver*> >
        (
            numericFluxScheme
        )(); 

    RiemannSolver* & riemannSolver = 
        const_cast<RiemannSolver*&>(rs);
