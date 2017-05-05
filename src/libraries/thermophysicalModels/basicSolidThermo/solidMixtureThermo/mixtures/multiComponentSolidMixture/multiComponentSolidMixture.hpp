/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::multiComponentSolidMixture

Description
    CML::multiComponentSolidMixture

\*---------------------------------------------------------------------------*/

#ifndef multiComponentSolidMixture_H
#define multiComponentSolidMixture_H

#include "PtrList.hpp"
#include "autoPtr.hpp"
#include "basicSolidMixture.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class multiComponentSolidMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoSolidType>
class multiComponentSolidMixture
:
    public basicSolidMixture
{

    // Private data

        //- Solid data
        PtrList<ThermoSolidType> solidData_;


    // Private Member Functions

        //- Correct the mass fractions to sum to 1
        void correctMassFractions();

        //- Return molar fraction for component i in celli and at T
        scalar X(label i, label celli, scalar T) const;


public:


    // Constructors

        //- Construct from dictionary and mesh
        multiComponentSolidMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~multiComponentSolidMixture()
    {}


    // Member Functions

        //- Return the raw solid data
        const PtrList<ThermoSolidType>& solidData() const
        {
            return solidData_;
        }

        //- Read dictionary
        void read(const dictionary&);


        // Cell-based properties

            //- Density
            virtual scalar rho(scalar T, label celli) const;

            //- Absorption coefficient
            virtual scalar kappa(scalar T, label celli) const;

            //- Scatter coefficient
            virtual scalar sigmaS(scalar T, label celli) const;

            //- Thermal conductivity
            virtual scalar K(scalar T, label celli) const;

            //- Emissivity coefficient
            virtual scalar emissivity(scalar T, label celli) const;

            //- Formation enthalpy
            virtual scalar hf(scalar T, label celli) const;

            //- Sensible enthalpy
            virtual scalar hs(scalar T, label celli) const;

            //- Total enthalpy
            virtual scalar h(scalar T, label celli) const;

            //- Specific heat capacity
            virtual scalar Cp(scalar T, label celli) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ThermoSolidType>
void CML::multiComponentSolidMixture<ThermoSolidType>::correctMassFractions()
{
    volScalarField Yt("Yt", Y_[0]);

    for (label n=1; n<Y_.size(); n++)
    {
        Yt += Y_[n];
    }

    forAll(Y_, n)
    {
        Y_[n] /= Yt;
    }


}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::X
(
    label iComp, label celli, scalar T
) const
{
    scalar rhoInv = 0.0;
    forAll(solidData_, i)
    {
        rhoInv += Y_[i][celli]/solidData_[i].rho(T);
    }

    scalar X = Y_[iComp][celli]/solidData_[iComp].rho(T);

    return (X/rhoInv);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoSolidType>
CML::multiComponentSolidMixture<ThermoSolidType>::multiComponentSolidMixture
(
    const dictionary& thermoSolidDict,
    const fvMesh& mesh
)
:
    basicSolidMixture
    (
        thermoSolidDict.lookup("solidComponents"),
        mesh
    ),
    solidData_(components_.size())
{

    forAll(components_, i)
    {
        solidData_.set
        (
            i,
            new ThermoSolidType
            (
                thermoSolidDict.subDict(components_[i] + "Coeffs")
            )
        );
    }
    correctMassFractions();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::rho
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].rho(T)*X(i, celli, T);
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::hf
(
    scalar, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].hf()*Y_[i][celli];
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::hs
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].hs(T)*Y_[i][celli];
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::h
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].h(T)*Y_[i][celli];
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::kappa
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].kappa(T)*X(i, celli, T);
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::sigmaS
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].sigmaS(T)*X(i, celli, T);
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::K
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].K(T)*X(i, celli, T);
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::emissivity
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].emissivity(T)*X(i, celli, T);
    }
    return tmp;
}


template<class ThermoSolidType>
CML::scalar CML::multiComponentSolidMixture<ThermoSolidType>::Cp
(
    scalar T, label celli
) const
{
    scalar tmp = 0.0;
    forAll(solidData_, i)
    {
        tmp += solidData_[i].Cp(T)*Y_[i][celli];
    }
    return tmp;
}


template<class ThermoSolidType>
void CML::multiComponentSolidMixture<ThermoSolidType>::read
(
    const dictionary& thermoDict
)
{
    forAll(components_, i)
    {
        solidData_[i] =
            ThermoSolidType(thermoDict.subDict(components_[i] + "Coeffs"));
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
