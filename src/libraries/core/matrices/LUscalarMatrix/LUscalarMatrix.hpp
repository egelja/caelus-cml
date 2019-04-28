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

        //- Construct null
        LUscalarMatrix();

        //- Construct from and perform LU decomposition of the matrix M
        LUscalarMatrix(const scalarSquareMatrix& M);

        //- Construct from lduMatrix and perform LU decomposition
        LUscalarMatrix
        (
            const lduMatrix&,
            const FieldField<Field, scalar>& interfaceCoeffs,
            const lduInterfaceFieldPtrsList& interfaces
        );


    // Member Functions

        //- Perform the LU decomposition of the matrix M
        void decompose(const scalarSquareMatrix& M);

        //- Solve the linear system with the given source
        //  and returning the solution in the Field argument x.
        //  This function may be called with the same field for x and source.
        template<class Type>
        void solve(Field<Type>& x, const Field<Type>& source) const;

        //- Solve the linear system with the given source
        //  returning the solution
        template<class Type>
        tmp<Field<Type>> solve(const Field<Type>& source) const;

        //- Set M to the inverse of this square matrix
        void inv(scalarSquareMatrix& M) const;
};


} // End namespace CML

#include "SubField.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::LUscalarMatrix::solve
(
    Field<Type>& x,
    const Field<Type>& source
) const
{
    // If x and source are different initialize x = source
    if (&x != &source)
    {
        x = source;
    }

    if (Pstream::parRun())
    {
        Field<Type> X(m());

        if (Pstream::master())
        {
            typename Field<Type>::subField
            (
                X,
                x.size()
            ).assign(x);

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
                        &(X[procOffsets_[slave]])
                    ),
                    (procOffsets_[slave+1]-procOffsets_[slave])*sizeof(Type)
                );
            }
        }
        else
        {
            OPstream::write
            (
                Pstream::scheduled,
                Pstream::masterNo(),
                reinterpret_cast<const char*>(x.begin()),
                x.byteSize()
            );
        }

        if (Pstream::master())
        {
            LUBacksubstitute(*this, pivotIndices_, X);

            x = typename Field<Type>::subField
            (
                X,
                x.size()
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
                        &(X[procOffsets_[slave]])
                    ),
                    (procOffsets_[slave + 1]-procOffsets_[slave])*sizeof(Type)
                );
            }
        }
        else
        {
            IPstream::read
            (
                Pstream::scheduled,
                Pstream::masterNo(),
                reinterpret_cast<char*>(x.begin()),
                x.byteSize()
            );
        }
    }
    else
    {
        LUBacksubstitute(*this, pivotIndices_, x);
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::LUscalarMatrix::solve
(
    const Field<Type>& source
) const
{
    tmp<Field<Type> > tx(new Field<Type>(m()));
    Field<Type>& x = tx();

    solve(x, source);

    return tx;
}


#endif
