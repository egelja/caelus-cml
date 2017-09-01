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
    CML::relaxationShapes::relaxationShape

Description
    An abstract base class for generating different relaxation shapes through
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
    relaxationShape.cpp
    newRelaxationShape.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef relaxationShape_HPP
#define relaxationShape_HPP

#include "volFields.hpp"
#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

/*---------------------------------------------------------------------------*\
                      Class relaxationShape Declaration
\*---------------------------------------------------------------------------*/

class relaxationShape
:
    public IOdictionary
{

protected:

    // Protected data
        const fvMesh& mesh_;

        const dictionary coeffDict_;
        vector direction_;

        labelList cells_;
        scalarField sigma_;

        word relaxType_;

        scalar PI_;

        label refreshIndexCells_, refreshIndexSigma_;

    // Protected member functions
        virtual void computeSigmaCoordinate() = 0;

        virtual void findComputationalCells() = 0;

private:

    // Private Member Functions
        void refreshCells();

        void refreshSigma();

        //- Disallow default bitwise copy construct
        relaxationShape(const relaxationShape&);

        //- Disallow default bitwise assignment
        void operator=(const relaxationShape&);

public:

    //- Runtime type information
    TypeName("relaxationShape");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            relaxationShape,
            dictionary,
            (
                const word& subDictName, const fvMesh& mesh_
            ),
            (subDictName, mesh_)
        );

    // Constructors

        //- Construct from components
        relaxationShape
        (
            const word& type,
            const fvMesh& mesh_
        );


    // Selectors

        //- Return a reference to the selected relaxation shape
        static autoPtr<relaxationShape> New
        (
            const word& subDictName,
            const fvMesh& mesh_
        );

        //- Return a reference to the selected relaxation shape
        static autoPtr<relaxationShape> New
        (
            const word& subDictName,
            const word relaxationShapeType,
            const fvMesh& mesh_
        );

    // Destructor

        virtual ~relaxationShape();


    // Member Functions

        // Access
        inline const labelList& cells() {refreshCells(); return cells_;};

        inline const scalarField& sigma() {refreshSigma(); return sigma_;};

        virtual const pointField& pointSet() = 0;

        virtual scalar interpolation(const scalarField&, const point&) const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
