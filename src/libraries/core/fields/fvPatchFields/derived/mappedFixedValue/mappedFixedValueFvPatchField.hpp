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
    CML::mappedFixedValueFvPatchField

Description
    This boundary condition maps the value at a set of cells or patch faces
    back to *this.  

    The sample mode is set by the underlying mapping engine, provided by the
    mappedPatchBase class.
    
    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        fieldName    | name of field to be mapped | no       | this field name
        setAverage   | flag to activate setting of average value | yes |
        average      | average value to apply if \c setAverage = yes | yes |
        interpolationScheme | type of interpolation scheme | no |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            mapped;
        fieldName       T;
        setAverage      no;
        average         0;
        interpolationScheme cell;
        value           uniform 0;
    }
    \endverbatim

    When employing the \c nearestCell sample mode, the user must also specify
    the interpolation scheme using the \c interpolationScheme entry.

    In case of interpolation (where scheme != cell) the limitation is that
    there is only one value per cell.  For example, if you have a cell with two
    boundary faces and both faces sample into the cell, both faces will get the
    same value.

Note
    It is not possible to sample internal faces since volume fields are not
    defined on faces.

SeeAlso
    CML::interpolation
    CML::fixedValueFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef mappedFixedValueFvPatchField_H
#define mappedFixedValueFvPatchField_H

#include "fixedValueFvPatchFields.hpp"
#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class mappedFixedValueFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>
{

protected:

    // Protected data

        //- Name of field to sample - defaults to field associated with this
        //  patchField if not specified
        word fieldName_;

        //- If true adjust the mapped field to maintain average value average_
        const bool setAverage_;

        //- Average value the mapped field is adjusted to maintain if
        //  setAverage_ is set true
        const Type average_;

        //- Interpolation scheme to use for nearestcell mode
        word interpolationScheme_;


    // Protected Member Functions

        //- Field to sample. Either on my or nbr mesh
		const GeometricField<Type, fvPatchField, volMesh>& sampleField() const;


public:

    //- Runtime type information
    TypeName("mapped");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given mappedFixedValueFvPatchField
        //  onto a new patch
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&,
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
                new mappedFixedValueFvPatchField<Type>(*this, iF)
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

#include "mappedPatchBase.hpp"
#include "volFields.hpp"
#include "interpolationCell.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
mappedFixedValueFvPatchField<Type>::mappedFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    fieldName_(iF.name()),
    setAverage_(false),
    average_(pTraits<Type>::zero),
    interpolationScheme_(interpolationCell<Type>::typeName)
{}


template<class Type>
mappedFixedValueFvPatchField<Type>::mappedFixedValueFvPatchField
(
    const mappedFixedValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{
    if (!isA<mappedPatchBase>(this->patch().patch()))
    {
        FatalErrorInFunction
            << "\n    patch type '" << p.type()
            << "' not type '" << mappedPatchBase::typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalError);
    }
}


template<class Type>
mappedFixedValueFvPatchField<Type>::mappedFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF, dict),
    fieldName_(dict.lookupOrDefault<word>("fieldName", iF.name())),
    setAverage_(readBool(dict.lookup("setAverage"))),
    average_(pTraits<Type>(dict.lookup("average"))),
    interpolationScheme_(interpolationCell<Type>::typeName)
{
    if (!isA<mappedPatchBase>(this->patch().patch()))
    {
        FatalErrorInFunction
            << "\n    patch type '" << p.type()
            << "' not type '" << mappedPatchBase::typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalError);
    }

    const mappedPatchBase& mpp = refCast<const mappedPatchBase>
    (
        mappedFixedValueFvPatchField<Type>::patch().patch()
    );
    if (mpp.mode() == mappedPatchBase::NEARESTCELL)
    {
        dict.lookup("interpolationScheme") >> interpolationScheme_;
    }
}


template<class Type>
mappedFixedValueFvPatchField<Type>::mappedFixedValueFvPatchField
(
    const mappedFixedValueFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{}


template<class Type>
mappedFixedValueFvPatchField<Type>::mappedFixedValueFvPatchField
(
    const mappedFixedValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    fieldName_(ptf.fieldName_),
    setAverage_(ptf.setAverage_),
    average_(ptf.average_),
    interpolationScheme_(ptf.interpolationScheme_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const GeometricField<Type, fvPatchField, volMesh>&
mappedFixedValueFvPatchField<Type>::sampleField() const
{	
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
	
    const mappedPatchBase& mpp = refCast<const mappedPatchBase>
    (
        mappedFixedValueFvPatchField<Type>::patch().patch()
    );
    const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());

    if (mpp.sameRegion())
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
            return thisMesh.lookupObject<fieldType>(fieldName_);
        }
    }
    else
    {
        return nbrMesh.lookupObject<fieldType>(fieldName_);
    }
}


template<class Type>
void mappedFixedValueFvPatchField<Type>::updateCoeffs()
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

    // Get the scheduling information from the mappedPatchBase
    const mappedPatchBase& mpp = refCast<const mappedPatchBase>
    (
        mappedFixedValueFvPatchField<Type>::patch().patch()
    );

    const fvMesh& thisMesh = this->patch().boundaryMesh().mesh();
    const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());

    // Result of obtaining remote values
    Field<Type> newValues;

    switch (mpp.mode())
    {
        case mappedPatchBase::NEARESTCELL:
        {
            const mapDistribute& distMap = mpp.map();

            if (interpolationScheme_ != interpolationCell<Type>::typeName)
            {
                // Send back sample points to the processor that holds the cell
                vectorField samples(mpp.samplePoints());
                distMap.reverseDistribute
                (
                    (mpp.sameRegion() ? thisMesh.nCells() : nbrMesh.nCells()),
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

            distMap.distribute(newValues);

            break;
        }
        case mappedPatchBase::NEARESTPATCHFACE:
        case mappedPatchBase::NEARESTPATCHFACEAMI:
        {
            const label nbrPatchID =
                nbrMesh.boundaryMesh().findPatchID(mpp.samplePatch());

            if (nbrPatchID < 0)
            {
                FatalErrorInFunction
                 << "Unable to find sample patch " << mpp.samplePatch()
                 << " in region " << mpp.sampleRegion()
                 << " for patch " << this->patch().name() << nl
                 << abort(FatalError);
            }

            const fieldType& nbrField = sampleField();

            newValues = nbrField.boundaryField()[nbrPatchID];
            mpp.distribute(newValues);

            break;
        }
        case mappedPatchBase::NEARESTFACE:
        {
            Field<Type> allValues(nbrMesh.nFaces(), pTraits<Type>::zero);

            const fieldType& nbrField = sampleField();

            forAll(nbrField.boundaryField(), patchI)
            {
                const fvPatchField<Type>& pf =
                    nbrField.boundaryField()[patchI];
                label faceStart = pf.patch().start();

                forAll(pf, faceI)
                {
                    allValues[faceStart++] = pf[faceI];
                }
            }

            mpp.distribute(allValues);
            newValues.transfer(allValues);

            break;
        }
        default:
        {
            FatalErrorInFunction
             << "Unknown sampling mode: " << mpp.mode()
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
        Info<< "mapped on field:"
            << this->dimensionedInternalField().name()
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
void mappedFixedValueFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
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
