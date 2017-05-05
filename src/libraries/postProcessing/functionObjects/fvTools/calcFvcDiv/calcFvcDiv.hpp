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
    CML::calcFvcDiv

Group
    grpFVFunctionObjects

Description
    This function object calculates the divergence of a field.  The operation is
    limited to surfaceScalarFields and volumeVector fields, and the output is a
    volume scalar field.

SourceFiles
    calcFvcDiv.cpp
    IOcalcFvcDiv.hpp

\*---------------------------------------------------------------------------*/

#ifndef calcFvcDiv_H
#define calcFvcDiv_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class dimensionSet;

/*---------------------------------------------------------------------------*\
                         Class calcFvcDiv Declaration
\*---------------------------------------------------------------------------*/

class calcFvcDiv
{
    // Private data

        //- Name of this calcFvcDiv object
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Name of field to process
        word fieldName_;

        //- Name of result field
        word resultName_;


    // Private Member Functions

        //- Helper function to create/store/return the divergence field
        volScalarField& divField
        (
            const word& gradName,
            const dimensionSet& dims
        );

        //- Helper function to calculate the divergence of different field types
        template<class FieldType>
        void calcDiv
        (
            const word& fieldName,
            const word& resultName,
            bool& processed
        );

        //- Disallow default bitwise copy construct
        calcFvcDiv(const calcFvcDiv&);

        //- Disallow default bitwise assignment
        void operator=(const calcFvcDiv&);


public:

    //- Runtime type information
    TypeName("calcFvcDiv");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        calcFvcDiv
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~calcFvcDiv();


    // Member Functions

        //- Return name of the set of calcFvcDiv
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the calcFvcDiv data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the calcFvcDiv and write
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

#include "fvMesh.hpp"
#include "fvcDiv.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class FieldType>
void CML::calcFvcDiv::calcDiv
(
    const word& fieldName,
    const word& resultName,
    bool& processed
)
{
    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (mesh.foundObject<FieldType>(fieldName))
    {
        const FieldType& vf = mesh.lookupObject<FieldType>(fieldName);

        volScalarField& field = divField(resultName, vf.dimensions());

        field = fvc::div(vf);

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
