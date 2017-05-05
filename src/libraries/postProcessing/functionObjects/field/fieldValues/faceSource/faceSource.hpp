/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::fieldValues::faceSource

Description
    Face source variant of field value function object. Values of user-
    specified fields reported for collections of faces.

    faceObj1                        // Name also used to identify output folder
    {
        type            faceSource;
        functionObjectLibs ("libfieldFunctionObjects.so");
        enabled         true;
        outputControl   outputTime;
        log             true;       // log to screen?
        valueOutput     true;       // Write values at run-time output times?
        source          faceZone;   // Type of face source:
                                    // faceZone,patch,sampledSurface
        sourceName      f0;         // faceZone name, see below
        operation       sum;
        weightField     alpha1;     // optional weight field (weightedAverage
                                    // only)
        fields
        (
            p
            phi
            U
        );
    }

    source:
    - faceZone       : requires a 'sourceName' entry to specify the faceZone
    - patch          :                    ""                        patch
    - sampledSurface : requires a 'sampledSurfaceDict' subdictionary. See e.g.
                       sampleDict.

    operation is one of:
      - none
      - sum
      - average
      - weightedAverage
      - areaAverage
      - areaIntegrate
      - min
      - max
      - CoV (Coefficient of variation: standard deviation/mean)
      - areaNormalAverage   (vector with first component (average of) inproduct
                             of value and face area vector)
      - areaNormalIntegrate (   ,,          ,,           (sum of)       ,,

    Notes:
    - faces on empty patches get ignored
    - if the field is a volField the faceZone can only consist of boundary
      faces.
    - all fields get oriented according to the faceZone (so you might e.g. see
      negative pressure)
    - using sampledSurfaces:
            - they do not do surface fields
            - if interpolate=true they use interpolationCellPoint
              otherwise they use cell values
            - each triangle in sampledSurface is logically only in one cell
              so interpolation will be wrong when triangles are larger than
              cells. This can only happen for sampling on triSurfaceMesh.
            - take care when using isoSurfaces - these might have duplicate
              triangles so integration might be wrong

SourceFiles
    faceSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceSource_H
#define faceSource_H

#include "NamedEnum.hpp"
#include "fieldValue.hpp"
#include "surfaceFieldsFwd.hpp"
#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class sampledSurface;

namespace fieldValues
{

/*---------------------------------------------------------------------------*\
                         Class faceSource Declaration
\*---------------------------------------------------------------------------*/

class faceSource
:
    public fieldValue
{

public:

    // Public data types

        //- Source type enumeration
        enum sourceType
        {
            stFaceZone,
            stPatch,
            stSampledSurface
        };

        //- Source type names
        static const NamedEnum<sourceType, 3> sourceTypeNames_;


        //- Operation type enumeration
        enum operationType
        {
            opNone,
            opSum,
            opAverage,
            opWeightedAverage,
            opAreaAverage,
            opAreaIntegrate,
            opMin,
            opMax,
            opCoV,
            opAreaNormalAverage,
            opAreaNormalIntegrate
        };

        //- Operation type names
        static const NamedEnum<operationType, 11> operationTypeNames_;


private:

    // Private Member Functions

        //- Set faces to evaluate based on a face zone
        void setFaceZoneFaces();

        //- Set faces to evaluate based on a patch
        void setPatchFaces();

        //- Set faces according to sampledSurface
        void sampledSurfaceFaces(const dictionary&);


protected:

    // Protected data

        //- Source type
        sourceType source_;

        //- Operation to apply to values
        operationType operation_;

        //- Weight field name - optional
        word weightFieldName_;

        //- Global number of faces
        label nFaces_;


        // If operating on mesh faces (faceZone,patch)

            //- Local list of face IDs
            labelList faceId_;

            //- Local list of patch ID per face
            labelList facePatchId_;

            //- List of +1/-1 representing face flip map
            //  (1 use as is, -1 negate)
            labelList faceSign_;

        // If operating on sampledSurface

            //- underlying sampledSurface
            autoPtr<sampledSurface> surfacePtr_;


    // Protected Member Functions

        //- Initialise, e.g. face addressing
        void initialise(const dictionary& dict);

        //- Return true if the field name is valid
        template<class Type>
        bool validField(const word& fieldName) const;

        //- Return field values by looking up field name
        template<class Type>
        tmp<Field<Type> > getFieldValues
        (
            const word& fieldName,
            const bool mustGet = false
        ) const;

        //- Apply the 'operation' to the values. Operation has to
        //  preserve Type.
        template<class Type>
        Type processSameTypeValues
        (
            const Field<Type>& values,
            const vectorField& Sf,
            const scalarField& weightField
        ) const;

        //- Apply the 'operation' to the values. Wrapper around
        //  processSameTypeValues. See also template specialisation below.
        template<class Type>
        Type processValues
        (
            const Field<Type>& values,
            const vectorField& Sf,
            const scalarField& weightField
        ) const;

        //- Output file header information
        virtual void writeFileHeader();


public:

    //- Run-time type information
    TypeName("faceSource");


    //- Construct from components
    faceSource
    (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~faceSource();


    // Public Member Functions

        // Access

            //- Return the source type
            inline const sourceType& source() const;

            //- Return the local list of face IDs
            inline const labelList& faceId() const;

            //- Return the local list of patch ID per face
            inline const labelList& facePatch() const;

            //- Return the list of +1/-1 representing face flip map
            inline const labelList& faceSign() const;


        // Function object functions

            //- Read from dictionary
            virtual void read(const dictionary&);

            //- Calculate and write
            virtual void write();

            //- Templated helper function to output field values
            template<class Type>
            bool writeValues(const word& fieldName);

            //- Filter a surface field according to faceIds
            template<class Type>
            tmp<Field<Type> > filterField
            (
                const GeometricField<Type, fvsPatchField, surfaceMesh>& field,
                const bool applyOrientation
            ) const;

            //- Filter a volume field according to faceIds
            template<class Type>
            tmp<Field<Type> > filterField
            (
                const GeometricField<Type, fvPatchField, volMesh>& field,
                const bool applyOrientation
            ) const;
};


//- Specialisation of processing vectors for opAreaNormalAverage,
//  opAreaNormalIntegrate (use inproduct - dimension reducing operation)
template<>
vector faceSource::processValues
(
    const Field<vector>& values,
    const vectorField& Sf,
    const scalarField& weightField
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fieldValues
} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::fieldValues::faceSource::sourceType&
CML::fieldValues::faceSource::source() const
{
    return source_;
}


inline const CML::labelList&
CML::fieldValues::faceSource::faceId() const
{
    return faceId_;
}


inline const CML::labelList&
CML::fieldValues::faceSource::facePatch() const
{
    return facePatchId_;
}


inline const CML::labelList&
CML::fieldValues::faceSource::faceSign() const
{
    return faceSign_;
}


#include "surfaceFields.hpp"
#include "volFields.hpp"
#include "sampledSurface.hpp"
#include "interpolationCellPoint.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
bool CML::fieldValues::faceSource::validField(const word& fieldName) const
{
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sf;
    typedef GeometricField<Type, fvPatchField, volMesh> vf;

    if (source_ != stSampledSurface && obr_.foundObject<sf>(fieldName))
    {
        return true;
    }
    else if (obr_.foundObject<vf>(fieldName))
    {
        return true;
    }

    return false;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fieldValues::faceSource::getFieldValues
(
    const word& fieldName,
    const bool mustGet
) const
{
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sf;
    typedef GeometricField<Type, fvPatchField, volMesh> vf;

    if (source_ != stSampledSurface && obr_.foundObject<sf>(fieldName))
    {
        return filterField(obr_.lookupObject<sf>(fieldName), true);
    }
    else if (obr_.foundObject<vf>(fieldName))
    {
        const vf& fld = obr_.lookupObject<vf>(fieldName);

        if (surfacePtr_.valid())
        {
            if (surfacePtr_().interpolate())
            {
                const interpolationCellPoint<Type> interp(fld);
                tmp<Field<Type> > tintFld(surfacePtr_().interpolate(interp));
                const Field<Type>& intFld = tintFld();

                // Average
                const faceList& faces = surfacePtr_().faces();
                tmp<Field<Type> > tavg
                (
                    new Field<Type>(faces.size(), pTraits<Type>::zero)
                );
                Field<Type>& avg = tavg();

                forAll(faces, faceI)
                {
                    const face& f = faces[faceI];
                    forAll(f, fp)
                    {
                        avg[faceI] += intFld[f[fp]];
                    }
                    avg[faceI] /= f.size();
                }

                return tavg;
            }
            else
            {
                return surfacePtr_().sample(fld);
            }
        }
        else
        {
            return filterField(fld, true);
        }
    }

    if (mustGet)
    {
        FatalErrorIn
        (
            "CML::tmp<CML::Field<Type> > "
            "CML::fieldValues::faceSource::getFieldValues"
            "("
                "const word&, "
                "const bool"
            ") const"
        )   << "Field " << fieldName << " not found in database"
            << abort(FatalError);
    }

    return tmp<Field<Type> >(new Field<Type>(0));
}


template<class Type>
Type CML::fieldValues::faceSource::processSameTypeValues
(
    const Field<Type>& values,
    const vectorField& Sf,
    const scalarField& weightField
) const
{
    Type result = pTraits<Type>::zero;
    switch (operation_)
    {
        case opSum:
        {
            result = sum(values);
            break;
        }
        case opAverage:
        {
            result = sum(values)/values.size();
            break;
        }
        case opWeightedAverage:
        {
            result = sum(values)/sum(weightField);
            break;
        }
        case opAreaAverage:
        {
            const scalarField magSf(mag(Sf));

            result = sum(values*magSf)/sum(magSf);
            break;
        }
        case opAreaIntegrate:
        {
            const scalarField magSf(mag(Sf));

            result = sum(values*magSf);
            break;
        }
        case opMin:
        {
            result = min(values);
            break;
        }
        case opMax:
        {
            result = max(values);
            break;
        }
        case opCoV:
        {
            const scalarField magSf(mag(Sf));

            Type meanValue = sum(values*magSf)/sum(magSf);

            const label nComp = pTraits<Type>::nComponents;

            for (direction d=0; d<nComp; ++d)
            {
                scalarField vals(values.component(d));
                scalar mean = component(meanValue, d);
                scalar& res = setComponent(result, d);

                res = sqrt(sum(magSf*sqr(vals - mean))/sum(magSf))/mean;
            }

            break;
        }
        default:
        {
            // Do nothing
        }
    }

    return result;
}


template<class Type>
Type CML::fieldValues::faceSource::processValues
(
    const Field<Type>& values,
    const vectorField& Sf,
    const scalarField& weightField
) const
{
    return processSameTypeValues(values, Sf, weightField);
}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool CML::fieldValues::faceSource::writeValues(const word& fieldName)
{
    const bool ok = validField<Type>(fieldName);

    if (ok)
    {
        Field<Type> values(getFieldValues<Type>(fieldName));
        scalarField weightField(values.size(), 1.0);

        if (weightFieldName_ != "none")
        {
            weightField = getFieldValues<scalar>(weightFieldName_, true);
        }

        vectorField Sf;

        if (surfacePtr_.valid())
        {
            // Get oriented Sf
            Sf = surfacePtr_().Sf();
        }
        else
        {
            // Get oriented Sf
            Sf = filterField(mesh().Sf(), false);
        }

        // Combine onto master
        combineFields(values);
        combineFields(Sf);
        combineFields(weightField);

        // apply weight field
        values *= weightField;


        if (Pstream::master())
        {
            Type result = processValues(values, Sf, weightField);

            if (valueOutput_)
            {
                IOField<Type>
                (
                    IOobject
                    (
                        fieldName + "_" + sourceTypeNames_[source_] + "-"
                            + sourceName_,
                        obr_.time().timeName(),
                        obr_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    values
                ).write();
            }

            outputFilePtr_()<< tab << result;

            if (log_)
            {
                Info<< "    " << operationTypeNames_[operation_]
                    << "(" << sourceName_ << ") for " << fieldName
                    <<  " = " << result << endl;
            }
        }
    }

    return ok;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fieldValues::faceSource::filterField
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const bool applyOrientation
) const
{
    tmp<Field<Type> > tvalues(new Field<Type>(faceId_.size()));
    Field<Type>& values = tvalues();

    forAll(values, i)
    {
        label faceI = faceId_[i];
        label patchI = facePatchId_[i];
        if (patchI >= 0)
        {
            values[i] = field.boundaryField()[patchI][faceI];
        }
        else
        {
            FatalErrorIn
            (
                "fieldValues::faceSource::filterField"
                "("
                    "const GeometricField<Type, fvPatchField, volMesh>&"
                ") const"
            )   << type() << " " << name_ << ": "
                << sourceTypeNames_[source_] << "(" << sourceName_ << "):"
                << nl
                << "    Unable to process internal faces for volume field "
                << field.name() << nl << abort(FatalError);
        }
    }

    if (applyOrientation)
    {
        forAll(values, i)
        {
            values[i] *= faceSign_[i];
        }
    }

    return tvalues;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fieldValues::faceSource::filterField
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& field,
    const bool applyOrientation
) const
{
    tmp<Field<Type> > tvalues(new Field<Type>(faceId_.size()));
    Field<Type>& values = tvalues();

    forAll(values, i)
    {
        label faceI = faceId_[i];
        label patchI = facePatchId_[i];
        if (patchI >= 0)
        {
            values[i] = field.boundaryField()[patchI][faceI];
        }
        else
        {
            values[i] = field[faceI];
        }
    }

    if (applyOrientation)
    {
        forAll(values, i)
        {
            values[i] *= faceSign_[i];
        }
    }

    return tvalues;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
