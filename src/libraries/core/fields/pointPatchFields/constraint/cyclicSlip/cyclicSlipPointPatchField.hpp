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
    CML::cyclicSlipPointPatchField

Description
    Cyclic + slip constraints


\*---------------------------------------------------------------------------*/

#ifndef cyclicSlipPointPatchField_H
#define cyclicSlipPointPatchField_H

#include "cyclicPointPatchField.hpp"
#include "cyclicSlipPointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class cyclicSlipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicSlipPointPatchField
:
    public cyclicPointPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(cyclicSlipPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        cyclicSlipPointPatchField
        (
            const cyclicSlipPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new cyclicSlipPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        cyclicSlipPointPatchField
        (
            const cyclicSlipPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new cyclicSlipPointPatchField<Type>
                (
                    *this, iF
                )
            );
        }


    // Member functions

        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "transformField.hpp"
#include "symmTransformField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::cyclicSlipPointPatchField<Type>::cyclicSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    cyclicPointPatchField<Type>(p, iF)
{}


template<class Type>
CML::cyclicSlipPointPatchField<Type>::cyclicSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    cyclicPointPatchField<Type>(p, iF, dict)
{}


template<class Type>
CML::cyclicSlipPointPatchField<Type>::cyclicSlipPointPatchField
(
    const cyclicSlipPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    cyclicPointPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::cyclicSlipPointPatchField<Type>::cyclicSlipPointPatchField
(
    const cyclicSlipPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    cyclicPointPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::cyclicSlipPointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    const vectorField& nHat = this->patch().pointNormals();

    tmp<Field<Type> > tvalues =
    (
        (
            this->patchInternalField()
          + transform(I - 2.0*sqr(nHat), this->patchInternalField())
        )/2.0
    );

    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, tvalues());
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
