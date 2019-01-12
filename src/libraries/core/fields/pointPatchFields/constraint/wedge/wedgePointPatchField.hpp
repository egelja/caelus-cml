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
    CML::wedgePointPatchField

Description
    Wedge front and back plane patch field

SourceFiles
    wedgePointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef wedgePointPatchField_H
#define wedgePointPatchField_H

#include "pointPatchField.hpp"
#include "wedgePointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class wedgePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class wedgePointPatchField
:
    public pointPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(wedgePointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        wedgePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        wedgePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        wedgePointPatchField
        (
            const wedgePointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new wedgePointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        wedgePointPatchField
        (
            const wedgePointPatchField<Type>&,
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
                new wedgePointPatchField<Type>
                (
                    *this, iF
                )
            );
        }


    // Member functions

        //- Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return type();
            }

        // Evaluation functions

            //- Update the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "transformField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::wedgePointPatchField<Type>::wedgePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(p, iF)
{}


template<class Type>
CML::wedgePointPatchField<Type>::wedgePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    pointPatchField<Type>(p, iF, dict)
{
    if (!isType<wedgePointPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "patch " << this->patch().index() << " not wedge type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
CML::wedgePointPatchField<Type>::wedgePointPatchField
(
    const wedgePointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper&
)
:
    pointPatchField<Type>(p, iF)
{
    if (!isType<wedgePointPatch>(this->patch()))
    {
        FatalErrorInFunction
            << "Field type does not correspond to patch type for patch "
            << this->patch().index() << "." << endl
            << "Field type: " << typeName << endl
            << "Patch type: " << this->patch().type()
            << exit(FatalError);
    }
}


template<class Type>
CML::wedgePointPatchField<Type>::wedgePointPatchField
(
    const wedgePointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::wedgePointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    // In order to ensure that the wedge patch is always flat, take the
    // normal vector from the first point
    const vector& nHat = this->patch().pointNormals()[0];

    tmp<Field<Type> > tvalues =
        transform(I - nHat*nHat, this->patchInternalField());

    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, tvalues());
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
