/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 Applied CCM
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
    CML::dynCompressionModel

Description
    Abstract base class for dynamic interface compression models.

Author
    Darrin Stephens

SourceFiles
    dynCompressionModel.cpp
    newDynCompressionModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef dynCompressionModel_HPP
#define dynCompressionModel_HPP

#include "fvMesh.hpp"
#include "IOdictionary.hpp"
#include "dictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "fvc.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class compressionModel Declaration
\*---------------------------------------------------------------------------*/

class dynCompressionModel
{
protected:

    // Protected data

        //- Mesh
        const fvMesh& mesh_;

        //- Velocity
        const volVectorField& U_;

        //- Flux
        const surfaceScalarField& phi_;

        //- Volume fraction
        const volScalarField& alpha1_;

        //- Initial compression coefficient
        scalar cAlpha0_;

        //- Isotropic compression coefficient
        scalar icAlpha_;

        //- Face compression speed
        surfaceScalarField phic_;

        //- Current time index
        label curTimeIndex_;

    // Protected Member Functions
        void updateCompression();

private:
    // Private Member Functions

        //- Disallow default bitwise copy construct
        dynCompressionModel(const dynCompressionModel&);

        //- Disallow default bitwise assignment
        void operator=(const dynCompressionModel&);


public:

    //- Runtime type information
    TypeName("dynCompressionModel");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            dynCompressionModel,
            dictionary,
            (
                const fvMesh& mesh,
                const volVectorField& U,
                const surfaceScalarField& phi,
                const volScalarField& alpha1
            ),
            (mesh, U, phi, alpha1)
        );


    // Constructors

        //- Construct from components
        dynCompressionModel
        (
            const fvMesh&,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const volScalarField& alpha1
        );


    // Selectors

        //- Return a reference to the selected compression model
        static autoPtr<dynCompressionModel> New
        (
            const fvMesh&,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const volScalarField& alpha1
        );


    // Destructor

        virtual ~dynCompressionModel()
        {}


    // Member Functions

        //- Return the interface compression coefficient at face centres
        virtual const surfaceScalarField& cAlphaf() const = 0;

        //- Return the interface compression coefficient at cell centres
        virtual const volScalarField& cAlpha() const = 0;

        virtual void correct() = 0;

        //- Return the face compression speed
        const surfaceScalarField& phic();

        //- Read compression coefficients
        bool read();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
