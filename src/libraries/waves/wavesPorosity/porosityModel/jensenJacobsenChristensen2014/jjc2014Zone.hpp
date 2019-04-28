/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen, Deltares.
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::jjc2014Zone

Description
    A simplified version of the original OF porosity module, now added the
    functionality of runTime selection of the type of resistance coefficients.

See Also
    jjc2014Zones and coordinateSystems

SourceFiles
    jjc2014Zone.cpp
    jjc2014ZoneTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef jjc2014Zone_HPP
#define jjc2014Zone_HPP

#include "dictionary.hpp"
#include "coordinateSystem.hpp"
#include "coordinateSystems.hpp"
#include "wordList.hpp"
#include "labelList.hpp"
#include "dimensionedScalar.hpp"
#include "dimensionedTensor.hpp"
#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"

#include "fvMesh.hpp"

#include "porosityCoefficient.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class jjc2014Zone;

// Forward declaration of friend functions and operators
Ostream& operator<<(Ostream&, const jjc2014Zone&);


/*---------------------------------------------------------------------------*\
                        Class jjc2014Zone Declaration
\*---------------------------------------------------------------------------*/

class jjc2014Zone
{
    // Private data

        //- Name of this zone
        word name_;

        //- Reference to the finite volume mesh this zone is part of
        const fvMesh& mesh_;

        //- Dictionary containing the parameters
        dictionary dict_;

        //- Cell zone ID
        label cellZoneID_;

        //- Coordinate system used for the zone (Cartesian)
        coordinateSystem coordSys_;

        //- porosity of the zone (0 < porosity <= 1)
        //  Placeholder for treatment of temporal terms.
        scalar porosity_;

        scalar addedMassCoeff_;

        //- Darcy coefficient
        dimensionedTensor D_;

        //- Forchheimer coefficient
        dimensionedTensor F_;


    // Private Member Functions

        //- adjust negative resistance values to be multiplier of max value
        static void checkNegativeResistance(dimensionedVector& resist);

        //- Viscous and inertial resistance
        template<class RhoFieldType>
        void addViscousInertialResistance
        (
            scalarField& Udiag,
            vectorField& Usource,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const scalarField& mu,
            const vectorField& U
        ) const;

        //- Viscous and inertial resistance
        template<class RhoFieldType>
        void addViscousInertialResistance
        (
            tensorField& AU,
            const labelList& cells,
            const RhoFieldType& rho,
            const scalarField& mu,
            const vectorField& U
        ) const;


        //- Disallow default bitwise copy construct
        jjc2014Zone(const jjc2014Zone&);

        //- Disallow default bitwise assignment
        void operator=(const jjc2014Zone&);


public:

    // Constructors

        //- Construct from components
        jjc2014Zone(const word& name, const fvMesh&, const dictionary&);

        //- Return clone
        autoPtr<jjc2014Zone> clone() const
        {
            NotImplemented;
            return autoPtr<jjc2014Zone>(nullptr);
        }

        //- Return pointer to new jjc2014Zone created on freestore from Istream
        class iNew
        {
            //- Reference to the finite volume mesh this zone is part of
            const fvMesh& mesh_;

        public:

            iNew(const fvMesh& mesh)
            :
                mesh_(mesh)
            {}

            autoPtr<jjc2014Zone> operator()(Istream& is) const
            {
                word name(is);
                dictionary dict(is);

                return autoPtr<jjc2014Zone>(new jjc2014Zone(name, mesh_, dict));
            }
        };


    //- Destructor
    virtual ~jjc2014Zone()
    {}


    // Member Functions

        // Access

            //- cellZone name
            const word& zoneName() const
            {
                return name_;
            }

            //- Return mesh
            const fvMesh& mesh() const
            {
                return mesh_;
            }

            //- cellZone number
            label zoneId() const
            {
                return cellZoneID_;
            }

            //- dictionary values used for the jjc2014Zone
            const dictionary& dict() const
            {
                return dict_;
            }

            //- Return coordinate system
            const coordinateSystem& coordSys() const
            {
                return coordSys_;
            }

            //- Return origin
            const point& origin() const
            {
                return coordSys_.origin();
            }

            //- Return porosity
            scalar porosity() const
            {
                return porosity_;
            }

            //- Edit access to porosity
            scalar& porosity()
            {
                return porosity_;
            }

            //- Add the local porosities into the porosity field
            void porosity(volScalarField &) const;

        //- Modify time derivative elements according to porosity
        template<class Type>
        void modifyDdt(fvMatrix<Type>&) const;

        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation
        void addResistance(fvVectorMatrix& UEqn) const;

        //- Add the viscous and inertial resistance force contribution
        //  to the tensorial diagonal.
        //  Optionally correct the processor BCs of AU.
        void addResistance
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU,
            bool correctAUprocBC = true
        ) const;

        //- Write the jjc2014Zone dictionary
        virtual void writeDict(Ostream&, bool subDict = true) const;



    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const jjc2014Zone&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::jjc2014Zone::modifyDdt(fvMatrix<Type>& m) const
{
    if (porosity_ < 1)
    {
        const labelList& cells = mesh_.cellZones()[cellZoneID_];

        scalar coeff((1.0 + addedMassCoeff_*(1 - porosity_)/porosity_)/porosity_);

        forAll(cells, i)
        {
            m.diag()[cells[i]]   *= coeff;
            m.source()[cells[i]] *= coeff;
        }
    }
}

template<class RhoFieldType>
void CML::jjc2014Zone::addViscousInertialResistance
(
    scalarField& Udiag,
    vectorField& Usource,
    const labelList& cells,
    const scalarField& V,
    const RhoFieldType& rho,
    const scalarField& mu,
    const vectorField& U
) const
{
    const tensor& D = D_.value();
    const tensor& F = F_.value();

    forAll (cells, i)
    {
        tensor dragCoeff = mu[cells[i]]*D + (rho[cells[i]]*mag(U[cells[i]]))*F;
        scalar isoDragCoeff = tr(dragCoeff);

        Udiag[cells[i]] += V[cells[i]]*isoDragCoeff;
        Usource[cells[i]] -=
            V[cells[i]]*((dragCoeff - I*isoDragCoeff) & U[cells[i]]);
    }
}

template<class RhoFieldType>
void CML::jjc2014Zone::addViscousInertialResistance
(
    tensorField& AU,
    const labelList& cells,
    const RhoFieldType& rho,
    const scalarField& mu,
    const vectorField& U
) const
{
    const tensor& D = D_.value();
    const tensor& F = F_.value();

    forAll (cells, i)
    {
        AU[cells[i]] += mu[cells[i]]*D + (rho[cells[i]]*mag(U[cells[i]]))*F;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
