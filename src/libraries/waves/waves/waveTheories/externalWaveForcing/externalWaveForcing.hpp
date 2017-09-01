/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Deltares
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::waveTheories::externalWaveForcing

Description


SourceFiles
    externalWaveForcing.cpp

Author
    Niels Gjøl Jacobsen, Deltares.
    Bo Terp Paulsen, Deltares.


\*---------------------------------------------------------------------------*/

#ifndef externalWaveForcing_HPP
#define externalWaveForcing_HPP

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "uniformDimensionedFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

/*---------------------------------------------------------------------------*\
                         Class externalWaveForcing Declaration
\*---------------------------------------------------------------------------*/

class externalWaveForcing
:
    public regIOobject
{

protected:

    // Protected data
        Time& rT_;

        const fvMesh& mesh_;

    // Protected member functions


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        externalWaveForcing(const externalWaveForcing&);

        //- Disallow default bitwise assignment
        void operator=(const externalWaveForcing&);

public:

    //- Runtime type information
    TypeName("externalWaveForcing");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            externalWaveForcing,
            externalWaveForcing,
            (
                IOobject io,
                Time& rT,
                const fvMesh& mesh
            ),
            (io, rT, mesh)
        );

    // Constructors

        //- Construct from components
        externalWaveForcing
        (
            IOobject io,
            Time& rT,
            const fvMesh& mesh
        );


    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<externalWaveForcing> New
        (
            IOobject io,
            Time& rT,
            const fvMesh& mesh
        );

    // Destructor

        virtual ~externalWaveForcing();

    // Member Functions
        virtual void step() = 0;


        virtual void close() = 0;


        virtual scalar eta
        (
            const point&,
            const scalar&
        ) const = 0;


        virtual scalar ddxPd
        (
            const point&,
            const scalar&,
            const vector&
        ) const = 0;


        virtual vector U
        (
            const point&,
            const scalar&
        ) const = 0;


        virtual scalar p
        (
            const point&,
            const scalar&
        ) const = 0;


        virtual bool writeData(CML::Ostream&) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
