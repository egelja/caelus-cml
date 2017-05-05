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
    CML::LUscalarMatrix

Description
    CML::LUscalarMatrix


\*---------------------------------------------------------------------------*/

#ifndef LUscalarMatrix_H
#define LUscalarMatrix_H

#include "scalarMatrices.hpp"
#include "labelList.hpp"
#include "FieldField.hpp"
#include "lduInterfaceFieldPtrsList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class lduMatrix;
class procLduMatrix;

/*---------------------------------------------------------------------------*\
                           Class LUscalarMatrix Declaration
\*---------------------------------------------------------------------------*/

class LUscalarMatrix
:
    public scalarSquareMatrix
{
    // Private data

        //- Processor matrix offsets
        labelList procOffsets_;

        //- The pivot indices used in the LU decomposition
        labelList pivotIndices_;

        //- Convert the given lduMatrix into this LUscalarMatrix
        void convert
        (
            const lduMatrix& ldum,
            const FieldField<Field, scalar>& interfaceCoeffs,
            const lduInterfaceFieldPtrsList& interfaces
        );

        //- Convert the given list of procLduMatrix into this LUscalarMatrix
        //  on the master processor
        void convert(const PtrList<procLduMatrix>& lduMatrices);


        //- Print the ratio of the mag-sum of the off-diagonal coefficients
        //  to the mag-diagonal
        void printDiagonalDominance() const;


public:

    // Constructors

        //- Construct from scalarSquareMatrix and perform LU decomposition
        LUscalarMatrix(const scalarSquareMatrix&);

        //- Construct from lduMatrix and perform LU decomposition
        LUscalarMatrix
        (
            const lduMatrix&,
            const FieldField<Field, scalar>& interfaceCoeffs,
            const lduInterfaceFieldPtrsList& interfaces
        );


    // Member Functions

        //- Solve the matrix using the LU decomposition with pivoting
        //  returning the solution in the source
        template<class T>
        void solve(Field<T>& source) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::LUscalarMatrix::solve(Field<Type>& sourceSol) const
{
    if (Pstream::parRun())
    {
        Field<Type> completeSourceSol(n());

        if (Pstream::master())
        {
            typename Field<Type>::subField
            (
                completeSourceSol,
                sourceSol.size()
            ).assign(sourceSol);

            for
            (
                int slave=Pstream::firstSlave();
                slave<=Pstream::lastSlave();
                slave++
            )
            {
                IPstream::read
                (
                    Pstream::scheduled,
                    slave,
                    reinterpret_cast<char*>
                    (
                        &(completeSourceSol[procOffsets_[slave]])
                    ),
                    (procOffsets_[slave + 1] - procOffsets_[slave])*sizeof(Type)
                );
            }
        }
        else
        {
            OPstream::write
            (
                Pstream::scheduled,
                Pstream::masterNo(),
                reinterpret_cast<const char*>(sourceSol.begin()),
                sourceSol.byteSize()
            );
        }

        if (Pstream::master())
        {
            LUBacksubstitute(*this, pivotIndices_, completeSourceSol);

            sourceSol = typename Field<Type>::subField
            (
                completeSourceSol,
                sourceSol.size()
            );

            for
            (
                int slave=Pstream::firstSlave();
                slave<=Pstream::lastSlave();
                slave++
            )
            {
                OPstream::write
                (
                    Pstream::scheduled,
                    slave,
                    reinterpret_cast<const char*>
                    (
                        &(completeSourceSol[procOffsets_[slave]])
                    ),
                    (procOffsets_[slave + 1] - procOffsets_[slave])*sizeof(Type)
                );
            }
        }
        else
        {
            IPstream::read
            (
                Pstream::scheduled,
                Pstream::masterNo(),
                reinterpret_cast<char*>(sourceSol.begin()),
                sourceSol.byteSize()
            );
        }
    }
    else
    {
        LUBacksubstitute(*this, pivotIndices_, sourceSol);
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
