/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::ash

Description
    Coal ash solid properties

SourceFiles
    ash.cpp

\*---------------------------------------------------------------------------*/

#ifndef solid_ash_HPP
#define solid_ash_HPP

#include "solidProperties.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                              Class ash Declaration
\*---------------------------------------------------------------------------*/

class ash
:
    public solidProperties
{

public:

    //- Runtime type information
    TypeName("ash");


    // Constructors

        //- Construct null
        ash();

        //- Construct from dictionary
        ash(const dictionary& dict);

        //- Construct and return clone
        virtual autoPtr<solidProperties> clone() const
        {
            return autoPtr<solidProperties>(new ash(*this));
        }


    // I-O

        //- Write the function coefficients
        void writeData(Ostream& os) const;

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const ash& s);
};


Ostream& operator<<(Ostream& os, const ash& s);


} // End namespace CML


#endif


