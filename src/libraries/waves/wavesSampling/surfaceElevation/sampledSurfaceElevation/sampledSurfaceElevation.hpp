/*---------------------------------------------------------------------------*\
 Copyright Niels G. Jacobsen and Sopheak Seng Technical University of Denmark
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
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
    CML::sampledSurfaceElevation

Description
    Integrate along sampling lines (ought to be vertical), and output the
    surface elevation into the root case.

    Based on sampledSets.cpp.

SourceFiles
    sampledSurfaceElevation.cpp

Authors
    Niels G. Jacobsen and Sopheak Seng
    Technical University of Denmark

\*---------------------------------------------------------------------------*/

#ifndef sampledSurfaceElevation_HPP
#define sampledSurfaceElevation_HPP

#include "sampledSet.hpp"
#include "volFieldsFwd.hpp"
#include "meshSearch.hpp"
#include "interpolation.hpp"
#include "coordSet.hpp"
#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class objectRegistry;
class dictionary;
class fvMesh;

/*---------------------------------------------------------------------------*\
                      Class sampledSets Declaration
\*---------------------------------------------------------------------------*/

class sampledSurfaceElevation
:
    public PtrList<sampledSet>
{
    // Private classes

        //- Class used for grouping field types
        template<class Type>
        class fieldGroup
        :
            public wordList
        {
        public:

            //- Set formatter
            autoPtr<writer<Type> > formatter;

            //- Construct null
            fieldGroup()
            :
                wordList(0),
                formatter(nullptr)
            {}

            void clear()
            {
                wordList::clear();
                formatter.clear();
            }
        };


        //- Class used for sampling volFields
        template <class Type>
        class volFieldSampler
        :
            public List<Field<Type> >
        {
            //- Name of this collection of values
            const word name_;

        public:

            //- Construct interpolating field to the sampledSurfaceElevation
            volFieldSampler
            (
                const word& interpolationScheme,
                const GeometricField<Type, fvPatchField, volMesh>& field,
                const PtrList<sampledSet>&
            );

            //- Construct mapping field to the sampledSurfaceElevation
            volFieldSampler
            (
                const GeometricField<Type, fvPatchField, volMesh>& field,
                const PtrList<sampledSet>&
            );

            //- Construct from components
            volFieldSampler
            (
                const List<Field<Type> >& values,
                const word& name
            );

            //- Return the field name
            const word& name() const
            {
                return name_;
            }
        };


    // Static data members

        //- output verbosity
        static bool verbose_;


    // Private data

        //- Name of this set of sets,
        //  Also used as the name of the sampledSets directory.
        word name_;

        //- Const reference to fvMesh
        const fvMesh& mesh_;

        //- Keep the dictionary to recreate sets for moving mesh cases
        dictionary dict_;

        //- Load fields from files (not from objectRegistry)
        bool loadFromFiles_;

        //- Output path
        fileName outputPath_;

        //- Mesh search engine
        meshSearch searchEngine_;

        // Read from dictonary

            //- Names of fields to sample
            wordList fieldNames_;

            //- Interpolation scheme to use
            word interpolationScheme_;

            //- Output format to use
            word writeFormat_;


        // Categorized scalar/vector/tensor fields

            fieldGroup<scalar> scalarFields_;


        // Merging structures

            PtrList<coordSet> masterSampledSets_;
            labelListList indexSets_;

        // Output file
            autoPtr<OFstream> surfaceElevationFilePtr_;

    // Private Member Functions

        //- Classify field types, return true if nFields > 0
        bool checkFieldTypes();

        //- Find the fields in the list of the given type, return count
        template<class Type>
        label grep
        (
            fieldGroup<Type>& fieldList,
            const wordList& fieldTypes
        ) const;

        //- Combine points from all processors. Sort by curveDist and produce
        //  index list. Valid result only on master processor.
        void combineSampledSets
        (
            PtrList<coordSet>& masterSampledSets,
            labelListList& indexSets
        );

        //- Combine values from all processors.
        //  Valid result only on master processor.
        template<class T>
        void combineSampledValues
        (
            const PtrList<volFieldSampler<T> >& sampledFields,
            const labelListList& indexSets,
            PtrList<volFieldSampler<T> >& masterFields
        );

        void sampleIntegrateAndWrite(fieldGroup<scalar>& fields);

        //- Disallow default bitwise copy construct and assignment
        sampledSurfaceElevation(const sampledSurfaceElevation&);
        void operator=(const sampledSurfaceElevation&);


public:

    //- Runtime type information
    TypeName("surfaceElevation");


    // Constructors

        //- Construct for given objectRegistry and dictionary
        //  allow the possibility to load fields from files
        sampledSurfaceElevation
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    // Destructor

        virtual ~sampledSurfaceElevation();


    // Member Functions

        //- Return name of the set of probes
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

        //- Read the sampledSets
        virtual void read(const dictionary&);

        //- Correct for mesh changes
        void correct();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&);

        //- Update for mesh point-motion
        virtual void movePoints(const pointField&);

        virtual bool timeSet();

        //- Update for changes of mesh due to readUpdate
        virtual void readUpdate(const polyMesh::readUpdateState state);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


