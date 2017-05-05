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
    CML::fv::steadyStateD2dt2Scheme

Description
    SteadyState d2dt2 which returns 0.


\*---------------------------------------------------------------------------*/

#ifndef steadyStateD2dt2Scheme_H
#define steadyStateD2dt2Scheme_H

#include "d2dt2Scheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class steadyStateD2dt2Scheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class steadyStateD2dt2Scheme
:
    public fv::d2dt2Scheme<Type>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        steadyStateD2dt2Scheme(const steadyStateD2dt2Scheme&);

        //- Disallow default bitwise assignment
        void operator=(const steadyStateD2dt2Scheme&);


public:

    //- Runtime type information
    TypeName("steadyState");


    // Constructors

        //- Construct from mesh
        steadyStateD2dt2Scheme(const fvMesh& mesh)
        :
            d2dt2Scheme<Type>(mesh)
        {}

        //- Construct from mesh and Istream
        steadyStateD2dt2Scheme(const fvMesh& mesh, Istream& is)
        :
            d2dt2Scheme<Type>(mesh, is)
        {}


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return fv::d2dt2Scheme<Type>::mesh();
        }

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmD2dt2
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmD2dt2
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmD2dt2
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvcDiv.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateD2dt2Scheme<Type>::fvcD2dt2
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "d2dt2("+vf.name()+')',
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                vf.dimensions()/dimTime/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateD2dt2Scheme<Type>::fvcD2dt2
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "d2dt2("+rho.name()+','+vf.name()+')',
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                rho.dimensions()*vf.dimensions()/dimTime/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            vf.dimensions()*dimVol/dimTime/dimTime
        )
    );

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime/dimTime
        )
    );

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime/dimTime
        )
    );

    return tfvm;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
