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
    CML::waveTheories::externalSource

Description


SourceFiles
    externalSource.cpp

Author
    Niels Gjøl Jacobsen, Deltares.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef externalSource_HPP
#define externalSource_HPP

#include "waveTheory.hpp"
#include "externalWaveForcing.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

/*---------------------------------------------------------------------------*\
                           Class externalSource Declaration
\*---------------------------------------------------------------------------*/

class externalSource
:
    public waveTheory
{

protected:

    // Protected data
        const externalWaveForcing& external_;

    // Protected member functions

        scalar factor(const scalar&) const;
public:

    //- Runtime type information
    TypeName("externalSource");

    // Constructors

        //- from components
        externalSource
        (
            const word&,
            const fvMesh& mesh_
        );


    // Destructor

        ~externalSource()
        {}


    // Member Functions
        virtual Switch conflictTSoftInitialise() const
        {
            return true;
        };

        void printCoeffs();

        virtual scalar eta
        (
            const point&,
            const scalar&
        ) const;

        virtual scalar ddxPd
        (
            const point&,
            const scalar&,
            const vector&
        ) const;

        virtual scalar p
        (
            const point&,
            const scalar&
        ) const;

        virtual vector U
        (
            const point&,
            const scalar&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
