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
    CML::waveTheories::solitaryFirst

Description
    Implementation of a solitary wave theory, based on the limit solution for
    infinite wave length in the KdV equation, which is also the one describing
    the solution for regular cnoidal waves. Implemented following the
    description by

        Hydrodynamics of Coastal Regions
        Ib A. Svendsen and Ivar G. Jonsson
        1982
        Den private ingenioerfond
        Technical University of Denmark

    The vertical velocity component is obtained by a vertical integration of
    the continuity equation.

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
    solitaryFirst.cpp

Author
    Bjarne Jensen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef solitaryFirst_HPP
#define solitaryFirst_HPP

#include "waveTheory.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

/*---------------------------------------------------------------------------*\
                         Class solitaryFirst Declaration
\*---------------------------------------------------------------------------*/

class solitaryFirst
:
    public waveTheory
{

protected:

    // Protected data
        // Read from waveProperties
        scalar H_;
        scalar h_;
        vector propagationDirection_;
        point  x0_;

        // Computed
        scalar G_;
        scalar c_;

    // Protected member functions

        scalar factor(const scalar& ) const;
public:

    //- Runtime type information
    TypeName("solitaryFirst");

    // Constructors

        //- from components
        solitaryFirst
        (
            const word&,
            const fvMesh& mesh_
        );


    // Destructor

        ~solitaryFirst()
        {}


    // Member Functions
        virtual Switch conflictTSoftInitialise() const
        {
            return false;
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

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
