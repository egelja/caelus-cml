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
    CML::fv::CrankNicolsonDdtScheme

Description
    Second-oder CrankNicolson implicit ddt using the current and
    previous time-step fields as well as the previous time-step ddt.

    The Crank-Nicolson scheme is often unstable for complex flows in complex
    geometries and it is necessary to "off-centre" the scheme to stabilize it
    while retaining greater temporal accuracy than the first-order
    Euler-implicit scheme.  Off-centering is specified via the mandatory
    coefficient \c ocCoeff in the range [0,1] following the scheme name e.g.
    \verbatim
    ddtSchemes
    {
        default         CrankNicolson 0.9;
    }
    \endverbatim
    or with an optional "ramp" function to transition from the Euler scheme to
    Crank-Nicolson over a initial period to avoid start-up problems, e.g.
    \verbatim
    ddtSchemes
    {
        default         CrankNicolson
        ocCoeff
        {
            type scale;
            scale linearRamp;
            duration 0.01;
            value 0.9;
        };
    }
    \endverbatim
    With a coefficient of 1 the scheme is fully centred and second-order,
    with a coefficient of 0 the scheme is equivalent to Euler-implicit.
    A coefficient of 0.9 has been found to be suitable for a range of cases for
    which higher-order accuracy in time is needed and provides similar accuracy
    and stability to the backward scheme.  However, the backward scheme has
    been found to be more robust and provides formal second-order accuracy in
    time.

    The advantage of the Crank-Nicolson scheme over backward is that only the
    new and old-time values are needed, the additional terms relating to the
    fluxes and sources are evaluated at the mid-point of the time-step which
    provides the opportunity to limit the fluxes in such a way as to ensure
    boundedness while maintaining greater accuracy in time compared to the
    Euler-implicit scheme.  This approach is now used with MULES in the
    interFoam family of solvers.  Boundedness cannot be guaranteed with the
    backward scheme.

Note
    The Crank-Nicolson coefficient for the implicit part of the RHS is related
    to the off-centering coefficient by
    \verbatim
        cnCoeff = 1.0/(1.0 + ocCoeff);
    \endverbatim

See also
    CML::Euler
    CML::backward

SourceFiles
    CrankNicolsonDdtScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef CrankNicolsonDdtScheme_H
#define CrankNicolsonDdtScheme_H

