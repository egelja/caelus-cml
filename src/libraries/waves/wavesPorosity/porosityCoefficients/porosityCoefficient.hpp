/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
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
    CML::porosityCoefficient

Description
    Abstract base class for the computation of resistance coefficients in the
    case of porous simulations.

SourceFiles
    porosityCoefficient.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef porosityCoefficient_HPP
#define porosityCoefficient_HPP

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class porosityCoefficient Declaration
\*---------------------------------------------------------------------------*/

class porosityCoefficient
{

protected:

    // Protected data
        const dictionary & poroProperties_;

        dimensionedVector linearCoefficient_;
        dimensionedVector quadraticCoefficient_;
        dimensionedVector KCQuadraticCoefficient_;
        dimensionedScalar scaledKC_;

    // Protected member functions
        scalar readResistancePorosity(const dictionary&) const;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        porosityCoefficient(const porosityCoefficient&);

        //- Disallow default bitwise assignment
        void operator=(const porosityCoefficient&);

public:

    //- Runtime type information
    TypeName("porosityCoefficient");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            porosityCoefficient,
            porosityCoefficient,
            (
                const dictionary & poroProp
            ),
            (poroProp)
        );



    // Constructors

        //- Construct from components
        porosityCoefficient(const dictionary &);

    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<porosityCoefficient> New(const dictionary &);

    // Destructor

        virtual ~porosityCoefficient();

    // Member Functions

        // Access
        const dimensionedVector& linearCoefficient() const
        {
            return linearCoefficient_;
        };

        const dimensionedVector& quadraticCoefficient() const
        {
            return quadraticCoefficient_;
        };

        const dimensionedVector& KCquadraticCoefficinet() const
        {
        	return KCQuadraticCoefficient_;
        }

        const dimensionedScalar& scaledKC() const
        {
        	return scaledKC_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
