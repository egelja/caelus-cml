/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::advectiveFvPatchField

Description
    This boundary condition provides an advective outflow condition, based on
    solving DDt(W, field) = 0 at the boundary where \c W is the wave velocity
    and \c field is the field to which this boundary condition is applied.

    The standard (Euler, backward, CrankNicolson, localEuler) time schemes are
    supported.  Additionally an optional mechanism to relax the value at
    the boundary to a specified far-field value is provided which is
    switched on by specifying the relaxation length-scale \c lInf and the
    far-field value \c fieldInf.

    The flow/wave speed \c (w) at the outlet is provided by the virtual function
    advectionSpeed() the default implementation of which requires the name of
    the flux field \c (phi) and optionally the density \c (rho) if the
    mass-flux rather than the volumetric-flux is given.

    The flow/wave speed at the outlet can be changed by deriving a specialised
    BC from this class and over-riding advectionSpeed()  e.g. in
    waveTransmissiveFvPatchField the advectionSpeed() calculates and returns
    the flow-speed plus the acoustic wave speed creating an acoustic wave
    transmissive boundary condition.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        rho          | density field name      | no          | rho
        fieldInf     | value of field beyond patch | no      |
        lInf         | distance beyond patch for \c fieldInf | no |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            advective;
        phi             phi;
    }
    \endverbatim

Note
    If \c lInf is specified, \c fieldInf will be required; \c rho is only
    required in the case of a mass-based flux.

SourceFiles
    advectiveFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef advectiveFvPatchField_H
#define advectiveFvPatchField_H

