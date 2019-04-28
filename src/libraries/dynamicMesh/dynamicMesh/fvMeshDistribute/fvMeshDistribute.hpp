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
    CML::fvMeshDistribute

Description
    Sends/receives parts of mesh+fvfields to neighbouring processors.
    Used in load balancing.

    Input is per local cell the processor it should move to. Moves meshes
    and volFields/surfaceFields and returns map which can be used to
    distribute other.

    Notes:
    - does not handle cyclics. Will probably handle separated proc patches.
    - if all cells move off processor also all its processor patches will
      get deleted so comms might be screwed up (since e.g. globalMeshData
      expects procPatches on all)
    - initial mesh has to have procPatches last and all normal patches common
      to all processors and in the same order. This is checked.
    - faces are matched topologically but points on the faces are not. So
      expect problems -on separated patches (cyclics?) -on zero sized processor
      edges.

SourceFiles
    fvMeshDistribute.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvMeshDistribute_H
#define fvMeshDistribute_H

#include "Field.hpp"
#include "fvMeshSubset.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class mapAddedPolyMesh;
class mapDistributePolyMesh;

/*---------------------------------------------------------------------------*\
                      Class fvMeshDistribute Declaration
\*---------------------------------------------------------------------------*/

class fvMeshDistribute
{
    // Private data

        //- Underlying fvMesh
        fvMesh& mesh_;

        //- Absolute merging tolerance (constructing meshes gets done using
        //  geometric matching)
        const scalar mergeTol_;


    // Private Member Functions

        static void inplaceRenumberWithFlip
        (
            const labelUList& oldToNew,
            const bool oldToNewHasFlip,
            const bool lstHasFlip,
            labelUList& lst
        );

        //- Find indices with value
        static labelList select
        (
            const bool selectEqual,
            const labelList& values,
            const label value
        );

        //- Check all procs have same names and in exactly same order.
        static void checkEqualWordList(const string&, const wordList&);

        //- Merge wordlists over all processors
        static wordList mergeWordList(const wordList&);


        // Patch handling

            //- Find patch to put exposed faces into.
            label findNonEmptyPatch() const;

            //- Save boundary fields
            template<class T, class Mesh>
            void saveBoundaryFields
            (
                PtrList<FieldField<fvsPatchField, T> >& bflds
            ) const;

            //- Map boundary fields
            template<class T, class Mesh>
            void mapBoundaryFields
            (
                const mapPolyMesh& map,
                const PtrList<FieldField<fvsPatchField, T> >& oldBflds
            );

            //- Save internal fields of surfaceFields
            template<class T>
            void saveInternalFields(PtrList<Field<T> >& iflds) const;

            //- Set value of patch faces resulting from internal faces
            template<class T>
            void mapExposedFaces
            (
                const mapPolyMesh& map,
                const PtrList<Field<T> >& oldFlds
            );

            //- Init patch fields of certain type
            template<class GeoField, class PatchFieldType>
            void initPatchFields
            (
                const typename GeoField::value_type& initVal
            );

            //- Call correctBoundaryConditions on fields
            template<class GeoField>
            void correctBoundaryConditions();

            //- Delete all processor patches. Move any processor faces into
            //  patchi.
            autoPtr<mapPolyMesh> deleteProcPatches(const label patchi);

            //- Repatch the mesh. This is only necessary for the proc
            //  boundary faces. newPatchID is over all boundary faces: -1 or
            //  new patchID. constructFaceMap is being adapted for the
            //  possible new face position (since proc faces get automatically
            //  matched)
            autoPtr<mapPolyMesh> repatch
            (
                const labelList& newPatchID,
                labelListList& constructFaceMap
            );

            //- Merge any shared points that are geometrically shared. Needs
            //  parallel valid mesh - uses globalMeshData.
            //  constructPointMap is adapted for the new point labels.
            autoPtr<mapPolyMesh> mergeSharedPoints
            (
                labelListList& constructPointMap
            );

        // Coupling information

            //- Construct the local environment of all boundary faces.
            void getNeighbourData
            (
                const labelList& distribution,
                labelList& sourceFace,
                labelList& sourceProc,
                labelList& sourcePatch,
                labelList& sourceNewProc
            ) const;

