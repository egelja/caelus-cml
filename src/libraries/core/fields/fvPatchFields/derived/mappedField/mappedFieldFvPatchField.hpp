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
    CML::mappedFieldFvPatchField

Description
    This boundary condition provides a self-contained version of the \c mapped
    condition.  It does not use information on the patch; instead it holds
    thr data locally.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        fieldName    | name of field to be mapped | no       | this field name
        setAverage   | flag to activate setting of average value | yes |
        average      | average value to apply if \c setAverage = yes | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            mappedField;
        fieldName       T;              // optional field name
        setAverage      no;             // apply an average value
        average         0;              // average to apply if setAverage
        value           uniform 0;      // place holder
    }
    \endverbatim

Note
    Since this condition can be applied on a per-field and per-patch basis,
    it is possible to duplicate the mapping information.  If possible, employ
    the \c mapped condition in preference to avoid this situation, and only
    employ this condition if it is not possible to change the underlying
    geometric (poly) patch type to \c mapped.

SeeAlso
    CML::mappedPatchBase
    CML::mappedPolyPatch
    CML::mappedFvPatch
    CML::fixedValueFvPatchField

\*---------------------------------------------------------------------------*/

#ifndef mappedFieldFvPatchField_H
#define mappedFieldFvPatchField_H

#include "mappedPatchBase.hpp"
#include "fixedValueFvPatchFields.hpp"
#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class mappedFieldFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFieldFvPatchField
:
    public mappedPatchBase,
    public fixedValueFvPatchField<Type>
{
    // Private data

        //- Name of field to sample - defaults to field associated with this
        //  patchField if not specified
        word fieldName_;

        //- If true adjust the mapped field to maintain average value average_
        const bool setAverage_;

        //- Average value the mapped field is adjusted to maintain if
        //  setAverage_ is set true
        const Type average_;

        //- Interpolation scheme to use for nearestCell mode
        word interpolationScheme_;


    // Private Member Functions

        //- Field to sample. Either on my or nbr mesh
        const GeometricField<Type, CML::fvPatchField, volMesh>& sampleField() const;


public:

    //- Runtime type information
    TypeName("mappedField");


    // Constructors

        //- Construct from patch and internal field
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct from patch, internal field and distance for normal type
        //  sampling
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,

            // mappedPatchBase
            const word& sampleRegion,
            const sampleMode sampleMode,
            const word& samplePatch,
            const scalar distance,

            // My settings
            const word& fieldName,
            const bool setAverage,
            const Type average,
            const word& interpolationScheme
        );

        //- Construct by mapping given
        //  mappedFieldFvPatchField
        //  onto a new patch
        mappedFieldFvPatchField
        (
            const mappedFieldFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedFieldFvPatchField
        (
            const mappedFieldFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFieldFvPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        mappedFieldFvPatchField
        (
            const mappedFieldFvPatchField<Type>&,
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
                new mappedFieldFvPatchField<Type>
                (
                    *this,
                    iF
                )
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

#include "volFields.hpp"
#include "interpolationCell.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedPatchBase(p.patch()),
    fixedValueFvPatchField<Type>(p, iF),
    fieldName_(iF.name()),
    setAverage_(false),
    average_(pTraits<Type>::zero),
    interpolationScheme_(interpolationCell<Type>::typeName)
{}


template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const mappedFieldFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mappedPatchBase(p.patch(), ptf),
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{}


template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mappedPatchBase(p.patch(), dict),
    fixedValueFvPatchField<Type>(p, iF, dict),
    fieldName_(dict.template lookupOrDefault<word>("fieldName", iF.name())),
    setAverage_(readBool(dict.lookup("setAverage"))),
    average_(pTraits<Type>(dict.lookup("average"))),
    interpolationScheme_(interpolationCell<Type>::typeName)
{
    if (mode() == mappedPatchBase::NEARESTCELL)
    {
        dict.lookup("interpolationScheme") >> interpolationScheme_;
    }
}


template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,

    // mappedPatchBase
    const word& sampleRegion,
    const sampleMode sampleMode,
    const word& samplePatch,
    const scalar distance,

    // My settings
    const word& fieldName,
    const bool setAverage,
    const Type average,
    const word& interpolationScheme
)
:
    mappedPatchBase
    (
        p.patch(),
        sampleRegion,
        sampleMode,
        samplePatch,
        distance
    ),
    fixedValueFvPatchField<Type>(p, iF),
    fieldName_(fieldName),
    setAverage_(setAverage),
    average_(average),
    interpolationScheme_(interpolationScheme)
{}


template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const mappedFieldFvPatchField<Type>& ptf
)
:
    mappedPatchBase(ptf.patch().patch(), ptf),
    fixedValueFvPatchField<Type>(ptf),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{}


template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
    const mappedFieldFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedPatchBase(ptf.patch().patch(), ptf),
    fixedValueFvPatchField<Type>(ptf, iF),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const GeometricField<Type, fvPatchField, volMesh>&
mappedFieldFvPatchField<Type>::sampleField() const
{	
	typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    const fvMesh& nbrMesh = refCast<const fvMesh>(sampleMesh());

    if (sameRegion())
    {
        if (fieldName_ == this->dimensionedInternalField().name())
        {
            // Optimisation: bypass field lookup
            return
                dynamic_cast<const fieldType&>
                (
                    this->dimensionedInternalField()
                );
        }
        else
        {
            const fvMesh& thisMesh = this->patch().boundaryMesh().mesh();
            return thisMesh.template lookupObject<fieldType>(fieldName_);
        }
    }
    else
    {
        return nbrMesh.template lookupObject<fieldType>(fieldName_);
    }
}


template<class Type>
void mappedFieldFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;

    const fvMesh& thisMesh = this->patch().boundaryMesh().mesh();
    const fvMesh& nbrMesh = refCast<const fvMesh>(sampleMesh());

    // Result of obtaining remote values
    Field<Type> newValues;

    switch (mode())
    {
        case NEARESTCELL:
        {
            const mapDistribute& mapDist = this->mappedPatchBase::map();

            if (interpolationScheme_ != interpolationCell<Type>::typeName)
            {
                // Need to do interpolation so need cells to sample

                // Send back sample points to the processor that holds the cell
                vectorField samples(samplePoints());
                mapDist.reverseDistribute
                (
                    (sameRegion() ? thisMesh.nCells() : nbrMesh.nCells()),
                    point::max,
                    samples
                );


                autoPtr<interpolation<Type> > interpolator
                (
                    interpolation<Type>::New
                    (
                        interpolationScheme_,
                        sampleField()
                    )
                );
                const interpolation<Type>& interp = interpolator();

                newValues.setSize(samples.size(), pTraits<Type>::max);
                forAll(samples, cellI)
                {
                    if (samples[cellI] != point::max)
                    {
                        newValues[cellI] = interp.interpolate
                        (
                            samples[cellI],
                            cellI
                        );
                    }
                }
            }
            else
            {
                newValues = sampleField();
            }

            mapDist.distribute(newValues);

            break;
        }
        case NEARESTPATCHFACE: case NEARESTPATCHFACEAMI:
        {
            const label nbrPatchID =
                nbrMesh.boundaryMesh().findPatchID(samplePatch());
            if (nbrPatchID < 0)
            {
                FatalErrorInFunction
                    << "Unable to find sample patch " << samplePatch()
                    << " in region " << sampleRegion()
                    << " for patch " << this->patch().name() << nl
                    << abort(FatalError);
            }

            const fieldType& nbrField = sampleField();

            newValues = nbrField.boundaryField()[nbrPatchID];
            this->distribute(newValues);

            break;
        }
        case NEARESTFACE:
        {
            Field<Type> allValues(nbrMesh.nFaces(), pTraits<Type>::zero);

            const fieldType& nbrField = sampleField();

            forAll(nbrField.boundaryField(), patchI)
            {
                const fvPatchField<Type>& pf =
                    nbrField.boundaryField()[patchI];
                label faceStart = pf.patch().patch().start();

                forAll(pf, faceI)
                {
                    allValues[faceStart++] = pf[faceI];
                }
            }

            this->distribute(allValues);
            newValues.transfer(allValues);

            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown sampling mode: " << mode()
                << nl << abort(FatalError);
        }
    }

    if (setAverage_)
    {
        Type averagePsi =
            gSum(this->patch().magSf()*newValues)
           /gSum(this->patch().magSf());

        if (mag(averagePsi)/mag(average_) > 0.5)
        {
            newValues *= mag(average_)/mag(averagePsi);
        }
        else
        {
            newValues += (average_ - averagePsi);
        }
    }

    this->operator==(newValues);

    if (debug)
    {
        Info<< "operating on field:" << this->dimensionedInternalField().name()
            << " patch:" << this->patch().name()
            << "  avg:" << gAverage(*this)
            << "  min:" << gMin(*this)
            << "  max:" << gMax(*this)
            << endl;
    }

    // Restore tag
    UPstream::msgType() = oldTag;

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mappedFieldFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    mappedPatchBase::write(os);
    os.writeKeyword("fieldName") << fieldName_ << token::END_STATEMENT << nl;
    os.writeKeyword("setAverage") << setAverage_ << token::END_STATEMENT << nl;
    os.writeKeyword("average") << average_ << token::END_STATEMENT << nl;
    os.writeKeyword("interpolationScheme") << interpolationScheme_
        << token::END_STATEMENT << nl;
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
