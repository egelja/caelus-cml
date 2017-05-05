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
    CML::calcMag

Group
    grpFVFunctionObjects

Description
    This function object calculates the magnitude of a field.  The operation
    can be applied to any volume or surface fieldsm and the output is a
    volume or surface scalar field.

SourceFiles
    calcMag.cpp
    IOcalcMag.hpp

\*---------------------------------------------------------------------------*/

#ifndef calcMag_H
#define calcMag_H

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
                           Class calcMag Declaration
\*---------------------------------------------------------------------------*/

class calcMag
{
    // Private data

        //- Name of this calcMag object
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

        //- Helper function to create/store/return the mag field
        template<class FieldType>
        FieldType& magField(const word& magName, const dimensionSet& dims);

        //- Helper function to calculate the magnitude of different field types
        template<class Type>
        void calc
        (
            const word& fieldName,
            const word& resultName,
            bool& processed
        );

        //- Disallow default bitwise copy construct
        calcMag(const calcMag&);

        //- Disallow default bitwise assignment
        void operator=(const calcMag&);


public:

    //- Runtime type information
    TypeName("calcMag");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        calcMag
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~calcMag();


    // Member Functions

        //- Return name of the set of calcMag
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the calcMag data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the calcMag and write
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
#include "Time.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

template<class FieldType>
FieldType& CML::calcMag::magField
(
    const word& magName,
    const dimensionSet& dims
)
{
    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (!mesh.foundObject<FieldType>(magName))
    {
        FieldType* magFieldPtr
        (
            new FieldType
            (
                IOobject
                (
                    magName,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dims, 0.0)
            )
        );

        mesh.objectRegistry::store(magFieldPtr);
    }

    const FieldType& f = mesh.lookupObject<FieldType>(magName);

    return const_cast<FieldType&>(f);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::calcMag::calc
(
    const word& fieldName,
    const word& resultName,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (mesh.foundObject<vfType>(fieldName))
    {
        const vfType& vf = mesh.lookupObject<vfType>(fieldName);

        volScalarField& field =
            magField<volScalarField>(resultName_, vf.dimensions());

        field = mag(vf);

        processed = true;
    }
    else if (mesh.foundObject<sfType>(fieldName))
    {
        const sfType& sf = mesh.lookupObject<sfType>(fieldName);

        surfaceScalarField& field =
            magField<surfaceScalarField>(resultName_, sf.dimensions());

        field = mag(sf);

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
