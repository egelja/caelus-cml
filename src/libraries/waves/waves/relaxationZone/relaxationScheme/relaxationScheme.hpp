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
    CML::relaxationSchemes::relaxationScheme

Description
    An abstract base class for generating different relaxation schemes through
    a dictionary lookup. See e.g.

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
    relaxationScheme.cpp
    newRelaxationScheme.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef relaxationScheme_HPP
#define relaxationScheme_HPP

#include "volFields.hpp"

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "convexPolyhedral.hpp"
#include "relaxationShape.hpp"
#include "relaxationWeight.hpp"
#include "numericalBeach.hpp"
#include "waveTheory.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationSchemes
{

/*---------------------------------------------------------------------------*\
                      Class relaxationScheme Declaration
\*---------------------------------------------------------------------------*/

class relaxationScheme
:
    public IOdictionary,
    public convexPolyhedral
{

protected:

    // Protected data
        const fvMesh& mesh_;
        vectorField& U_;
        scalarField& alpha_;
        scalarField  alphaTarget_;
        vectorField  UTarget_;

        const dictionary coeffDict_;

        word relaxScheme_;

        autoPtr<relaxationShapes::relaxationShape> relaxShape_;

        autoPtr<relaxationWeights::relaxationWeight> relaxWeight_;

        autoPtr<numericalBeaches::numericalBeach> numBeach_;

        autoPtr<waveTheories::waveTheory> waveProps_;

    // Protected member functions

        //- Returns a scalarField of the signed distance to an arbitrary
        //  surface
        virtual void signedPointToSurfaceDistance
        (
            const pointField&,
            scalarField&
        );


        //- Returns a scalar of the signed distance to an arbitrary surface
        virtual scalar signedPointToSurfaceDistance
        (
            const point&
        ) const;

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        relaxationScheme(const relaxationScheme&);

        //- Disallow default bitwise assignment
        void operator=(const relaxationScheme&);

public:

    //- Runtime type information
    TypeName("relaxationScheme");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            relaxationScheme,
            dictionary,
            (
                const word& subDictName,
                const fvMesh& mesh,
                vectorField& U,
                scalarField& alpha
            ),
            (subDictName, mesh, U, alpha)
        );

    // Constructors

        //- Construct from components
        relaxationScheme
        (
            const word& type,
            const fvMesh& mesh,
            vectorField& U,
            scalarField& alpha,
            const bool empty = false
        );


    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<relaxationScheme> New
        (
            const word& subDictName,
            const fvMesh& mesh,
            vectorField& U,
            scalarField& alpha
        );

    // Destructor

        virtual ~relaxationScheme();


    // Member Functions

        // Access
        virtual void correct() = 0;

        inline const labelList& cells() {return relaxShape_->cells();};
        inline const scalarField& sigma() {return relaxShape_->sigma();};

        void targetAlphaField(volScalarField&);

        void targetVelocityField(volVectorField&);

        void numericalBeach(volScalarField&);

        virtual void addSup(fvMatrix<vector>& eqn);

        virtual void constrain(fvMatrix<vector>& eqn);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationSchemes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
