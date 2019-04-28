/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Tyler Voskuilen
Copyright (C) 2018 Applied CCM Pty Ltd
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
    CML::dynamicRefineBalancedFvMesh

Description
    A fvMesh with built-in refinement and run-time load balancing.

SourceFiles
    dynamicRefineBalancedFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef dynamicRefineBalancedFvMesh_HPP
#define dynamicRefineBalancedFvMesh_HPP

#include "dynamicFvMesh.hpp"
#include "dynamicRefineFvMesh.hpp"
#include "hexRef8.hpp"
#include "PackedBoolList.hpp"
#include "Switch.hpp"
#include "decompositionMethod.hpp"
#include "fvMeshDistribute.hpp"
#include "mapDistributePolyMesh.hpp"
#include "HashTable.hpp"
#include "topoSetSource.hpp"
#include "cellSet.hpp"
#include "PtrDictionary.hpp"
#include "dictionaryEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class dynamicRefineBalancedFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dynamicRefineBalancedFvMesh
:
    public dynamicRefineFvMesh
{

private:

    //- volScalarField of used for refinement
    volScalarField* internalRefinementFieldPtr_;

    //- Hash table of gradieint fields
    HashTable< Pair<scalar> > gradFields_;

    //- Hash table of regions
    PtrList<entry> refinedRegions_;

    //- Enable refinement using controls for this class
    Switch enableRefinementControl_;

    //- Write cell distribution
    Switch dumpCellDist_;

    //- Disallow default bitwise copy construct
    dynamicRefineBalancedFvMesh(const dynamicRefineBalancedFvMesh&);

    //- Disallow default bitwise assignment
    void operator=(const dynamicRefineBalancedFvMesh&);

    // Return parent cellID for this cell
    label topParentID(label p);

    //- Update field used for refinement
    void updateRefinementField();

    //- Read refinement dictionary
    void readRefinementDict();

    //- Read max/min level for refinement
    Pair<scalar> readRefinementPoints();

public:

    //- Runtime type information
    TypeName("dynamicRefineBalancedFvMesh");

    //- Construct from IOobject
    explicit dynamicRefineBalancedFvMesh(const IOobject& io);

    //- Destructor
    virtual ~dynamicRefineBalancedFvMesh();

    // Member Functions

    //- Update the mesh for both mesh motion and topology change
    virtual bool update();

    //- Write using given format, version and compression
    virtual bool writeObject
    (
        IOstream::streamFormat fmt,
        IOstream::versionNumber ver,
        IOstream::compressionType cmp
    ) const;

    //- Template to update all volField boundaries
    template<class Type> void correctBoundaries();

};


} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "GeometricField.hpp"
#include "volMesh.hpp"
#include "fvPatchField.hpp"

template<class Type>
void CML::dynamicRefineBalancedFvMesh::correctBoundaries()
{
    typedef GeometricField<Type, fvPatchField, volMesh> GeoField;

    HashTable<GeoField*> flds(this->objectRegistry::lookupClass<GeoField>());

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();
        
        // Mimic "evaluate" but only for coupled patches (processor or cyclic)
        // and only for blocking or nonBlocking comms (no scheduled comms)
        if
        (
            Pstream::defaultCommsType == Pstream::blocking
         || Pstream::defaultCommsType == Pstream::nonBlocking
        )
        {
            label nReq = Pstream::nRequests();

            forAll(fld.boundaryField(), patchi)
            {
                if (fld.boundaryField()[patchi].coupled())
                {
                    fld.boundaryField()[patchi].initEvaluate
                    (
                        Pstream::defaultCommsType
                    );
                }
            }

            // Block for any outstanding requests
            if
            (
                Pstream::parRun()
             && Pstream::defaultCommsType == Pstream::nonBlocking
            )
            {
                Pstream::waitRequests(nReq);
            }

            forAll(fld.boundaryField(), patchi)
            {
                if (fld.boundaryField()[patchi].coupled())
                {
                    fld.boundaryField()[patchi].evaluate
                    (
                        Pstream::defaultCommsType
                    );
                }
            }
        }
        else
        {
            // Scheduled patch updates not supported
            FatalErrorInFunction
                << "Unsupported communications type "
                << Pstream::commsTypeNames[Pstream::defaultCommsType]
                << exit(FatalError);
        }
    }
}


#endif
