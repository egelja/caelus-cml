/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::fv::ddtScheme

Description
    Abstract base class for ddt schemes.

SourceFiles
    ddtScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef ddtScheme_H
#define ddtScheme_H

#include "tmp.hpp"
#include "dimensionedType.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class fvMatrix;

class fvMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                           Class ddtScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class ddtScheme
:
    public refCount
{

protected:

    // Protected data

        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow copy construct
        ddtScheme(const ddtScheme&);

        //- Disallow default bitwise assignment
        void operator=(const ddtScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            ddtScheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        ddtScheme(const fvMesh& mesh)
        :
            mesh_(mesh)
        {}

        //- Construct from mesh and Istream
        ddtScheme(const fvMesh& mesh, Istream&)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return a pointer to a new ddtScheme created on freestore
        static tmp<ddtScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~ddtScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const dimensioned<Type>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const volScalarField& alpha,
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvcDdt
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        );

        virtual tmp<fvMatrix<Type> > fvmDdt
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmDdt
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmDdt
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmDdt
        (
            const volScalarField& alpha,
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& vf
        ) = 0;

        typedef GeometricField
        <
            typename flux<Type>::type,
            fvsPatchField,
            surfaceMesh
        > fluxFieldType;

        tmp<surfaceScalarField> fvcDdtPhiCoeff
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi,
            const fluxFieldType& phiCorr
        );

        tmp<surfaceScalarField> fvcDdtPhiCoeff
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi
        );

        virtual tmp<fluxFieldType> fvcDdtUfCorr
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
        ) = 0;

        virtual tmp<fluxFieldType> fvcDdtPhiCorr
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi
        ) = 0;

        virtual tmp<fluxFieldType> fvcDdtUfCorr
        (
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
        ) = 0;

        virtual tmp<fluxFieldType> fvcDdtPhiCorr
        (
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi
        ) = 0;

        virtual tmp<surfaceScalarField> meshPhi
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvDdtTypeScheme(SS, Type)                                          \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            ddtScheme<Type>::addIstreamConstructorToTable<SS<Type> >           \
                add##SS##Type##IstreamConstructorToTable_;                     \
        }                                                                      \
    }

#define makeFvDdtScheme(SS)                                                    \
                                                                               \
makeFvDdtTypeScheme(SS, scalar)                                                \
makeFvDdtTypeScheme(SS, vector)                                                \
makeFvDdtTypeScheme(SS, sphericalTensor)                                       \
makeFvDdtTypeScheme(SS, symmTensor)                                            \
makeFvDdtTypeScheme(SS, tensor)                                                \
                                                                               \
namespace CML                                                                  \
{                                                                              \
namespace fv                                                                   \
{                                                                              \
                                                                               \
template<>                                                                     \
tmp<surfaceScalarField> SS<scalar>::fvcDdtUfCorr                               \
(                                                                              \
    const volScalarField& U,                                                   \
    const surfaceScalarField& Uf                                               \
)                                                                              \
{                                                                              \
    NotImplemented;                                                            \
    return surfaceScalarField::null();                                         \
}                                                                              \
                                                                               \
template<>                                                                     \
tmp<surfaceScalarField> SS<scalar>::fvcDdtPhiCorr                              \
(                                                                              \
    const volScalarField& U,                                                   \
    const surfaceScalarField& phi                                              \
)                                                                              \
{                                                                              \
    NotImplemented;                                                            \
    return surfaceScalarField::null();                                         \
}                                                                              \
                                                                               \
template<>                                                                     \
tmp<surfaceScalarField> SS<scalar>::fvcDdtUfCorr                               \
(                                                                              \
    const volScalarField& rho,                                                 \
    const volScalarField& U,                                                   \
    const surfaceScalarField& Uf                                               \
)                                                                              \
{                                                                              \
    NotImplemented;                                                            \
    return surfaceScalarField::null();                                         \
}                                                                              \
                                                                               \
template<>                                                                     \
tmp<surfaceScalarField> SS<scalar>::fvcDdtPhiCorr                              \
(                                                                              \
    const volScalarField& rho,                                                 \
    const volScalarField& U,                                                   \
    const surfaceScalarField& phi                                              \
)                                                                              \
{                                                                              \
    NotImplemented;                                                            \
    return surfaceScalarField::null();                                         \
}                                                                              \
                                                                               \
}                                                                              \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "HashTable.hpp"
#include "surfaceInterpolate.hpp"
#include "cyclicAMIFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<ddtScheme<Type> > ddtScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        InfoInFunction << "Constructing ddtScheme<Type>" << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction
        (
            schemeData
        )   << "Ddt scheme not specified" << endl << endl
            << "Valid ddt schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction
        (
            schemeData
        )   << "Unknown ddt scheme " << schemeName << nl << nl
            << "Valid ddt schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
ddtScheme<Type>::~ddtScheme()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > ddtScheme<Type>::fvcDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    NotImplemented;

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        GeometricField<Type, fvPatchField, volMesh>::null()
    );
}


