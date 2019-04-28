/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
Copyright (C) 2019 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::outletMappedUniformInletFvPatchField

Group
    grpInletBoundaryConditions

Description
    This boundary conditon averages the field over the "outlet" patch specified
    by name "outletPatchName" and applies this as the uniform value of the
    field over this patch. An optional fraction can be supplied to apply at
    fraction of the average outlet value to this patch

    \heading Patch usage

    \table
        Property        | Description             | Required    | Default value
        outletPatchName | name of outlet patch    | yes         |
        phi             | flux field name         | no          | phi
        fraction        | fraction of outlet to   | no          | 1.0
                          apply
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            outletMappedUniformInlet;
        outletPatchName aPatch;
        phi             phi;
        fraction        0.5;
        value           uniform 0;
    }
    \endverbatim

SeeAlso
    CML::fixedValueFvPatchField

\*---------------------------------------------------------------------------*/

#ifndef outletMappedUniformInletFvPatchField_HPP
#define outletMappedUniformInletFvPatchField_HPP

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
            Class outletMappedUniformInletFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class outletMappedUniformInletFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    // Private data

        //- Name of the outlet patch to be mapped
        word outletPatchName_;

       //- Name of the flux transporting the field
        word phiName_;

       //- Fraction of outlet value to map to inlet
       scalar fraction_;


public:

    //- Runtime type information
    TypeName("outletMappedUniformInlet");


    // Constructors

        //- Construct from patch and internal field
        outletMappedUniformInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        outletMappedUniformInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given outletMappedUniformInletFvPatchField
        //  onto a new patch
        outletMappedUniformInletFvPatchField
        (
            const outletMappedUniformInletFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        outletMappedUniformInletFvPatchField
        (
            const outletMappedUniformInletFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new outletMappedUniformInletFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        outletMappedUniformInletFvPatchField
        (
            const outletMappedUniformInletFvPatchField<Type>&,
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
                new outletMappedUniformInletFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Name of the outlet patch to be mapped
            const word& outletPatchName() const
            {
                return outletPatchName_;
            }


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


} // End namespace CML


#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    outletPatchName_(),
    phiName_("phi"),
    fraction_(1.0)
{}


template<class Type>
CML::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
    const outletMappedUniformInletFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    outletPatchName_(ptf.outletPatchName_),
    phiName_(ptf.phiName_),
    fraction_(ptf.fraction_)
{}


template<class Type>
CML::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF, dict),
    outletPatchName_(dict.lookup("outletPatchName")),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    fraction_(dict.lookupOrDefault<scalar>("fraction", 1.0))
{}


template<class Type>
CML::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
    const outletMappedUniformInletFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    outletPatchName_(ptf.outletPatchName_),
    phiName_(ptf.phiName_),
    fraction_(ptf.fraction_)
{}



template<class Type>
CML::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
    const outletMappedUniformInletFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    outletPatchName_(ptf.outletPatchName_),
    phiName_(ptf.phiName_),
    fraction_(ptf.fraction_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::outletMappedUniformInletFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const GeometricField<Type, fvPatchField, volMesh>& f
    (
        dynamic_cast<const GeometricField<Type, fvPatchField, volMesh>&>
        (
            this->dimensionedInternalField()
        )
    );

    const fvPatch& p = this->patch();
    label outletPatchID =
        p.patch().boundaryMesh().findPatchID(outletPatchName_);

    if (outletPatchID < 0)
    {
        FatalErrorIn
        (
            "void outletMappedUniformInletFvPatchField<Type>::updateCoeffs()"
        )   << "Unable to find outlet patch " << outletPatchName_
            << abort(FatalError);
    }

    const fvPatch& outletPatch = p.boundaryMesh()[outletPatchID];

    const fvPatchField<Type>& outletPatchField =
        f.boundaryField()[outletPatchID];

    const surfaceScalarField& phi =
        this->db().objectRegistry::template lookupObject<surfaceScalarField>
        (phiName_);

    const scalarField& outletPatchPhi = phi.boundaryField()[outletPatchID];
    scalar sumOutletPatchPhi = gSum(outletPatchPhi);

    if (sumOutletPatchPhi > SMALL)
    {
        Type averageOutletField =
            fraction_*gSum(outletPatchPhi*outletPatchField)
           /sumOutletPatchPhi;

        this->operator==(averageOutletField);
    }
    else
    {
        Type averageOutletField =
            fraction_*gSum(outletPatch.magSf()*outletPatchField)
           /gSum(outletPatch.magSf());

        this->operator==(averageOutletField);
    }

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::outletMappedUniformInletFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeKeyword("outletPatchName")
        << outletPatchName_ << token::END_STATEMENT << nl;
    if (phiName_ != "phi")
    {
        os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    }
    os.writeKeyword("fraction") << fraction_ << token::END_STATEMENT << nl;
    this->writeEntry("value", os);
}


#endif
