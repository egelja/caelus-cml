/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "surfaceInterpolateFields.hpp"
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
                Info<< "        surface field " << sName << " already exists"
                    << endl;
            }
            else
            {
                label sz = sflds.size();
                sflds.setSize(sz+1);
                sflds.set(sz, new sfType(sName, linearInterpolate(fld)));

                Info<< "        interpolated " << fld.name() << " to create "
                    << sflds[sz].name() << endl;
            }
        }
    }
}


// ************************************************************************* //
