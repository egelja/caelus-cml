/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2013 OpenFOAM Foundation
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
    CML::scalarTransport

Description
    Evolves a passive scalar transport equation.

    - the field can be zeroed on start-up using the resetOnStartUp flag
    - to employ the same numerical schemes as the flow velocity, use the
      autoSchemes flag
    - the diffusivity can be set manually using the DT entry, or retrieved
      from the turbulence model (if applicable)

SourceFiles
    scalarTransport.cpp
    IOscalarTransport.hpp

\*---------------------------------------------------------------------------*/

#ifndef scalarTransport_H
#define scalarTransport_H

#include "volFields.hpp"
#include "surfaceFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "fvMatricesFwd.hpp"
#include "fvOptionList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class scalarTransport Declaration
\*---------------------------------------------------------------------------*/

class scalarTransport
{
    // Private data

        //- Name of field to process
        word fieldName_;

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- On/off switch
        bool active_;

        //- Name of flux field (optional)
        word phiName_;

        //- Name of density field (optional)
        word rhoName_;

        //- Diffusion coefficient (optional)
        scalar D_;

        //- Flag to indicate whether a constant, uniform D_ is specified
        bool constantD_;

        //- Laminar diffusion coefficient (optional)
        scalar alphaD_;

        //- Turbulent diffusion coefficient (optional)
        scalar alphaDt_;

        //- Number of corrector iterations (optional)
        label nCorr_;

        //- Name of field whose schemes are used (optional)
        word schemesField_;

        //- Run-time selectable finite volume options, e.g. sources, constraints
        fv::optionList fvOptions_;

        //- The scalar field
        volScalarField s_;


    // Private Member Functions

        //- Return the diffusivity field
        tmp<volScalarField> D(const surfaceScalarField& phi) const;

        //- Disallow default bitwise copy construct
        scalarTransport(const scalarTransport&);

        //- Disallow default bitwise assignment
        void operator=(const scalarTransport&);


public:

    //- Runtime type information
    TypeName("scalarTransport");


    // Constructors

        //- Construct for given objectRegistry and dictionary
        scalarTransport
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~scalarTransport();


    // Member Functions

        //- Read the scalarTransport data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the scalarTransport and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const pointField&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
