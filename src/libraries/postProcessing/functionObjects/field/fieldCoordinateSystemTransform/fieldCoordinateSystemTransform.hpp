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
    CML::fieldCoordinateSystemTransform

Description
    Transforms fields from global cartesian co-ordinates to local co-ordinate
    system

SourceFiles
    fieldCoordinateSystemTransform.cpp
    IOfieldCoordinateSystemTransform.hpp

\*---------------------------------------------------------------------------*/

#ifndef fieldCoordinateSystemTransform_H
#define fieldCoordinateSystemTransform_H

#include "OFstream.hpp"
#include "pointFieldFwd.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "coordinateSystem.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "Time.hpp"
#include "transformGeometricField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
               Class fieldCoordinateSystemTransform Declaration
\*---------------------------------------------------------------------------*/

class fieldCoordinateSystemTransform
{
protected:

    // Protected data

        //- Name
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Fields to transform
        wordList fieldSet_;

        //- Co-ordinate system to transform to
        coordinateSystem coordSys_;


    // Protected Member Functions

        //- Disallow default bitwise copy construct
        fieldCoordinateSystemTransform(const fieldCoordinateSystemTransform&);

        //- Disallow default bitwise assignment
        void operator=(const fieldCoordinateSystemTransform&);

        template<class Type>
        void transform(const word& fieldName) const;

        template<class Type>
        void transformField(const Type& field) const;


public:

    //- Runtime type information
    TypeName("fieldCoordinateSystemTransform");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        fieldCoordinateSystemTransform
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~fieldCoordinateSystemTransform();


    // Member Functions

        //- Return name of the fieldCoordinateSystemTransform object
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the input data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

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

template<class Type>
void CML::fieldCoordinateSystemTransform::transformField
(
    const Type& field
) const
{
    const word& fieldName = field.name() + "Transformed";

    dimensionedTensor R("R", field.dimensions(), coordSys_.R());

    if (obr_.foundObject<Type>(fieldName))
    {
        Type& transField =
            const_cast<Type&>(obr_.lookupObject<Type>(fieldName));

        transField == field;

        forAll(field, i)
        {
            CML::transform(transField, R, transField);
        }

        transField.write();
    }
    else
    {
        Type& transField = obr_.store
        (
            new Type
            (
                IOobject
                (
                    fieldName,
                    obr_.time().timeName(),
                    obr_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                field
            )
        );

        forAll(field, i)
        {
            CML::transform(transField, R, transField);
        }

        transField.write();
    }
}


template<class Type>
void CML::fieldCoordinateSystemTransform::transform
(
    const word& fieldName
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    if (obr_.foundObject<vfType>(fieldName))
    {
        if (debug)
        {
            Info<< type() << ": Field " << fieldName << " already in database"
                << endl;
        }

        transformField<vfType>(obr_.lookupObject<vfType>(fieldName));
    }
    else if (obr_.foundObject<sfType>(fieldName))
    {
        if (debug)
        {
            Info<< type() << ": Field " << fieldName << " already in database"
                << endl;
        }

        transformField<sfType>(obr_.lookupObject<sfType>(fieldName));
    }
    else
    {
        IOobject fieldHeader
        (
            fieldName,
            obr_.time().timeName(),
            obr_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if
        (
            fieldHeader.headerOk()
         && fieldHeader.headerClassName() == vfType::typeName
        )
        {
            if (debug)
            {
                Info<< type() << ": Field " << fieldName << " read from file"
                    << endl;
            }

            transformField<vfType>(obr_.lookupObject<vfType>(fieldName));
        }
        else if
        (
            fieldHeader.headerOk()
         && fieldHeader.headerClassName() == sfType::typeName
        )
        {
            if (debug)
            {
                Info<< type() << ": Field " << fieldName << " read from file"
                    << endl;
            }

            transformField<sfType>(obr_.lookupObject<sfType>(fieldName));
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
