/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::streamLine

Group
    grpFieldFunctionObjects

Description
    Generates streamline data by sampling a set of user-specified fields along a
    particle track, transported by a user-specified velocity field.

    Example of function object specification:
    \verbatim
    streamLine1
    {
        type            streamLine;
        functionObjectLibs ("libfieldFunctionObjects.so");
        ...
        setFormat       vtk;
        U               U;
        trackForward    yes;

        fields
        (
            U
            p
        );

        lifeTime        10000;
        trackLength     1e-3;
        nSubCycle       5;
        cloudName       particleTracks;
        seedSampleSet   uniform;
        uniformCoeffs
        {
            type        uniform;
            axis        x;  //distance;
            start       (-0.0205 0.0001 0.00001);
            end         (-0.0205 0.0005 0.00001);
            nPoints     100;
        }
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | Type name: streamLine   | yes         |
        setFormat    | Output data type        | yes         |
        U            | Tracking velocity field name | yes    |
        fields       | Fields to sample        | yes         |
        lifetime     | Maximum number of particle tracking steps | yes |
        trackLength  | Tracking segment length | no          |
        nSubCycle    | Number of tracking steps per cell | no|
        cloudName    | Cloud name to use       | yes         |
        seedSampleSet| Seeding method (see below)| yes       |
    \endtable

    \linebreak
    Where \c seedSampleSet is typically one of
    \plaintable
        uniform | uniform particle seeding
        cloud   | cloud of points
        triSurfaceMeshPointSet | points according to a tri-surface mesh
    \endplaintable

Note
    When specifying the track resolution, the \c trackLength OR \c nSubCycle
    option should be used

SeeAlso
    CML::functionObject
    CML::OutputFilterFunctionObject
    CML::sampledSet
    CML::wallBoundedStreamLine

SourceFiles
    streamLine.cpp

\*---------------------------------------------------------------------------*/

#ifndef streamLine_H
#define streamLine_H

#include "volFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "Switch.hpp"
#include "DynamicList.hpp"
#include "scalarList.hpp"
#include "vectorList.hpp"
#include "polyMesh.hpp"
#include "writer.hpp"
#include "indirectPrimitivePatch.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class meshSearch;
class sampledSet;

/*---------------------------------------------------------------------------*\
                         Class streamLine Declaration
\*---------------------------------------------------------------------------*/

class streamLine
{
public:

    // Public data types

        //- Track direction enumerations
        enum trackDirection
        {
            FORWARD,
            BACKWARD,
            BOTH
        };

        //- Track direction enumeration names
        static const NamedEnum<trackDirection, 3> trackDirectionNames_;


private:

    // Private data

        //- Input dictionary
        dictionary dict_;

        //- Name of this set of field averages.
        word name_;

        //- Database this class is registered to
        const objectRegistry& obr_;

        //- Load fields from files (not from objectRegistry)
        bool loadFromFiles_;

        //- On/off switch
        bool active_;

        //- List of fields to sample
        wordList fields_;

        //- Field to transport particle with
        word UName_;

        //- Interpolation scheme to use
        word interpolationScheme_;

        //- The direction in which to track
        trackDirection trackDirection_;

        //- Maximum lifetime (= number of cells) of particle
        label lifeTime_;

        //- Number of subcycling steps
        label nSubCycle_;

        //- Track length
        scalar trackLength_;

        //- Optional specified name of particles
        word cloudName_;

        //- Type of seed
        word seedSet_;

        //- Names of scalar fields
        wordList scalarNames_;

        //- Names of vector fields
        wordList vectorNames_;


        // Demand driven

            //- Mesh searching enigne
            autoPtr<meshSearch> meshSearchPtr_;

            //- Seed set engine
            autoPtr<sampledSet> sampledSetPtr_;

            //- Axis of the sampled points to output
            word sampledSetAxis_;

            //- File writer for scalar data
            autoPtr<writer<scalar> > scalarFormatterPtr_;

            //- File writer for vector data
            autoPtr<writer<vector> > vectorFormatterPtr_;


        // Generated data

            //- All tracks. Per particle the points it passed through
            DynamicList<List<point> > allTracks_;

            //- Per scalarField, per particle, the sampled value.
            List<DynamicList<scalarList> > allScalars_;

            //- Per scalarField, per particle, the sampled value.
            List<DynamicList<vectorList> > allVectors_;


        //- Construct patch out of all wall patch faces
        autoPtr<indirectPrimitivePatch> wallPatch() const;

        //- Do all seeding and tracking
        void track();

        //- Disallow default bitwise copy construct
        streamLine(const streamLine&);

        //- Disallow default bitwise assignment
        void operator=(const streamLine&);


public:

    //- Runtime type information
    TypeName("streamLine");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        streamLine
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~streamLine();


    // Member Functions

        //- Return name of the set of field averages
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the field average data
        virtual void read(const dictionary&);

        //- Execute the averaging
        virtual void execute();

        //- Execute the averaging at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the field average data and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&);

        //- Update for mesh point-motion
        virtual void movePoints(const pointField&);

        ////- Update for changes of mesh due to readUpdate
        //virtual void readUpdate(const polyMesh::readUpdateState state);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
