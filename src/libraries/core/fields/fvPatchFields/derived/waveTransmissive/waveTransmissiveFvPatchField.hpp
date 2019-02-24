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
    CML::waveTransmissiveFvPatchField

Description
    This boundary condition provides a wave transmissive outflow condition,
    based on solving DDt(W, field) = 0 at the boundary \c W is the wave velocity
    and \c field is the field to which this boundary condition is applied.

    The wave speed is calculated using:

        \f[
            w_p = \frac{\phi_p}{|Sf|} + \sqrt{\frac{\gamma}{\psi_p}}
        \f]

    where

    \vartable
        w_p     | patch wave speed
        \phi_p  | patch face flux
        \psi_p  | patch compressibility
        Sf      | patch face area vector
        \gamma  | ratio of specific heats
    \endvartable

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        rho          | density field name      | no          | rho
        psi          | compressibility field name | no       | psi
        gamma        | ratio of specific heats (Cp/Cv) | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            waveTransmissive;
        phi             phi;
        psi             psi;
        gamma           1.4;
    }
    \endverbatim

See also
    CML::advectiveFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef waveTransmissiveFvPatchField_HPP
#define waveTransmissiveFvPatchField_HPP

#include "advectiveFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class waveTransmissiveFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class waveTransmissiveFvPatchField
:
    public advectiveFvPatchField<Type>
{

    // Private data

        //- Name of the compressibility field used to calculate the wave speed
        word psiName_;

        //- Heat capacity ratio
        scalar gamma_;


public:

    //- Runtime type information
    TypeName("waveTransmissive");


    // Constructors

        //- Construct from patch and internal field
        waveTransmissiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        waveTransmissiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given waveTransmissiveFvPatchField
        //  onto a new patch
        waveTransmissiveFvPatchField
        (
            const waveTransmissiveFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        waveTransmissiveFvPatchField
        (
            const waveTransmissiveFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new waveTransmissiveFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        waveTransmissiveFvPatchField
        (
            const waveTransmissiveFvPatchField&,
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
                new waveTransmissiveFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the heat capacity ratio
            scalar gamma() const
            {
                return gamma_;
            }

            //- Return reference to the heat capacity ratio to allow adjustment
            scalar& gamma()
            {
                return gamma_;
            }


        // Evaluation functions

            //- Calculate and return the advection speed at the boundary
            virtual tmp<scalarField> advectionSpeed() const;


        //- Write
        virtual void write(Ostream&) const;
};


} // End namespace CML


#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "EulerDdtScheme.hpp"
#include "CrankNicolsonDdtScheme.hpp"
#include "backwardDdtScheme.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    advectiveFvPatchField<Type>(p, iF),
    psiName_("psi"),
    gamma_(0.0)
{}


template<class Type>
CML::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    advectiveFvPatchField<Type>(ptf, p, iF, mapper),
    psiName_(ptf.psiName_),
    gamma_(ptf.gamma_)
{}


template<class Type>
CML::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    advectiveFvPatchField<Type>(p, iF, dict),
    psiName_(dict.lookupOrDefault<word>("psi", "psi")),
    gamma_(readScalar(dict.lookup("gamma")))
{}


template<class Type>
CML::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptpsf
)
:
    advectiveFvPatchField<Type>(ptpsf),
    psiName_(ptpsf.psiName_),
    gamma_(ptpsf.gamma_)
{}


template<class Type>
CML::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptpsf,
    const DimensionedField<Type, volMesh>& iF
)
:
    advectiveFvPatchField<Type>(ptpsf, iF),
    psiName_(ptpsf.psiName_),
    gamma_(ptpsf.gamma_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::scalarField>
CML::waveTransmissiveFvPatchField<Type>::advectionSpeed() const
{
    // Lookup the velocity and compressibility of the patch
    const fvPatchField<scalar>& psip =
        this->patch().template
            lookupPatchField<volScalarField, scalar>(psiName_);

    const surfaceScalarField& phi =
        this->db().template lookupObject<surfaceScalarField>(this->phiName_);

    fvsPatchField<scalar> phip =
        this->patch().template
            lookupPatchField<surfaceScalarField, scalar>(this->phiName_);

    if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const fvPatchScalarField& rhop =
            this->patch().template
                lookupPatchField<volScalarField, scalar>(this->rhoName_);

        phip /= rhop;
    }

    // Calculate the speed of the field wave w
    // by summing the component of the velocity normal to the boundary
    // and the speed of sound (sqrt(gamma_/psi)).
    return phip/this->patch().magSf() + sqrt(gamma_/psip);
}


template<class Type>
void CML::waveTransmissiveFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);

    this->template
        writeEntryIfDifferent<word>(os, "phi", "phi", this->phiName_);
    this->template
        writeEntryIfDifferent<word>(os, "rho", "rho", this->rhoName_);
    this->template
        writeEntryIfDifferent<word>(os, "psi", "psi", psiName_);

    os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;

    if (this->lInf_ > SMALL)
    {
        os.writeKeyword("fieldInf") << this->fieldInf_
            << token::END_STATEMENT << nl;
        os.writeKeyword("lInf") << this->lInf_
            << token::END_STATEMENT << nl;
    }

    this->writeEntry("value", os);
}


#endif
