/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2013 OpenFOAM Foundation
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
    CML::fieldValues::fieldValueDelta

Group
    grpFieldFunctionObjects

Description
    This function object provides a differencing option between two 'field
    value' function objects.

    Example of function object specification:
    \verbatim
    fieldValueDelta1
    {
        type            fieldValueDelta;
        functionObjectLibs ("libfieldFunctionObjects.so");
        operation       subtract;

        fieldValue1
        {
            ...
        }
        fieldValue2
        {
            ...
        }
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | type name: fieldValueDelta   | yes    |
    \endtable

    \linebreak
    The \c operation is one of:
    \plaintable
       add           | add
       subtract      | subtract
       min           | minimum
       max           | maximum
       average       | average
    \endplaintable
SeeAlso
    CML::fieldValue

SourceFiles
    fieldValueDelta.cpp

\*---------------------------------------------------------------------------*/

#ifndef fieldValueDelta_H
#define fieldValueDelta_H

#include "functionObjectFile.hpp"
#include "fieldValue.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace fieldValues
{

/*---------------------------------------------------------------------------*\
                       Class fieldValueDelta Declaration
\*---------------------------------------------------------------------------*/

class fieldValueDelta
:
    public functionObjectFile
{
public:
        //- Operation type enumeration
        enum operationType
        {
            opAdd,
            opSubtract,
            opMin,
            opMax,
            opAverage
        };

        //- Operation type names
        static const NamedEnum<operationType, 5> operationTypeNames_;


private:

    // Private data

        //- Name of this fieldValue object
        word name_;

        //- Database this class is registered to
        const objectRegistry& obr_;

        //- Flag to indicate to load from files
        bool loadFromFiles_;

        //- Switch to send output to Info as well as to file
        Switch log_;

        //- Operation to apply to values
        operationType operation_;

        //- Field value source object 1
        autoPtr<fieldValue> source1Ptr_;

        //- Field value source object 2
        autoPtr<fieldValue> source2Ptr_;


    // Private Member Functions

        //- Templated function to process common fields
        template<class Type>
        void processFields(bool& found);

        //- Templated function to apply the operation
        template<class Type>
        Type applyOperation(const Type& value1, const Type& value2) const;


protected:

    // Functions to be over-ridden from IOoutputFilter class

        //- Output file header information
        virtual void writeFileHeader(const label i);


public:

    //- Run-time type information
    TypeName("fieldValueDelta");


    //- Construct from components
    fieldValueDelta
    (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~fieldValueDelta();


    // Public Member Functions

        // Function object functions

            //- Read from dictionary
            virtual void read(const dictionary&);

            //- Calculate and write
            virtual void write();

            //- Execute
            virtual void execute();

            //- Execute the at the final time-loop, currently does nothing
            virtual void end();

             //- Called when time was set at the end of the Time::operator++
            virtual void timeSet();

            //- Update mesh
            virtual void updateMesh(const mapPolyMesh&);

            //- Move points
            virtual void movePoints(const pointField&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fieldValues
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "GeometricField.hpp"
#include "volMesh.hpp"
#include "surfaceMesh.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::fieldValues::fieldValueDelta::applyOperation
(
    const Type& value1,
    const Type& value2
) const
{
    Type result = pTraits<Type>::zero;

    switch (operation_)
    {
        case opAdd:
        {
            result = value1 + value2;
            break;
        }
        case opSubtract:
        {
            result = value1 - value2;
            break;
        }
        case opMin:
        {
            result = min(value1, value2);
            break;
        }
        case opMax:
        {
            result = max(value1, value2);
            break;
        }
        case opAverage:
        {
            result = 0.5*(value1 + value2);
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unable to process operation "
                << operationTypeNames_[operation_]
                << abort(FatalError);
        }
    }

    return result;
}


template<class Type>
void CML::fieldValues::fieldValueDelta::processFields(bool& found)
{
    typedef GeometricField<Type, fvPatchField, volMesh> vf;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sf;

    const wordList& fields1 = source1Ptr_->fields();

    const dictionary& results1 = source1Ptr_->resultDict();
    const dictionary& results2 = source2Ptr_->resultDict();

    Type r1(pTraits<Type>::zero);
    Type r2(pTraits<Type>::zero);

    forAll(fields1, i)
    {
        const word& fieldName = fields1[i];

        if
        (
            (obr_.foundObject<vf>(fieldName) || obr_.foundObject<sf>(fieldName))
         && results2.found(fieldName)
        )
        {
            results1.lookup(fieldName) >> r1;
            results2.lookup(fieldName) >> r2;

            Type result = applyOperation(r1, r2);

            Info(log_)<< "    " << operationTypeNames_[operation_]
                << "(" << fieldName << ") = " << result
                << endl;

            if (Pstream::master())
            {
                file()<< tab << result;
            }

            found = true;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
