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
    CML::dynamicRefineFvMesh

Description
    A fvMesh with built-in refinement.

    Determines which cells to refine/unrefine and does all in update().

       // Refinement mode - Options: timeStepInterval or timeInterval
       refineMode timeStepInterval;

        // How often to refine in timestepInterval mode
        refineInterval  10;

        // How often to refine in timeInterval mode
        refineTimeInterval 0.0001;

        // Number of steps remaining before timestep can be changed
        // (default = 5)
        nStepsToStartTimeChange 5;

        // Field to be refinement on
        field           alpha1;

        // Refine field inbetween lower..upper
        lowerRefineLevel 0.001;
        upperRefineLevel 0.999;

        // Allow unrefinement (optional, default = true) 
        allowUnrefinement true;

        // If value < unrefineLevel (default=GREAT) unrefine
        unrefineLevel   10;

        // Have slower than 2:1 refinement
        nBufferLayers   1;

        // Refine cells only up to maxRefinement levels
        maxRefinement   2;

        // Stop refinement if maxCells reached
        maxCells        200000;

        // Flux field and corresponding velocity field. Fluxes on changed
        // faces get recalculated by interpolating the velocity. Use 'none'
        // on surfaceScalarFields that do not need to be reinterpolated, use
        // NaN to detect use of mapped variable
        correctFluxes
        (
            (phi none)  //NaN)   //none)
            (nHatf none)   //none)
            (rho*phi none)   //none)
            (ghf none)  //NaN)   //none)
        );

        // Minimum cell volume 
        minimumVolume 1.25e-10;
        
        // Write the refinement level and protected cells as a volScalarField
        dumpLevel       true;


SourceFiles
    dynamicRefineFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef dynamicRefineFvMesh_H
#define dynamicRefineFvMesh_H

#include "dynamicFvMesh.hpp"
#include "hexRef8.hpp"
#include "PackedBoolList.hpp"
#include "Switch.hpp"
#include "volFields.hpp"
#include "cellSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class dynamicRefineFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dynamicRefineFvMesh
:
    public dynamicFvMesh
{
public:

    //- The refine mode options
    enum refineModes
    {
        rmIteration,      /*!< execution is coupled to the iteration */
        rmTime,           /*!< execution is coupled to the time */
    };

protected:

        //- Mesh cutting engine
        hexRef8 meshCutter_;

        //- Dump cellLevel for postprocessing
        Switch dumpLevel_;

        //- Minimum cell volume 
        scalar minVolume_;

        //- Fluxes to map
        HashTable<word> correctFluxes_;

        //- Number of refinement/unrefinement steps done so far.
        label nRefinementIterations_;

        //- Protected cells (usually since not hexes)
        PackedBoolList protectedCell_;


    // Private Member Functions

        //- Count set/unset elements in packedlist.
        static label count(const PackedBoolList&, const unsigned int);

        //- Calculate cells that cannot be refined since would trigger
        //  refinement of protectedCell_ (since 2:1 refinement cascade)
        void calculateProtectedCells(PackedBoolList& unrefineableCell) const;

        //- Read the projection parameters from dictionary
        void readDict();


        //- Refine cells. Update mesh and fields.
        autoPtr<mapPolyMesh> refine(const labelList&);

        //- Unrefine cells. Gets passed in centre points of cells to combine.
        autoPtr<mapPolyMesh> unrefine(const labelList&);


        // Selection of cells to un/refine

            //- Calculates approximate value for refinement level so
            //  we don't go above maxCell
            scalar getRefineLevel
            (
                const label maxCells,
                const label maxRefinement,
                const scalar refineLevel,
                const scalarField&
            ) const;

            //- Get per cell max of connected point
            scalarField maxPointField(const scalarField&) const;

            //- Get point max of connected cell
            scalarField maxCellField(const volScalarField&) const;

            scalarField cellToPoint(const scalarField& vFld) const;

            scalarField error
            (
                const scalarField& fld,
                const scalar minLevel,
                const scalar maxLevel
            ) const;

            //- Select candidate cells for refinement
            virtual void selectRefineCandidates
            (
                const scalar lowerRefineLevel,
                const scalar upperRefineLevel,
                const scalarField& vFld,
                PackedBoolList& candidateCell
            ) const;

            //- Subset candidate cells for refinement
            virtual labelList selectRefineCells
            (
                const label maxCells,
                const label maxRefinement,
                const PackedBoolList& candidateCell
            ) const;

            //- Select points that can be unrefined.
            virtual labelList selectUnrefinePoints
            (
                const scalar unrefineLevel,
                const PackedBoolList& markedCell,
                const scalarField& pFld
            ) const;

            //- Extend markedCell with cell-face-cell.
            void extendMarkedCells(PackedBoolList& markedCell) const;

            //- Check all cells have 8 anchor points
            void checkEightAnchorPoints
            (
                PackedBoolList& protectedCell,
                label& nProtected
            ) const;

private:

        //- String representation of refineModes enums
        static const NamedEnum<refineModes, 2> refineModeNames_;

        //- Type of refine mode
        refineModes refineMode_;

        //- The execution interval (in time steps) when using Iteration mode
        label refineInterval_;

        //- Counter for rmTime
        label refineTimeIndex_;

        //- The execution time when using Time mode
        scalar refineTimeInterval_;

        //- Number of steps before the refine time in which the deltaT
        // will start to change
        label nStepsToStartTimeChange_;

        // Has the time step been change by the refiner
        bool deltaTchanged_;

        // Time step when updated due to refinement adjustment
        scalar deltaTSave_;

        // Allow unrefinement
        bool allowUnrefinement_;

        //- Disallow default bitwise copy construct
        dynamicRefineFvMesh(const dynamicRefineFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const dynamicRefineFvMesh&);

public:

    //- Runtime type information
    TypeName("dynamicRefineFvMesh");


    // Constructors

        //- Construct from IOobject
        explicit dynamicRefineFvMesh(const IOobject& io);


    //- Destructor
    virtual ~dynamicRefineFvMesh();


    // Member Functions

        //- Direct access to the refinement engine
        const hexRef8& meshCutter() const
        {
            return meshCutter_;
        }

        //- Cells which should not be refined/unrefined
        const PackedBoolList& protectedCell() const
        {
            return protectedCell_;
        }

        //- Cells which should not be refined/unrefined
        PackedBoolList& protectedCell()
        {
            return protectedCell_;
        }

        //- Update the mesh for both mesh motion and topology change
        virtual bool update();

        //- Adjust the time step so that refinement occurs at the specified time
        void adjustDeltaT();


    // Writing

        //- Write using given format, version and compression
        virtual bool writeObject
        (
            IOstream::streamFormat fmt,
            IOstream::versionNumber ver,
            IOstream::compressionType cmp
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
