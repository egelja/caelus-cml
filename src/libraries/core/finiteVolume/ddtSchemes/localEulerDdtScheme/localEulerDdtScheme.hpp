/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fv::localEulerDdtScheme

Description
    Local time-step first-order Euler implicit/explicit ddt.

    The reciprocal of the local time-step field is looked-up from the database.

    This scheme should only be used for steady-state computations using
    transient codes where local time-stepping is preferably to under-relaxation
    for transport consistency reasons.

See also
    CML::fv::CoEulerDdtScheme

SourceFiles
    localEulerDdt.cpp
    localEulerDdtSchemes.cpp

\*---------------------------------------------------------------------------*/

#ifndef localEulerDdtScheme_H
#define localEulerDdtScheme_H

#include "ddtScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class localEulerDdt Declaration
\*---------------------------------------------------------------------------*/

class localEulerDdt
{
public:

    //- Name of the reciprocal local time-step field
    static word rDeltaTName;

    //- Name of the reciprocal local face time-step field
    static word rDeltaTfName;

    //- Name of the reciprocal local sub-cycling time-step field
    static word rSubDeltaTName;


    // Constructors

        localEulerDdt()
        {}


    // Member Functions

        //- Return true if LTS is enabled
        static bool enabled(const fvMesh& mesh);

        //- Return the reciprocal of the local time-step
        //  looked-up from the objectRegistry
        static const volScalarField& localRDeltaT(const fvMesh& mesh);

        //- Return the reciprocal of the local face time-step
        //  looked-up from the objectRegistry
        static const surfaceScalarField& localRDeltaTf(const fvMesh& mesh);

        //- Calculate and return the reciprocal of the local sub-cycling
        //  time-step
        static tmp<volScalarField> localRSubDeltaT
        (
            const fvMesh& mesh,
            const label nAlphaSubCycles
        );
};


