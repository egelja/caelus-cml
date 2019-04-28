/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::readAndUpdateFields

Description
    Reads fields at startup and updates the boundary conditions at every
    timestep. Also makes sure that the fields are automatically written

SourceFiles
    readAndUpdateFields.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#ifndef readAndUpdateFields_H
#define readAndUpdateFields_H

#include "OFstream.hpp"
#include "pointField.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class polyMesh;

/*---------------------------------------------------------------------------*\
                         Class readAndUpdateFields Declaration
\*---------------------------------------------------------------------------*/

class readAndUpdateFields
{
protected:

    // Protected data

        //- Name of this set of readAndUpdateFields object
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Fields to load
        wordList fieldSet_;

        //- Loaded fields
        PtrList<volScalarField> vsf_;
        PtrList<volVectorField> vvf_;
        PtrList<volSphericalTensorField> vSpheretf_;
        PtrList<volSymmTensorField> vSymmtf_;
        PtrList<volTensorField> vtf_;

        PtrList<pointScalarField> psf_;
        PtrList<pointVectorField> pvf_;
        PtrList<pointSphericalTensorField> pSpheretf_;
        PtrList<pointSymmTensorField> pSymmtf_;
        PtrList<pointTensorField> ptf_;

        autoPtr<pointMesh> pMesh_;

        const pointMesh &pMesh(const polyMesh &mesh);

    // Protected Member Functions

        //- Disallow default bitwise copy construct
        readAndUpdateFields(const readAndUpdateFields&);

        //- Disallow default bitwise assignment
        void operator=(const readAndUpdateFields&);

        template<class Type>
        bool loadField
        (
            const word&,
            PtrList<GeometricField<Type, fvPatchField, volMesh> >&,
            PtrList<GeometricField<Type, pointPatchField, pointMesh> >&
        );

        template <class FType>
        void correctBoundaryConditions(
            PtrList<FType> &
        );
public:

    //- Runtime type information
    TypeName("readAndUpdateFields");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        readAndUpdateFields
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~readAndUpdateFields();


    // Member Functions

        //- Return name of the readAndUpdateFields object
        virtual const word& name() const
        {
            return name_;
        }

        //- Called when time was set at the end of the Time::operator++ 
        virtual void timeSet();         

        //- Read the field min/max data
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

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "pointFields.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template <class FType>
void CML::readAndUpdateFields::correctBoundaryConditions(
    PtrList<FType> &flst
) {
    forAll(flst,i)
    {
	flst[i].correctBoundaryConditions();
        if(
            this->obr_.time().outputTime()
            &&
            flst[i].writeOpt()==IOobject::AUTO_WRITE
        ) {
            flst[i].write();
        }
    }
}

template<class Type>
bool CML::readAndUpdateFields::loadField
(
    const word& fieldName,
    PtrList<GeometricField<Type, fvPatchField, volMesh> >& vflds,
    PtrList<GeometricField<Type, pointPatchField, pointMesh> >& pflds
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
    typedef GeometricField<Type, pointPatchField, pointMesh> pfType;

    if (obr_.foundObject<vfType>(fieldName))
    {
        if (debug)
        {
            Info<< "readAndUpdateFields : Field " << fieldName << " already in database"
                << endl;
        }
    }
    else if (obr_.foundObject<sfType>(fieldName))
    {
        if (debug)
        {
            Info<< "readAndUpdateFields : Field " << fieldName << " already in database"
                << endl;
        }
    }
    else if (obr_.foundObject<pfType>(fieldName))
    {
        if (debug)
        {
            Info<< "readAndUpdateFields : Field " << fieldName << " already in database"
                << endl;
        }
    }
    else
    {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        IOobject fieldHeader
        (
            fieldName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        );

        if
        (
            fieldHeader.headerOk()
         && fieldHeader.headerClassName() == vfType::typeName
        )
        {
            // store field locally
            Info<< "    Reading " << fieldName << endl;
            label sz = vflds.size();
            vflds.setSize(sz+1);
            vflds.set(sz, new vfType(fieldHeader, mesh));
            //            vflds[sz].writeOpt()=IOobject::AUTO_WRITE;

            return true;
        }
        else if
        (
            fieldHeader.headerOk()
         && fieldHeader.headerClassName() == pfType::typeName
        )
        {
            // store field locally
            Info<< "    Reading " << fieldName << endl;
            label sz = pflds.size();
            pflds.setSize(sz+1);
            pflds.set(sz, new pfType(fieldHeader, this->pMesh(mesh)));
            //            pflds[sz].writeOpt()=IOobject::AUTO_WRITE;

            return true;
        }
        else if
        (
            fieldHeader.headerOk()
         && fieldHeader.headerClassName() == sfType::typeName
        )
        {
            WarningInFunction
                << "Field " << fieldName << " is a "
                    << sfType::typeName
                    << " and surface-fields don't support correctBoundaryConditions"
                    << endl << "-> Not read"
                    << endl;

            return true;
        }
    }
    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
