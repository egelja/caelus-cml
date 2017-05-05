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
    CML::fieldValue

Description
    Base class for field value -based function objects.

SourceFiles
    fieldValue.cpp

\*---------------------------------------------------------------------------*/

#ifndef fieldValue_H
#define fieldValue_H

#include "Switch.hpp"
#include "pointFieldFwd.hpp"
#include "OFstream.hpp"
#include "ListListOps.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class dictionary;
class objectRegistry;
class fvMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class fieldValue Declaration
\*---------------------------------------------------------------------------*/

class fieldValue
{

protected:

    // Protected data

        //- Name of this fieldValue object
        word name_;

        //- Database this class is registered to
        const objectRegistry& obr_;

        //- Active flag
        bool active_;

        //- Switch to send output to Info as well as to file
        Switch log_;

        //- Name of source object
        word sourceName_;

        //- List of field names to operate on
        wordList fields_;

        //- Output field values flag
        Switch valueOutput_;

        //- Output file pointer
        autoPtr<OFstream> outputFilePtr_;


        // Functions to be over-ridden from IOoutputFilter class

            //- Make the output file
            virtual void makeFile();

            //- Write the output file header
            virtual void writeFileHeader() = 0;

            //- Update mesh
            virtual void updateMesh(const mapPolyMesh&);

            //- Move points
            virtual void movePoints(const Field<point>&);


public:

    //- Run-time type information
    TypeName("fieldValue");


    //- Construct from components
    fieldValue
    (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~fieldValue();


    // Public Member Functions

        // Access

            //- Return the name of the geometric source
            inline const word& name() const;

            //- Return the reference to the object registry
            inline const objectRegistry& obr() const;

            //- Return the active flag
            inline bool active() const;

            //- Return the switch to send output to Info as well as to file
            inline const Switch& log() const;

            //- Return the source name
            inline const word& sourceName() const;

            //- Return the list of field names
            inline const wordList& fields() const;

            //- Return the output field values flag
            inline const Switch& valueOutput() const;

            //- Helper function to return the reference to the mesh
            inline const fvMesh& mesh() const;


        // Function object functions

            //- Read from dictionary
            virtual void read(const dictionary& dict);

            //- Write to screen/file
            virtual void write();

            //- Execute
            virtual void execute();

            //- Execute the at the final time-loop, currently does nothing
            virtual void end();

            //- Combine fields from all processor domains into single field
            template<class Type>
            void combineFields(Field<Type>& field);

            //- Combine fields from all processor domains into single field
            template<class Type>
            void combineFields(tmp<Field<Type> >&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::fieldValue::name() const
{
    return name_;
}


inline const CML::objectRegistry& CML::fieldValue::obr() const
{
    return obr_;
}


inline bool CML::fieldValue::active() const
{
    return active_;
}


inline const CML::Switch& CML::fieldValue::log() const
{
    return log_;
}


inline const CML::word& CML::fieldValue::sourceName() const
{
    return sourceName_;
}


inline const CML::wordList& CML::fieldValue::fields() const
{
    return fields_;
}


inline const CML::Switch& CML::fieldValue::valueOutput() const
{
    return valueOutput_;
}


inline const CML::fvMesh& CML::fieldValue::mesh() const
{
    return refCast<const fvMesh>(obr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fieldValue::combineFields(Field<Type>& field)
{
    List<Field<Type> > allValues(Pstream::nProcs());

    allValues[Pstream::myProcNo()] = field;

    Pstream::gatherList(allValues);

    if (Pstream::master())
    {
        field =
            ListListOps::combine<Field<Type> >
            (
                allValues,
                accessOp<Field<Type> >()
            );
    }
}


template<class Type>
void CML::fieldValue::combineFields(tmp<Field<Type> >& field)
{
    combineFields(field());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
