/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fieldValues::faceSource

Group
    grpFieldFunctionObjects

Description
    This function object provides a 'face source' variant of the fieldValues
    function object.  Given a list of user-specified fields and a selection
    of mesh (or general surface) faces, a number of operations can be
    performed, such as sums, averages and integrations.

    \linebreak
    For example, to calculate the volumetric or mass flux across a patch,
    apply the 'sum' operator to the flux field (typically \c phi)

    Example of function object specification:
    \verbatim
    faceSource1
    {
        type            faceSource;
        functionObjectLibs ("libfieldFunctionObjects.so");
        ...
        log             yes;
        valueOutput     true;
        surfaceFormat   none;
        source          faceZone;
        sourceName      f0;
        operation       sum;
        weightField     alpha1;
        fields
        (
            p
            phi
            U
        );
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | type name: faceSource   | yes         |
        log          | write data to standard output | no    | no
        valueOutput  | write the output values | yes         |
        writeArea    | Write the area of the faceSource | no |
        surfaceFormat | output value format    | no          |
        source       | face source: see below  | yes         |
        sourceName   | name of face source if required  | no |
        operation    | operation to perform    | yes         |
        weightField  | name of field to apply weighting | no |
        orientedWeightField  | name of oriented field to apply weighting | no |
        scaleFactor  | scale factor            | no          | 1
        fields       | list of fields to operate on | yes    |
        orientedFields | list of oriented fields to operate on | no |
    \endtable

    \linebreak
    Where \c source is defined by
    \plaintable
        faceZone     | requires a 'sourceName' entry to specify the faceZone
        patch        | requires a 'sourceName' entry to specify the patch
        sampledSurface | requires a 'sampledSurfaceDict' sub-dictionary
    \endplaintable

    \linebreak
    The \c operation is one of:
    \plaintable
       none          | no operation
       sum           | sum
       sumMag        | sum of component magnitudes
       sumDirection  | sum values which are positive in given direction
       sumDirectionBalance | sum of balance of values in given direction
       average       | ensemble average
       weightedAverage | weighted average
       areaAverage   | area weighted average
       areaIntegrate | area integral
       min           | minimum
       max           | maximum
       CoV           | coefficient of variation: standard deviation/mean
       areaNormalAverage| area weighted average in face normal direction
       areaNormalIntegrate | area weighted integral in face normal directon
    \endplaintable

Note
    - The values reported by the areaNormalAverage and areaNormalIntegrate
      operations are written as the first component of a field with the same
      rank as the input field.
    - faces on empty patches get ignored
    - if the field is a volField the \c faceZone can only consist of boundary
      faces
    - the `oriented' entries relate to mesh-oriented fields, such as the
      flux, phi.  These fields will be oriented according to the face normals.
    - using \c sampledSurfaces:
        - not available for surface fields
        - if interpolate=true they use \c interpolationCellPoint
          otherwise they use cell values
        - each triangle in \c sampledSurface is logically only in one cell
          so interpolation will be wrong when triangles are larger than
          cells.  This can only happen for sampling on a \c triSurfaceMesh
        - take care when using isoSurfaces - these might have duplicate
          triangles and so integration might be wrong

SeeAlso
    CML::fieldValues
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    faceSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceSource_H
#define faceSource_H

#include "NamedEnum.hpp"
#include "fieldValue.hpp"
#include "surfaceFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceWriter.hpp"

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
            opSumMag,
            opSumDirection,
            opSumDirectionBalance,
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
        static const NamedEnum<operationType, 14> operationTypeNames_;


private:

    // Private Member Functions

        //- Set faces to evaluate based on a face zone
        void setFaceZoneFaces();

        //- Set faces to evaluate based on a patch
        void setPatchFaces();

        //- Set faces according to sampledSurface
        void sampledSurfaceFaces(const dictionary&);

        //- Combine mesh faces and points from multiple processors
        void combineMeshGeometry
        (
            faceList& faces,
            pointField& points
        ) const;

        //- Combine surface faces and points from multiple processors
        void combineSurfaceGeometry
        (
            faceList& faces,
            pointField& points
        ) const;

        //- Calculate and return total area of the faceSource: sum(magSf)
        scalar totalArea() const;


protected:

    // Protected data

        //- Surface writer
        autoPtr<surfaceWriter> surfaceWriterPtr_;

        //- Source type
        sourceType source_;

        //- Operation to apply to values
        operationType operation_;

        //- Weight field name - optional
        word weightFieldName_;

        //- Flag to indicate if flipMap should be applied to the weight field
        bool orientWeightField_;

        //- Start index of fields that require application of flipMap
        label orientedFieldsStart_;

        //- Scale factor - optional
        scalar scaleFactor_;

        //- Total area of the faceSource
        scalar totalArea_;

        //- Optionally write the area of the faceSource
        bool writeArea_;

        //- Global number of faces
        label nFaces_;


        // If operating on mesh faces (faceZone, patch)

            //- Local list of face IDs
            labelList faceId_;

            //- Local list of patch ID per face
            labelList facePatchId_;

            //- List of +1/-1 representing face flip map
            //  (1 use as is, -1 negate)
            labelList faceSign_;


        // If operating on sampledSurface

            //- Underlying sampledSurface
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
            const bool mustGet = false,
            const bool applyOrientation = false
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
        virtual void writeFileHeader(const label i);


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
            bool writeValues
            (
                const word& fieldName,
                const scalarField& weightField,
                const bool orient
            );

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


//- Specialisation of processing scalars
template<>
scalar faceSource::processValues
(
    const Field<scalar>& values,
    const vectorField& Sf,
    const scalarField& weightField
) const;


//- Specialisation of processing vectors
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "faceSourceI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "faceSource.hpp"
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
    const bool mustGet,
    const bool applyOrientation
) const
{
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sf;
    typedef GeometricField<Type, fvPatchField, volMesh> vf;

    if (source_ != stSampledSurface && obr_.foundObject<sf>(fieldName))
    {
        return filterField(obr_.lookupObject<sf>(fieldName), applyOrientation);
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
            return filterField(fld, applyOrientation);
        }
    }

    if (mustGet)
    {
        FatalErrorInFunction
            << "Field " << fieldName << " not found in database"
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
        case opSumMag:
        {
            result = sum(cmptMag(values));
            break;
        }
        case opSumDirection:
        {
            FatalErrorInFunction
                << "Operation " << operationTypeNames_[operation_]
                << " not available for values of type "
                << pTraits<Type>::typeName
                << exit(FatalError);

            result = pTraits<Type>::zero;
            break;
        }
        case opSumDirectionBalance:
        {
            FatalErrorInFunction
                << "Operation " << operationTypeNames_[operation_]
                << " not available for values of type "
                << pTraits<Type>::typeName
                << exit(FatalError);

            result = pTraits<Type>::zero;
            break;
        }
        case opAverage:
        {
            result = sum(values)/values.size();
            break;
        }
        case opWeightedAverage:
        {
            if (weightField.size())
            {
                result = sum(values)/sum(weightField);
            }
            else
            {
                result = sum(values)/values.size();
            }
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
bool CML::fieldValues::faceSource::writeValues
(
    const word& fieldName,
    const scalarField& weightField,
    const bool orient
)
{
    const bool ok = validField<Type>(fieldName);

    if (ok)
    {
        Field<Type> values(getFieldValues<Type>(fieldName, true, orient));

        vectorField Sf;
        if (surfacePtr_.valid())
        {
            // Get oriented Sf
            Sf = surfacePtr_().Sf();
        }
        else
        {
            // Get oriented Sf
            Sf = filterField(mesh().Sf(), true);
        }

        // Combine onto master
        combineFields(values);
        combineFields(Sf);

        // Write raw values on surface if specified
        if (surfaceWriterPtr_.valid())
        {
            faceList faces;
            pointField points;

            if (surfacePtr_.valid())
            {
                combineSurfaceGeometry(faces, points);
            }
            else
            {
                combineMeshGeometry(faces, points);
            }

            if (Pstream::master())
            {
                fileName outputDir =
                    baseFileDir()/name_/"surface"/obr_.time().timeName();

                surfaceWriterPtr_->write
                (
                    outputDir,
                    word(sourceTypeNames_[source_]) + "_" + sourceName_,
                    points,
                    faces,
                    fieldName,
                    values,
                    false
                );
            }
        }


        // apply scale factor and weight field
        values *= scaleFactor_;
        if (weightField.size())
        {
            values *= weightField;
        }

        if (Pstream::master())
        {
            Type result = processValues(values, Sf, weightField);

            // add to result dictionary, over-writing any previous entry
            resultDict_.add(fieldName, result, true);

            file()<< tab << result;

            Info(log_)<< "    " << operationTypeNames_[operation_]
                << "(" << sourceName_ << ") of " << fieldName
                <<  " = " << result << endl;
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
            FatalErrorInFunction
                << type() << " " << name_ << ": "
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
