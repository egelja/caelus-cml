/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::powerLaw

Description
    Power law porosity model, given by:

        \f[
            S = - \rho C_0 |U|^{(C_1 - 1)} U
        \f]

    where
    \vartable
        C_0      | model linear coefficient
        C_1      | model exponent coefficient
    \endvartable


SourceFiles
    powerLawPorosity.cpp

\*---------------------------------------------------------------------------*/

#ifndef powerLaw_H
#define powerLaw_H

#include "porosityModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace porosityModels
{

/*---------------------------------------------------------------------------*\
                      Class powerLaw Declaration
\*---------------------------------------------------------------------------*/

class powerLaw
:
    public porosityModel
{
private:

    // Private data

        //- C0 coefficient
        scalar C0_;

        //- C1 coefficient
        scalar C1_;

        //- Name of density field
        word rhoName_;


    // Private Member Functions

        //- Apply resistance
        template<class RhoFieldType>
        void apply
        (
            scalarField& Udiag,
            const scalarField& V,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Apply resistance
        template<class RhoFieldType>
        void apply
        (
            tensorField& AU,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Disallow default bitwise copy construct
        powerLaw(const powerLaw&);

        //- Disallow default bitwise assignment
        void operator=(const powerLaw&);


public:

    //- Runtime type information
    TypeName("powerLaw");

    //- Constructor
    powerLaw
    (
        const word& name,
        const word& modelType,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& cellZoneName
    );

    //- Destructor
    virtual ~powerLaw();


    // Member Functions

        //- Transform the model data wrt mesh changes
        virtual void calcTranformModelData();

        //- Calculate the porosity force
        virtual void calcForce
        (
            const volVectorField& U,
            const volScalarField& rho,
            const volScalarField& mu,
            vectorField& force
        ) const;

        //- Add resistance
        virtual void correct(fvVectorMatrix& UEqn) const;

        //- Add resistance
        virtual void correct
        (
            fvVectorMatrix& UEqn,
            const volScalarField& rho,
            const volScalarField& mu
        ) const;

        //- Add resistance
        virtual void correct
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU            
        ) const;


    // I-O

        //- Write
        bool writeData(Ostream& os) const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace porosityModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class RhoFieldType>
void CML::porosityModels::powerLaw::apply
(
    scalarField& Udiag,
    const scalarField& V,
    const RhoFieldType& rho,
    const vectorField& U
) const
{
    const scalar C0 = C0_;
    const scalar C1m1b2 = (C1_ - 1.0)/2.0;

    forAll(cellZoneIDs_, zoneI)
    {
        const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];

        forAll(cells, i)
        {
            const label cellI = cells[i];

            Udiag[cellI] +=
                V[cellI]*rho[cellI]*C0*pow(magSqr(U[cellI]), C1m1b2);
        }
    }
}


template<class RhoFieldType>
void CML::porosityModels::powerLaw::apply
(
    tensorField& AU,
    const RhoFieldType& rho,
    const vectorField& U
) const
{
    const scalar C0 = C0_;
    const scalar C1m1b2 = (C1_ - 1.0)/2.0;

    forAll(cellZoneIDs_, zoneI)
    {
        const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];

        forAll(cells, i)
        {
            const label cellI = cells[i];

            AU[cellI] =
                AU[cellI] + I*(rho[cellI]*C0*pow(magSqr(U[cellI]), C1m1b2));
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
