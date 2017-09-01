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
    CML::waveTheories::emptyExternal

Description


SourceFiles
    emptyExternal.cpp

Author
    Niels Gjøl Jacobsen, Deltares.
    Bo Terp Paulsen, Deltares.

\*---------------------------------------------------------------------------*/

#ifndef emptyExternal_HPP
#define emptyExternal_HPP

#include "externalWaveForcing.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

/*---------------------------------------------------------------------------*\
                           Class emptyExternal Declaration
\*---------------------------------------------------------------------------*/

class emptyExternal
:
    public externalWaveForcing
{

protected:

    // Protected data

    // Protected member functions

public:

    //- Runtime type information
    TypeName("emptyExternal");

    // Constructors

        //- from components
        emptyExternal
        (
            IOobject,
            Time&,
            const fvMesh&
        );


    // Destructor

        ~emptyExternal()
        {}


    // Member Functions
        virtual void step();


        virtual inline void close()
        {
        	// Nothing to be done
        };


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


        virtual vector U
        (
            const point&,
            const scalar&
        ) const;


        virtual scalar p
        (
            const point&,
            const scalar&
        ) const;


        virtual bool writeData(CML::Ostream&) const
        {
            return true;
        };

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
