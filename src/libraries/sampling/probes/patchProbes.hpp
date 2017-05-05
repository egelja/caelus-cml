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
    CML::patchProbes

Description
    Set of locations to sample.at patches

    Call write() to sample and write files.


\*---------------------------------------------------------------------------*/

#ifndef patchProbes_H
#define patchProbes_H

#include "probes.hpp"
#include "volFields.hpp"
#include "IOmanip.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fvMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                          Class patchProbes Declaration
\*---------------------------------------------------------------------------*/

class patchProbes
:
    public probes
{
    // Private data

        //- Patch name
        word patchName_;


    // Private Member Functions

        //- Sample and write a particular volume field
        template<class Type>
        void sampleAndWrite
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        );


        //- Sample and write a particular surface field
        template<class Type>
        void sampleAndWrite
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        );


        //- Sample and write all the fields of the given type
        template <class Type>
        void sampleAndWrite(const fieldGroup<Type>&);


         //- Sample and write all the surface fields of the given type
        template<class Type>
        void sampleAndWriteSurfaceFields(const fieldGroup<Type>&);


        //- Sample a volume field at all locations
        template<class Type>
        tmp<Field<Type> > sample
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;


        //- Sample a surface field at all locations
        template<class Type>
        tmp<Field<Type> > sample
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        ) const;


        //- Sample a single field on all sample locations
        template <class Type>
        tmp<Field<Type> > sample(const word& fieldName) const;


        //- Disallow default bitwise copy construct
        patchProbes(const patchProbes&);

        //- Disallow default bitwise assignment
        void operator=(const patchProbes&);


public:

    //- Runtime type information
    TypeName("patchProbes");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        patchProbes
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~patchProbes();

    //- Public members

        //- Sample and write
        virtual void write();

        //- Read
        virtual void read(const dictionary&);

        //- Find elements containing patchProbes
        virtual void findElements(const fvMesh&);


};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::patchProbes::sampleAndWrite
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
)
{
    Field<Type> values(sample(vField));

    if (Pstream::master())
    {
        unsigned int w = IOstream::defaultPrecision() + 7;
        OFstream& probeStream = *probeFilePtrs_[vField.name()];

        probeStream << setw(w) << vField.time().value();

        forAll(values, probeI)
        {
            probeStream << ' ' << setw(w) << values[probeI];
        }
        probeStream << endl;
    }
}


template<class Type>
void CML::patchProbes::sampleAndWrite
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
)
{
    Field<Type> values(sample(sField));

    if (Pstream::master())
    {
        unsigned int w = IOstream::defaultPrecision() + 7;
        OFstream& probeStream = *probeFilePtrs_[sField.name()];

        probeStream << setw(w) << sField.time().value();

        forAll(values, probeI)
        {
            probeStream << ' ' << setw(w) << values[probeI];
        }
        probeStream << endl;
    }
}


template <class Type>
void CML::patchProbes::sampleAndWrite
(
    const fieldGroup<Type>& fields
)
{
    forAll(fields, fieldI)
    {
        if (loadFromFiles_)
        {
            sampleAndWrite
            (
                GeometricField<Type, fvPatchField, volMesh>
                (
                    IOobject
                    (
                        fields[fieldI],
                        mesh_.time().timeName(),
                        mesh_,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    mesh_
                )
            );
        }
        else
        {
            objectRegistry::const_iterator iter = mesh_.find(fields[fieldI]);

            if
            (
                iter != objectRegistry::end()
             && iter()->type()
             == GeometricField<Type, fvPatchField, volMesh>::typeName
            )
            {
                sampleAndWrite
                (
                    mesh_.lookupObject
                    <GeometricField<Type, fvPatchField, volMesh> >
                    (
                        fields[fieldI]
                    )
                );
            }
        }
    }
}


template<class Type>
void CML::patchProbes::sampleAndWriteSurfaceFields
(
    const fieldGroup<Type>& fields
)
{
    forAll(fields, fieldI)
    {
        if (loadFromFiles_)
        {
            sampleAndWrite
            (
                GeometricField<Type, fvsPatchField, surfaceMesh>
                (
                    IOobject
                    (
                        fields[fieldI],
                        mesh_.time().timeName(),
                        mesh_,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    mesh_
                )
            );
        }
        else
        {
            objectRegistry::const_iterator iter = mesh_.find(fields[fieldI]);

            if
            (
                iter != objectRegistry::end()
             && iter()->type()
             == GeometricField<Type, fvsPatchField, surfaceMesh>::typeName
            )
            {
                sampleAndWrite
                (
                    mesh_.lookupObject
                    <GeometricField<Type, fvsPatchField, surfaceMesh> >
                    (
                        fields[fieldI]
                    )
                );
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> >
CML::patchProbes::sample
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
    const Type unsetVal(-VGREAT*pTraits<Type>::one);

    tmp<Field<Type> > tValues
    (
        new Field<Type>(this->size(), unsetVal)
    );

    Field<Type>& values = tValues();

    const polyBoundaryMesh& patches = mesh_.boundaryMesh();

    forAll(*this, probeI)
    {
        label faceI = elementList_[probeI];

        if (faceI >= 0)
        {
            label patchI = patches.whichPatch(faceI);
            label localFaceI = patches[patchI].whichFace(faceI);
            values[probeI] = vField.boundaryField()[patchI][localFaceI];
        }
    }

    Pstream::listCombineGather(values, isNotEqOp<Type>());
    Pstream::listCombineScatter(values);

    return tValues;
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::patchProbes::sample(const word& fieldName) const
{
    return sample
    (
        mesh_.lookupObject<GeometricField<Type, fvPatchField, volMesh> >
        (
            fieldName
        )
    );
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::patchProbes::sample
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
) const
{
    const Type unsetVal(-VGREAT*pTraits<Type>::one);

    tmp<Field<Type> > tValues
    (
        new Field<Type>(this->size(), unsetVal)
    );

    Field<Type>& values = tValues();

    const polyBoundaryMesh& patches = mesh_.boundaryMesh();

    forAll(*this, probeI)
    {
        label faceI = elementList_[probeI];

        if (faceI >= 0)
        {
            label patchI = patches.whichPatch(faceI);
            label localFaceI = patches[patchI].whichFace(faceI);
            values[probeI] = sField.boundaryField()[patchI][localFaceI];
        }
    }

    Pstream::listCombineGather(values, isNotEqOp<Type>());
    Pstream::listCombineScatter(values);

    return tValues;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
