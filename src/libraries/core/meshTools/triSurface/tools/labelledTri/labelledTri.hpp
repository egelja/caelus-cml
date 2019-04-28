/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::labelledTri

Description
    Triangle with additional region number.

SourceFiles
    labelledTriI.hpp

\*---------------------------------------------------------------------------*/

#ifndef labelledTri_H
#define labelledTri_H

#include "triFace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class labelledTri;

Istream& operator>>(Istream&, labelledTri&);
Ostream& operator<<(Ostream&, const labelledTri&);


/*---------------------------------------------------------------------------*\
                           Class labelledTri Declaration
\*---------------------------------------------------------------------------*/

class labelledTri
:
    public triFace
{
    // Private data

        label region_;


public:

    // Constructors

        //- Construct null
        inline labelledTri();

        //- Construct from triFace and a region label
        inline labelledTri
        (
            const triFace&,
            const label region
        );

        //- Construct from three point labels and a region label
        inline labelledTri
        (
            const label a,
            const label b,
            const label c,
            const label region
        );

        //- Construct from Istream
        inline labelledTri(Istream&);


    // Member Functions

        // Access

            //- Return region label
            inline label region() const;

            //- Return region label
            inline label& region();


        // Check

        // Edit

        // Write


    // Friend Functions

    // Friend Operators

    // IOstream Operators

        inline friend Istream& operator>>(Istream&, labelledTri&);
        inline friend Ostream& operator<<(Ostream&, const labelledTri&);
};


template<>
inline bool contiguous<labelledTri>()  {return true;}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "labelledTriI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