#include "ddtScheme.hpp"
#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class CrankNicolsonDdtScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class CrankNicolsonDdtScheme
:
    public fv::ddtScheme<Type>
{
    // Private Data

        //- Class to store the ddt0 fields on the objectRegistry for use in the
        //  next time-step.  The start-time index of the CN scheme is also
        //  stored to help handle the transition from Euler to CN
        template<class GeoField>
        class DDt0Field
        :
            public GeoField
        {
            label startTimeIndex_;

        public:

            //- Constructor from file for restart.
            DDt0Field
            (
                const IOobject& io,
                const fvMesh& mesh
            );

            //- Constructor from components, initisalised to zero with given
            //  dimensions.
            DDt0Field
            (
                const IOobject& io,
                const fvMesh& mesh,
                const dimensioned<typename GeoField::value_type>& dimType
            );

            //- Return the start-time index
            label startTimeIndex() const;

            //- Cast to the underlying GeoField
            GeoField& operator()();

            //- Assignment to a GeoField
            void operator=(const GeoField& gf);
        };


        //- Off-centering coefficient function
        //  1 -> CN, less than one blends with EI
        autoPtr<DataEntry<scalar> > ocCoeff_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        CrankNicolsonDdtScheme(const CrankNicolsonDdtScheme&);

        //- Disallow default bitwise assignment
        void operator=(const CrankNicolsonDdtScheme&);

        template<class GeoField>
        DDt0Field<GeoField>& ddt0_
        (
            const word& name,
            const dimensionSet& dims
        );

        //- Check if the ddt0 needs to be evaluated for this time-step
        template<class GeoField>
        bool evaluate(const DDt0Field<GeoField>& ddt0) const;

        //- Return the coefficient for Euler scheme for the first time-step
        //  for and CN thereafter
        template<class GeoField>
        scalar coef_(const DDt0Field<GeoField>&) const;

        //- Return the old time-step coefficient for Euler scheme for the
        //  second time-step and for CN thereafter
        template<class GeoField>
        scalar coef0_(const DDt0Field<GeoField>&) const;

        //- Return the reciprocal time-step coefficient for Euler for the
        //  first time-step and CN thereafter
        template<class GeoField>
        dimensionedScalar rDtCoef_(const DDt0Field<GeoField>&) const;

        //- Return the reciprocal old time-step coefficient for Euler for the
        //  second time-step and CN thereafter
        template<class GeoField>
        dimensionedScalar rDtCoef0_(const DDt0Field<GeoField>&) const;

        //- Return ddt0 multiplied by the off-centreing coefficient
        template<class GeoField>
        tmp<GeoField> offCentre_(const GeoField& ddt0) const;


public:

    //- Runtime type information
    TypeName("CrankNicolson");


    // Constructors

        //- Construct from mesh
        CrankNicolsonDdtScheme(const fvMesh& mesh);

        //- Construct from mesh and Istream
        CrankNicolsonDdtScheme(const fvMesh& mesh, Istream& is);


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return fv::ddtScheme<Type>::mesh();
        }

        //- Return the current off-centreing coefficient
        scalar ocCoeff() const
        {
            return ocCoeff_->value(mesh().time().value());
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
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtUfCorr
(
    const GeometricField<scalar, fvPatchField, volMesh>& U,
    const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtPhiCorr
(
    const volScalarField& U,
    const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtUfCorr
(
    const volScalarField& rho,
    const volScalarField& U,
    const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtPhiCorr
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
template<class GeoField>
CrankNicolsonDdtScheme<Type>::DDt0Field<GeoField>::DDt0Field
(
    const IOobject& io,
    const fvMesh& mesh
)
:
    GeoField(io, mesh),
    startTimeIndex_(-2) // This field is for a restart and thus correct so set
                        // the start time-index to correspond to a previous run
{
    // Set the time-index to the beginning of the run to ensure the field
    // is updated during the first time-step
    this->timeIndex() = mesh.time().startTimeIndex();
}


template<class Type>
template<class GeoField>
CrankNicolsonDdtScheme<Type>::DDt0Field<GeoField>::DDt0Field
(
    const IOobject& io,
    const fvMesh& mesh,
    const dimensioned<typename GeoField::value_type>& dimType
)
:
    GeoField(io, mesh, dimType),
    startTimeIndex_(mesh.time().timeIndex())
{}


template<class Type>
template<class GeoField>
label CrankNicolsonDdtScheme<Type>::DDt0Field<GeoField>::
startTimeIndex() const
{
    return startTimeIndex_;
}


template<class Type>
template<class GeoField>
GeoField& CrankNicolsonDdtScheme<Type>::DDt0Field<GeoField>::
operator()()
{
    return *this;
}


template<class Type>
template<class GeoField>
void CrankNicolsonDdtScheme<Type>::DDt0Field<GeoField>::
operator=(const GeoField& gf)
{
    GeoField::operator=(gf);
}


template<class Type>
template<class GeoField>
typename CrankNicolsonDdtScheme<Type>::template
    DDt0Field<GeoField>&
CrankNicolsonDdtScheme<Type>::ddt0_
(
    const word& name,
    const dimensionSet& dims
)
{
    if (!mesh().objectRegistry::template foundObject<GeoField>(name))
    {
        const Time& runTime = mesh().time();
        word startTimeName = runTime.timeName(runTime.startTime().value());

        if
        (
            (
                runTime.timeIndex() == runTime.startTimeIndex()
             || runTime.timeIndex() == runTime.startTimeIndex() + 1
            )
         && IOobject
            (
                name,
                startTimeName,
                mesh()
            ).headerOk()
        )
        {
            regIOobject::store
            (
                new DDt0Field<GeoField>
                (
                    IOobject
                    (
                        name,
                        startTimeName,
                        mesh(),
                        IOobject::MUST_READ,
                        IOobject::AUTO_WRITE
                    ),
                    mesh()
                )
            );
        }
        else
        {
            regIOobject::store
            (
                new DDt0Field<GeoField>
                (
                    IOobject
                    (
                        name,
                        mesh().time().timeName(),
                        mesh(),
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    mesh(),
                    dimensioned<typename GeoField::value_type>
                    (
                        "0",
                        dims/dimTime,
                        pTraits<typename GeoField::value_type>::zero
                    )
                )
            );
        }
    }

    DDt0Field<GeoField>& ddt0 = static_cast<DDt0Field<GeoField>&>
    (
        const_cast<GeoField&>
        (
            mesh().objectRegistry::template lookupObject<GeoField>(name)
        )
    );

    return ddt0;
}


template<class Type>
template<class GeoField>
bool CrankNicolsonDdtScheme<Type>::evaluate
(
    const DDt0Field<GeoField>& ddt0
) const
{
    return ddt0.timeIndex() != mesh().time().timeIndex();
}

template<class Type>
template<class GeoField>
scalar CrankNicolsonDdtScheme<Type>::coef_
(
    const DDt0Field<GeoField>& ddt0
) const
{
    if (mesh().time().timeIndex() > ddt0.startTimeIndex())
    {
        return 1 + ocCoeff();
    }
    else
    {
        return 1;
    }
}


template<class Type>
template<class GeoField>
scalar CrankNicolsonDdtScheme<Type>::coef0_
(
    const DDt0Field<GeoField>& ddt0
) const
{
    if (mesh().time().timeIndex() > ddt0.startTimeIndex() + 1)
    {
        return 1 + ocCoeff();
    }
    else
    {
        return 1;
    }
}


template<class Type>
template<class GeoField>
dimensionedScalar CrankNicolsonDdtScheme<Type>::rDtCoef_
(
    const DDt0Field<GeoField>& ddt0
) const
{
    return coef_(ddt0)/mesh().time().deltaT();
}


template<class Type>
template<class GeoField>
dimensionedScalar CrankNicolsonDdtScheme<Type>::rDtCoef0_
(
    const DDt0Field<GeoField>& ddt0
) const
{
    return coef0_(ddt0)/mesh().time().deltaT0();
}


template<class Type>
template<class GeoField>
tmp<GeoField> CrankNicolsonDdtScheme<Type>::offCentre_
(
    const GeoField& ddt0
) const
{
    if (ocCoeff() < 1)
    {
        return ocCoeff()*ddt0;
    }
    else
    {
        return ddt0;
    }
}


template<class Type>
const FieldField<fvPatchField, Type>& ff
(
    const FieldField<fvPatchField, Type>& bf
)
{
    return bf;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CrankNicolsonDdtScheme<Type>::CrankNicolsonDdtScheme(const fvMesh& mesh)
:
    ddtScheme<Type>(mesh),
    ocCoeff_(new DataEntryTypes::Constant<scalar>("ocCoeff", 1))
{
    // Ensure the old-old-time cell volumes are available
    // for moving meshes
    if (mesh.moving())
    {
        mesh.V00();
    }
}


template<class Type>
CrankNicolsonDdtScheme<Type>::CrankNicolsonDdtScheme
(
    const fvMesh& mesh,
    Istream& is
)
:
    ddtScheme<Type>(mesh, is)
{
    token firstToken(is);

    if (firstToken.isNumber())
    {
        const scalar ocCoeff = firstToken.number();
        if (ocCoeff < 0 || ocCoeff > 1)
        {
            FatalIOErrorInFunction
            (
                is
            )   << "Off-centreing coefficient = " << ocCoeff
                << " should be >= 0 and <= 1"
                << exit(FatalIOError);
        }

        ocCoeff_ = new DataEntryTypes::Constant<scalar>
        (
            "ocCoeff",
            ocCoeff
        );
    }
    else
    {
        is.putBack(firstToken);
        dictionary dict(is);
        ocCoeff_ = DataEntry<scalar>::New("ocCoeff", dict);
    }

    // Ensure the old-old-time cell volumes are available
    // for moving meshes
    if (mesh.moving())
    {
        mesh.V00();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
CrankNicolsonDdtScheme<Type>::fvcDdt
(
    const dimensioned<Type>& dt
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + dt.name() + ')',
            dt.dimensions()
        );

    IOobject ddtIOobject
    (
        "ddt(" + dt.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    tmp<GeometricField<Type, fvPatchField, volMesh> > tdtdt
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
            )
        )
    );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            dimensionedScalar rDtCoef0 = rDtCoef0_(ddt0);

            ddt0.dimensionedInternalField() =
            (
                (rDtCoef0*dt)*(mesh().V0() - mesh().V00())
              - mesh().V00()*offCentre_(ddt0.dimensionedInternalField())
            )/mesh().V0();
        }

        tdtdt().dimensionedInternalField() =
        (
            (rDtCoef*dt)*(mesh().V() - mesh().V0())
          - mesh().V0()*offCentre_(ddt0.dimensionedInternalField())
        )/mesh().V();
    }

    return tdtdt;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
CrankNicolsonDdtScheme<Type>::fvcDdt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + vf.name() + ')',
            vf.dimensions()
        );

    IOobject ddtIOobject
    (
        "ddt(" + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()*vf.oldTime().internalField()
                  - mesh().V00()*vf.oldTime().oldTime().internalField()
                ) - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    vf.oldTime().boundaryField()
                  - vf.oldTime().oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                mesh(),
                rDtCoef.dimensions()*vf.dimensions(),
                (
                    rDtCoef.value()*
                    (
                        mesh().V()*vf.internalField()
                      - mesh().V0()*vf.oldTime().internalField()
                    ) - mesh().V0()*offCentre_(ddt0.internalField())
                )/mesh().V(),
                rDtCoef.value()*
                (
                    vf.boundaryField() - vf.oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            )
        );
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*(vf.oldTime() - vf.oldTime().oldTime())
                 - offCentre_(ddt0());
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                rDtCoef*(vf - vf.oldTime()) - offCentre_(ddt0())
            )
        );
    }
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
CrankNicolsonDdtScheme<Type>::fvcDdt
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + rho.name() + ',' + vf.name() + ')',
            rho.dimensions()*vf.dimensions()
        );

    IOobject ddtIOobject
    (
        "ddt(" + rho.name() + ',' + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*rho.value()*
                (
                    mesh().V0()*vf.oldTime().internalField()
                  - mesh().V00()*vf.oldTime().oldTime().internalField()
                ) - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*rho.value()*
                (
                    vf.oldTime().boundaryField()
                  - vf.oldTime().oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                mesh(),
                rDtCoef.dimensions()*rho.dimensions()*vf.dimensions(),
                (
                    rDtCoef.value()*rho.value()*
                    (
                        mesh().V()*vf.internalField()
                      - mesh().V0()*vf.oldTime().internalField()
                    ) - mesh().V0()*offCentre_(ddt0.internalField())
                )/mesh().V(),
                rDtCoef.value()*rho.value()*
                (
                    vf.boundaryField() - vf.oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            )
        );
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*rho*(vf.oldTime() - vf.oldTime().oldTime())
                 - offCentre_(ddt0());
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                rDtCoef*rho*(vf - vf.oldTime()) - offCentre_(ddt0())
            )
        );
    }
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
CrankNicolsonDdtScheme<Type>::fvcDdt
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + rho.name() + ',' + vf.name() + ')',
            rho.dimensions()*vf.dimensions()
        );

    IOobject ddtIOobject
    (
        "ddt(" + rho.name() + ',' + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()*rho.oldTime().internalField()
                   *vf.oldTime().internalField()
                  - mesh().V00()*rho.oldTime().oldTime().internalField()
                   *vf.oldTime().oldTime().internalField()
                ) - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()
                  - rho.oldTime().oldTime().boundaryField()
                   *vf.oldTime().oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                mesh(),
                rDtCoef.dimensions()*rho.dimensions()*vf.dimensions(),
                (
                    rDtCoef.value()*
                    (
                        mesh().V()*rho.internalField()*vf.internalField()
                      - mesh().V0()*rho.oldTime().internalField()
                       *vf.oldTime().internalField()
                    ) - mesh().V00()*offCentre_(ddt0.internalField())
                )/mesh().V(),
                rDtCoef.value()*
                (
                    rho.boundaryField()*vf.boundaryField()
                  - rho.oldTime().boundaryField()*vf.oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            )
        );
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*
            (
                rho.oldTime()*vf.oldTime()
              - rho.oldTime().oldTime()*vf.oldTime().oldTime()
            ) - offCentre_(ddt0());
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                rDtCoef*(rho*vf - rho.oldTime()*vf.oldTime())
              - offCentre_(ddt0())
            )
        );
    }
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
CrankNicolsonDdtScheme<Type>::fvcDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + alpha.name() + ',' + rho.name() + ',' + vf.name() + ')',
            alpha.dimensions()*rho.dimensions()*vf.dimensions()
        );

    IOobject ddtIOobject
    (
        "ddt(" + alpha.name() + ',' + rho.name() + ',' + vf.name() + ')',
        mesh().time().timeName(),
        mesh()
    );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()
                   *alpha.oldTime().internalField()
                   *rho.oldTime().internalField()
                   *vf.oldTime().internalField()

                  - mesh().V00()
                   *alpha.oldTime().oldTime().internalField()
                   *rho.oldTime().oldTime().internalField()
                   *vf.oldTime().oldTime().internalField()
                ) - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    alpha.oldTime().boundaryField()
                   *rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()

                  - alpha.oldTime().oldTime().boundaryField()
                   *rho.oldTime().oldTime().boundaryField()
                   *vf.oldTime().oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                mesh(),
                rDtCoef.dimensions()
               *alpha.dimensions()*rho.dimensions()*vf.dimensions(),
                (
                    rDtCoef.value()*
                    (
                        mesh().V()
                       *alpha.internalField()
                       *rho.internalField()
                       *vf.internalField()

                      - mesh().V0()
                       *alpha.oldTime().internalField()
                       *rho.oldTime().internalField()
                       *vf.oldTime().internalField()
                    ) - mesh().V00()*offCentre_(ddt0.internalField())
                )/mesh().V(),
                rDtCoef.value()*
                (
                    alpha.boundaryField()
                   *rho.boundaryField()
                   *vf.boundaryField()

                  - alpha.oldTime().boundaryField()
                   *rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()
                ) - offCentre_(ff(ddt0.boundaryField()))
            )
        );
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*
            (
                alpha.oldTime()
               *rho.oldTime()
               *vf.oldTime()

              - alpha.oldTime().oldTime()
               *rho.oldTime().oldTime()
               *vf.oldTime().oldTime()
            ) - offCentre_(ddt0());
        }

        return tmp<GeometricField<Type, fvPatchField, volMesh> >
        (
            new GeometricField<Type, fvPatchField, volMesh>
            (
                ddtIOobject,
                rDtCoef
               *(
                   alpha*rho*vf
                 - alpha.oldTime()*rho.oldTime()*vf.oldTime()
                )
              - offCentre_(ddt0())
            )
        );
    }
}


