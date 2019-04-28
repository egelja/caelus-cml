/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
Copyright (C) 2016 OpenCFD Ltd
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
    CML::fvMatrix

Description
    A special matrix type and solver, designed for finite volume
    solutions of scalar equations.
    Face addressing is used to make all matrix assembly
    and solution loops vectorise.

SourceFiles
    fvMatrix.cpp
    fvMatrixSolve.cpp
    fvScalarMatrix.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvMatrix_H
#define fvMatrix_H

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "lduMatrix.hpp"
#include "tmp.hpp"
#include "autoPtr.hpp"
#include "dimensionedTypes.hpp"
#include "zero.hpp"
#include "className.hpp"
#include "fvc.hpp"
#include "profiling.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class fvMatrix;

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const fvMatrix<Type>&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const fvMatrix<Type>&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const fvMatrix<Type>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const tmp<fvMatrix<Type> >&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const tmp<fvMatrix<Type> >&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > operator&
(
    const tmp<fvMatrix<Type> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
Ostream& operator<<(Ostream&, const fvMatrix<Type>&);

template<class T> class UIndirectList;


/*---------------------------------------------------------------------------*\
                           Class fvMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fvMatrix
:
    public refCount,
    public lduMatrix
{
    // Private data

        //- Const reference to GeometricField<Type, fvPatchField, volMesh>
        //  Converted into a non-const reference at the point of solution.
        const GeometricField<Type, fvPatchField, volMesh>& psi_;

        //- Dimension set
        dimensionSet dimensions_;

        //- Source term
        Field<Type> source_;

        //- Boundary scalar field containing pseudo-matrix coeffs
        //  for internal cells
        FieldField<Field, Type> internalCoeffs_;

        //- Boundary scalar field containing pseudo-matrix coeffs
        //  for boundary cells
        FieldField<Field, Type> boundaryCoeffs_;


        //- Face flux field for non-orthogonal correction
        mutable GeometricField<Type, fvsPatchField, surfaceMesh>
            *faceFluxCorrectionPtr_;


protected:

    //- Declare friendship with the fvSolver class
    friend class fvSolver;

    // Protected Member Functions

        //- Add patch contribution to internal field
        template<class Type2>
        void addToInternalField
        (
            const labelUList& addr,
            const Field<Type2>& pf,
            Field<Type2>& intf
        ) const;

        template<class Type2>
        void addToInternalField
        (
            const labelUList& addr,
            const tmp<Field<Type2> >& tpf,
            Field<Type2>& intf
        ) const;

        //- Subtract patch contribution from internal field
        template<class Type2>
        void subtractFromInternalField
        (
            const labelUList& addr,
            const Field<Type2>& pf,
            Field<Type2>& intf
        ) const;

        template<class Type2>
        void subtractFromInternalField
        (
            const labelUList& addr,
            const tmp<Field<Type2> >& tpf,
            Field<Type2>& intf
        ) const;


        // Matrix manipulation functionality

            //- Set solution in given cells to the specified values
            template<template<class> class ListType>
            void setValuesFromList
            (
                const labelUList& cells,
                const ListType<Type>& values
            );


public:

    //- Solver class returned by the solver function
    //  used for systems in which it is useful to cache the solver for reuse
    //  e.g. if the solver is potentially expensive to construct (AMG) and can
    //  be used several times (PISO)
    class fvSolver
    {
        fvMatrix<Type>& fvMat_;

        autoPtr<lduMatrix::solver> solver_;

    public:

        // Constructors

            fvSolver(fvMatrix<Type>& fvMat, autoPtr<lduMatrix::solver> sol)
            :
                fvMat_(fvMat),
                solver_(sol)
            {}


        // Member functions

            //- Solve returning the solution statistics.
            //  Use the given solver controls
            solverPerformance solve(const dictionary&);

            //- Solve returning the solution statistics.
            //  Solver controls read from fvSolution
            solverPerformance solve();
    };


    ClassName("fvMatrix");


    // Constructors

        //- Construct given a field to solve for
        fvMatrix
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const dimensionSet&
        );

        //- Construct as copy
        fvMatrix(const fvMatrix<Type>&);

        //- Construct as copy of tmp<fvMatrix<Type> > deleting argument
        #ifdef ConstructFromTmp
        fvMatrix(const tmp<fvMatrix<Type> >&);
        #endif

        //- Construct from Istream given field to solve for
        fvMatrix(const GeometricField<Type, fvPatchField, volMesh>&, Istream&);


    //- Destructor
    virtual ~fvMatrix();


    // Member functions

        // Access

            const GeometricField<Type, fvPatchField, volMesh>& psi() const
            {
                return psi_;
            }

            const dimensionSet& dimensions() const
            {
                return dimensions_;
            }

            Field<Type>& source()
            {
                return source_;
            }

            const Field<Type>& source() const
            {
                return source_;
            }

            //- Access to fvBoundary scalar field containing
            //  pseudo-matrix coeffs for internal cells
            const FieldField<Field, Type>& internalCoeffs() const
            {
                return internalCoeffs_;
            }

            //- fvBoundary scalar field containing pseudo-matrix coeffs
            //  for internal cells
            FieldField<Field, Type>& internalCoeffs()
            {
                return internalCoeffs_;
            }

            //- Access to fvBoundary scalar field containing
            //  pseudo-matrix coeffs for boundary cells
            const FieldField<Field, Type>& boundaryCoeffs() const
            {
                return boundaryCoeffs_;
            }

            //- fvBoundary scalar field containing pseudo-matrix coeffs
            //  for boundary cells
            FieldField<Field, Type>& boundaryCoeffs()
            {
                return boundaryCoeffs_;
            }


            //- Declare return type of the faceFluxCorrectionPtr() function
            typedef GeometricField<Type, fvsPatchField, surfaceMesh>
                *surfaceTypeFieldPtr;

            //- Return pointer to face-flux non-orthogonal correction field
            surfaceTypeFieldPtr& faceFluxCorrectionPtr()
            {
                return faceFluxCorrectionPtr_;
            }


        // Matrix completion functionality

            void addBoundaryDiag
            (
                scalarField& diag,
                const direction cmpt
            ) const;

            void addCmptAvBoundaryDiag(scalarField& diag) const;

            void addBoundarySource
            (
                Field<Type>& source,
                const bool couples=true
            ) const;


        // Operations

            //- Set solution in given cells to the specified values
            //  and eliminate the corresponding equations from the matrix.
            void setValues
            (
                const labelUList& cells,
                const UList<Type>& values
            );

            //- Set solution in given cells to the specified values
            //  and eliminate the corresponding equations from the matrix.
            void setValues
            (
                const labelUList& cells,
                const UIndirectList<Type>& values
            );

            //- Set reference level for solution
            void setReference
            (
                const label celli,
                const Type& value,
                const bool forceReference = false
            );

            //- Set reference level for a component of the solution
            //  on a given patch face
            void setComponentReference
            (
                const label patchi,
                const label facei,
                const direction cmpt,
                const scalar value
            );

            //- Relax matrix (for steady-state solution).
            //  alpha = 1 : diagonally equal
            //  alpha < 1 : diagonally dominant
            //  alpha = 0 : do nothing
            //  Note: Requires positive diagonal.
            void relax(const scalar alpha);

            //- Relax matrix (for steady-state solution).
            //  alpha is read from controlDict
            void relax();

            //- Manipulate based on a boundary field
            void boundaryManipulate
            (
                typename GeometricField<Type, fvPatchField, volMesh>::
                    GeometricBoundaryField& values
            );

            //- Construct and return the solver
            //  Use the given solver controls
            autoPtr<fvSolver> solver(const dictionary&);

            //- Construct and return the solver
            //  Solver controls read from fvSolution
            autoPtr<fvSolver> solver();

            //- Solve returning the solution statistics.
            //  Use the given solver controls
            solverPerformance solve(const dictionary&);

            //- Solve returning the solution statistics.
            //  Solver controls read from fvSolution
            solverPerformance solve();

            //- Return the matrix residual
            tmp<Field<Type> > residual() const;

            //- Return the matrix scalar diagonal
            tmp<scalarField> D() const;

            //- Return the matrix Type diagonal
            tmp<Field<Type> > DD() const;

            //- Return the central coefficient
            tmp<volScalarField> A() const;

            //- Return the H operation source
            tmp<GeometricField<Type, fvPatchField, volMesh> > H() const;

            //- Return H(1)
            tmp<volScalarField> H1() const;

            //- Return the face-flux field from the matrix
            tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
                flux() const;

            // Diagonal based on row sum
            tmp<volScalarField> Ac() const;

            // Diagonal based on SPAI(0)
            tmp<volScalarField> spai0() const;

            // Residual for vector equations
            tmp<GeometricField<Type, fvPatchField, volMesh> > R() const;


    // Member operators

        void operator=(const fvMatrix<Type>&);
        void operator=(const tmp<fvMatrix<Type> >&);

        void negate();

        void operator+=(const fvMatrix<Type>&);
        void operator+=(const tmp<fvMatrix<Type> >&);

        void operator-=(const fvMatrix<Type>&);
        void operator-=(const tmp<fvMatrix<Type> >&);

        void operator+=
        (
            const DimensionedField<Type, volMesh>&
        );
        void operator+=
        (
            const tmp<DimensionedField<Type, volMesh> >&
        );
        void operator+=
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        );

        void operator-=
        (
            const DimensionedField<Type, volMesh>&
        );
        void operator-=
        (
            const tmp<DimensionedField<Type, volMesh> >&
        );
        void operator-=
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        );

        void operator+=(const dimensioned<Type>&);
        void operator-=(const dimensioned<Type>&);

        void operator+=(const zero&);
        void operator-=(const zero&);

        void operator*=(const DimensionedField<scalar, volMesh>&);
        void operator*=(const tmp<DimensionedField<scalar, volMesh> >&);
        void operator*=(const tmp<volScalarField>&);

        void operator*=(const dimensioned<scalar>&);


    // Friend operators

        friend tmp<GeometricField<Type, fvPatchField, volMesh> >
        operator& <Type>
        (
            const fvMatrix<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        friend tmp<GeometricField<Type, fvPatchField, volMesh> >
        operator& <Type>
        (
            const fvMatrix<Type>&,
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        );

        friend tmp<GeometricField<Type, fvPatchField, volMesh> >
        operator& <Type>
        (
            const tmp<fvMatrix<Type> >&,
            const DimensionedField<Type, volMesh>&
        );

        friend tmp<GeometricField<Type, fvPatchField, volMesh> >
        operator& <Type>
        (
            const tmp<fvMatrix<Type> >&,
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        );


    // Ostream operator

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const fvMatrix<Type>&
        );
};


// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type>
void checkMethod
(
    const fvMatrix<Type>&,
    const fvMatrix<Type>&,
    const char*
);

template<class Type>
void checkMethod
(
    const fvMatrix<Type>&,
    const DimensionedField<Type, volMesh>&,
    const char*
);

template<class Type>
void checkMethod
(
    const fvMatrix<Type>&,
    const dimensioned<Type>&,
    const char*
);


//- Solve returning the solution statistics given convergence tolerance
//  Use the given solver controls
template<class Type>
solverPerformance solve(fvMatrix<Type>&, const dictionary&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Use the given solver controls
template<class Type>
solverPerformance solve
(
    const tmp<fvMatrix<Type> >&,
    const dictionary&
);


//- Solve returning the solution statistics given convergence tolerance
//  Solver controls read fvSolution
template<class Type>
solverPerformance solve(fvMatrix<Type>&);


//- Solve returning the solution statistics given convergence tolerance,
//  deleting temporary matrix after solution.
//  Solver controls read fvSolution
template<class Type>
solverPerformance solve(const tmp<fvMatrix<Type> >&);


//- Return the correction form of the given matrix
//  by subtracting the matrix multiplied by the current field
template<class Type>
tmp<fvMatrix<Type> > correction(const fvMatrix<Type>&);


//- Return the correction form of the given temporary matrix
//  by subtracting the matrix multiplied by the current field
template<class Type>
tmp<fvMatrix<Type> > correction(const tmp<fvMatrix<Type> >&);


// * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const dimensioned<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const dimensioned<Type>&
);


template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const fvMatrix<Type>&,
    const zero&
);

template<class Type>
tmp<fvMatrix<Type> > operator==
(
    const tmp<fvMatrix<Type> >&,
    const zero&
);


template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const DimensionedField<Type, volMesh>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<DimensionedField<Type, volMesh> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const DimensionedField<Type, volMesh>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<DimensionedField<Type, volMesh> >&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const fvMatrix<Type>&,
    const dimensioned<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const tmp<fvMatrix<Type> >&,
    const dimensioned<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const dimensioned<Type>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator+
(
    const dimensioned<Type>&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const DimensionedField<Type, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const tmp<DimensionedField<Type, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const DimensionedField<Type, volMesh>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<DimensionedField<Type, volMesh> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const DimensionedField<Type, volMesh>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<DimensionedField<Type, volMesh> >&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const fvMatrix<Type>&,
    const dimensioned<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const tmp<fvMatrix<Type> >&,
    const dimensioned<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const dimensioned<Type>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator-
(
    const dimensioned<Type>&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const DimensionedField<scalar, volMesh>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const tmp<DimensionedField<scalar, volMesh> >&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const tmp<volScalarField>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const DimensionedField<scalar, volMesh>&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const tmp<DimensionedField<scalar, volMesh> >&,
    const tmp<fvMatrix<Type> >&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const tmp<volScalarField>&,
    const tmp<fvMatrix<Type> >&
);


template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const dimensioned<scalar>&,
    const fvMatrix<Type>&
);

template<class Type>
tmp<fvMatrix<Type> > operator*
(
    const dimensioned<scalar>&,
    const tmp<fvMatrix<Type> >&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "calculatedFvPatchFields.hpp"
#include "extrapolatedCalculatedFvPatchFields.hpp"
#include "coupledFvPatchFields.hpp"
#include "UIndirectList.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
template<class Type2>
void CML::fvMatrix<Type>::addToInternalField
(
    const labelUList& addr,
    const Field<Type2>& pf,
    Field<Type2>& intf
) const
{
    if (addr.size() != pf.size())
    {
        FatalErrorInFunction
            << "sizes of addressing and field are different"
            << abort(FatalError);
    }

    forAll(addr, faceI)
    {
        intf[addr[faceI]] += pf[faceI];
    }
}


template<class Type>
template<class Type2>
void CML::fvMatrix<Type>::addToInternalField
(
    const labelUList& addr,
    const tmp<Field<Type2> >& tpf,
    Field<Type2>& intf
) const
{
    addToInternalField(addr, tpf(), intf);
    tpf.clear();
}


template<class Type>
template<class Type2>
void CML::fvMatrix<Type>::subtractFromInternalField
(
    const labelUList& addr,
    const Field<Type2>& pf,
    Field<Type2>& intf
) const
{
    if (addr.size() != pf.size())
    {
        FatalErrorInFunction
            << "sizes of addressing and field are different"
            << abort(FatalError);
    }

    forAll(addr, faceI)
    {
        intf[addr[faceI]] -= pf[faceI];
    }
}


template<class Type>
template<class Type2>
void CML::fvMatrix<Type>::subtractFromInternalField
(
    const labelUList& addr,
    const tmp<Field<Type2> >& tpf,
    Field<Type2>& intf
) const
{
    subtractFromInternalField(addr, tpf(), intf);
    tpf.clear();
}


template<class Type>
void CML::fvMatrix<Type>::addBoundaryDiag
(
    scalarField& diag,
    const direction solveCmpt
) const
{
    forAll(internalCoeffs_, patchI)
    {
        addToInternalField
        (
            lduAddr().patchAddr(patchI),
            internalCoeffs_[patchI].component(solveCmpt),
            diag
        );
    }
}


template<class Type>
void CML::fvMatrix<Type>::addCmptAvBoundaryDiag(scalarField& diag) const
{
    forAll(internalCoeffs_, patchI)
    {
        addToInternalField
        (
            lduAddr().patchAddr(patchI),
            cmptAv(internalCoeffs_[patchI]),
            diag
        );
    }
}


template<class Type>
void CML::fvMatrix<Type>::addBoundarySource
(
    Field<Type>& source,
    const bool couples
) const
{
    forAll(psi_.boundaryField(), patchI)
    {
        const fvPatchField<Type>& ptf = psi_.boundaryField()[patchI];
        const Field<Type>& pbc = boundaryCoeffs_[patchI];

        if (!ptf.coupled())
        {
            addToInternalField(lduAddr().patchAddr(patchI), pbc, source);
        }
        else if (couples)
        {
            tmp<Field<Type> > tpnf = ptf.patchNeighbourField();
            const Field<Type>& pnf = tpnf();

            const labelUList& addr = lduAddr().patchAddr(patchI);

            forAll(addr, facei)
            {
                source[addr[facei]] += cmptMultiply(pbc[facei], pnf[facei]);
            }
        }
    }
}


template<class Type>
template<template<class> class ListType>
void CML::fvMatrix<Type>::setValuesFromList
(
    const labelUList& cellLabels,
    const ListType<Type>& values
)
{
    const fvMesh& mesh = psi_.mesh();

    const cellList& cells = mesh.cells();
    const labelUList& own = mesh.owner();
    const labelUList& nei = mesh.neighbour();

    scalarField& Diag = diag();
    Field<Type>& psi =
        const_cast
        <
            GeometricField<Type, fvPatchField, volMesh>&
        >(psi_).internalField();

    forAll(cellLabels, i)
    {
        const label celli = cellLabels[i];
        const Type& value = values[i];

        psi[celli] = value;
        source_[celli] = value*Diag[celli];

        if (symmetric() || asymmetric())
        {
            const cell& c = cells[celli];

            forAll(c, j)
            {
                const label facei = c[j];

                if (mesh.isInternalFace(facei))
                {
                    if (symmetric())
                    {
                        if (celli == own[facei])
                        {
                            source_[nei[facei]] -= upper()[facei]*value;
                        }
                        else
                        {
                            source_[own[facei]] -= upper()[facei]*value;
                        }

                        upper()[facei] = 0.0;
                    }
                    else
                    {
                        if (celli == own[facei])
                        {
                            source_[nei[facei]] -= lower()[facei]*value;
                        }
                        else
                        {
                            source_[own[facei]] -= upper()[facei]*value;
                        }

                        upper()[facei] = 0.0;
                        lower()[facei] = 0.0;
                    }
                }
                else
                {
                    label patchi = mesh.boundaryMesh().whichPatch(facei);

                    if (internalCoeffs_[patchi].size())
                    {
                        label patchFacei =
                            mesh.boundaryMesh()[patchi].whichFace(facei);

                        internalCoeffs_[patchi][patchFacei] =
                            pTraits<Type>::zero;

                        boundaryCoeffs_[patchi][patchFacei] =
                            pTraits<Type>::zero;
                    }
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fvMatrix<Type>::fvMatrix
(
    const GeometricField<Type, fvPatchField, volMesh>& psi,
    const dimensionSet& ds
)
:
    lduMatrix(psi.mesh()),
    psi_(psi),
    dimensions_(ds),
    source_(psi.size(), pTraits<Type>::zero),
    internalCoeffs_(psi.mesh().boundary().size()),
    boundaryCoeffs_(psi.mesh().boundary().size()),
    faceFluxCorrectionPtr_(nullptr)
{
    if (debug)
    {
        Info<< "fvMatrix<Type>(GeometricField<Type, fvPatchField, volMesh>&,"
               " const dimensionSet&) : "
               "constructing fvMatrix<Type> for field " << psi_.name()
            << endl;
    }

    // Initialise coupling coefficients
    forAll(psi.mesh().boundary(), patchI)
    {
        internalCoeffs_.set
        (
            patchI,
            new Field<Type>
            (
                psi.mesh().boundary()[patchI].size(),
                pTraits<Type>::zero
            )
        );

        boundaryCoeffs_.set
        (
            patchI,
            new Field<Type>
            (
                psi.mesh().boundary()[patchI].size(),
                pTraits<Type>::zero
            )
        );
    }

    // Update the boundary coefficients of psi without changing its event No.
    GeometricField<Type, fvPatchField, volMesh>& psiRef =
       const_cast<GeometricField<Type, fvPatchField, volMesh>&>(psi_);

    label currentStatePsi = psiRef.eventNo();
    psiRef.boundaryField().updateCoeffs();
    psiRef.eventNo() = currentStatePsi;
}


template<class Type>
CML::fvMatrix<Type>::fvMatrix(const fvMatrix<Type>& fvm)
:
    refCount(),
    lduMatrix(fvm),
    psi_(fvm.psi_),
    dimensions_(fvm.dimensions_),
    source_(fvm.source_),
    internalCoeffs_(fvm.internalCoeffs_),
    boundaryCoeffs_(fvm.boundaryCoeffs_),
    faceFluxCorrectionPtr_(nullptr)
{
    if (debug)
    {
        Info<< "fvMatrix<Type>::fvMatrix(const fvMatrix<Type>&) : "
            << "copying fvMatrix<Type> for field " << psi_.name()
            << endl;
    }

    if (fvm.faceFluxCorrectionPtr_)
    {
        faceFluxCorrectionPtr_ = new
        GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            *(fvm.faceFluxCorrectionPtr_)
        );
    }
}


#ifdef ConstructFromTmp
template<class Type>
CML::fvMatrix<Type>::fvMatrix(const tmp<fvMatrix<Type> >& tfvm)
:
    refCount(),
    lduMatrix
    (
        const_cast<fvMatrix<Type>&>(tfvm()),
        tfvm.isTmp()
    ),
    psi_(tfvm().psi_),
    dimensions_(tfvm().dimensions_),
    source_
    (
        const_cast<fvMatrix<Type>&>(tfvm()).source_,
        tfvm.isTmp()
    ),
    internalCoeffs_
    (
        const_cast<fvMatrix<Type>&>(tfvm()).internalCoeffs_,
        tfvm.isTmp()
    ),
    boundaryCoeffs_
    (
        const_cast<fvMatrix<Type>&>(tfvm()).boundaryCoeffs_,
        tfvm.isTmp()
    ),
    faceFluxCorrectionPtr_(nullptr)
{
    if (debug)
    {
        Info<< "fvMatrix<Type>::fvMatrix(const tmp<fvMatrix<Type> >&) : "
            << "copying fvMatrix<Type> for field " << psi_.name()
            << endl;
    }

    if (tfvm().faceFluxCorrectionPtr_)
    {
        if (tfvm.isTmp())
        {
            faceFluxCorrectionPtr_ = tfvm().faceFluxCorrectionPtr_;
            tfvm().faceFluxCorrectionPtr_ = nullptr;
        }
        else
        {
            faceFluxCorrectionPtr_ = new
                GeometricField<Type, fvsPatchField, surfaceMesh>
                (
                    *(tfvm().faceFluxCorrectionPtr_)
                );
        }
    }

    tfvm.clear();
}
#endif


template<class Type>
CML::fvMatrix<Type>::fvMatrix
(
    const GeometricField<Type, fvPatchField, volMesh>& psi,
    Istream& is
)
:
    lduMatrix(psi.mesh()),
    psi_(psi),
    dimensions_(is),
    source_(is),
    internalCoeffs_(psi.mesh().boundary().size()),
    boundaryCoeffs_(psi.mesh().boundary().size()),
    faceFluxCorrectionPtr_(nullptr)
{
    if (debug)
    {
        Info<< "fvMatrix<Type>"
               "(GeometricField<Type, fvPatchField, volMesh>&, Istream&) : "
               "constructing fvMatrix<Type> for field " << psi_.name()
            << endl;
    }

    // Initialise coupling coefficients
    forAll(psi.mesh().boundary(), patchI)
    {
        internalCoeffs_.set
        (
            patchI,
            new Field<Type>
            (
                psi.mesh().boundary()[patchI].size(),
                pTraits<Type>::zero
            )
        );

        boundaryCoeffs_.set
        (
            patchI,
            new Field<Type>
            (
                psi.mesh().boundary()[patchI].size(),
                pTraits<Type>::zero
            )
        );
    }

}


template<class Type>
CML::fvMatrix<Type>::~fvMatrix()
{
    if (debug)
    {
        Info<< "fvMatrix<Type>::~fvMatrix<Type>() : "
            << "destroying fvMatrix<Type> for field " << psi_.name()
            << endl;
    }

    if (faceFluxCorrectionPtr_)
    {
        delete faceFluxCorrectionPtr_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fvMatrix<Type>::setValues
(
    const labelUList& cellLabels,
    const UList<Type>& values
)
{
    this->setValuesFromList(cellLabels, values);
}


template<class Type>
void CML::fvMatrix<Type>::setValues
(
    const labelUList& cellLabels,
    const UIndirectList<Type>& values
)
{
    this->setValuesFromList(cellLabels, values);
}


template<class Type>
void CML::fvMatrix<Type>::setReference
(
    const label celli,
    const Type& value,
    const bool forceReference
)
{
    if ((forceReference || psi_.needReference()) && celli >= 0)
    {
        source()[celli] += diag()[celli]*value;
        diag()[celli] += diag()[celli];
    }
}


template<class Type>
void CML::fvMatrix<Type>::relax(const scalar alpha)
{
    if (alpha <= 0)
    {
        return;
    }

    if (debug)
    {
        InfoInFunction
            << "Relaxing " << psi_.name() << " by " << alpha
            << endl;
    }

    Field<Type>& S = source();
    scalarField& D = diag();

    // Store the current unrelaxed diagonal for use in updating the source
    scalarField D0(D);

    // Calculate the sum-mag off-diagonal from the interior faces
    scalarField sumOff(D.size(), 0.0);
    sumMagOffDiag(sumOff);

    // Handle the boundary contributions to the diagonal
    forAll(psi_.boundaryField(), patchI)
    {
        const fvPatchField<Type>& ptf = psi_.boundaryField()[patchI];

        if (ptf.size())
        {
            const labelUList& pa = lduAddr().patchAddr(patchI);
            Field<Type>& iCoeffs = internalCoeffs_[patchI];

            if (ptf.coupled())
            {
                const Field<Type>& pCoeffs = boundaryCoeffs_[patchI];

                // For coupled boundaries add the diagonal and
                // off-diagonal contributions
                forAll(pa, face)
                {
                    D[pa[face]] += component(iCoeffs[face], 0);
                    sumOff[pa[face]] += mag(component(pCoeffs[face], 0));
                }
            }
            else
            {
                // For non-coupled boundaries
                // add the difference between maximised boundary diagonal
                // contribution and the boundary diagonal contribution
                // to both the diagonal and the sum of the off-diagonal fields
                // to ensure the relaxation operates on the appropriately
                // normalised diagonal
                forAll(pa, face)
                {
                    D[pa[face]] += cmptAv(iCoeffs[face]);
                }
            }
        }
    }


    if (debug)
    {
        // Calculate amount of non-dominance.
        label nNon = 0;
        scalar maxNon = 0.0;
        scalar sumNon = 0.0;
        forAll(D, celli)
        {
            scalar d = (sumOff[celli] - D[celli])/mag(D[celli]);

            if (d > 0)
            {
                nNon++;
                maxNon = max(maxNon, d);
                sumNon += d;
            }
        }

        reduce(nNon, sumOp<label>());
        reduce(maxNon, maxOp<scalar>());
        reduce(sumNon, sumOp<scalar>());
        sumNon /= returnReduce(D.size(), sumOp<label>());

        InfoInFunction
            << "Matrix dominance test for " << psi_.name() << nl
            << "    number of non-dominant cells   : " << nNon << nl
            << "    maximum relative non-dominance : " << maxNon << nl
            << "    average relative non-dominance : " << sumNon << nl
            << endl;
    }


    // Ensure the matrix is diagonally dominant...
    // Assumes that the central coefficient is positive and ensures it is
    forAll(D, celli)
    {
        D[celli] = max(mag(D[celli]), sumOff[celli]);
    }

    // ... then relax
    D /= alpha;

    // Now remove the diagonal contribution from coupled boundaries
    forAll(psi_.boundaryField(), patchI)
    {
        const fvPatchField<Type>& ptf = psi_.boundaryField()[patchI];

        if (ptf.size())
        {
            const labelUList& pa = lduAddr().patchAddr(patchI);
            Field<Type>& iCoeffs = internalCoeffs_[patchI];

            if (ptf.coupled())
            {
                forAll(pa, face)
                {
                    D[pa[face]] -= component(iCoeffs[face], 0);
                }
            }
            else
            {
                forAll(pa, face)
                {
                    D[pa[face]] -= cmptAv(iCoeffs[face]);
                }
            }
        }
    }

    // Finally add the relaxation contribution to the source.
    S += (D - D0)*psi_.internalField();
}


template<class Type>
void CML::fvMatrix<Type>::relax()
{
    word name = psi_.select
    (
        psi_.mesh().data::template lookupOrDefault<bool>
        ("finalIteration", false)
    );

    if (psi_.mesh().relaxEquation(name))
    {
        relax(psi_.mesh().equationRelaxationFactor(name));
    }
}


template<class Type>
void CML::fvMatrix<Type>::boundaryManipulate
(
    typename GeometricField<Type, fvPatchField, volMesh>::
        GeometricBoundaryField& bFields
)
{
    forAll(bFields, patchI)
    {
        bFields[patchI].manipulateMatrix(*this);
    }
}


template<class Type>
CML::tmp<CML::scalarField> CML::fvMatrix<Type>::D() const
{
    tmp<scalarField> tdiag(new scalarField(diag()));
    addCmptAvBoundaryDiag(tdiag());
    return tdiag;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fvMatrix<Type>::DD() const
{
    tmp<Field<Type> > tdiag(pTraits<Type>::one*diag());

    forAll(psi_.boundaryField(), patchI)
    {
        const fvPatchField<Type>& ptf = psi_.boundaryField()[patchI];

        if (!ptf.coupled() && ptf.size())
        {
            addToInternalField
            (
                lduAddr().patchAddr(patchI),
                internalCoeffs_[patchI],
                tdiag()
            );
        }
    }

    return tdiag;
}


template<class Type>
CML::tmp<CML::volScalarField> CML::fvMatrix<Type>::A() const
{
    tmp<volScalarField> tAphi
    (
        new volScalarField
        (
            IOobject
            (
                "A("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/psi_.dimensions()/dimVol,
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    tAphi().internalField() = D()/psi_.mesh().V();
    tAphi().correctBoundaryConditions();

    return tAphi;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::fvMatrix<Type>::H() const
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tHphi
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "H("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/dimVol,
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );
    GeometricField<Type, fvPatchField, volMesh>& Hphi = tHphi();

    // Loop over field components
    for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
    {
        scalarField psiCmpt(psi_.internalField().component(cmpt));

        scalarField boundaryDiagCmpt(psi_.size(), 0.0);
        addBoundaryDiag(boundaryDiagCmpt, cmpt);
        boundaryDiagCmpt.negate();
        addCmptAvBoundaryDiag(boundaryDiagCmpt);

        Hphi.internalField().replace(cmpt, boundaryDiagCmpt*psiCmpt);
    }

    Hphi.internalField() += lduMatrix::H(psi_.internalField()) + source_;
    addBoundarySource(Hphi.internalField());

    Hphi.internalField() /= psi_.mesh().V();
    Hphi.correctBoundaryConditions();

    typename Type::labelType validComponents
    (
        pow
        (
            psi_.mesh().solutionD(),
            pTraits<typename powProduct<Vector<label>, Type::rank>::type>::zero
        )
    );

    for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
    {
        if (validComponents[cmpt] == -1)
        {
            Hphi.replace
            (
                cmpt,
                dimensionedScalar("0", Hphi.dimensions(), 0.0)
            );
        }
    }

    return tHphi;
}


template<class Type>
CML::tmp<CML::volScalarField> CML::fvMatrix<Type>::H1() const
{
    tmp<volScalarField> tH1
    (
        new volScalarField
        (
            IOobject
            (
                "H(1)",
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/(dimVol*psi_.dimensions()),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );
    volScalarField& H1_ = tH1();

    H1_.internalField() = lduMatrix::H1();

    forAll(psi_.boundaryField(), patchI)
    {
        const fvPatchField<Type>& ptf = psi_.boundaryField()[patchI];

        if (ptf.coupled() && ptf.size())
        {
            addToInternalField
            (
                lduAddr().patchAddr(patchI),
                boundaryCoeffs_[patchI].component(0),
                H1_
            );
        }
    }

    H1_.internalField() /= psi_.mesh().V();
    H1_.correctBoundaryConditions();

    return tH1;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::fvMatrix<Type>::
flux() const
{
    if (!psi_.mesh().fluxRequired(psi_.name()))
    {
        FatalErrorInFunction
            << "flux requested but " << psi_.name()
            << " not specified in the fluxRequired sub-dictionary"
               " of fvSchemes."
            << abort(FatalError);
    }

    // construct GeometricField<Type, fvsPatchField, surfaceMesh>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tfieldFlux
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "flux("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions()
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& fieldFlux = tfieldFlux();

    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
    {
        fieldFlux.internalField().replace
        (
            cmpt,
            lduMatrix::faceH(psi_.internalField().component(cmpt))
        );
    }

    FieldField<Field, Type> InternalContrib = internalCoeffs_;

    forAll(InternalContrib, patchI)
    {
        InternalContrib[patchI] =
            cmptMultiply
            (
                InternalContrib[patchI],
                psi_.boundaryField()[patchI].patchInternalField()
            );
    }

    FieldField<Field, Type> NeighbourContrib = boundaryCoeffs_;

    forAll(NeighbourContrib, patchI)
    {
        if (psi_.boundaryField()[patchI].coupled())
        {
            NeighbourContrib[patchI] =
                cmptMultiply
                (
                    NeighbourContrib[patchI],
                    psi_.boundaryField()[patchI].patchNeighbourField()
                );
        }
    }

    forAll(fieldFlux.boundaryField(), patchI)
    {
        fieldFlux.boundaryField()[patchI] =
            InternalContrib[patchI] - NeighbourContrib[patchI];
    }

    if (faceFluxCorrectionPtr_)
    {
        fieldFlux += *faceFluxCorrectionPtr_;
    }

    return tfieldFlux;
}

template<class Type> CML::tmp<CML::volScalarField> 
CML::fvMatrix<Type>::Ac() const
{
    tmp<volScalarField> tAphi
    (
        new volScalarField
        (
            IOobject
            (
                "Ac("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/(psi_.dimensions()*dimVol),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    register const label nCells = psi_.mesh().V().size();
    scalarField s(nCells);
    lduMatrix::rowSum(s);
    tAphi().internalField() = s/psi_.mesh().V();
    tAphi().correctBoundaryConditions();

    return tAphi;
}

template<class Type> CML::tmp<CML::volScalarField> 
CML::fvMatrix<Type>::spai0() const
{
    tmp<volScalarField> tAphi
    (
        new volScalarField
        (
            IOobject
            (
                "spai0("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/(psi_.dimensions()*dimVol),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    register const label nCells = psi_.mesh().V().size();
    scalarField s(nCells);
    lduMatrix::spai0(s);
    tAphi().internalField() = s/psi_.mesh().V();
    tAphi().correctBoundaryConditions();

    return tAphi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::fvMatrix<Type>::R() const
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tRphi
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "R("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/dimVol,
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    GeometricField<Type, fvPatchField, volMesh>& Rphi = tRphi();

    Rphi.internalField() = this->residual();
    Rphi.correctBoundaryConditions();

    return tRphi;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::fvMatrix<Type>::operator=(const fvMatrix<Type>& fvmv)
{
    if (this == &fvmv)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (&psi_ != &(fvmv.psi_))
    {
        FatalErrorInFunction
            << "different fields"
            << abort(FatalError);
    }

    dimensions_ = fvmv.dimensions_;
    lduMatrix::operator=(fvmv);
    source_ = fvmv.source_;
    internalCoeffs_ = fvmv.internalCoeffs_;
    boundaryCoeffs_ = fvmv.boundaryCoeffs_;

    if (faceFluxCorrectionPtr_ && fvmv.faceFluxCorrectionPtr_)
    {
        *faceFluxCorrectionPtr_ = *fvmv.faceFluxCorrectionPtr_;
    }
    else if (fvmv.faceFluxCorrectionPtr_)
    {
        faceFluxCorrectionPtr_ =
            new GeometricField<Type, fvsPatchField, surfaceMesh>
        (*fvmv.faceFluxCorrectionPtr_);
    }
}


template<class Type>
void CML::fvMatrix<Type>::operator=(const tmp<fvMatrix<Type> >& tfvmv)
{
    operator=(tfvmv());
    tfvmv.clear();
}


template<class Type>
void CML::fvMatrix<Type>::negate()
{
    lduMatrix::negate();
    source_.negate();
    internalCoeffs_.negate();
    boundaryCoeffs_.negate();

    if (faceFluxCorrectionPtr_)
    {
        faceFluxCorrectionPtr_->negate();
    }
}


template<class Type>
void CML::fvMatrix<Type>::operator+=(const fvMatrix<Type>& fvmv)
{
    checkMethod(*this, fvmv, "+=");

    dimensions_ += fvmv.dimensions_;
    lduMatrix::operator+=(fvmv);
    source_ += fvmv.source_;
    internalCoeffs_ += fvmv.internalCoeffs_;
    boundaryCoeffs_ += fvmv.boundaryCoeffs_;

    if (faceFluxCorrectionPtr_ && fvmv.faceFluxCorrectionPtr_)
    {
        *faceFluxCorrectionPtr_ += *fvmv.faceFluxCorrectionPtr_;
    }
    else if (fvmv.faceFluxCorrectionPtr_)
    {
        faceFluxCorrectionPtr_ = new
        GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            *fvmv.faceFluxCorrectionPtr_
        );
    }
}


template<class Type>
void CML::fvMatrix<Type>::operator+=(const tmp<fvMatrix<Type> >& tfvmv)
{
    operator+=(tfvmv());
    tfvmv.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator-=(const fvMatrix<Type>& fvmv)
{
    checkMethod(*this, fvmv, "-=");

    dimensions_ -= fvmv.dimensions_;
    lduMatrix::operator-=(fvmv);
    source_ -= fvmv.source_;
    internalCoeffs_ -= fvmv.internalCoeffs_;
    boundaryCoeffs_ -= fvmv.boundaryCoeffs_;

    if (faceFluxCorrectionPtr_ && fvmv.faceFluxCorrectionPtr_)
    {
        *faceFluxCorrectionPtr_ -= *fvmv.faceFluxCorrectionPtr_;
    }
    else if (fvmv.faceFluxCorrectionPtr_)
    {
        faceFluxCorrectionPtr_ =
            new GeometricField<Type, fvsPatchField, surfaceMesh>
        (-*fvmv.faceFluxCorrectionPtr_);
    }
}


template<class Type>
void CML::fvMatrix<Type>::operator-=(const tmp<fvMatrix<Type> >& tfvmv)
{
    operator-=(tfvmv());
    tfvmv.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator+=
(
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(*this, su, "+=");
    source() -= su.mesh().V()*su.field();
}


template<class Type>
void CML::fvMatrix<Type>::operator+=
(
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    operator+=(tsu());
    tsu.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator+=
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    operator+=(tsu());
    tsu.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator-=
(
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(*this, su, "-=");
    source() += su.mesh().V()*su.field();
}


template<class Type>
void CML::fvMatrix<Type>::operator-=
(
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    operator-=(tsu());
    tsu.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator-=
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    operator-=(tsu());
    tsu.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator+=
(
    const dimensioned<Type>& su
)
{
    source() -= psi().mesh().V()*su;
}


template<class Type>
void CML::fvMatrix<Type>::operator-=
(
    const dimensioned<Type>& su
)
{
    source() += psi().mesh().V()*su;
}


template<class Type>
void CML::fvMatrix<Type>::operator+=
(
    const zero&
)
{}


template<class Type>
void CML::fvMatrix<Type>::operator-=
(
    const zero&
)
{}


template<class Type>
void CML::fvMatrix<Type>::operator*=
(
    const DimensionedField<scalar, volMesh>& dsf
)
{
    dimensions_ *= dsf.dimensions();
    lduMatrix::operator*=(dsf.field());
    source_ *= dsf.field();

    forAll(boundaryCoeffs_, patchI)
    {
        scalarField pisf
        (
            dsf.mesh().boundary()[patchI].patchInternalField(dsf.field())
        );

        internalCoeffs_[patchI] *= pisf;
        boundaryCoeffs_[patchI] *= pisf;
    }

    if (faceFluxCorrectionPtr_)
    {
        FatalErrorInFunction
            << "cannot scale a matrix containing a faceFluxCorrection"
            << abort(FatalError);
    }
}


template<class Type>
void CML::fvMatrix<Type>::operator*=
(
    const tmp<DimensionedField<scalar, volMesh> >& tdsf
)
{
    operator*=(tdsf());
    tdsf.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator*=
(
    const tmp<volScalarField>& tvsf
)
{
    operator*=(tvsf());
    tvsf.clear();
}


template<class Type>
void CML::fvMatrix<Type>::operator*=
(
    const dimensioned<scalar>& ds
)
{
    dimensions_ *= ds.dimensions();
    lduMatrix::operator*=(ds.value());
    source_ *= ds.value();
    internalCoeffs_ *= ds.value();
    boundaryCoeffs_ *= ds.value();

    if (faceFluxCorrectionPtr_)
    {
        *faceFluxCorrectionPtr_ *= ds.value();
    }
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::checkMethod
(
    const fvMatrix<Type>& fvm1,
    const fvMatrix<Type>& fvm2,
    const char* op
)
{
    if (&fvm1.psi() != &fvm2.psi())
    {
        FatalErrorInFunction
            << "incompatible fields for operation "
            << endl << "    "
            << "[" << fvm1.psi().name() << "] "
            << op
            << " [" << fvm2.psi().name() << "]"
            << abort(FatalError);
    }

    if (dimensionSet::debug && fvm1.dimensions() != fvm2.dimensions())
    {
        FatalErrorInFunction
            << "incompatible dimensions for operation "
            << endl << "    "
            << "[" << fvm1.psi().name() << fvm1.dimensions()/dimVolume << " ] "
            << op
            << " [" << fvm2.psi().name() << fvm2.dimensions()/dimVolume << " ]"
            << abort(FatalError);
    }
}


template<class Type>
void CML::checkMethod
(
    const fvMatrix<Type>& fvm,
    const DimensionedField<Type, volMesh>& df,
    const char* op
)
{
    if (dimensionSet::debug && fvm.dimensions()/dimVolume != df.dimensions())
    {
        FatalErrorInFunction
            <<  "incompatible dimensions for operation "
            << endl << "    "
            << "[" << fvm.psi().name() << fvm.dimensions()/dimVolume << " ] "
            << op
            << " [" << df.name() << df.dimensions() << " ]"
            << abort(FatalError);
    }
}


template<class Type>
void CML::checkMethod
(
    const fvMatrix<Type>& fvm,
    const dimensioned<Type>& dt,
    const char* op
)
{
    if (dimensionSet::debug && fvm.dimensions()/dimVolume != dt.dimensions())
    {
        FatalErrorInFunction
            << "incompatible dimensions for operation "
            << endl << "    "
            << "[" << fvm.psi().name() << fvm.dimensions()/dimVolume << " ] "
            << op
            << " [" << dt.name() << dt.dimensions() << " ]"
            << abort(FatalError);
    }
}


template<class Type>
CML::solverPerformance CML::solve
(
    fvMatrix<Type>& fvm,
    const dictionary& solverControls
)
{
    return fvm.solve(solverControls);
}

template<class Type>
CML::solverPerformance CML::solve
(
    const tmp<fvMatrix<Type> >& tfvm,
    const dictionary& solverControls
)
{
    solverPerformance solverPerf =
        const_cast<fvMatrix<Type>&>(tfvm()).solve(solverControls);

    tfvm.clear();

    return solverPerf;
}


template<class Type>
CML::solverPerformance CML::solve(fvMatrix<Type>& fvm)
{
    return fvm.solve();
}

template<class Type>
CML::solverPerformance CML::solve(const tmp<fvMatrix<Type> >& tfvm)
{
    solverPerformance solverPerf =
        const_cast<fvMatrix<Type>&>(tfvm()).solve();

    tfvm.clear();

    return solverPerf;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::correction
(
    const fvMatrix<Type>& A
)
{
    tmp<CML::fvMatrix<Type> > tAcorr = A - (A & A.psi());

    if
    (
        (A.hasUpper() || A.hasLower())
     && A.psi().mesh().fluxRequired(A.psi().name())
    )
    {
        tAcorr().faceFluxCorrectionPtr() = (-A.flux()).ptr();
    }

    return tAcorr;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::correction
(
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<CML::fvMatrix<Type> > tAcorr = tA - (tA() & tA().psi());

    // Note the matrix coefficients are still that of matrix A
    const fvMatrix<Type>& A = tAcorr();

    if
    (
        (A.hasUpper() || A.hasLower())
     && A.psi().mesh().fluxRequired(A.psi().name())
    )
    {
        tAcorr().faceFluxCorrectionPtr() = (-A.flux()).ptr();
    }

    return tAcorr;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const fvMatrix<Type>& B
)
{
    checkMethod(A, B, "==");
    return (A - B);
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const fvMatrix<Type>& B
)
{
    checkMethod(tA(), B, "==");
    return (tA - B);
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "==");
    return (A - tB);
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "==");
    return (tA - tB);
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(A, su, "==");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "==");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "==");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(tA(), su, "==");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "==");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "==");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const dimensioned<Type>& su
)
{
    checkMethod(A, su, "==");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += A.psi().mesh().V()*su.value();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const dimensioned<Type>& su
)
{
    checkMethod(tA(), su, "==");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += tC().psi().mesh().V()*su.value();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const fvMatrix<Type>& A,
    const zero&
)
{
    return A;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator==
(
    const tmp<fvMatrix<Type> >& tA,
    const zero&
)
{
    return tA;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A
)
{
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().negate();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().negate();
    return tC;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const fvMatrix<Type>& B
)
{
    checkMethod(A, B, "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() += B;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const fvMatrix<Type>& B
)
{
    checkMethod(tA(), B, "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() += B;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "+");
    tmp<fvMatrix<Type> > tC(tB.ptr());
    tC() += A;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() += tB();
    tB.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(A, su, "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(tA(), su, "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const DimensionedField<Type, volMesh>& su,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, su, "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<DimensionedField<Type, volMesh> >& tsu,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, tsu(), "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, tsu(), "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const DimensionedField<Type, volMesh>& su,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), su, "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<DimensionedField<Type, volMesh> >& tsu,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), tsu(), "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), tsu(), "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const fvMatrix<Type>& B
)
{
    checkMethod(A, B, "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() -= B;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const fvMatrix<Type>& B
)
{
    checkMethod(tA(), B, "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() -= B;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(A, tB(), "-");
    tmp<fvMatrix<Type> > tC(tB.ptr());
    tC() -= A;
    tC().negate();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<fvMatrix<Type> >& tB
)
{
    checkMethod(tA(), tB(), "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() -= tB();
    tB.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(A, su, "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(A, tsu(), "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const DimensionedField<Type, volMesh>& su
)
{
    checkMethod(tA(), su, "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<DimensionedField<Type, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu
)
{
    checkMethod(tA(), tsu(), "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const DimensionedField<Type, volMesh>& su,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, su, "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().negate();
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<DimensionedField<Type, volMesh> >& tsu,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, tsu(), "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().negate();
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, tsu(), "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().negate();
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const DimensionedField<Type, volMesh>& su,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), su, "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().negate();
    tC().source() -= su.mesh().V()*su.field();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<DimensionedField<Type, volMesh> >& tsu,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), tsu(), "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().negate();
    tC().source() -= tsu().mesh().V()*tsu().field();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), tsu(), "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().negate();
    tC().source() -= tsu().mesh().V()*tsu().internalField();
    tsu.clear();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const fvMatrix<Type>& A,
    const dimensioned<Type>& su
)
{
    checkMethod(A, su, "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= su.value()*A.psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const tmp<fvMatrix<Type> >& tA,
    const dimensioned<Type>& su
)
{
    checkMethod(tA(), su, "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= su.value()*tC().psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const dimensioned<Type>& su,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, su, "+");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() -= su.value()*A.psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator+
(
    const dimensioned<Type>& su,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), su, "+");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() -= su.value()*tC().psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const fvMatrix<Type>& A,
    const dimensioned<Type>& su
)
{
    checkMethod(A, su, "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().source() += su.value()*tC().psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const tmp<fvMatrix<Type> >& tA,
    const dimensioned<Type>& su
)
{
    checkMethod(tA(), su, "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().source() += su.value()*tC().psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const dimensioned<Type>& su,
    const fvMatrix<Type>& A
)
{
    checkMethod(A, su, "-");
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC().negate();
    tC().source() -= su.value()*A.psi().mesh().V();
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator-
(
    const dimensioned<Type>& su,
    const tmp<fvMatrix<Type> >& tA
)
{
    checkMethod(tA(), su, "-");
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC().negate();
    tC().source() -= su.value()*tC().psi().mesh().V();
    return tC;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const DimensionedField<scalar, volMesh>& dsf,
    const fvMatrix<Type>& A
)
{
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() *= dsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const tmp< DimensionedField<scalar, volMesh> >& tdsf,
    const fvMatrix<Type>& A
)
{
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() *= tdsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const tmp<volScalarField>& tvsf,
    const fvMatrix<Type>& A
)
{
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() *= tvsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const DimensionedField<scalar, volMesh>& dsf,
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() *= dsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const tmp<DimensionedField<scalar, volMesh> >& tdsf,
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() *= tdsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const tmp<volScalarField>& tvsf,
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() *= tvsf;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const dimensioned<scalar>& ds,
    const fvMatrix<Type>& A
)
{
    tmp<fvMatrix<Type> > tC(new fvMatrix<Type>(A));
    tC() *= ds;
    return tC;
}

template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::operator*
(
    const dimensioned<scalar>& ds,
    const tmp<fvMatrix<Type> >& tA
)
{
    tmp<fvMatrix<Type> > tC(tA.ptr());
    tC() *= ds;
    return tC;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const fvMatrix<Type>& M,
    const DimensionedField<Type, volMesh>& psi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMphi
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "M&" + psi.name(),
                psi.instance(),
                psi.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi.mesh(),
            M.dimensions()/dimVol,
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );
    GeometricField<Type, fvPatchField, volMesh>& Mphi = tMphi();

    // Loop over field components
    if (M.hasDiag())
    {
        for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
        {
            scalarField psiCmpt(psi.field().component(cmpt));
            scalarField boundaryDiagCmpt(M.diag());
            M.addBoundaryDiag(boundaryDiagCmpt, cmpt);
            Mphi.internalField().replace(cmpt, -boundaryDiagCmpt*psiCmpt);
        }
    }
    else
    {
        Mphi.internalField() = pTraits<Type>::zero;
    }

    Mphi.internalField() += M.lduMatrix::H(psi.field()) + M.source();
    M.addBoundarySource(Mphi.internalField());

    Mphi.internalField() /= -psi.mesh().V();
    Mphi.correctBoundaryConditions();

    return tMphi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const fvMatrix<Type>& M,
    const tmp<DimensionedField<Type, volMesh> >& tpsi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMpsi = M & tpsi();
    tpsi.clear();
    return tMpsi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const fvMatrix<Type>& M,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tpsi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMpsi = M & tpsi();
    tpsi.clear();
    return tMpsi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const tmp<fvMatrix<Type> >& tM,
    const DimensionedField<Type, volMesh>& psi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMpsi = tM() & psi;
    tM.clear();
    return tMpsi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const tmp<fvMatrix<Type> >& tM,
    const tmp<DimensionedField<Type, volMesh> >& tpsi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMpsi = tM() & tpsi();
    tM.clear();
    tpsi.clear();
    return tMpsi;
}

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::operator&
(
    const tmp<fvMatrix<Type> >& tM,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tpsi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tMpsi = tM() & tpsi();
    tM.clear();
    tpsi.clear();
    return tMpsi;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<(Ostream& os, const fvMatrix<Type>& fvm)
{
    os  << static_cast<const lduMatrix&>(fvm) << nl
        << fvm.dimensions_ << nl
        << fvm.source_ << nl
        << fvm.internalCoeffs_ << nl
        << fvm.boundaryCoeffs_ << endl;

    os.check("Ostream& operator<<(Ostream&, fvMatrix<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * Solvers * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fvMatrix<Type>::setComponentReference
(
    const label patchi,
    const label facei,
    const direction cmpt,
    const scalar value
)
{
    if (psi_.needReference())
    {
        if (Pstream::master())
        {
            internalCoeffs_[patchi][facei].component(cmpt) +=
                diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]];

            boundaryCoeffs_[patchi][facei].component(cmpt) +=
                diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]]
               *value;
        }
    }
}


template<class Type>
CML::solverPerformance CML::fvMatrix<Type>::solve
(
    const dictionary& solverControls
)
{
    addProfiling(solve, "fvMatrix::solve." + psi_.name());

    if (debug)
    {
        Info<< "fvMatrix<Type>::solve(const dictionary& solverControls) : "
               "solving fvMatrix<Type>"
            << endl;
    }

    GeometricField<Type, fvPatchField, volMesh>& psi =
       const_cast<GeometricField<Type, fvPatchField, volMesh>&>(psi_);

    if (psi.mesh().defectCorr())
    {
        tmp<GeometricField<Type, fvPatchField, volMesh> > Residual = this->R();

        tmp<GeometricField<Type, fvPatchField, volMesh> > Correction = psi.mesh().defectCorrVecs() & fvc::grad(Residual());

        source_ += psi.mesh().V()*Correction().internalField();

        Residual.clear();
        Correction.clear();
    }

    solverPerformance solverPerfVec
    (
        "fvMatrix<Type>::solve",
        psi.name()
    );

    scalarField saveDiag(diag());

    Field<Type> source(source_);

    // At this point include the boundary source from the coupled boundaries.
    // This is corrected for the implict part by updateMatrixInterfaces within
    // the component loop.
    addBoundarySource(source);

    typename Type::labelType validComponents
    (
        pow
        (
            psi.mesh().solutionD(),
            pTraits<typename powProduct<Vector<label>, Type::rank>::type>::zero
        )
    );

    for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
    {
        if (validComponents[cmpt] == -1) continue;

        // copy field and source

        scalarField psiCmpt(psi.internalField().component(cmpt));
        addBoundaryDiag(diag(), cmpt);

        scalarField sourceCmpt(source.component(cmpt));

        FieldField<Field, scalar> bouCoeffsCmpt
        (
            boundaryCoeffs_.component(cmpt)
        );

        FieldField<Field, scalar> intCoeffsCmpt
        (
            internalCoeffs_.component(cmpt)
        );

        lduInterfaceFieldPtrsList interfaces =
            psi.boundaryField().interfaces();

        // Use the initMatrixInterfaces and updateMatrixInterfaces to correct
        // bouCoeffsCmpt for the explicit part of the coupled boundary
        // conditions
        initMatrixInterfaces
        (
            bouCoeffsCmpt,
            interfaces,
            psiCmpt,
            sourceCmpt,
            cmpt
        );

        updateMatrixInterfaces
        (
            bouCoeffsCmpt,
            interfaces,
            psiCmpt,
            sourceCmpt,
            cmpt
        );

        solverPerformance solverPerf;

        // Solver call
        solverPerf = lduMatrix::solver::New
        (
            psi.name() + pTraits<Type>::componentNames[cmpt],
            *this,
            bouCoeffsCmpt,
            intCoeffsCmpt,
            interfaces,
            solverControls
        )->solve(psiCmpt, sourceCmpt, cmpt);

        solverPerf.print();

        solverPerfVec = max(solverPerfVec, solverPerf);
        solverPerfVec.solverName() = solverPerf.solverName();

        psi.internalField().replace(cmpt, psiCmpt);
        diag() = saveDiag;
    }

    psi.correctBoundaryConditions();

    psi.mesh().setSolverPerformance(psi.name(), solverPerfVec);

    return solverPerfVec;
}


template<class Type>
CML::autoPtr<typename CML::fvMatrix<Type>::fvSolver>
CML::fvMatrix<Type>::solver()
{
    return solver
    (
        psi_.mesh().solverDict
        (
            psi_.select
            (
                psi_.mesh().data::template lookupOrDefault<bool>
                ("finalIteration", false)
            )
        )
    );
}


template<class Type>
CML::solverPerformance CML::fvMatrix<Type>::fvSolver::solve()
{
    return solve
    (
        fvMat_.psi_.mesh().solverDict
        (
            fvMat_.psi_.select
            (
                fvMat_.psi_.mesh().data::template lookupOrDefault<bool>
                ("finalIteration", false)
            )
        )
    );
}


template<class Type>
CML::solverPerformance CML::fvMatrix<Type>::solve()
{
    return solve
    (
        psi_.mesh().solverDict
        (
            psi_.select
            (
                psi_.mesh().data::template lookupOrDefault<bool>
                ("finalIteration", false)
            )
        )
    );
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fvMatrix<Type>::residual() const
{
    tmp<Field<Type> > tres(new Field<Type>(source_));
    Field<Type>& res = tres();

    addBoundarySource(res);

    // Loop over field components
    for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
    {
        scalarField psiCmpt(psi_.internalField().component(cmpt));

        scalarField boundaryDiagCmpt(psi_.size(), 0.0);
        addBoundaryDiag(boundaryDiagCmpt, cmpt);

        FieldField<Field, scalar> bouCoeffsCmpt
        (
            boundaryCoeffs_.component(cmpt)
        );

        res.replace
        (
            cmpt,
            lduMatrix::residual
            (
                psiCmpt,
                res.component(cmpt) - boundaryDiagCmpt*psiCmpt,
                bouCoeffsCmpt,
                psi_.boundaryField().interfaces(),
                cmpt
            )
        );
    }

    return tres;
}



// Specialisation for scalars
#include "fvScalarMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