/*---------------------------------------------------------------------------*\
                       Class localEulerDdtScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class localEulerDdtScheme
:
    public localEulerDdt,
    public fv::ddtScheme<Type>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        localEulerDdtScheme(const localEulerDdtScheme&);

        //- Disallow default bitwise assignment
        void operator=(const localEulerDdtScheme&);

        //- Return the reciprocal of the local time-step
        const volScalarField& localRDeltaT() const;

        //- Return the reciprocal of the local face time-step
        const surfaceScalarField& localRDeltaTf() const;


public:

    //- Runtime type information
    TypeName("localEuler");


    // Constructors

        //- Construct from mesh
        localEulerDdtScheme(const fvMesh& mesh)
        :
            ddtScheme<Type>(mesh)
        {}

        //- Construct from mesh and Istream
        localEulerDdtScheme(const fvMesh& mesh, Istream& is)
        :
            ddtScheme<Type>(mesh, is)
        {}


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return fv::ddtScheme<Type>::mesh();
        }

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const dimensioned<Type>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
        (
            const volScalarField& alpha,
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );

        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvcDdt
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        );

        tmp<fvMatrix<Type> > fvmDdt
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmDdt
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmDdt
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        tmp<fvMatrix<Type> > fvmDdt
        (
            const volScalarField& alpha,
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );

        typedef typename ddtScheme<Type>::fluxFieldType fluxFieldType;

        tmp<fluxFieldType> fvcDdtUfCorr
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
        );

        tmp<fluxFieldType> fvcDdtPhiCorr
        (
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi
        );

        tmp<fluxFieldType> fvcDdtUfCorr
        (
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
        );

        tmp<fluxFieldType> fvcDdtPhiCorr
        (
            const volScalarField& rho,
            const GeometricField<Type, fvPatchField, volMesh>& U,
            const fluxFieldType& phi
        );

        tmp<surfaceScalarField> meshPhi
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );
};


template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtUfCorr
(
    const GeometricField<scalar, fvPatchField, volMesh>& U,
    const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
    const volScalarField& U,
    const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtUfCorr
(
    const volScalarField& rho,
    const volScalarField& U,
    const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
    const volScalarField& rho,
    const volScalarField& U,
    const surfaceScalarField& phi
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "surfaceInterpolate.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
const volScalarField& localEulerDdtScheme<Type>::localRDeltaT() const
{
    return localEulerDdt::localRDeltaT(mesh());
}


template<class Type>
const surfaceScalarField& localEulerDdtScheme<Type>::localRDeltaTf() const
{
    return localEulerDdt::localRDeltaTf(mesh());
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const dimensioned<Type>& dt
)
{
    IOobject ddtIOobject
    (
        "ddt(" + dt.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            ddtIOobject,
            mesh(),
            dimensioned<Type>
            (
                "0",
                dt.dimensions()/dimTime,
                pTraits<Type>::zero
            ),
            calculatedFvPatchField<Type>::typeName
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const volScalarField& rDeltaT = localRDeltaT();

    IOobject ddtIOobject
    (
        "ddt(" + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            ddtIOobject,
            rDeltaT*(vf - vf.oldTime())
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const volScalarField& rDeltaT = localRDeltaT();

    IOobject ddtIOobject
    (
        "ddt(" + rho.name() + ',' + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            ddtIOobject,
            rDeltaT*rho*(vf - vf.oldTime())
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const volScalarField& rDeltaT = localRDeltaT();

    IOobject ddtIOobject
    (
        "ddt(" + rho.name() + ',' + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            ddtIOobject,
            rDeltaT*(rho*vf - rho.oldTime()*vf.oldTime())
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const volScalarField& rDeltaT = localRDeltaT();

    IOobject ddtIOobject
    (
        "ddt("+alpha.name()+','+rho.name()+','+vf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            ddtIOobject,
            rDeltaT
           *(
               alpha*rho*vf
             - alpha.oldTime()*rho.oldTime()*vf.oldTime()
           )
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
localEulerDdtScheme<Type>::fvcDdt
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf
)
{
    const surfaceScalarField& rDeltaT = localRDeltaTf();

    IOobject ddtIOobject
    (
        "ddt("+sf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            ddtIOobject,
            rDeltaT*(sf - sf.oldTime())
        )
    );
}


template<class Type>
tmp<fvMatrix<Type> >
localEulerDdtScheme<Type>::fvmDdt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            vf.dimensions()*dimVol/dimTime
        )
    );

    fvMatrix<Type>& fvm = tfvm();

    const scalarField& rDeltaT = localRDeltaT().internalField();

    fvm.diag() = rDeltaT*mesh().Vsc();
    fvm.source() = rDeltaT*vf.oldTime().internalField()*mesh().Vsc();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
localEulerDdtScheme<Type>::fvmDdt
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
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalarField& rDeltaT = localRDeltaT().internalField();

    fvm.diag() = rDeltaT*rho.value()*mesh().Vsc();

    fvm.source() =
        rDeltaT*rho.value()*vf.oldTime().internalField()*mesh().Vsc();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
localEulerDdtScheme<Type>::fvmDdt
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
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalarField& rDeltaT = localRDeltaT().internalField();

    fvm.diag() = rDeltaT*rho.internalField()*mesh().Vsc();

    fvm.source() = rDeltaT
       *rho.oldTime().internalField()
       *vf.oldTime().internalField()*mesh().Vsc();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
localEulerDdtScheme<Type>::fvmDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            alpha.dimensions()*rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalarField& rDeltaT = localRDeltaT().internalField();

    fvm.diag() = rDeltaT*alpha.internalField()*rho.internalField()*mesh().Vsc();

    fvm.source() = rDeltaT
       *alpha.oldTime().internalField()
       *rho.oldTime().internalField()
       *vf.oldTime().internalField()*mesh().Vsc();

    return tfvm;
}


template<class Type>
tmp<typename localEulerDdtScheme<Type>::fluxFieldType>
localEulerDdtScheme<Type>::fvcDdtUfCorr
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(localRDeltaT()));

    fluxFieldType phiUf0(mesh().Sf() & Uf.oldTime());
    fluxFieldType phiCorr
    (
        phiUf0 - fvc::dotInterpolate(mesh().Sf(), U.oldTime())
    );

    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtCorr(" + U.name() + ',' + Uf.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            this->fvcDdtPhiCoeff(U.oldTime(), phiUf0, phiCorr)
           *rDeltaT*phiCorr
        )
    );
}


template<class Type>
tmp<typename localEulerDdtScheme<Type>::fluxFieldType>
localEulerDdtScheme<Type>::fvcDdtPhiCorr
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(localRDeltaT()));

    fluxFieldType phiCorr
    (
        phi.oldTime() - fvc::dotInterpolate(mesh().Sf(), U.oldTime())
    );

    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtCorr(" + U.name() + ',' + phi.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            this->fvcDdtPhiCoeff(U.oldTime(), phi.oldTime(), phiCorr)
           *rDeltaT*phiCorr
        )
    );
}


template<class Type>
tmp<typename localEulerDdtScheme<Type>::fluxFieldType>
localEulerDdtScheme<Type>::fvcDdtUfCorr
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(localRDeltaT()));

    if
    (
        U.dimensions() == dimVelocity
     && Uf.dimensions() == dimDensity*dimVelocity
    )
    {
        GeometricField<Type, fvPatchField, volMesh> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        fluxFieldType phiUf0(mesh().Sf() & Uf.oldTime());
        fluxFieldType phiCorr(phiUf0 - fvc::dotInterpolate(mesh().Sf(), rhoU0));

        return tmp<fluxFieldType>
        (
            new fluxFieldType
            (
                IOobject
                (
                    "ddtCorr("
                  + rho.name() + ',' + U.name() + ',' + Uf.name() + ')',
                    mesh().time().timeName(),
                    mesh()
                ),
                this->fvcDdtPhiCoeff(rhoU0, phiUf0, phiCorr)*rDeltaT*phiCorr
            )
        );
    }
    else if
    (
        U.dimensions() == dimDensity*dimVelocity
     && Uf.dimensions() == dimDensity*dimVelocity
    )
    {
        return fvcDdtUfCorr(U, Uf);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of Uf are not correct"
            << abort(FatalError);

        return fluxFieldType::null();
    }
}


template<class Type>
tmp<typename localEulerDdtScheme<Type>::fluxFieldType>
localEulerDdtScheme<Type>::fvcDdtPhiCorr
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(localRDeltaT()));

    if
    (
        U.dimensions() == dimVelocity
     && phi.dimensions() == rho.dimensions()*dimVelocity*dimArea
    )
    {
        GeometricField<Type, fvPatchField, volMesh> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        fluxFieldType phiCorr
        (
            phi.oldTime() - fvc::dotInterpolate(mesh().Sf(), rhoU0)
        );

        return tmp<fluxFieldType>
        (
            new fluxFieldType
            (
                IOobject
                (
                    "ddtCorr("
                  + rho.name() + ',' + U.name() + ',' + phi.name() + ')',
                    mesh().time().timeName(),
                    mesh()
                ),
                this->fvcDdtPhiCoeff(rhoU0, phi.oldTime(), phiCorr)
               *rDeltaT*phiCorr
            )
        );
    }
    else if
    (
        U.dimensions() == rho.dimensions()*dimVelocity
     && phi.dimensions() == rho.dimensions()*dimVelocity*dimArea
    )
    {
        return fvcDdtPhiCorr(U, phi);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of phi are not correct"
            << abort(FatalError);

        return fluxFieldType::null();
    }
}


template<class Type>
tmp<surfaceScalarField> localEulerDdtScheme<Type>::meshPhi
(
    const GeometricField<Type, fvPatchField, volMesh>&
)
{
    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            IOobject
            (
                "meshPhi",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh(),
            dimensionedScalar("0", dimVolume/dimTime, 0.0)
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
