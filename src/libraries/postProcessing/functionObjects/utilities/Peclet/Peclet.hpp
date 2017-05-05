/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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
    CML::Peclet

Description
    This function object calculates and outputs the Peclet number as a
    surfaceScalarField.

SourceFiles
    Peclet.cpp
    IOPeclet.hpp

\*---------------------------------------------------------------------------*/

#ifndef Peclet_H
#define Peclet_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"
#include "pointFieldFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                          Class Peclet Declaration
\*---------------------------------------------------------------------------*/

class Peclet
{
    // Private data

        //- Name of this set of Peclet objects
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Name of flux field, default is "phi"
        word phiName_;

        //- Name of density field (compressible cases only), default is "rho"
        word rhoName_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        Peclet(const Peclet&);

        //- Disallow default bitwise assignment
        void operator=(const Peclet&);


public:

    //- Runtime type information
    TypeName("Peclet");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        Peclet
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~Peclet();


    // Member Functions

        //- Return name of the set of Peclet
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the Peclet data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the Peclet and write
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
