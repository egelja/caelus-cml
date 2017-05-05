/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2014 OpenFOAM Foundation
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
    CML::writeDictionary

Group
    grpUtilitiesFunctionObjects

Description
    This function object writes dictionaries on start-up, and on change

SourceFiles
    writeDictionary.cpp
    IOwriteDictionary.hpp

\*---------------------------------------------------------------------------*/

#ifndef writeDictionary_H
#define writeDictionary_H

#include "pointFieldFwd.hpp"
#include "wordList.hpp"
#include "runTimeSelectionTables.hpp"
#include "SHA1Digest.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class writeDictionary Declaration
\*---------------------------------------------------------------------------*/

class writeDictionary
{
protected:

    // Private data

        //- Name of this set of writeDictionary
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- Names of dictionaries to monitor
        wordList dictNames_;

        //- List of changed dictionaries (only those registered to database)
        List<SHA1Digest> digests_;


    // Private Member Functions

        //- Helper function to write the dictionary if found at location
        bool tryDirectory
        (
            const label dictI,
            const word& location,
            bool& firstDict
        );

        //- Disallow default bitwise copy construct
        writeDictionary(const writeDictionary&);

        //- Disallow default bitwise assignment
        void operator=(const writeDictionary&);


public:

    //- Runtime type information
    TypeName("writeDictionary");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        writeDictionary
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~writeDictionary();


    // Member Functions

        //- Return name of the writeDictionary
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the writeDictionary data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Write the writeDictionary
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const pointField&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