template<class Type>
tmp<fvMatrix<Type> >
CrankNicolsonDdtScheme<Type>::fvmDdt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + vf.name() + ')',
            vf.dimensions()
        );

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            vf.dimensions()*dimVol/dimTime
        )
    );

    fvMatrix<Type>& fvm = tfvm();

    const scalar rDtCoef = rDtCoef_(ddt0).value();
    fvm.diag() = rDtCoef*mesh().V();

    vf.oldTime().oldTime();

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()*vf.oldTime().internalField()
                  - mesh().V00()*vf.oldTime().oldTime().internalField()
                )
              - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    vf.oldTime().boundaryField()
                  - vf.oldTime().oldTime().boundaryField()
                )
              - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        fvm.source() =
        (
            rDtCoef*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V0();
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*(vf.oldTime() - vf.oldTime().oldTime())
                 - offCentre_(ddt0());
        }

        fvm.source() =
        (
            rDtCoef*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
CrankNicolsonDdtScheme<Type>::fvmDdt
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + rho.name() + ',' + vf.name() + ')',
            rho.dimensions()*vf.dimensions()
        );

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalar rDtCoef = rDtCoef_(ddt0).value();
    fvm.diag() = rDtCoef*rho.value()*mesh().V();

    vf.oldTime().oldTime();

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*rho.value()*
                (
                    mesh().V0()*vf.oldTime().internalField()
                  - mesh().V00()*vf.oldTime().oldTime().internalField()
                )
              - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*rho.value()*
                (
                    vf.oldTime().boundaryField()
                  - vf.oldTime().oldTime().boundaryField()
                )
              - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        fvm.source() =
        (
            rDtCoef*rho.value()*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V0();
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*rho*(vf.oldTime() - vf.oldTime().oldTime())
                 - offCentre_(ddt0());
        }

        fvm.source() =
        (
            rDtCoef*rho.value()*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
CrankNicolsonDdtScheme<Type>::fvmDdt
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + rho.name() + ',' + vf.name() + ')',
            rho.dimensions()*vf.dimensions()
        );

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalar rDtCoef = rDtCoef_(ddt0).value();
    fvm.diag() = rDtCoef*rho.internalField()*mesh().V();

    vf.oldTime().oldTime();
    rho.oldTime().oldTime();

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()*rho.oldTime().internalField()
                   *vf.oldTime().internalField()
                  - mesh().V00()*rho.oldTime().oldTime().internalField()
                   *vf.oldTime().oldTime().internalField()
                )
              - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()
                  - rho.oldTime().oldTime().boundaryField()
                   *vf.oldTime().oldTime().boundaryField()
                )
              - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        fvm.source() =
        (
            rDtCoef*rho.oldTime().internalField()*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V0();
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*
            (
                rho.oldTime()*vf.oldTime()
              - rho.oldTime().oldTime()*vf.oldTime().oldTime()
            ) - offCentre_(ddt0());
        }

        fvm.source() =
        (
            rDtCoef*rho.oldTime().internalField()*vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
CrankNicolsonDdtScheme<Type>::fvmDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddt0(" + alpha.name() + ',' + rho.name() + ',' + vf.name() + ')',
            alpha.dimensions()*rho.dimensions()*vf.dimensions()
        );

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            alpha.dimensions()*rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    const scalar rDtCoef = rDtCoef_(ddt0).value();
    fvm.diag() = rDtCoef*alpha.internalField()*rho.internalField()*mesh().V();

    vf.oldTime().oldTime();
    alpha.oldTime().oldTime();
    rho.oldTime().oldTime();

    if (mesh().moving())
    {
        if (evaluate(ddt0))
        {
            const scalar rDtCoef0 = rDtCoef0_(ddt0).value();

            ddt0.internalField() =
            (
                rDtCoef0*
                (
                    mesh().V0()
                   *alpha.oldTime().internalField()
                   *rho.oldTime().internalField()
                   *vf.oldTime().internalField()

                  - mesh().V00()
                   *alpha.oldTime().oldTime().internalField()
                   *rho.oldTime().oldTime().internalField()
                   *vf.oldTime().oldTime().internalField()
                )
              - mesh().V00()*offCentre_(ddt0.internalField())
            )/mesh().V0();

            ddt0.boundaryField() =
            (
                rDtCoef0*
                (
                    alpha.oldTime().boundaryField()
                   *rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()

                  - alpha.oldTime().oldTime().boundaryField()
                   *rho.oldTime().oldTime().boundaryField()
                   *vf.oldTime().oldTime().boundaryField()
                )
              - offCentre_(ff(ddt0.boundaryField()))
            );
        }

        fvm.source() =
        (
            rDtCoef
           *alpha.oldTime().internalField()
           *rho.oldTime().internalField()
           *vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V0();
    }
    else
    {
        if (evaluate(ddt0))
        {
            ddt0 = rDtCoef0_(ddt0)*
            (
                alpha.oldTime()
               *rho.oldTime()
               *vf.oldTime()

              - alpha.oldTime().oldTime()
               *rho.oldTime().oldTime()
               *vf.oldTime().oldTime()
            ) - offCentre_(ddt0());
        }

        fvm.source() =
        (
            rDtCoef
           *alpha.oldTime().internalField()
           *rho.oldTime().internalField()
           *vf.oldTime().internalField()
          + offCentre_(ddt0.internalField())
        )*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<typename CrankNicolsonDdtScheme<Type>::fluxFieldType>
CrankNicolsonDdtScheme<Type>::fvcDdtUfCorr
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddtCorrDdt0(" + U.name() + ')',
            U.dimensions()
        );

    DDt0Field<GeometricField<Type, fvsPatchField, surfaceMesh> >& dUfdt0 =
        ddt0_<GeometricField<Type, fvsPatchField, surfaceMesh> >
        (
            "ddtCorrDdt0(" + Uf.name() + ')',
            Uf.dimensions()
        );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (evaluate(ddt0))
    {
        ddt0 =
            rDtCoef0_(ddt0)*(U.oldTime() - U.oldTime().oldTime())
          - offCentre_(ddt0());
    }

    if (evaluate(dUfdt0))
    {
        dUfdt0 =
            rDtCoef0_(dUfdt0)*(Uf.oldTime() - Uf.oldTime().oldTime())
          - offCentre_(dUfdt0());
    }

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
            this->fvcDdtPhiCoeff(U.oldTime(), mesh().Sf() & Uf.oldTime())
           *(
                mesh().Sf()
              & (
                    (rDtCoef*Uf.oldTime() + offCentre_(dUfdt0()))
                  - fvc::interpolate(rDtCoef*U.oldTime() + offCentre_(ddt0()))
                )
            )
        )
    );
}


