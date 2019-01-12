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
    CML::mappedFixedInternalValueFvPatchField

Description
    This boundary condition maps the boundary and internal values of a
    neighbour patch field to the boundary and internal values of *this.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        fieldName    | name of field to be mapped | no       | this field name
        setAverage   | flag to activate setting of average value | yes |
        average      | average value to apply if \c setAverage = yes | yes |
    \endtable

    \verbatim
    myPatch
    {
        type            mappedFixedInternalValue;
        fieldName       T;
        setAverage      no;
        average         0;
        value           uniform 0;
    }
    \endverbatim

Note
    This boundary condition can only be applied to patches that are of
    the \c mappedPolyPatch type.

SeeAlso
    CML::mappedPatchBase
    CML::mappedPolyPatch
    CML::mappedFvPatch
    CMK::mappedFixedValueFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef mappedFixedInternalValueFvPatchField_H
#define mappedFixedInternalValueFvPatchField_H

#include "mappedFixedValueFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
           Class mappedFixedInternalValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedInternalValueFvPatchField
:
    public mappedFixedValueFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("mappedFixedInternalValue");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        // mappedFixedInternalValueFvPatchField  onto a new patch
        mappedFixedInternalValueFvPatchField
        (
            const mappedFixedInternalValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedFixedInternalValueFvPatchField
        (
            const mappedFixedInternalValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedInternalValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mappedFixedInternalValueFvPatchField
        (
            const mappedFixedInternalValueFvPatchField<Type>&,
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
                new mappedFixedInternalValueFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UIndirectList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedFixedValueFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
    const mappedFixedInternalValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mappedFixedValueFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mappedFixedValueFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
CML::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
    const mappedFixedInternalValueFvPatchField<Type>& ptf
)
:
    mappedFixedValueFvPatchField<Type>(ptf)
{}


template<class Type>
CML::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
    const mappedFixedInternalValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedFixedValueFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::mappedFixedInternalValueFvPatchField<Type>::updateCoeffs()
{	
	typedef GeometricField<Type, fvPatchField, volMesh> FieldType;

    if (this->updated())
    {
        return;
    }

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;

    // Retrieve the neighbour values and assign to this patch boundary field
    mappedFixedValueFvPatchField<Type>::updateCoeffs();

    // Get the coupling information from the mappedPatchBase
    const mappedPatchBase& mpp =
        refCast<const mappedPatchBase>(this->patch().patch());
    const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());

    Field<Type> nbrIntFld;

    switch (mpp.mode())
    {
        case mappedPatchBase::NEARESTCELL:
        {
            FatalErrorInFunction
                << "Cannot apply "
                << mappedPatchBase::sampleModeNames_
                   [
                       mappedPatchBase::NEARESTCELL
                   ]
                << " mapping mode for patch " << this->patch().name()
                << exit(FatalError);

            break;
        }
        case mappedPatchBase::NEARESTPATCHFACE:
        case mappedPatchBase::NEARESTPATCHFACEAMI:
        {
            const label samplePatchI = mpp.samplePolyPatch().index();
            const fvPatchField<Type>& nbrPatchField =
                this->sampleField().boundaryField()[samplePatchI];
            nbrIntFld = nbrPatchField.patchInternalField();
            mpp.distribute(nbrIntFld);

            break;
        }
        case mappedPatchBase::NEARESTFACE:
        {
            Field<Type> allValues(nbrMesh.nFaces(), pTraits<Type>::zero);

            const FieldType& nbrField = this->sampleField();

            forAll(nbrField.boundaryField(), patchI)
            {
                const fvPatchField<Type>& pf = nbrField.boundaryField()[patchI];
                const Field<Type> pif(pf.patchInternalField());

                label faceStart = pf.patch().start();

                forAll(pf, faceI)
                {
                    allValues[faceStart++] = pif[faceI];
                }
            }

            mpp.distribute(allValues);
            nbrIntFld.transfer(allValues);

            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown sampling mode: " << mpp.mode()
                << abort(FatalError);
        }
    }

    // Restore tag
    UPstream::msgType() = oldTag;

    // Assign to (this) patch internal field its neighbour values
    Field<Type>& intFld = const_cast<Field<Type>&>(this->internalField());
    UIndirectList<Type>(intFld, this->patch().faceCells()) = nbrIntFld;
}


template<class Type>
void CML::mappedFixedInternalValueFvPatchField<Type>::write
(
    Ostream& os
) const
{
    mappedFixedValueFvPatchField<Type>::write(os);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
