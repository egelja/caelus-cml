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
    CML::relaxationWeights::relaxationWeight

Description
    An abstract base class for generating different relaxation Weights through
    a dictionary lookup.

    See e.g.

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    relaxationWeight.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef relaxationWeight_HPP
#define relaxationWeight_HPP

#include "volFields.hpp"

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "relaxationWeightCourantCorrection.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationWeights
{

/*---------------------------------------------------------------------------*\
                      Class relaxationShape Declaration
\*---------------------------------------------------------------------------*/

class relaxationWeight
:
    public IOdictionary
{

protected:

    // Protected data
        const fvMesh& mesh_;

        const dictionary coeffDict_;

        scalar PI_;

        relaxationWeightCourantCorrection rwcc_;

    // Protected member functions
        virtual void computeWeights
        (
            const labelList&,
            const scalarField&,
            scalarField&
        ) = 0;

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        relaxationWeight(const relaxationWeight&);

        //- Disallow default bitwise assignment
        void operator=(const relaxationWeight&);

public:

    //- Runtime type information
    TypeName("relaxationWeight");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            relaxationWeight,
            dictionary,
            (
                const word& subDictName, const fvMesh& mesh_
            ),
            (subDictName, mesh_)
        );

    // Constructors

        //- Construct from components
        relaxationWeight
        (
            const word& type,
            const fvMesh& mesh_
        );


    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<relaxationWeight> New
        (
            const word& subDictName,
            const fvMesh& mesh_
        );

    // Destructor

        virtual ~relaxationWeight();


    // Member Functions

        // Access
        virtual void weights
        (
            const labelList&,
            const scalarField&,
            scalarField&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationWeights
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
