/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::partialWrite

Group
    grpIOFunctionObjects

Description
    This function object allows user-selected fields/registered objects to be
    written at a custom write interval. The interval is given in terms of
    number of overall dumps

    Example of function object specification:
    \verbatim
    partialWrite1
    {
        type        partialWrite;
        functionObjectLibs ("libIOFunctionObjects.so");
        ...
        objectNames (p U T);
        writeInterval 100;
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | type name: partialWrite | yes         |
        objectNames  | objects to write        | yes         |
        writeInterval | write interval         | yes         |
    \endtable

SeeAlso
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    partialWrite.cpp
    IOpartialWrite.hpp

\*---------------------------------------------------------------------------*/

#ifndef partialWrite_H
#define partialWrite_H

#include "HashSet.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                        Class partialWrite Declaration
\*---------------------------------------------------------------------------*/

class partialWrite
{
protected:

    // Private data

        //- Name of this set of partialWrite
        word name_;

        const objectRegistry& obr_;

        //- Loaded fields
        UPtrList<volScalarField> vsf_;
        UPtrList<volVectorField> vvf_;
        UPtrList<volSphericalTensorField> vSpheretf_;
        UPtrList<volSymmTensorField> vSymmtf_;
        UPtrList<volTensorField> vtf_;

        UPtrList<surfaceScalarField> ssf_;
        UPtrList<surfaceVectorField> svf_;
        UPtrList<surfaceSphericalTensorField> sSpheretf_;
        UPtrList<surfaceSymmTensorField> sSymmtf_;
        UPtrList<surfaceTensorField> stf_;

        // Read from dictionary

            //- Names of objects to dump always
            HashSet<word> objectNames_;

            //- Write interval for restart dump
            label writeInterval_;



        //- Current dump instance. If reaches writeInterval do a full write.
        label writeInstance_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        partialWrite(const partialWrite&);

        //- Disallow default bitwise assignment
        void operator=(const partialWrite&);


        //- Load objects in the objectNames
        template<class Type>
        void loadField
        (
            const word&,
            UPtrList<GeometricField<Type, fvPatchField, volMesh> >&,
            UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
        ) const;

        template<class Type>
        void changeWriteOptions
        (
            UPtrList<GeometricField<Type, fvPatchField, volMesh> >&,
            UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&,
            const IOobject::writeOption
        ) const;


public:

    //- Runtime type information
    TypeName("partialWrite");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        partialWrite
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~partialWrite();


    // Member Functions

        //- Return name of the partialWrite
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the partialWrite data
        virtual void read(const dictionary&);

        //- Execute
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Write the partialWrite
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

#include "partialWrite.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::partialWrite::loadField
(
    const word& fieldName,
    UPtrList<GeometricField<Type, fvPatchField, volMesh> >& vflds,
    UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    if (obr_.foundObject<vfType>(fieldName))
    {
        if (debug)
        {
            Info<< "partialWrite : Field "
                << fieldName << " found in database" << endl;
        }

        vfType& vField =
            const_cast<vfType&>
            (
                obr_.lookupObject<vfType>(fieldName)
            );


        const unsigned int sz = vflds.size();
        vflds.setSize(sz + 1);
        vflds.set(sz, &vField);
    }
    else if (obr_.foundObject<sfType>(fieldName))
    {
        if (debug)
        {
            Info<< "partialWrite : Field " << fieldName
                << " found in database" << endl;
        }

         sfType& sField =
            const_cast<sfType&>
            (
                obr_.lookupObject<sfType>(fieldName)
            );


        const unsigned int sz = sflds.size();
        sflds.setSize(sz + 1);
        sflds.set(sz, &sField);
    }
}


template<class Type>
void CML::partialWrite::changeWriteOptions
(
    UPtrList<GeometricField<Type, fvPatchField, volMesh> >& vflds,
    UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds,
    const IOobject::writeOption wOption
) const
{
    forAll(vflds , i)
    {
        vflds[i].writeOpt() = wOption;
    }

    forAll(sflds , i)
    {
        sflds[i].writeOpt() = wOption;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
