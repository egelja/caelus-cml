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
    CML::surfaceInterpolateFields

Description
    Linear interpolates volFields to surfaceFields

    - at write it writes the fields
    - it executes every time step

    so it can either be used to calculate and write the interpolate or
    (since the interpolates are registered) use some other functionObject
    to work on them.

    sampleSomeFields
    {
        type            surfaceInterpolateFields;
        ..
        enabled         true;
        outputControl   outputTime;
        ..
        // Name of volField and corresponding surfaceField
        fields          ((p pInterpolate)(U UInterpolate));
    }


SourceFiles
    surfaceInterpolateFields.cpp
    IOsurfaceInterpolateFields.hpp

\*---------------------------------------------------------------------------*/

#ifndef surfaceInterpolateFields_H
#define surfaceInterpolateFields_H

#include "OFstream.hpp"
#include "surfaceFields.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                         Class surfaceInterpolateFields Declaration
\*---------------------------------------------------------------------------*/

class surfaceInterpolateFields
{
protected:

    // Protected data

        //- Name of this set of surfaceInterpolateFields object
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Fields to process
        //wordList fieldSet_;
        List<Tuple2<word, word> > fieldSet_;

        //- Locally constructed fields
        PtrList<surfaceScalarField> ssf_;
        PtrList<surfaceVectorField> svf_;
        PtrList<surfaceSphericalTensorField> sSpheretf_;
        PtrList<surfaceSymmTensorField> sSymmtf_;
        PtrList<surfaceTensorField> stf_;


    // Protected Member Functions

        //- Disallow default bitwise copy construct
        surfaceInterpolateFields(const surfaceInterpolateFields&);

        //- Disallow default bitwise assignment
        void operator=(const surfaceInterpolateFields&);

        template<class Type>
        void interpolateFields
        (
            PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
        ) const;


public:

    //- Runtime type information
    TypeName("surfaceInterpolateFields");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        surfaceInterpolateFields
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~surfaceInterpolateFields();


    // Member Functions

        //- Return name of the surfaceInterpolateFields object
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

#include "volFields.hpp"
#include "linear.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::surfaceInterpolateFields::interpolateFields
(
    PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    // Convert field to map
    HashTable<word> fieldMap(2*fieldSet_.size());
    forAll(fieldSet_, i)
    {
        fieldMap.insert(fieldSet_[i].first(), fieldSet_[i].second());
    }


    HashTable<const vfType*> flds(obr_.lookupClass<vfType>());

    forAllConstIter(typename HashTable<const vfType*>, flds, iter)
    {
        const vfType& fld = *iter();

        if (fieldMap.found(fld.name()))
        {
            //const word sName = "interpolate(" + fld.name() + ')';
            const word& sName = fieldMap[fld.name()];

            if (obr_.found(sName))
            {
                Info<< "    a surfaceField " << sName << " already exists"
                    << endl;
            }
            else
            {
                label sz = sflds.size();
                sflds.setSize(sz+1);
                sflds.set(sz, new sfType(sName, linearInterpolate(fld)));

                Info<< "    interpolated " << fld.name() << " to create "
                    << sflds[sz].name() << endl;
            }
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
