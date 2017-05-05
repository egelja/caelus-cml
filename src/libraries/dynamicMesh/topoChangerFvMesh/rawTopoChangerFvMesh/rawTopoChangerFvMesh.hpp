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
    CML::rawTopoChangerFvMesh

Description
    topoChangerFvMesh without any added functionality.

    Note: run without CAELUS_SETNAN. Temporary has unitialised patch
    data when faces get created out of nothing.


\*---------------------------------------------------------------------------*/

#ifndef rawTopoChangerFvMesh_H
#define rawTopoChangerFvMesh_H

#include "topoChangerFvMesh.hpp"
#include "PackedBoolList.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                      Class rawTopoChangerFvMesh Declaration
\*---------------------------------------------------------------------------*/

class rawTopoChangerFvMesh
:
    public topoChangerFvMesh
{
    // Private Member Functions

        //- Set unmapped values
        template<class Type, template<class> class PatchField, class GeoMesh>
        static void setUnmappedValues
        (
            GeometricField<Type, PatchField, GeoMesh>& fld,
            const PackedBoolList& mappedFace,
            const GeometricField<Type, PatchField, GeoMesh>& baseFld
        );

        template<class Type, template<class> class PatchField, class GeoMesh>
        void zeroUnmappedValues(const PackedBoolList&) const;

        //- Disallow default bitwise copy construct
        rawTopoChangerFvMesh(const rawTopoChangerFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const rawTopoChangerFvMesh&);

public:

    //- Runtime type information
    TypeName("rawTopoChangerFvMesh");


    // Constructors

        //- Construct from database
        explicit rawTopoChangerFvMesh(const IOobject& io);

    //- Destructor
    virtual ~rawTopoChangerFvMesh();


    // Member Functions

        //- Update the mesh for both mesh motion and topology change
        virtual bool update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::rawTopoChangerFvMesh::setUnmappedValues
(
    GeometricField<Type, PatchField, GeoMesh>& fld,
    const PackedBoolList& mappedFace,
    const GeometricField<Type, PatchField, GeoMesh>& baseFld
)
{
    //Pout<< "Checking field " << fld.name() << endl;

    forAll(fld.boundaryField(), patchI)
    {
        PatchField<Type>& fvp = const_cast<PatchField<Type>&>
        (
            fld.boundaryField()[patchI]
        );

        const label start = fvp.patch().start();
        forAll(fvp, i)
        {
            if (!mappedFace[start+i])
            {
                //Pout<< "** Resetting unassigned value on patch "
                //    << fvp.patch().name()
                //    << " localface:" << i
                //    << " to:" << baseFld.boundaryField()[patchI][i] << endl;
                fvp[i] = baseFld.boundaryField()[patchI][i];
            }
        }
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::rawTopoChangerFvMesh::zeroUnmappedValues
(
    const PackedBoolList& mappedFace
) const
{
    typedef GeometricField<Type, PatchField, GeoMesh> FieldType;

    const wordList fldNames(names(FieldType::typeName));

    forAll(fldNames, i)
    {
        //Pout<< "Checking field " << fldNames[i] << endl;

        FieldType& fld = const_cast<FieldType&>
        (
            lookupObject<FieldType>(fldNames[i])
        );

        setUnmappedValues
        (
            fld,
            mappedFace,
            FieldType
            (
                IOobject
                (
                    "zero",
                    time().timeName(),
                    *this,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                *this,
                dimensioned<Type>("0", fld.dimensions(), pTraits<Type>::zero)
            )
        );
    }
}


#endif

// ************************************************************************* //
