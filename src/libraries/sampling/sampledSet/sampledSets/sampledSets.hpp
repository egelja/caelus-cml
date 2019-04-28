/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::sampledSets

Description
    Set of sets to sample.
    Call sampledSets.write() to sample&write files.

SourceFiles
    sampledSets.cpp

\*---------------------------------------------------------------------------*/

#ifndef sampledSets_H
#define sampledSets_H

#include "sampledSet.hpp"
#include "volFieldsFwd.hpp"
#include "meshSearch.hpp"
#include "interpolation.hpp"
#include "coordSet.hpp"
#include "writer.hpp"
#include "wordReList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class objectRegistry;
class dictionary;
class fvMesh;

/*---------------------------------------------------------------------------*\
                         Class sampledSets Declaration
\*---------------------------------------------------------------------------*/

class sampledSets
:
    public PtrList<sampledSet>
{
    // Private classes

        //- Class used for grouping field types
        template<class Type>
        class fieldGroup
        :
            public DynamicList<word>
        {
        public:

            //- The set formatter
            autoPtr< writer<Type> > formatter;

            //- Construct null
            fieldGroup()
            :
                DynamicList<word>(0),
                formatter(nullptr)
            {}

            //- Construct for a particular format
            fieldGroup(const word& writeFormat)
            :
                DynamicList<word>(0),
                formatter(writer<Type>::New(writeFormat))
            {}

            //- Reset format and field list
            void clear()
            {
                DynamicList<word>::clear();
                formatter.clear();
            }

            //- Assign a new formatter
            void operator=(const word& writeFormat)
            {
                formatter = writer<Type>::New(writeFormat);
            }

        };


        //- Class used for sampling volFields
        template<class Type>
        class volFieldSampler
        :
            public List<Field<Type> >
        {
            //- Name of this collection of values
            const word name_;

        public:

            //- Construct interpolating field to the sampleSets
            volFieldSampler
            (
                const word& interpolationScheme,
                const GeometricField<Type, fvPatchField, volMesh>& field,
                const PtrList<sampledSet>&
            );

            //- Construct mapping field to the sampleSets
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
            wordReList fieldSelection_;

            //- Interpolation scheme to use
            word interpolationScheme_;

            //- Output format to use
            word writeFormat_;


        // Categorized scalar/vector/tensor fields

            fieldGroup<scalar> scalarFields_;
            fieldGroup<vector> vectorFields_;
            fieldGroup<sphericalTensor> sphericalTensorFields_;
            fieldGroup<symmTensor> symmTensorFields_;
            fieldGroup<tensor> tensorFields_;


        // Merging structures

            PtrList<coordSet> masterSampledSets_;
            labelListList indexSets_;


    // Private Member Functions

        //- Clear old field groups
        void clearFieldGroups();

        //- Append fieldName to the appropriate group
        label appendFieldGroup(const word& fieldName, const word& fieldType);

        //- Classify field types, returns the number of fields
        label classifyFields();

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

        template<class Type>
        void writeSampleFile
        (
            const coordSet& masterSampleSet,
            const PtrList<volFieldSampler<Type> >& masterFields,
            const label setI,
            const fileName& timeDir,
            const writer<Type>& formatter
        );

        template<class Type>
        void sampleAndWrite(fieldGroup<Type>& fields);


        //- Disallow default bitwise copy construct and assignment
        sampledSets(const sampledSets&);
        void operator=(const sampledSets&);


public:

    //- Runtime type information
    TypeName("sets");


    // Constructors

        //- Construct for given objectRegistry and dictionary
        //  allow the possibility to load fields from files
        sampledSets
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~sampledSets();


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

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

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

        //- Update for changes of mesh due to readUpdate
        virtual void readUpdate(const polyMesh::readUpdateState state);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
CML::sampledSets::volFieldSampler<Type>::volFieldSampler
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

    forAll(samplers, setI)
    {
        Field<Type>& values = this->operator[](setI);
        const sampledSet& samples = samplers[setI];

        values.setSize(samples.size());
        forAll(samples, sampleI)
        {
            const point& samplePt = samples[sampleI];
            label cellI = samples.cells()[sampleI];
            label faceI = samples.faces()[sampleI];

            if (cellI == -1 && faceI == -1)
            {
                // Special condition for illegal sampling points
                values[sampleI] = pTraits<Type>::max;
            }
            else
            {
                values[sampleI] = interpolator().interpolate
                (
                    samplePt,
                    cellI,
                    faceI
                );
            }
        }
    }
}


template<class Type>
CML::sampledSets::volFieldSampler<Type>::volFieldSampler
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const PtrList<sampledSet>& samplers
)
:
    List<Field<Type> >(samplers.size()),
    name_(field.name())
{
    forAll(samplers, setI)
    {
        Field<Type>& values = this->operator[](setI);
        const sampledSet& samples = samplers[setI];

        values.setSize(samples.size());
        forAll(samples, sampleI)
        {
            label cellI = samples.cells()[sampleI];

            if (cellI ==-1)
            {
                values[sampleI] = pTraits<Type>::max;
            }
            else
            {
                values[sampleI] = field[cellI];
            }
        }
    }
}


