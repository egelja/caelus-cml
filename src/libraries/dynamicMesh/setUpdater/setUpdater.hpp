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
    CML::setUpdater

Description
    Keeps cell/face/vertex sets uptodate. Both the ones loaded and the ones
    on disk.

SourceFiles
    setUpdater.cpp

\*---------------------------------------------------------------------------*/

#ifndef setUpdater_H
#define setUpdater_H

#include "polyMeshModifier.hpp"
#include "polyMesh.hpp"
#include "Time.hpp"
#include "mapPolyMesh.hpp"
#include "IOobjectList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class setUpdater Declaration
\*---------------------------------------------------------------------------*/

class setUpdater
:
    public polyMeshModifier
{
    // Private Member Functions

        //- Updates all sets
        template<class Type>
        void updateSets(const mapPolyMesh& morphMap) const;

        //- Disallow default bitwise copy construct
        setUpdater(const setUpdater&);

        //- Disallow default bitwise assignment
        void operator=(const setUpdater&);


public:

    //- Runtime type information
    TypeName("setUpdater");


    // Constructors

        //- Construct from dictionary
        setUpdater
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const polyTopoChanger& mme
        );


    //- Destructor
    virtual ~setUpdater();


    // Member Functions

        //- Check for topology change
        virtual bool changeTopology() const;

        //- Insert the layer addition/removal instructions
        //  into the topological change
        virtual void setRefinement(polyTopoChange&) const;

        //- Modify motion points to comply with the topological change
        virtual void modifyMotionPoints(pointField& motionPoints) const;

        //- Force recalculation of locally stored data on topological change
        virtual void updateMesh(const mapPolyMesh&);

        //- Write
        virtual void write(Ostream&) const;

        //- Write dictionary
        virtual void writeDict(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::setUpdater::updateSets(const mapPolyMesh& morphMap) const
{
    //
    // Update all sets in memory.
    //

    HashTable<const Type*> memSets =
        morphMap.mesh().objectRegistry::lookupClass<Type>();

    forAllIter(typename HashTable<const Type*>, memSets, iter)
    {
        Type& set = const_cast<Type&>(*iter());

        if (debug)
        {
            Pout<< "Set:" << set.name() << " size:" << set.size()
                << " updated in memory" << endl;
        }

        set.updateMesh(morphMap);

        // Write or not? Debatable.
        set.write();
    }


    //
    // Update all sets on disk
    //

    // Get last valid mesh (discard points-only change)
    IOobjectList Objects
    (
        morphMap.mesh().time(),
        morphMap.mesh().facesInstance(),
        "polyMesh/sets"
    );

    IOobjectList fileSets(Objects.lookupClass(Type::typeName));

    forAllConstIter(IOobjectList, fileSets, iter)
    {
        if (!memSets.found(iter.key()))
        {
            // Not in memory. Load it.
            Type set(*iter());

            if (debug)
            {
                Pout<< "Set:" << set.name() << " size:" << set.size()
                    << " updated on disk" << endl;
            }

            set.updateMesh(morphMap);

            set.write();
        }
        else
        {
            if (debug)
            {
                Pout<< "Set:" << iter.key() << " already updated from memory"
                    << endl;
            }
        }
    }
}


#endif

// ************************************************************************* //
