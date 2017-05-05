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
    Numeric flux class for approxiamte Riemann solvers

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef riemannSolver_HPP
#define riemannSolver_HPP

#include "roeFDSFlux.hpp"
#include "llfFlux.hpp"
#include "hllcFlux.hpp"
#include "ktpFlux.hpp"

namespace CML
{
    class RiemannSolver 
    {
    public:
        virtual void update() = 0;
        virtual surfaceScalarField const& rhoFlux()  const = 0;
        virtual surfaceVectorField const& rhoUFlux() const = 0;
        virtual surfaceScalarField const& rhoEFlux() const = 0;
        virtual ~RiemannSolver(){};
    };

    class roeFluxVLimiter : public RiemannSolver
    {
    private:
        numericFlux<roeFlux,VLimiter> flux_;
 
    public:
        roeFluxVLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };


    class roeFluxBJLimiter : public RiemannSolver
    {
    private:
        numericFlux<roeFlux,BJLimiter> flux_;
 
    public:
        roeFluxBJLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class llfFluxBJLimiter : public RiemannSolver
    {
    private:
        numericFlux<llfFlux,BJLimiter> flux_;
 
    public:
        llfFluxBJLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class llfFluxVLimiter : public RiemannSolver
    {
    private:
        numericFlux<llfFlux,VLimiter> flux_;
 
    public:
        llfFluxVLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class hllcFluxBJLimiter : public RiemannSolver
    {
    private:
        numericFlux<hllcFlux,BJLimiter> flux_;
 
    public:
        hllcFluxBJLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class hllcFluxVLimiter : public RiemannSolver
    {
    private:
      numericFlux<hllcFlux,VLimiter> flux_;
 
    public:
        hllcFluxVLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class ktpFluxBJLimiter : public RiemannSolver
    {
    private:
        numericFlux<ktpFlux,BJLimiter> flux_;
 
    public:
        ktpFluxBJLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

    class ktpFluxVLimiter : public RiemannSolver
    {
    private:
      numericFlux<ktpFlux,VLimiter> flux_;
 
    public:
        ktpFluxVLimiter
        (
            volScalarField const& p,
            volVectorField const& U,
            volScalarField const& T,
            basicThermo& thermo
        ) : 
            RiemannSolver(),
            flux_(p,U,T,thermo)
        {}

        void update()
        {
            this->flux_.update();
        }

        surfaceScalarField const & rhoFlux() const
        {
            return this->flux_.rhoFlux();
        }

        surfaceVectorField const& rhoUFlux() const
        {
            return this->flux_.rhoUFlux();
        }

        surfaceScalarField const& rhoEFlux() const
        {
            return this->flux_.rhoEFlux();
        }

    };

}
#endif