template<class Type>
CML::sampledSets::volFieldSampler<Type>::volFieldSampler
(
    const List<Field<Type> >& values,
    const word& name
)
:
    List<Field<Type> >(values),
    name_(name)
{}


template<class Type>
void CML::sampledSets::writeSampleFile
(
    const coordSet& masterSampleSet,
    const PtrList<volFieldSampler<Type> >& masterFields,
    const label setI,
    const fileName& timeDir,
    const writer<Type>& formatter
)
{
    wordList valueSetNames(masterFields.size());
    List<const Field<Type>*> valueSets(masterFields.size());

    forAll(masterFields, fieldi)
    {
        valueSetNames[fieldi] = masterFields[fieldi].name();
        valueSets[fieldi] = &masterFields[fieldi][setI];
    }

    fileName fName
    (
        timeDir/formatter.getFileName(masterSampleSet, valueSetNames)
    );

    OFstream ofs(fName);
    if (ofs.opened())
    {
        formatter.write
        (
            masterSampleSet,
            valueSetNames,
            valueSets,
            ofs
        );
    }
    else
    {
        WarningInFunction
            << "File " << ofs.name() << " could not be opened. "
            << "No data will be written" << endl;
    }
}


template<class T>
void CML::sampledSets::combineSampledValues
(
    const PtrList<volFieldSampler<T> >& sampledFields,
    const labelListList& indexSets,
    PtrList<volFieldSampler<T> >& masterFields
)
{
    forAll(sampledFields, fieldi)
    {
        List<Field<T> > masterValues(indexSets.size());

        forAll(indexSets, setI)
        {
            // Collect data from all processors
            List<Field<T> > gatheredData(Pstream::nProcs());
            gatheredData[Pstream::myProcNo()] = sampledFields[fieldi][setI];
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

                masterValues[setI] = UIndirectList<T>
                (
                    allData,
                    indexSets[setI]
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


template<class Type>
void CML::sampledSets::sampleAndWrite
(
    fieldGroup<Type>& fields
)
{
    if (fields.size())
    {
        bool interpolate = interpolationScheme_ != "cell";

        // Create or use existing writer
        if (fields.formatter.empty())
        {
            fields = writeFormat_;
        }

        // Storage for interpolated values
        PtrList<volFieldSampler<Type> > sampledFields(fields.size());

        forAll(fields, fieldi)
        {
            if (Pstream::master() && verbose_)
            {
                Pout<< "sampledSets::sampleAndWrite: "
                    << fields[fieldi] << endl;
            }

            if (loadFromFiles_)
            {
                GeometricField<Type, fvPatchField, volMesh> vf
                (
                    IOobject
                    (
                        fields[fieldi],
                        mesh_.time().timeName(),
                        mesh_,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    mesh_
                );

                if (interpolate)
                {
                    sampledFields.set
                    (
                        fieldi,
                        new volFieldSampler<Type>
                        (
                            interpolationScheme_,
                            vf,
                            *this
                        )
                    );
                }
                else
                {
                    sampledFields.set
                    (
                        fieldi,
                        new volFieldSampler<Type>(vf, *this)
                    );
                }
            }
            else
            {
                if (interpolate)
                {
                    sampledFields.set
                    (
                        fieldi,
                        new volFieldSampler<Type>
                        (
                            interpolationScheme_,
                            mesh_.lookupObject
                            <GeometricField<Type, fvPatchField, volMesh> >
                            (fields[fieldi]),
                            *this
                        )
                    );
                }
                else
                {
                    sampledFields.set
                    (
                        fieldi,
                        new volFieldSampler<Type>
                        (
                            mesh_.lookupObject
                            <GeometricField<Type, fvPatchField, volMesh> >
                            (fields[fieldi]),
                            *this
                        )
                    );
                }
            }
        }

        // Combine sampled fields from processors.
        // Note: only master results are valid

        PtrList<volFieldSampler<Type> > masterFields(sampledFields.size());
        combineSampledValues(sampledFields, indexSets_, masterFields);

        if (Pstream::master())
        {
            forAll(masterSampledSets_, setI)
            {
                writeSampleFile
                (
                    masterSampledSets_[setI],
                    masterFields,
                    setI,
                    outputPath_/mesh_.time().timeName(),
                    fields.formatter()
                );
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