#include "mixedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class advectiveFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class advectiveFvPatchField
:
    public mixedFvPatchField<Type>
{
protected:

    // Private data

        //- Name of the flux transporting the field
        word phiName_;

        //- Name of the density field used to normalise the mass flux
        //  if necessary
        word rhoName_;

        //- Field value of the far-field
        Type fieldInf_;

        //- Relaxation length-scale
        scalar lInf_;


public:

    //- Runtime type information
    TypeName("advective");


    // Constructors

        //- Construct from patch and internal field
        advectiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        advectiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given advectiveFvPatchField
        //  onto a new patch
        advectiveFvPatchField
        (
            const advectiveFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        advectiveFvPatchField
        (
            const advectiveFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new advectiveFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        advectiveFvPatchField
        (
            const advectiveFvPatchField&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new advectiveFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the field at infinity
            const Type& fieldInf() const
            {
                return fieldInf_;
            }

            //- Return reference to the field at infinity to allow adjustment
            Type& fieldInf()
            {
                return fieldInf_;
            }

            //- Return the relaxation length-scale
            scalar lInf() const
            {
                return lInf_;
            }

            //- Return reference to the relaxation length-scale
            //  to allow adjustment
            scalar& lInf()
            {
                return lInf_;
            }


        // Evaluation functions

            //- Calculate and return the advection speed at the boundary
            virtual tmp<scalarField> advectionSpeed() const;

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "EulerDdtScheme.hpp"
#include "CrankNicolsonDdtScheme.hpp"
#include "backwardDdtScheme.hpp"
#include "localEulerDdtScheme.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::advectiveFvPatchField<Type>::advectiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF),
    phiName_("phi"),
    rhoName_("rho"),
    fieldInf_(pTraits<Type>::zero),
    lInf_(-GREAT)
{
    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
CML::advectiveFvPatchField<Type>::advectiveFvPatchField
(
    const advectiveFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    fieldInf_(ptf.fieldInf_),
    lInf_(ptf.lInf_)
{}


template<class Type>
CML::advectiveFvPatchField<Type>::advectiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    rhoName_(dict.lookupOrDefault<word>("rho", "rho")),
    fieldInf_(pTraits<Type>::zero),
    lInf_(-GREAT)
{
    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(this->patchInternalField());
    }

    this->refValue() = *this;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;

    if (dict.readIfPresent("lInf", lInf_))
    {
        dict.lookup("fieldInf") >> fieldInf_;

        if (lInf_ < 0.0)
        {
            FatalIOErrorInFunction(dict)
                << "unphysical lInf specified (lInf < 0)\n"
                << "    on patch " << this->patch().name()
                << " of field " << this->dimensionedInternalField().name()
                << " in file " << this->dimensionedInternalField().objectPath()
                << exit(FatalIOError);
        }
    }
}


template<class Type>
CML::advectiveFvPatchField<Type>::advectiveFvPatchField
(
    const advectiveFvPatchField& ptpsf
)
:
    mixedFvPatchField<Type>(ptpsf),
    phiName_(ptpsf.phiName_),
    rhoName_(ptpsf.rhoName_),
    fieldInf_(ptpsf.fieldInf_),
    lInf_(ptpsf.lInf_)
{}


template<class Type>
CML::advectiveFvPatchField<Type>::advectiveFvPatchField
(
    const advectiveFvPatchField& ptpsf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ptpsf, iF),
    phiName_(ptpsf.phiName_),
    rhoName_(ptpsf.rhoName_),
    fieldInf_(ptpsf.fieldInf_),
    lInf_(ptpsf.lInf_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::scalarField>
CML::advectiveFvPatchField<Type>::advectionSpeed() const
{
    const surfaceScalarField& phi =
        this->db().objectRegistry::template lookupObject<surfaceScalarField>
        (phiName_);

    fvsPatchField<scalar> phip =
        this->patch().template lookupPatchField<surfaceScalarField, scalar>
        (
            phiName_
        );

    if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const fvPatchScalarField& rhop =
            this->patch().template lookupPatchField<volScalarField, scalar>
            (
                rhoName_
            );

        return phip/(rhop*this->patch().magSf());
    }
    else
    {
        return phip/this->patch().magSf();
    }
}


template<class Type>
void CML::advectiveFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const fvMesh& mesh = this->dimensionedInternalField().mesh();

    word ddtScheme
    (
        mesh.ddtScheme(this->dimensionedInternalField().name())
    );
    scalar deltaT = this->db().time().deltaTValue();

    const GeometricField<Type, fvPatchField, volMesh>& field =
        this->db().objectRegistry::template
        lookupObject<GeometricField<Type, fvPatchField, volMesh> >
        (
            this->dimensionedInternalField().name()
        );

    // Calculate the advection speed of the field wave
    // If the wave is incoming set the speed to 0.
    const scalarField w(CML::max(advectionSpeed(), scalar(0)));

    // Calculate the field wave coefficient alpha (See notes)
    const scalarField alpha(w*deltaT*this->patch().deltaCoeffs());

    label patchi = this->patch().index();

    // Non-reflecting outflow boundary
    // If lInf_ defined setup relaxation to the value fieldInf_.
    if (lInf_ > 0)
    {
        // Calculate the field relaxation coefficient k (See notes)
        const scalarField k(w*deltaT/lInf_);

        if
        (
            ddtScheme == fv::EulerDdtScheme<scalar>::typeName
         || ddtScheme == fv::CrankNicolsonDdtScheme<scalar>::typeName
        )
        {
            this->refValue() =
            (
                field.oldTime().boundaryField()[patchi] + k*fieldInf_
            )/(1.0 + k);

            this->valueFraction() = (1.0 + k)/(1.0 + alpha + k);
        }
        else if (ddtScheme == fv::backwardDdtScheme<scalar>::typeName)
        {
            this->refValue() =
            (
                2.0*field.oldTime().boundaryField()[patchi]
              - 0.5*field.oldTime().oldTime().boundaryField()[patchi]
              + k*fieldInf_
            )/(1.5 + k);

            this->valueFraction() = (1.5 + k)/(1.5 + alpha + k);
        }
        else if
        (
            ddtScheme == fv::localEulerDdtScheme<scalar>::typeName
        )
        {
            const volScalarField& rDeltaT =
                fv::localEulerDdt::localRDeltaT(mesh);

            // Calculate the field wave coefficient alpha (See notes)
            const scalarField alpha
            (
                w*this->patch().deltaCoeffs()/rDeltaT.boundaryField()[patchi]
            );

            // Calculate the field relaxation coefficient k (See notes)
            const scalarField k(w/(rDeltaT.boundaryField()[patchi]*lInf_));

            this->refValue() =
            (
                field.oldTime().boundaryField()[patchi] + k*fieldInf_
            )/(1.0 + k);

            this->valueFraction() = (1.0 + k)/(1.0 + alpha + k);
        }
        else
        {
            FatalErrorInFunction
                << "    Unsupported temporal differencing scheme : "
                << ddtScheme
                << "\n    on patch " << this->patch().name()
                << " of field " << this->dimensionedInternalField().name()
                << " in file " << this->dimensionedInternalField().objectPath()
                << exit(FatalError);
        }
    }
    else
    {
        if
        (
            ddtScheme == fv::EulerDdtScheme<scalar>::typeName
         || ddtScheme == fv::CrankNicolsonDdtScheme<scalar>::typeName
        )
        {
            this->refValue() = field.oldTime().boundaryField()[patchi];

            this->valueFraction() = 1.0/(1.0 + alpha);
        }
        else if (ddtScheme == fv::backwardDdtScheme<scalar>::typeName)
        {
            this->refValue() =
            (
                2.0*field.oldTime().boundaryField()[patchi]
              - 0.5*field.oldTime().oldTime().boundaryField()[patchi]
            )/1.5;

            this->valueFraction() = 1.5/(1.5 + alpha);
        }
        else if
        (
            ddtScheme == fv::localEulerDdtScheme<scalar>::typeName
        )
        {
            const volScalarField& rDeltaT =
                fv::localEulerDdt::localRDeltaT(mesh);

            // Calculate the field wave coefficient alpha (See notes)
            const scalarField alpha
            (
                w*this->patch().deltaCoeffs()/rDeltaT.boundaryField()[patchi]
            );

            this->refValue() = field.oldTime().boundaryField()[patchi];

            this->valueFraction() = 1.0/(1.0 + alpha);
        }
        else
        {
            FatalErrorInFunction
                << "    Unsupported temporal differencing scheme : "
                << ddtScheme
                << "\n    on patch " << this->patch().name()
                << " of field " << this->dimensionedInternalField().name()
                << " in file " << this->dimensionedInternalField().objectPath()
                << exit(FatalError);
        }
    }

    mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::advectiveFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);

    if (phiName_ != "phi")
    {
        os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    }
    if (rhoName_ != "rho")
    {
        os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
    }

    if (lInf_ > SMALL)
    {
        os.writeKeyword("fieldInf") << fieldInf_ << token::END_STATEMENT << nl;
        os.writeKeyword("lInf") << lInf_ << token::END_STATEMENT << nl;
    }

    this->writeEntry("value", os);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
