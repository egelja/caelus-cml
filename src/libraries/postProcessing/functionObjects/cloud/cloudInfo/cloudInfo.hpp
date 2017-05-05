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
    CML::cloudInfo

Group
    grpCloudFunctionObjects

Description
    This function object outputs Lagrangian cloud information to a file.  The
    current outputs include:
    - total current number of parcels
    - total current mass of parcels

    Example of function object specification:
    \verbatim
    cloudInfo1
    {
        type        cloudInfo;
        functionObjectLibs ("libcloudFunctionObjects.so");
        ...
        clouds
        (
            kinematicCloud1
            thermoCloud1
        );
    }
    \endverbatim


    \heading Function object usage

    \table
        Property     | Description             | Required    | Default value
        type         | type name: cloudInfo    | yes         |
        clouds       | list of clouds names to process |yes  |
    \endtable

    The output data of each cloud is written to a file named \<cloudName\>.dat

SeeAlso
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    cloudInfo.cpp
    IOcloudInfo.hpp

\*---------------------------------------------------------------------------*/

#ifndef cloudInfo_H
#define cloudInfo_H

#include "functionObjectFile.hpp"
#include "PtrList.hpp"
#include "pointFieldFwd.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                         Class cloudInfo Declaration
\*---------------------------------------------------------------------------*/

class cloudInfo
:
    public functionObjectFile
{
protected:

    // Protected data

        //- Name of this set of cloudInfo object
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- on/off switch
        bool active_;


    // Protected Member Functions

        //- File header information
        virtual void writeFileHeader(const label i);

        //- Disallow default bitwise copy construct
        cloudInfo(const cloudInfo&);

        //- Disallow default bitwise assignment
        void operator=(const cloudInfo&);


public:

    //- Runtime type information
    TypeName("cloudInfo");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        cloudInfo
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~cloudInfo();


    // Member Functions

        //- Return name of the cloudInfo object
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the field min/max data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Write
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