            // Subset the neighbourCell/neighbourProc fields
            static void subsetBoundaryData
            (
                const fvMesh& mesh,
                const labelList& faceMap,
                const labelList& cellMap,

                const labelList& oldDistribution,
                const labelList& oldFaceOwner,
                const labelList& oldFaceNeighbour,
                const label oldInternalFaces,

                const labelList& sourceFace,
                const labelList& sourceProc,
                const labelList& sourcePatch,
                const labelList& sourceNewProc,

                labelList& subFace,
                labelList& subProc,
                labelList& subPatch,
                labelList& subNewProc
            );

            //- Find cells on mesh whose faceID/procID match the neighbour
            //  cell/proc of domainMesh. Store the matching face.
            static void findCouples
            (
                const primitiveMesh&,
                const labelList& sourceFace,
                const labelList& sourceProc,
                const labelList& sourcePatch,

                const label domain,
                const primitiveMesh& domainMesh,
                const labelList& domainFace,
                const labelList& domainProc,
                const labelList& domainPatch,

                labelList& masterCoupledFaces,
                labelList& slaveCoupledFaces
            );

            //- Map data on boundary faces to new mesh (resulting from adding
            //  two meshes)
            static labelList mapBoundaryData
            (
                const primitiveMesh& mesh,      // mesh after adding
                const mapAddedPolyMesh& map,
                const labelList& boundaryData0, // mesh before adding
                const label nInternalFaces1,
                const labelList& boundaryData1  // added mesh
            );


        // Other

            //- Remove cells. Add all exposed faces to patch oldInternalPatchi
            autoPtr<mapPolyMesh> doRemoveCells
            (
                const labelList& cellsToRemove,
                const label oldInternalPatchi
            );

            //- Add processor patches. Changes mesh and returns per neighbour
            //  proc the processor patchID.
            void addProcPatches
            (
                const labelList&, // processor that neighbour is now on
                const labelList&, // -1 or patch that face originated from
                List<Map<label> >& procPatchID
            );

            //- Get boundary faces to be repatched. Is -1 or new patchID
            static labelList getBoundaryPatch
            (
                const labelList& neighbourNewProc,  // new processor per b. face
                const labelList& referPatchID,      // -1 or original patch
                const List<Map<label> >& procPatchID// patchID
            );

            //- Send mesh and coupling data.
            static void sendMesh
            (
                const label domain,
                const fvMesh& mesh,
                const wordList& pointZoneNames,
                const wordList& facesZoneNames,
                const wordList& cellZoneNames,
                const labelList& sourceFace,
                const labelList& sourceProc,
                const labelList& sourcePatch,
                const labelList& sourceNewProc,
                UOPstream& toDomain
            );
            //- Send subset of fields
            template<class GeoField>
            static void sendFields
            (
                const label domain,
                const wordList& fieldNames,
                const fvMeshSubset&,
                UOPstream& toNbr
            );

            //- Receive mesh. Opposite of sendMesh
            static autoPtr<fvMesh> receiveMesh
            (
                const label domain,
                const wordList& pointZoneNames,
                const wordList& facesZoneNames,
                const wordList& cellZoneNames,
                const Time& runTime,
                labelList& domainSourceFace,
                labelList& domainSourceProc,
                labelList& domainSourcePatch,
                labelList& domainSourceNewProc,
                UIPstream& fromNbr
            );

            //- Receive fields. Opposite of sendFields
            template<class GeoField>
            static void receiveFields
            (
                const label domain,
                const wordList& fieldNames,
                fvMesh&,
                PtrList<GeoField>&,
                const dictionary& fieldDicts
            );

            //- Disallow default bitwise copy construct
            fvMeshDistribute(const fvMeshDistribute&);

            //- Disallow default bitwise assignment
            void operator=(const fvMeshDistribute&);

public:

    ClassName("fvMeshDistribute");


    // Constructors

        //- Construct from mesh and absolute merge tolerance
        fvMeshDistribute(fvMesh& mesh, const scalar mergeTol);


    // Member Functions

        //- Helper function: count cells per processor in wanted distribution
        static labelList countCells(const labelList&);

        //- Send cells to neighbours according to distribution
        //  (for every cell the new proc)
        autoPtr<mapDistributePolyMesh> distribute(const labelList& dist);

        // Debugging

            //- Print some info on coupling data
            static void printCoupleInfo
            (
                const primitiveMesh&,
                const labelList&,
                const labelList&,
                const labelList&,
                const labelList&
            );

