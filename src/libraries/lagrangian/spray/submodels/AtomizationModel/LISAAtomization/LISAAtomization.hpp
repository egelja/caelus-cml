/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::LISAAtomization

Description
    Primary Breakup Model for pressure swirl atomizers.

    Accurate description in
    @verbatim
    P.K. Senecal, D.P. Schmidt, I. Nouar, C.J. Rutland, R.D. Reitz, M. Corradini
    "Modeling high-speed viscous liquid sheet atomization"
    International Journal of Multiphase Flow 25 (1999) pags. 1073-1097
    @endverbatim

    and

    @verbatim
    D.P. Schmidt, I. Nouar, P.K. Senecal, C.J. Rutland, J.K. Martin, R.D. Reitz
    "Pressure-Swirl Atomization in the Near Field"
    SAE Techical Paper Series 1999-01-0496
    @endverbatim

\*---------------------------------------------------------------------------*/

#ifndef LISAAtomization_H
#define LISAAtomization_H

#include "AtomizationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class LISAAtomization Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LISAAtomization
:
    public AtomizationModel<CloudType>
{

public:

    //- Enumeration for SMD calculations
    enum SMDMethods
    {
        method1,
        method2
    };


private:

    // Private data

        scalar Cl_;
        scalar cTau_;
        scalar Q_;
        scalar lisaExp_;
        vector injectorDirection_;
        word SMDCalcMethod_;

        SMDMethods SMDMethod_;


public:

    //- Runtime type information
    TypeName("LISA");


    // Constructors

        //- Construct from dictionary
        LISAAtomization(const dictionary&, CloudType&);

        //- Construct copy
        LISAAtomization(const LISAAtomization<CloudType>& am);

        //- Construct and return a clone
        virtual autoPtr<AtomizationModel<CloudType> > clone() const
        {
            return autoPtr<AtomizationModel<CloudType> >
            (
                new LISAAtomization<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LISAAtomization();


    // Member Functions

        //- Initial value of liquidCore
        virtual scalar initLiquidCore() const;

        //- Flag to indicate if chi needs to be calculated
        virtual bool calcChi() const;

        virtual void update
        (
            const scalar dt,
            scalar& d,
            scalar& liquidCore,
            scalar& tc,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const scalar volFlowRate,
            const scalar rhoAv,
            const scalar Urel,
            const vector& pos,
            const vector& injectionPos,
            const scalar pAmbient,
            const scalar chi,
            Random& rndGen
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::LISAAtomization<CloudType>::LISAAtomization
(
    const dictionary& dict,
    CloudType& owner
)
:
    AtomizationModel<CloudType>(dict, owner, typeName),
    Cl_(readScalar(this->coeffDict().lookup("Cl"))),
    cTau_(readScalar(this->coeffDict().lookup("cTau"))),
    Q_(readScalar(this->coeffDict().lookup("Q"))),
    lisaExp_(readScalar(this->coeffDict().lookup("lisaExp"))),
    injectorDirection_(this->coeffDict().lookup("injectorDirection")),
    SMDCalcMethod_(this->coeffDict().lookup("SMDCalculationMethod"))
{
    // Note: Would be good if this could be picked up from the injector
    injectorDirection_ /= mag(injectorDirection_);

    if (SMDCalcMethod_ == "method1")
    {
        SMDMethod_ = method1;
    }
    else if (SMDCalcMethod_ == "method2")
    {
        SMDMethod_ = method2;
    }
    else
    {
        SMDMethod_ = method2;
        Info<< "Warning: SMDCalculationMethod " << SMDCalcMethod_
            << " unknown. Options are (method1 | method2). Using method2"
            << endl;
    }
}

template<class CloudType>
CML::LISAAtomization<CloudType>::LISAAtomization
(
    const LISAAtomization<CloudType>& am
)
:
    AtomizationModel<CloudType>(am),
    Cl_(am.Cl_),
    cTau_(am.cTau_),
    Q_(am.Q_),
    lisaExp_(am.lisaExp_),
    injectorDirection_(am.injectorDirection_),
    SMDCalcMethod_(am.SMDCalcMethod_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::LISAAtomization<CloudType>::~LISAAtomization()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::LISAAtomization<CloudType>::initLiquidCore() const
{
    return 1.0;
}


template<class CloudType>
bool CML::LISAAtomization<CloudType>::calcChi() const
{
    return true;
}


template<class CloudType>
void CML::LISAAtomization<CloudType>::update
(
    const scalar dt,
    scalar& d,
    scalar& liquidCore,
    scalar& tc,
    const scalar rho,
    const scalar mu,
    const scalar sigma,
    const scalar volFlowRate,
    const scalar rhoAv,
    const scalar Urel,
    const vector& pos,
    const vector& injectionPos,
    const scalar pAmbient,
    const scalar chi,
    Random& rndGen
) const
{
    if (volFlowRate < SMALL)
    {
        return;
    }

    scalar tau = 0.0;
    scalar dL = 0.0;
    scalar k = 0.0;

    // update atomization characteristic time
    tc += dt;

    scalar We = 0.5*rhoAv*sqr(Urel)*d/sigma;
    scalar nu = mu/rho;

    scalar Q = rhoAv/rho;

    vector diff = pos - injectionPos;
    scalar pWalk = mag(diff);
    scalar traveledTime = pWalk/Urel;

    scalar h = diff & injectorDirection_;
    scalar delta = sqrt(sqr(pWalk) - sqr(h));

    scalar hSheet = volFlowRate/(constant::mathematical::pi*delta*Urel);

    // update drop diameter
    d = min(d, hSheet);

    if (We > 27.0/16.0)
    {
        scalar kPos = 0.0;
        scalar kNeg = Q*sqr(Urel)*rho/sigma;

        scalar derivPos = sqrt(Q*sqr(Urel));

        scalar derivNeg =
            (
                8.0*sqr(nu)*pow3(kNeg)
              + Q*sqr(Urel)*kNeg
              - 3.0*sigma/2.0/rho*sqr(kNeg)
            )
          / sqrt
            (
                4.0*sqr(nu)*pow4(kNeg)
              + Q*sqr(Urel)*sqr(kNeg)
              - sigma*pow3(kNeg)/rho
            )
          - 4.0*nu*kNeg;

        scalar kOld = 0.0;

        for (label i=0; i<40; i++)
        {
            k = kPos - (derivPos/((derivNeg - derivPos)/(kNeg - kPos)));

            scalar derivk =
                (
                    8.0*sqr(nu)*pow3(k)
                  + Q*sqr(Urel)*k
                  - 3.0*sigma/2.0/rho*sqr(k)
                )
              / sqrt
                (
                    4.0*sqr(nu)*pow4(k)
                  + Q*sqr(Urel)*sqr(k)
                  - sigma*pow3(k)/rho
                )
              - 4.0*nu*k;

            if (derivk > 0)
            {
                derivPos = derivk;
                kPos = k;
            }
            else
            {
                derivNeg = derivk;
                kNeg = k;
            }

            if (mag(k - kOld)/k < 1e-4)
            {
                break;
            }

            kOld = k;
        }

        scalar omegaS =
          - 2.0*nu*sqr(k)
          + sqrt
            (
                4.0*sqr(nu)*pow4(k)
              + Q*sqr(Urel)*sqr(k)
              - sigma*pow3(k)/rho
            );

        tau = cTau_/omegaS;

        dL = sqrt(8.0*d/k);
    }
    else
    {
        k = rhoAv*sqr(Urel)/(2.0*sigma);

        scalar J = 0.5*traveledTime*hSheet;

        tau = pow(3.0*cTau_, 2.0/3.0)*cbrt(J*sigma/(sqr(Q)*pow4(Urel)*rho));

        dL = sqrt(4.0*d/k);
    }

    scalar kL = 1.0/(dL*sqrt(0.5 + 1.5 * mu/sqrt(rho*sigma*dL)));

    scalar dD = cbrt(3.0*constant::mathematical::pi*sqr(dL)/kL);

    scalar atmPressure = 1.0e+5;

    scalar pRatio = pAmbient/atmPressure;

    dD = dD*pow(pRatio, lisaExp_);

    scalar pExp = 0.135;

    //  modifying dD to take account of flash boiling
    dD = dD*(1.0 - chi*pow(pRatio, -pExp));
    scalar lBU = Cl_ * mag(Urel)*tau;

    if (pWalk > lBU)
    {
        scalar x = 0;

        switch (SMDMethod_)
        {
            case method1:
            {
                #include "LISASMDCalcMethod1.hpp"
                break;
            }
            case method2:
            {
                #include "LISASMDCalcMethod2.hpp"
                break;
            }
        }

        //  New droplet properties
        liquidCore = 0.0;
        d = x;
        tc = 0.0;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
