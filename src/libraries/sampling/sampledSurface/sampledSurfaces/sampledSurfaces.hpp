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
    CML::sampledSurfaces

Description
    Set of surfaces to sample.

    The write() method is used to sample and write files.


\*---------------------------------------------------------------------------*/

#ifndef sampledSurfaces_H
#define sampledSurfaces_H

#include "sampledSurface.hpp"
#include "surfaceWriter.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "wordReList.hpp"
#include "IOobjectList.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class dictionary;

/*---------------------------------------------------------------------------*\
                       Class sampledSurfaces Declaration
\*---------------------------------------------------------------------------*/

class sampledSurfaces
:
    public PtrList<sampledSurface>
{
    // Private classes

        //- Class used for surface merging information
        class mergeInfo
        {
        public:
            pointField points;
            faceList   faces;
            labelList  pointsMap;

            //- Clear all storage
            void clear()
            {
                points.clear();
                faces.clear();
                pointsMap.clear();
            }
        };


    // Static data members

        //- output verbosity
        static bool verbose_;

        //- Tolerance for merging points (fraction of mesh bounding box)
        static scalar mergeTol_;

    // Private data

        //- Name of this set of surfaces,
        //  Also used as the name of the sampledSurfaces directory.
        const word name_;

        //- Const reference to fvMesh
        const fvMesh& mesh_;

        //- Load fields from files (not from objectRegistry)
        const bool loadFromFiles_;

        //- output path
        fileName outputPath_;


        // Read from dictonary

            //- Names of fields to sample
            wordReList fieldSelection_;

            //- Interpolation scheme to use
            word interpolationScheme_;

        // surfaces

            //- Information for merging surfaces
            List<mergeInfo> mergeList_;


        // Calculated

            //- Surface formatter
            autoPtr<surfaceWriter> formatter_;


    // Private Member Functions


        // Return number of fields
        label classifyFields();

        //- Write geometry only
        void writeGeometry() const;

        //- Write sampled fieldName on surface and on outputDir path
        template<class Type>
        void writeSurface
        (
            const Field<Type>& values,
            const label surfI,
            const word& fieldName,
            const fileName& outputDir
        );

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

        //- Sample and write all sampled fields
        template<class Type>
        void sampleAndWrite(const IOobjectList&);

        //- Disallow default bitwise copy construct and assignment
        sampledSurfaces(const sampledSurfaces&);
        void operator=(const sampledSurfaces&);


public:

    //- Runtime type information
    TypeName("surfaces");


    // Constructors

        //- Construct for given objectRegistry and dictionary
        //  allow the possibility to load fields from files
        sampledSurfaces
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~sampledSurfaces();


    // Member Functions

        //- Does any of the surfaces need an update?
        virtual bool needsUpdate() const;

        //- Mark the surfaces as needing an update.
        //  May also free up unneeded data.
        //  Return false if all surfaces were already marked as expired.
        virtual bool expire();

        //- Update the surfaces as required and merge surface points (parallel).
        //  Return false if no surfaces required an update.
        virtual bool update();


        //- Return name of the set of surfaces
        virtual const word& name() const
        {
            return name_;
        }

        //- set verbosity level
        void verbose(const bool verbosity = true);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Sample and write
        virtual void write();

        //- Read the sampledSurfaces dictionary
        virtual void read(const dictionary&);

        //- Update for changes of mesh - expires the surfaces
        virtual void updateMesh(const mapPolyMesh&);

        //- Update for mesh point-motion - expires the surfaces
        virtual void movePoints(const pointField&);

        //- Update for changes of mesh due to readUpdate - expires the surfaces
        virtual void readUpdate(const polyMesh::readUpdateState state);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::sampledSurfaces::writeSurface
(
    const Field<Type>& values,
    const label surfI,
    const word& fieldName,
    const fileName& outputDir
)
{
    const sampledSurface& s = operator[](surfI);

    if (Pstream::parRun())
    {
        // Collect values from all processors
        List<Field<Type> > gatheredValues(Pstream::nProcs());
        gatheredValues[Pstream::myProcNo()] = values;
        Pstream::gatherList(gatheredValues);

        if (Pstream::master())
        {
            // Combine values into single field
            Field<Type> allValues
            (
                ListListOps::combine<Field<Type> >
                (
                    gatheredValues,
                    accessOp<Field<Type> >()
                )
            );

            // Renumber (point data) to correspond to merged points
            if (mergeList_[surfI].pointsMap.size() == allValues.size())
            {
                inplaceReorder(mergeList_[surfI].pointsMap, allValues);
                allValues.setSize(mergeList_[surfI].points.size());
            }

            // Write to time directory under outputPath_
            // skip surface without faces (eg, a failed cut-plane)
            if (mergeList_[surfI].faces.size())
            {
                formatter_->write
                (
                    outputDir,
                    s.name(),
                    mergeList_[surfI].points,
                    mergeList_[surfI].faces,
                    fieldName,
                    allValues,
                    s.interpolate()
                );
            }
        }
    }
    else
    {
        // Write to time directory under outputPath_
        // skip surface without faces (eg, a failed cut-plane)
        if (s.faces().size())
        {
            formatter_->write
            (
                outputDir,
                s.name(),
                s.points(),
                s.faces(),
                fieldName,
                values,
                s.interpolate()
            );
        }
    }
}


template<class Type>
void CML::sampledSurfaces::sampleAndWrite
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
)
{
    // interpolator for this field
    autoPtr< interpolation<Type> > interpolator;

    const word& fieldName   = vField.name();
    const fileName outputDir = outputPath_/vField.time().timeName();

    forAll(*this, surfI)
    {
        const sampledSurface& s = operator[](surfI);

        Field<Type> values;

        if (s.interpolate())
        {
            if (interpolator.empty())
            {
                interpolator = interpolation<Type>::New
                (
                    interpolationScheme_,
                    vField
                );
            }

            values = s.interpolate(interpolator());
        }
        else
        {
            values = s.sample(vField);
        }

        writeSurface<Type>(values, surfI, fieldName, outputDir);
    }
}


template<class Type>
void CML::sampledSurfaces::sampleAndWrite
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
)
{
    const word& fieldName   = sField.name();
    const fileName outputDir = outputPath_/sField.time().timeName();

    forAll(*this, surfI)
    {
        const sampledSurface& s = operator[](surfI);
        Field<Type> values = s.sample(sField);
        writeSurface<Type>(values, surfI, fieldName, outputDir);
    }
}


template<class GeoField>
void CML::sampledSurfaces::sampleAndWrite(const IOobjectList& allObjects)
{
    forAll (fieldSelection_, fieldI)
    {
        const wordRe field = fieldSelection_[fieldI];
        IOobject* fieldIOPtr = allObjects.lookup(field);

        if
        (
            fieldIOPtr != NULL
          && fieldIOPtr->headerClassName() == GeoField::typeName
        )
        {
            if (Pstream::master() && verbose_)
            {
                Pout<< "sampleAndWrite: " << field << endl;
            }

            if (loadFromFiles_)
            {
                const GeoField geoField
                (
                    IOobject
                    (
                        field,
                        mesh_.time().timeName(),
                        mesh_,
                        IOobject::MUST_READ
                    ),
                    mesh_
                );

                sampleAndWrite(geoField);
            }
            else
            {
                sampleAndWrite
                (
                    mesh_.thisDb().lookupObject<GeoField>(field)
                );
            }
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
