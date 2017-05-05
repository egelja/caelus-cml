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
    CML::timeVaryingUniformFixedValuePointPatchField

Description
    A time-varying form of a uniform fixed value boundary condition.

See Also
    CML::timeVaryingUniformFixedValueFvField

SourceFiles
    timeVaryingUniformFixedValuePointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef timeVaryingUniformFixedValuePointPatchField_H
#define timeVaryingUniformFixedValuePointPatchField_H

#include "fixedValuePointPatchField.hpp"
#include "interpolationTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
        Class timeVaryingUniformFixedValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class timeVaryingUniformFixedValuePointPatchField
:
    public fixedValuePointPatchField<Type>
{
    // Private data

        //- the time series being used, including the bounding treatment
        interpolationTable<Type> timeSeries_;


public:

    //- Runtime type information
    TypeName("timeVaryingUniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        timeVaryingUniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        timeVaryingUniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patch field onto a new patch
        timeVaryingUniformFixedValuePointPatchField
        (
            const timeVaryingUniformFixedValuePointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy
        timeVaryingUniformFixedValuePointPatchField
        (
            const timeVaryingUniformFixedValuePointPatchField<Type>&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new timeVaryingUniformFixedValuePointPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        timeVaryingUniformFixedValuePointPatchField
        (
            const timeVaryingUniformFixedValuePointPatchField<Type>&,
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
                new timeVaryingUniformFixedValuePointPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the time series used
            const interpolationTable<Type>& timeSeries() const
            {
                return timeSeries_;
            }


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"
#include "IFstream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(p, iF)
{}


template<class Type>
CML::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
    const timeVaryingUniformFixedValuePointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    fixedValuePointPatchField<Type>(ptf, p, iF, mapper),
    timeSeries_(ptf.timeSeries_)
{}


template<class Type>
CML::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    fixedValuePointPatchField<Type>(p, iF),
    timeSeries_(dict)
{
    updateCoeffs();
}


template<class Type>
CML::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
    const timeVaryingUniformFixedValuePointPatchField<Type>& ptf
)
:
    fixedValuePointPatchField<Type>(ptf),
    timeSeries_(ptf.timeSeries_)
{}


template<class Type>
CML::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
    const timeVaryingUniformFixedValuePointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(ptf, iF),
    timeSeries_(ptf.timeSeries_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::timeVaryingUniformFixedValuePointPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    this->operator==(timeSeries_(this->db().time().timeOutputValue()));
    fixedValuePointPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::timeVaryingUniformFixedValuePointPatchField<Type>::write
(
    Ostream& os
) const
{
    fixedValuePointPatchField<Type>::write(os);
    timeSeries_.write(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
