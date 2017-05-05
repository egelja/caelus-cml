/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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
    CML::Lambda2

Description
    This function object calculates and outputs the second largest eigenvalue
    of the sum of the square of the symmetrical and anti-symmetrical parts of
    the velocity gradient tensor.

SourceFiles
    Lambda2.cpp
    IOLambda2.hpp

\*---------------------------------------------------------------------------*/

#ifndef Lambda2_H
#define Lambda2_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"
#include "pointFieldFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                          Class Lambda2 Declaration
\*---------------------------------------------------------------------------*/

class Lambda2
{
    // Private data

        //- Name of this set of Lambda2 objects
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Name of velocity field, default is "U"
        word UName_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        Lambda2(const Lambda2&);

        //- Disallow default bitwise assignment
        void operator=(const Lambda2&);


public:

    //- Runtime type information
    TypeName("Lambda2");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        Lambda2
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~Lambda2();


    // Member Functions

        //- Return name of the set of Lambda2
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the Lambda2 data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the Lambda2 and write
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
