/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
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
    CML::waveTheories::potentialCurrent

Description
    Implementation of a potential current velocity profile, e.g. uniform
    velocity distribution over the vertical. In the special case of
    U = vector::zero it can be used as outlet sponge layer wave type.

    A description of the general wave framework is given in

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    potentialCurrent.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#ifndef potentialCurrent_HPP
#define potentialCurrent_HPP

#include "waveTheory.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

/*---------------------------------------------------------------------------*\
                           Class potentialCurrent Declaration
\*---------------------------------------------------------------------------*/

class potentialCurrent
:
    public waveTheory
{

protected:

    // Protected data
        vector U_;
        scalar Tsoft_;
        scalar localSeaLevel_;

    // Protected member functions

        scalar factor(const scalar&) const;
public:

    //- Runtime type information
    TypeName("potentialCurrent");

    // Constructors

        //- from components
        potentialCurrent
        (
            const word&,
            const fvMesh& mesh_
        );


    // Destructor

        ~potentialCurrent()
        {}


    // Member Functions
        virtual Switch conflictTSoftInitialise() const
        {
            if (CML::mag(U_) > SMALL && Tsoft_ > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
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