            //- Print some field info
            template<class GeoField>
            static void printFieldInfo(const fvMesh&);

            //- Print some info on mesh.
            static void printMeshInfo(const fvMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "mapPolyMesh.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class GeoField>
void CML::fvMeshDistribute::printFieldInfo(const fvMesh& mesh)
{
    HashTable<const GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllConstIter(typename HashTable<const GeoField*>, flds, iter)
    {
        const GeoField& fld = *iter();

        Pout<< "Field:" << iter.key() << " internalsize:" << fld.size()
            //<< " value:" << fld
            << endl;

        forAll(fld.boundaryField(), patchi)
        {
            Pout<< "    " << patchi
                << ' ' << fld.boundaryField()[patchi].patch().name()
                << ' ' << fld.boundaryField()[patchi].type()
                << ' ' << fld.boundaryField()[patchi].size()
                << endl;
        }
    }
}


template <class T, class Mesh>
void CML::fvMeshDistribute::saveBoundaryFields
(
    PtrList<FieldField<fvsPatchField, T> >& bflds
) const
{
    // Save whole boundary field

    typedef GeometricField<T, fvsPatchField, Mesh> fldType;

    HashTable<const fldType*> flds
    (
        static_cast<const fvMesh&>(mesh_).objectRegistry::lookupClass<fldType>()
    );

    bflds.setSize(flds.size());

    label i = 0;

    forAllConstIter(typename HashTable<const fldType*>, flds, iter)
    {
        const fldType& fld = *iter();

        bflds.set(i, fld.boundaryField().clone().ptr());

        i++;
    }
}


template <class T, class Mesh>
void CML::fvMeshDistribute::mapBoundaryFields
(
    const mapPolyMesh& map,
    const PtrList<FieldField<fvsPatchField, T> >& oldBflds
)
{
    // Map boundary field

    const labelList& oldPatchStarts = map.oldPatchStarts();
    const labelList& faceMap = map.faceMap();

    typedef GeometricField<T, fvsPatchField, Mesh> fldType;

    HashTable<fldType*> flds
    (
        mesh_.objectRegistry::lookupClass<fldType>()
    );

    if (flds.size() != oldBflds.size())
    {
        FatalErrorInFunction << "problem"
            << abort(FatalError);
    }

    label fieldi = 0;

    forAllIter(typename HashTable<fldType*>, flds, iter)
    {
        fldType& fld = *iter();
        typename fldType::GeometricBoundaryField& bfld = fld.boundaryField();

        const FieldField<fvsPatchField, T>& oldBfld = oldBflds[fieldi++];

        // Pull from old boundary field into bfld.

        forAll(bfld, patchi)
        {
            fvsPatchField<T>& patchFld = bfld[patchi];
            label facei = patchFld.patch().start();

            forAll(patchFld, i)
            {
                label oldFacei = faceMap[facei++];

                // Find patch and local patch face oldFacei was in.
                forAll(oldPatchStarts, oldPatchi)
                {
                    label oldLocalI = oldFacei - oldPatchStarts[oldPatchi];

                    if (oldLocalI >= 0 && oldLocalI < oldBfld[oldPatchi].size())
                    {
                        patchFld[i] = oldBfld[oldPatchi][oldLocalI];
                    }
                }
            }
        }
    }
}


template<class T>
void CML::fvMeshDistribute::saveInternalFields
(
    PtrList<Field<T> >& iflds
) const
{
    typedef GeometricField<T, fvsPatchField, surfaceMesh> fldType;

    HashTable<const fldType*> flds
    (
        static_cast<const fvMesh&>(mesh_).objectRegistry::lookupClass<fldType>()
    );

    iflds.setSize(flds.size());

    label i = 0;

    forAllConstIter(typename HashTable<const fldType*>, flds, iter)
    {
        const fldType& fld = *iter();

        iflds.set(i, fld.internalField().clone());

        i++;
    }
}


template<class T>
void CML::fvMeshDistribute::mapExposedFaces
(
    const mapPolyMesh& map,
    const PtrList<Field<T> >& oldFlds
)
{
    // Set boundary values of exposed internal faces

    const labelList& faceMap = map.faceMap();

    typedef GeometricField<T, fvsPatchField, surfaceMesh> fldType;

    HashTable<fldType*> flds
    (
        mesh_.objectRegistry::lookupClass<fldType>()
    );

    if (flds.size() != oldFlds.size())
    {
        FatalErrorInFunction << "problem"
            << abort(FatalError);
    }


    label fieldI = 0;

    forAllIter(typename HashTable<fldType*>, flds, iter)
    {
        fldType& fld = *iter();
        typename fldType::GeometricBoundaryField& bfld = fld.boundaryField();

        const Field<T>& oldInternal = oldFlds[fieldI++];

        // Pull from old internal field into bfld.

        forAll(bfld, patchi)
        {
            fvsPatchField<T>& patchFld = bfld[patchi];

            forAll(patchFld, i)
            {
                const label faceI = patchFld.patch().start()+i;

                label oldFaceI = faceMap[faceI];

                if (oldFaceI < oldInternal.size())
                {
                    patchFld[i] = oldInternal[oldFaceI];

                    if (map.flipFaceFlux().found(faceI))
                    {
                        patchFld[i] = flipOp()(patchFld[i]);
                    }
                }
            }
        }
    }
}


template<class GeoField, class PatchFieldType>
void CML::fvMeshDistribute::initPatchFields
(
    const typename GeoField::value_type& initVal
)
{
    // Init patch fields of certain type

    HashTable<GeoField*> flds
    (
        mesh_.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();

        typename GeoField::GeometricBoundaryField& bfld =
            fld.boundaryField();

        forAll(bfld, patchi)
        {
            if (isA<PatchFieldType>(bfld[patchi]))
            {
                bfld[patchi] == initVal;
            }
        }
    }
}


template<class GeoField>
void CML::fvMeshDistribute::correctBoundaryConditions()
{
    // correctBoundaryConditions patch fields of certain type

    HashTable<GeoField*> flds
    (
        mesh_.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        const GeoField& fld = *iter();
        fld.correctBoundaryConditions();
    }
}


template<class GeoField>
void CML::fvMeshDistribute::sendFields
(
    const label domain,
    const wordList& fieldNames,
    const fvMeshSubset& subsetter,
    UOPstream& toNbr
)
{
    // Send fields. Note order supplied so we can receive in exactly the same
    // order.
    // Note that field gets written as entry in dictionary so we
    // can construct from subdictionary.
    // (since otherwise the reading as-a-dictionary mixes up entries from
    // consecutive fields)
    // The dictionary constructed is:
    //  volScalarField
    //  {
    //      p {internalField ..; boundaryField ..;}
    //      k {internalField ..; boundaryField ..;}
    //  }
    //  volVectorField
    //  {
    //      U {internalField ...  }
    //  }

    // volVectorField {U {internalField ..; boundaryField ..;}}

    toNbr << GeoField::typeName << token::NL << token::BEGIN_BLOCK << token::NL;
    forAll(fieldNames, i)
    {
        if (debug)
        {
            Pout<< "Subsetting field " << fieldNames[i]
                << " for domain:" << domain << endl;
        }

        // Send all fieldNames. This has to be exactly the same set as is
        // being received!
        const GeoField& fld =
            subsetter.baseMesh().lookupObject<GeoField>(fieldNames[i]);

        tmp<GeoField> tsubfld = subsetter.interpolate(fld);

        toNbr
            << fieldNames[i] << token::NL << token::BEGIN_BLOCK
            << tsubfld
            << token::NL << token::END_BLOCK << token::NL;
    }
    toNbr << token::END_BLOCK << token::NL;
}


template<class GeoField>
void CML::fvMeshDistribute::receiveFields
(
    const label domain,
    const wordList& fieldNames,
    fvMesh& mesh,
    PtrList<GeoField>& fields,
    const dictionary& fieldDicts
)
{
    if (debug)
    {
        Pout<< "Receiving fields " << fieldNames
            << " from domain:" << domain << endl;
    }

    fields.setSize(fieldNames.size());

    forAll(fieldNames, i)
    {
        if (debug)
        {
            Pout<< "Constructing field " << fieldNames[i]
                << " from domain:" << domain << endl;
        }

        fields.set
        (
            i,
            new GeoField
            (
                IOobject
                (
                    fieldNames[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                fieldDicts.subDict(fieldNames[i])
            )
        );
    }
}


#endif

// ************************************************************************* //