template<class Type>
tmp<fvMatrix<Type> > ddtScheme<Type>::fvmDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    NotImplemented;

    return tmp<fvMatrix<Type> >
    (
        new fvMatrix<Type>
        (
            vf,
            alpha.dimensions()*rho.dimensions()
            *vf.dimensions()*dimVol/dimTime
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > ddtScheme<Type>::fvcDdt
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf
)
{
    NotImplemented;

    return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    (
        GeometricField<Type, fvsPatchField, surfaceMesh>::null()
    );
}



template<class Type>
tmp<surfaceScalarField> ddtScheme<Type>::fvcDdtPhiCoeff
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi,
    const fluxFieldType& phiCorr
)
{
    tmp<surfaceScalarField> tddtCouplingCoeff = scalar(1)
      - min
        (
            mag(phiCorr)
           /(mag(phi) + dimensionedScalar("small", phi.dimensions(), SMALL)),
            scalar(1)
        );

    surfaceScalarField& ddtCouplingCoeff = tddtCouplingCoeff();

    forAll(U.boundaryField(), patchi)
    {
        if
        (
            U.boundaryField()[patchi].fixesValue()
         || isA<cyclicAMIFvPatch>(mesh().boundary()[patchi])
        )
        {
            ddtCouplingCoeff.boundaryField()[patchi] = 0.0;
        }
    }

    if (debug > 1)
    {
        Info<< "ddtCouplingCoeff mean max min = "
            << gAverage(ddtCouplingCoeff.internalField())
            << " " << gMax(ddtCouplingCoeff.internalField())
            << " " << gMin(ddtCouplingCoeff.internalField())
            << endl;
    }

    return tddtCouplingCoeff;
}


template<class Type>
tmp<surfaceScalarField> ddtScheme<Type>::fvcDdtPhiCoeff
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();

    tmp<surfaceScalarField> tddtCouplingCoeff = scalar(1)
      - min
        (
            mag(phi - fvc::dotInterpolate(mesh().Sf(),U))
           /(mag(phi) + dimensionedScalar("small", phi.dimensions(), VSMALL)),
           //(rDeltaT*mesh().magSf()/mesh().deltaCoeffs()),
            scalar(1)
        );

    surfaceScalarField& ddtCouplingCoeff = tddtCouplingCoeff();

    forAll(U.boundaryField(), patchi)
    {
        if
        (
            U.boundaryField()[patchi].fixesValue()
         || isA<cyclicAMIFvPatch>(mesh().boundary()[patchi])
        )
        {
            ddtCouplingCoeff.boundaryField()[patchi] = 0.0;
        }
    }

    if (debug > 1)
    {
        Info<< "ddtCouplingCoeff mean max min = "
            << gAverage(ddtCouplingCoeff.internalField())
            << " " << gMax(ddtCouplingCoeff.internalField())
            << " " << gMin(ddtCouplingCoeff.internalField())
            << endl;
    }

    return tddtCouplingCoeff;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