template<class Type>
tmp<typename CrankNicolsonDdtScheme<Type>::fluxFieldType>
CrankNicolsonDdtScheme<Type>::fvcDdtPhiCorr
(
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
        ddt0_<GeometricField<Type, fvPatchField, volMesh> >
        (
            "ddtCorrDdt0(" + U.name() + ')',
            U.dimensions()
        );

    DDt0Field<fluxFieldType>& dphidt0 =
        ddt0_<fluxFieldType>
        (
            "ddtCorrDdt0(" + phi.name() + ')',
            phi.dimensions()
        );

    dimensionedScalar rDtCoef = rDtCoef_(ddt0);

    if (evaluate(ddt0))
    {
        ddt0 =
            rDtCoef0_(ddt0)*(U.oldTime() - U.oldTime().oldTime())
          - offCentre_(ddt0());
    }

    if (evaluate(dphidt0))
    {
        dphidt0 =
            rDtCoef0_(dphidt0)*(phi.oldTime() - phi.oldTime().oldTime())
          - offCentre_(dphidt0());
    }

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
            this->fvcDdtPhiCoeff(U.oldTime(), phi.oldTime())
           *(
                (rDtCoef*phi.oldTime() + offCentre_(dphidt0()))
              - fvc::dotInterpolate
                (
                    mesh().Sf(),
                    rDtCoef*U.oldTime() + offCentre_(ddt0())
                )
            )
        )
    );
}


