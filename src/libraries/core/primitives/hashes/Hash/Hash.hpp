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
    CML::Hash

Description
    Hash function class for primitives.  All non-primitives used to hash
    entries on hash tables likely need a specialized version of this class.

\*---------------------------------------------------------------------------*/

#ifndef Hash_H
#define Hash_H

#include "label.hpp"
#include "uLabel.hpp"
#include "Hasher.hpp"
#include "pTraits.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
//template<class Type> class Hash;
//template<> class Hash<label>;

/*---------------------------------------------------------------------------*\
                            Class Hash Declaration
\*---------------------------------------------------------------------------*/

template<class PrimitiveType>
class Hash
{
public:

    Hash()
    {}

    unsigned operator()(const PrimitiveType& p, unsigned seed) const
    {
        return Hasher(&p, sizeof(p), seed);
    }

    unsigned operator()(const PrimitiveType& p) const
    {
        return Hasher(&p, sizeof(p));
    }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Hash specialization for hashing labels
template<>
class Hash<CML::label>
{
public:

    Hash()
    {}

    //- Incrementally hash a label.
    //  This will necessarily return a different value than the
    //  non-incremental version.
    unsigned operator()(const label p, unsigned seed) const
    {
        return Hasher(&p, sizeof(label), seed);
    }

    //- Return the unsigned representation of a label.
    //  This helps if people have relied on the hash value corresponding to
    //  the natural order.
    unsigned operator()(const label p) const
    {
        return p;
    }
};


//- Hash specialization for hashing pointer addresses.
//  Treat a pointer like a long.
//  This should work for both 32-bit and 64-bit pointers.
template<>
class Hash<void*>
{
public:

#ifdef windows
    typedef intptr_t HashType;
#else
    typedef long HashType;
#endif
    Hash()
    {}

    unsigned operator()(const void* const& p, unsigned seed) const
    {
        return Hash<HashType>()(HashType(p), seed);
    }

    unsigned operator()(const void* const& p) const
    {
        return Hash<HashType>()(HashType(p));
    }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
