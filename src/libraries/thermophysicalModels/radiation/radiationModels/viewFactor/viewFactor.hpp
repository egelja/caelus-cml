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
    CML::radiation::viewFactor

Description
    View factor radiation model. The system solved is: C q = b
    where:
            Cij  = deltaij/Ej - (1/Ej - 1)Fij
            q    = heat flux
            b    = A eb - Ho
    and:
            eb   = sigma*T^4
            Ej   = emissivity
            Aij  = deltaij - Fij
            Fij  = view factor matrix


SourceFiles
    viewFactor.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationModelviewFactor_HPP
#define radiationModelviewFactor_HPP

#include "radiationModel.hpp"
#include "singleCellFvMesh.hpp"
#include "scalarMatrices.hpp"
#include "globalIndex.hpp"
#include "scalarListIOList.hpp"
#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                           Class viewFactor Declaration
\*---------------------------------------------------------------------------*/

class viewFactor
:
    public radiationModel
{

    //- Agglomeration List
    labelListIOList finalAgglom_;

    //- Map distributed
    autoPtr<mapDistribute> map_;

    //- Coarse mesh
    singleCellFvMesh coarseMesh_;

    //- Net radiative heat flux [W/m2]
    volScalarField qr_;

    //- View factor matrix
    autoPtr<scalarSquareMatrix> Fmatrix_;

    //- Inverse of C matrix
    autoPtr<scalarSquareMatrix> CLU_;

    //- Selected patches
    labelList selectedPatches_;

    //- Total global coarse faces
    label totalNCoarseFaces_;

    //- Total local coarse faces
    label nLocalCoarseFaces_;

    //- Constant emissivity
    bool constEmissivity_;

    //- Iterations Counter
    label iterCounter_;

    //- Pivot Indices for LU decomposition
    labelList pivotIndices_;


    //- Initialise
    void initialise();

    //- Insert view factors into main matrix
    void insertMatrixElements
    (
        const globalIndex& index,
        const label fromProci,
        const labelListList& globalFaceFaces,
        const scalarListList& viewFactors,
        scalarSquareMatrix& matrix
    );

    //- Disallow default bitwise copy construct
    viewFactor(const viewFactor&);

    //- Disallow default bitwise assignment
    void operator=(const viewFactor&);


public:

    //- Runtime type information
    TypeName("viewFactor");


    //- Construct from components
    viewFactor(const volScalarField& T);

    //- Construct from components
    viewFactor(const dictionary& dict, const volScalarField& T);


    //- Destructor
    virtual ~viewFactor()
    {}


    // Member functions

    // Edit

    //- Solve system of equation(s)
    void calculate();

    //- Read radiation properties dictionary
    bool read();

    //- Source term component (for power of T^4)
    virtual tmp<volScalarField> Rp() const;

    //- Source term component (constant)
    virtual tmp<DimensionedField<scalar, volMesh> > Ru() const;


    // Access

    //- Const access to total radiative heat flux field
    inline const volScalarField& qr() const
    {
        return qr_;
    }

};


} // End namespace radiation
} // End namespace CML


#endif
