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
    CML::cellMotionFvPatchField

Description
    CML::cellMotionFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef cellMotionFvPatchField_H
#define cellMotionFvPatchField_H

#include "Random.hpp"
#include "fixedValueFvPatchFields.hpp"
#include "fvMesh.hpp"
#include "volMesh.hpp"
#include "pointFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class cellMotionFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cellMotionFvPatchField
:
    public fixedValueFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("cellMotion");


    // Constructors

        //- Construct from patch and internal field
        cellMotionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cellMotionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given cellMotionFvPatchField
        //  onto a new patch
        cellMotionFvPatchField
        (
            const cellMotionFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        cellMotionFvPatchField
        (
            const cellMotionFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new cellMotionFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        cellMotionFvPatchField
        (
            const cellMotionFvPatchField<Type>&,
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
                new cellMotionFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
    const cellMotionFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF)
{
    fvPatchField<Type>::operator=(Field<Type>("value", dict, p.size()));
}


template<class Type>
CML::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
    const cellMotionFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf)
{}


template<class Type>
CML::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
    const cellMotionFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::cellMotionFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const fvPatch& p = this->patch();
    const polyPatch& pp = p.patch();
    const fvMesh& mesh = this->dimensionedInternalField().mesh();
    const pointField& points = mesh.points();

    word pfName = this->dimensionedInternalField().name();
    pfName.replace("cell", "point");

    const GeometricField<Type, pointPatchField, pointMesh>& pointMotion =
        this->db().objectRegistry::template
            lookupObject<GeometricField<Type, pointPatchField, pointMesh> >
            (pfName);

    forAll(p, i)
    {
        this->operator[](i) = pp[i].average(points, pointMotion);
    }

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::cellMotionFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
}

#endif

// ************************************************************************* //
