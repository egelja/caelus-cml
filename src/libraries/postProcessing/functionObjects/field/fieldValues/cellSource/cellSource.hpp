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
    CML::fieldValues::cellSource

Group
    grpFieldFunctionObjects

Description
    This function object provides a 'cell source' variant of the fieldValues
    function object.  Given a list of user-specified fields and a selection
    of mesh cells, a number of operations can be performed, such as sums,
    averages and integrations.


    Example of function object specification:
    \verbatim
    cellSource1
    {
        type            cellSource;
        functionObjectLibs ("libfieldFunctionObjects.so");
        ...
        log             true;
        valueOutput     true;
        source          cellZone;
        sourceName      c0;
        operation       volAverage;
        weightField     alpha1;
        fields
        (
            p
            U
        );
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | Type name: cellSource   | yes         |
        log          | Write data to standard output | no    | no
        valueOutput  | Write the raw output values | yes     |
        writeVolume  | Write the volume of the cellSource | no |
        source       | cell source: see below  | yes         |
        sourceName   | name of cell source if required  | no |
        operation    | operation to perform    | yes         |
        weightField  | name of field to apply weighting | no |
        fields       | list of fields to operate on | yes    |
    \endtable

    \linebreak
    Where \c source is defined by
    \plaintable
        cellZone     | requires a 'sourceName' entry to specify the cellZone
        all          | all cells
    \endplaintable

    \linebreak
    The \c operation is one of:
    \plaintable
       none          | no operation
       sum           | sum
       sumMag        | sum of component magnitudes
       average       | ensemble average
       weightedAverage | weighted average
       volAverage    | volume weighted average
       volIntegrate  | volume integral
       min           | minimum
       max           | maximum
       CoV           | coefficient of variation: standard deviation/mean
    \endplaintable

SeeAlso
    CML::fieldValues
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    cellSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellSource_H
#define cellSource_H

#include "NamedEnum.hpp"
#include "fieldValue.hpp"
#include "labelList.hpp"
#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fieldValues
{

/*---------------------------------------------------------------------------*\
                         Class cellSource Declaration
\*---------------------------------------------------------------------------*/

class cellSource
:
    public fieldValue
{

public:

    // Public data types

        //- Source type enumeration
        enum sourceType
        {
            stCellZone,
            stAll
        };

        //- Source type names
        static const NamedEnum<sourceType, 2> sourceTypeNames_;


        //- Operation type enumeration
        enum operationType
        {
            opNone,
            opSum,
            opSumMag,
            opAverage,
            opWeightedAverage,
            opVolAverage,
            opVolIntegrate,
            opMin,
            opMax,
            opCoV
        };

        //- Operation type names
        static const NamedEnum<operationType, 10> operationTypeNames_;


private:

    // Private Member Functions

        //- Set cells to evaluate based on a cell zone
        void setCellZoneCells();

        //- Set cells to evaluate based on a patch
        void setPatchCells();

        //- Calculate and return volume of the cellSource: sum(V)
        scalar volume() const;


protected:

    // Protected data

        //- Source type
        sourceType source_;

        //- Operation to apply to values
        operationType operation_;

        //- Global number of cells
        label nCells_;

        //- Local list of cell IDs
        labelList cellId_;

        //- Weight field name - only used for opWeightedAverage mode
        word weightFieldName_;

        //- Volume of the cellSource
        scalar volume_;

        //- Optionally write the volume of the cellSource
        bool writeVolume_;


    // Protected Member Functions

        //- Initialise, e.g. cell addressing
        void initialise(const dictionary& dict);

        //- Return true if the field name is valid
        template<class Type>
        bool validField(const word& fieldName) const;

        //- Insert field values into values list
        template<class Type>
        tmp<Field<Type> > setFieldValues
        (
            const word& fieldName,
            const bool mustGet = false
        ) const;

        //- Apply the 'operation' to the values
        template<class Type>
        Type processValues
        (
            const Field<Type>& values,
            const scalarField& V,
            const scalarField& weightField
        ) const;

        //- Output file header information
        virtual void writeFileHeader(const label i);


public:

    //- Run-time type information
    TypeName("cellSource");


    //- Construct from components
    cellSource
    (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~cellSource();


    // Public Member Functions

        // Access

            //- Return the source type
            inline const sourceType& source() const;

            //- Return the local list of cell IDs
            inline const labelList& cellId() const;


        // Function object functions

            //- Read from dictionary
            virtual void read(const dictionary&);

            //- Calculate and write
            virtual void write();

            //- Templated helper function to output field values
            template<class Type>
            bool writeValues(const word& fieldName);

            //- Filter a field according to cellIds
            template<class Type>
            tmp<Field<Type> > filterField(const Field<Type>& field) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fieldValues
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "cellSourceI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "cellSource.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
bool CML::fieldValues::cellSource::validField(const word& fieldName) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> vf;

    if (obr_.foundObject<vf>(fieldName))
    {
        return true;
    }

    return false;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fieldValues::cellSource::setFieldValues
(
    const word& fieldName,
    const bool mustGet
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> vf;

    if (obr_.foundObject<vf>(fieldName))
    {
        return filterField(obr_.lookupObject<vf>(fieldName));
    }

    if (mustGet)
    {
        FatalErrorInFunction
           << "Field " << fieldName << " not found in database"
            << abort(FatalError);
    }

    return tmp<Field<Type> >(new Field<Type>(0.0));
}


template<class Type>
Type CML::fieldValues::cellSource::processValues
(
    const Field<Type>& values,
    const scalarField& V,
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
        case opVolAverage:
        {
            result = sum(values*V)/sum(V);
            break;
        }
        case opVolIntegrate:
        {
            result = sum(values*V);
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
            Type meanValue = sum(values*V)/sum(V);

            const label nComp = pTraits<Type>::nComponents;

            for (direction d=0; d<nComp; ++d)
            {
                scalarField vals(values.component(d));
                scalar mean = component(meanValue, d);
                scalar& res = setComponent(result, d);

                res = sqrt(sum(V*sqr(vals - mean))/sum(V))/mean;
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


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool CML::fieldValues::cellSource::writeValues(const word& fieldName)
{
    const bool ok = validField<Type>(fieldName);

    if (ok)
    {
        Field<Type> values(setFieldValues<Type>(fieldName));
        scalarField V(filterField(mesh().V()));
        scalarField weightField(values.size(), 1.0);

        if (weightFieldName_ != "none")
        {
            weightField = setFieldValues<scalar>(weightFieldName_, true);
        }

        // Combine onto master
        combineFields(values);
        combineFields(V);
        combineFields(weightField);

        // apply weight field
        values *= weightField;

        if (Pstream::master())
        {
            Type result = processValues(values, V, weightField);

            // add to result dictionary, over-writing any previous entry
            resultDict_.add(fieldName, result, true);

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


            file()<< tab << result;

            Info(log_)<< "    " << operationTypeNames_[operation_]
                << "(" << sourceName_ << ") of " << fieldName
                <<  " = " << result << endl;
        }
    }

    return ok;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::fieldValues::cellSource::filterField
(
    const Field<Type>& field
) const
{
    return tmp<Field<Type> >(new Field<Type>(field, cellId_));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