template <class Type>
CML::sampledSurfaceElevation::volFieldSampler<Type>::volFieldSampler
(
    const word& interpolationScheme,
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const PtrList<sampledSet>& samplers
)
:
    List<Field<Type> >(samplers.size()),
    name_(field.name())
{
    autoPtr<interpolation<Type> > interpolator
    (
        interpolation<Type>::New(interpolationScheme, field)
    );

    forAll (samplers, seti)
    {
        Field<Type>& values = this->operator[](seti);
        const sampledSet& samples = samplers[seti];

        values.setSize(samples.size());
        forAll (samples, samplei)
        {
            const point& samplePt = samples[samplei];
            label celli = samples.cells()[samplei];
            label facei = samples.faces()[samplei];

            if (celli == -1 && facei == -1)
            {
                // Special condition for illegal sampling points
                values[samplei] = pTraits<Type>::max;
            }
            else
            {
                values[samplei] = interpolator().interpolate
                (
                    samplePt,
                    celli,
                    facei
                );
            }
        }
    }
}


template <class Type>
CML::sampledSurfaceElevation::volFieldSampler<Type>::volFieldSampler
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const PtrList<sampledSet>& samplers
)
:
    List<Field<Type> >(samplers.size()),
    name_(field.name())
{
    forAll (samplers, seti)
    {
        Field<Type>& values = this->operator[](seti);
        const sampledSet& samples = samplers[seti];

        values.setSize(samples.size());
        forAll (samples, samplei)
        {
            label celli = samples.cells()[samplei];

            if (celli == -1)
            {
                values[samplei] = pTraits<Type>::max;
            }
            else
            {
                values[samplei] = field[samples.cells()[samplei]];
            }
        }
    }
}


template <class Type>
CML::sampledSurfaceElevation::volFieldSampler<Type>::volFieldSampler
(
    const List<Field<Type> >& values,
    const word& name
)
:
    List<Field<Type> >(values),
    name_(name)
{}


template<class Type>
CML::label CML::sampledSurfaceElevation::grep
(
    fieldGroup<Type>& fieldList,
    const wordList& fieldTypes
) const
{
    fieldList.setSize(fieldNames_.size());
    label nFields = 0;

    forAll (fieldNames_, fieldi)
    {
        if
        (
            fieldTypes[fieldi]
         == GeometricField<Type, fvPatchField, volMesh>::typeName
        )
        {
            fieldList[nFields] = fieldNames_[fieldi];
            nFields++;
        }
    }

    fieldList.setSize(nFields);

    return nFields;
}


template<class T>
void CML::sampledSurfaceElevation::combineSampledValues
(
    const PtrList<volFieldSampler<T> >& sampledFields,
    const labelListList& indexSets,
    PtrList<volFieldSampler<T> >& masterFields
)
{
    forAll (sampledFields, fieldi)
    {
        List<Field<T> > masterValues(indexSets.size());

        forAll (indexSets, seti)
        {
            // Collect data from all processors
            List<Field<T> > gatheredData(Pstream::nProcs());
            gatheredData[Pstream::myProcNo()] = sampledFields[fieldi][seti];
            Pstream::gatherList(gatheredData);

            if (Pstream::master())
            {
                Field<T> allData
                (
                    ListListOps::combine<Field<T> >
                    (
                        gatheredData,
                        CML::accessOp<Field<T> >()
                    )
                );

                masterValues[seti] = UIndirectList<T>
                (
                    allData,
                    indexSets[seti]
                )();
            }
        }

        masterFields.set
        (
            fieldi,
            new volFieldSampler<T>
            (
                masterValues,
                sampledFields[fieldi].name()
            )
        );
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