template<class Type>
tmp<typename CrankNicolsonDdtScheme<Type>::fluxFieldType>
CrankNicolsonDdtScheme<Type>::fvcDdtUfCorr
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
    if
    (
        U.dimensions() == dimVelocity
     && Uf.dimensions() == rho.dimensions()*dimVelocity
    )
    {
        DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
            ddt0_<GeometricField<Type, fvPatchField, volMesh> >
            (
                "ddtCorrDdt0(" + rho.name() + ',' + U.name() + ')',
                rho.dimensions()*U.dimensions()
            );

        DDt0Field<GeometricField<Type, fvsPatchField, surfaceMesh> >& dUfdt0 =
            ddt0_<GeometricField<Type, fvsPatchField, surfaceMesh> >
            (
                "ddtCorrDdt0(" + Uf.name() + ')',
                Uf.dimensions()
            );

        dimensionedScalar rDtCoef = rDtCoef_(ddt0);

        GeometricField<Type, fvPatchField, volMesh> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        if (evaluate(ddt0))
        {
            ddt0 =
                rDtCoef0_(ddt0)
               *(rhoU0 - rho.oldTime().oldTime()*U.oldTime().oldTime())
              - offCentre_(ddt0());
        }

        if (evaluate(dUfdt0))
        {
            dUfdt0 =
                rDtCoef0_(dUfdt0)
               *(Uf.oldTime() - Uf.oldTime().oldTime())
              - offCentre_(dUfdt0());
        }

        tmp<fluxFieldType> ddtCorr
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
                this->fvcDdtPhiCoeff(rhoU0, mesh().Sf() & Uf.oldTime())
               *(
                    mesh().Sf()
                  & (
                        (rDtCoef*Uf.oldTime() + offCentre_(dUfdt0()))
                      - fvc::interpolate(rDtCoef*rhoU0 + offCentre_(ddt0()))
                    )
                )
            )
        );

        return ddtCorr;
    }
    else if
    (
        U.dimensions() == rho.dimensions()*dimVelocity
     && Uf.dimensions() == rho.dimensions()*dimVelocity
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
tmp<typename CrankNicolsonDdtScheme<Type>::fluxFieldType>
CrankNicolsonDdtScheme<Type>::fvcDdtPhiCorr
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    if
    (
        U.dimensions() == dimVelocity
     && phi.dimensions() == rho.dimensions()*dimVelocity*dimArea
    )
    {
        DDt0Field<GeometricField<Type, fvPatchField, volMesh> >& ddt0 =
            ddt0_<GeometricField<Type, fvPatchField, volMesh> >
            (
                "ddtCorrDdt0(" + rho.name() + ',' + U.name() + ')',
                rho.dimensions()*U.dimensions()
            );

        DDt0Field<fluxFieldType>& dphidt0 =
            ddt0_<fluxFieldType>
            (
                "ddtCorrDdt0(" + phi.name() + ')',
                phi.dimensions()
            );

        dimensionedScalar rDtCoef = rDtCoef_(ddt0);

        GeometricField<Type, fvPatchField, volMesh> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        if (evaluate(ddt0))
        {
            ddt0 =
                rDtCoef0_(ddt0)
               *(rhoU0 - rho.oldTime().oldTime()*U.oldTime().oldTime())
              - offCentre_(ddt0());
        }

        if (evaluate(dphidt0))
        {
            dphidt0 =
                rDtCoef0_(dphidt0)
              *(phi.oldTime() - phi.oldTime().oldTime())
              - offCentre_(dphidt0());
        }

        tmp<fluxFieldType> ddtCorr
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
                this->fvcDdtPhiCoeff(rhoU0, phi.oldTime())
               *(
                    (rDtCoef*phi.oldTime() + offCentre_(dphidt0()))
                  - fvc::dotInterpolate
                    (
                        mesh().Sf(),
                        rDtCoef*rhoU0 + offCentre_(ddt0())
                    )
                )
            )
        );

        return ddtCorr;
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
tmp<surfaceScalarField> CrankNicolsonDdtScheme<Type>::meshPhi
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    DDt0Field<surfaceScalarField>& meshPhi0 = ddt0_<surfaceScalarField>
    (
        "meshPhiCN_0",
        dimVolume
    );

    if (evaluate(meshPhi0))
    {
        meshPhi0 =
            coef0_(meshPhi0)*mesh().phi().oldTime() - offCentre_(meshPhi0());
    }

    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            IOobject
            (
                mesh().phi().name(),
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            coef_(meshPhi0)*mesh().phi() - offCentre_(meshPhi0())
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#endif

// ************************************************************************* //
