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
    CML::valuePointPatchField

Description
    CML::valuePointPatchField


\*---------------------------------------------------------------------------*/

#ifndef valuePointPatchField_H
#define valuePointPatchField_H

#include "pointPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class valuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class valuePointPatchField
:
    public pointPatchField<Type>,
    public Field<Type>
{
    // Private Member Functions

        void checkFieldSize() const;


public:

    //- Runtime type information
    TypeName("value");


    // Constructors

        //- Construct from patch and internal field
        valuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        valuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&,
            const bool valueRequired=true
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        valuePointPatchField
        (
            const valuePointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new valuePointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        valuePointPatchField
        (
            const valuePointPatchField<Type>&,
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
                new valuePointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        // Access

            //- Return size
            label size() const
            {
                return Field<Type>::size();
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const pointPatchFieldMapper&
            );

            //- Reverse map the given PointPatchField onto
            //  this PointPatchField
            virtual void rmap
            (
                const pointPatchField<Type>&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );


        //- Write
        virtual void write(Ostream&) const;


    // Member operators

        // Assignment operators

            virtual void operator=
            (
                const valuePointPatchField<Type>&
            );

            virtual void operator=
            (
                const pointPatchField<Type>&
            );

            virtual void operator=(const Field<Type>&);
            virtual void operator=(const Type&);


        // Force an assignment irrespective of form of patch

            virtual void operator==
            (
                const valuePointPatchField<Type>&
            );

            virtual void operator==
            (
                const pointPatchField<Type>&
            );

            virtual void operator==(const Field<Type>&);
            virtual void operator==(const Type&);
};


// This function is added to overide the hack in pointPatchField.H
// which enables simple backward compatability with verions using
// referenceLevel in GeometicField
template<class Type>
tmp<Field<Type> > operator+
(
    const valuePointPatchField<Type>& vsppf,
    const Type& t
)
{
    return static_cast<const Field<Type>&>(vsppf) + t;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pointPatchFieldMapper.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::valuePointPatchField<Type>::checkFieldSize() const
{
    if (this->size() != this->patch().size())
    {
        FatalErrorInFunction
            << "field does not correspond to patch. " << endl
            << "Field size: " << size() << " patch size: "
            << this->patch().size()
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

template<class Type>
CML::valuePointPatchField<Type>::valuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(p, iF),
    Field<Type>(p.size())
{}


template<class Type>
CML::valuePointPatchField<Type>::valuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict,
    const bool valueRequired
)
:
    pointPatchField<Type>(p, iF, dict),
    Field<Type>(p.size())
{
    if (dict.found("value"))
    {
        Field<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else if (!valueRequired)
    {
        Field<Type>::operator=(pTraits<Type>::zero);
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Essential entry 'value' missing"
            << exit(FatalIOError);
    }
}


template<class Type>
CML::valuePointPatchField<Type>::valuePointPatchField
(
    const valuePointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    pointPatchField<Type>(p, iF),
    Field<Type>(ptf, mapper)
{}


template<class Type>
CML::valuePointPatchField<Type>::valuePointPatchField
(
    const valuePointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(ptf, iF),
    Field<Type>(ptf)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::valuePointPatchField<Type>::autoMap
(
    const pointPatchFieldMapper& m
)
{
    Field<Type>::autoMap(m);
}


template<class Type>
void CML::valuePointPatchField<Type>::rmap
(
    const pointPatchField<Type>& ptf,
    const labelList& addr
)
{
    Field<Type>::rmap
    (
        refCast<const valuePointPatchField<Type> >
        (
            ptf
        ),
        addr
    );
}


template<class Type>
void CML::valuePointPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, *this);

    pointPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::valuePointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, *this);

    pointPatchField<Type>::evaluate();
}


template<class Type>
void CML::valuePointPatchField<Type>::write(Ostream& os) const
{
    pointPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::valuePointPatchField<Type>::operator=
(
    const valuePointPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void CML::valuePointPatchField<Type>::operator=
(
    const pointPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf.patchInternalField());
}


template<class Type>
void CML::valuePointPatchField<Type>::operator=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void CML::valuePointPatchField<Type>::operator=
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


// Force an assignment
template<class Type>
void CML::valuePointPatchField<Type>::operator==
(
    const valuePointPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void CML::valuePointPatchField<Type>::operator==
(
    const pointPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf.patchInternalField());
}


template<class Type>
void CML::valuePointPatchField<Type>::operator==
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void CML::valuePointPatchField<Type>::operator==
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
