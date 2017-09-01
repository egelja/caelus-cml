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
    CML::relaxationWeights::relaxationWeightCourantCorrection

Description
    This class performs an additional weighting of the relaxation zone weights,
    which are based on the local cell Courant number.

    This is based on a presentation by Sopheak Seng, Technical University of
    Denmark.

SourceFiles
    relaxationWeightCourantCorrection.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef relaxationWeightCourantCorrection_HPP
#define relaxationWeightCourantCorrection_HPP

#include "fvMesh.hpp"
#include "fvCFD.hpp"
#include "IOdictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationWeights
{

/*---------------------------------------------------------------------------*\
                      Class relaxationShape Declaration
\*---------------------------------------------------------------------------*/

class relaxationWeightCourantCorrection
{

protected:

private:
    // Private data
        const fvMesh& mesh_;

        Switch courantCorrection_;

    // Private Member Functions

        scalar courantNumber
        (
               const surfaceScalarField& phi,
               const surfaceScalarField& magSf,
               const surfaceScalarField& deltaCoeffs,
               const cell& cc
        ) const;

        //- Disallow default bitwise copy construct
        relaxationWeightCourantCorrection
        (
            const relaxationWeightCourantCorrection&
        );

        //- Disallow default bitwise assignment
        void operator=(const relaxationWeightCourantCorrection&);

public:

    //- Runtime type information
    TypeName("relaxationWeightCourantCorrection");

    // Constructors

        //- Construct from components
        relaxationWeightCourantCorrection
        (
            const fvMesh&,
            const dictionary&
        );

    // Destructor

        virtual ~relaxationWeightCourantCorrection();

    // Member Functions
        void courantCorrection
        (
            const labelList&,
            scalarField&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationWeights
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
