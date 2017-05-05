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
    CML::fieldValues::cellSource

Description
    Cell source variant of field value function object. Values of user-
    specified fields reported for collections of cells.

    cellObj1                        // Name also used to identify output folder
    {
        type            cellSource;
        functionObjectLibs ("libfieldFunctionObjects.so");
        enabled         true;
        outputControl   outputTime;
        log             true;       // log to screen?
        valueOutput     true;       // Write values at run-time output times?
        source          cellZone;   // Type of cell source
        sourceName      c0;
        operation       volAverage;
        fields
        (
            p
            U
        );
    }

    where operation is one of:
      - none
      - sum
      - volAverage
      - volIntegrate
      - weightedAverage
      - CoV (Coefficient of variation: standard deviation/mean)

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
            opVolAverage,
            opVolIntegrate,
            opWeightedAverage,
            opMin,
            opMax,
            opCoV
        };

        //- Operation type names
        static const NamedEnum<operationType, 8> operationTypeNames_;


private:

    // Private Member Functions

        //- Set cells to evaluate based on a cell zone
        void setCellZoneCells();

        //- Set cells to evaluate based on a patch
        void setPatchCells();


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
        virtual void writeFileHeader();


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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::fieldValues::cellSource::sourceType&
CML::fieldValues::cellSource::source() const
{
    return source_;
}


inline const CML::labelList&
CML::fieldValues::cellSource::cellId() const
{
    return cellId_;
}


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
        FatalErrorIn
        (
            "CML::tmp<CML::Field<Type> > "
            "CML::fieldValues::cellSource::setFieldValues"
            "("
                "const word&, "
                "const bool"
            ") const"
        )   << "Field " << fieldName << " not found in database"
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
        case opWeightedAverage:
        {
            result = sum(values*weightField)/sum(weightField);
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
        combineFields(values);

        scalarField V(filterField(mesh().V()));
        combineFields(V);

        scalarField weightField;

        if (operation_ == opWeightedAverage)
        {
            weightField = setFieldValues<scalar>(weightFieldName_, true);
        }

        combineFields(weightField);

        if (Pstream::master())
        {
            Type result = processValues(values, V, weightField);

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
