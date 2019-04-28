/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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
    CML::specieElement

Description


\*---------------------------------------------------------------------------*/

#ifndef specieElement_HPP
#define specieElement_HPP

#include "word.hpp"
#include "token.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
class specieElement;
Ostream& operator<<(Ostream&, const specieElement&);


/*---------------------------------------------------------------------------*\
                         Class specieElement Declaration
\*---------------------------------------------------------------------------*/

class specieElement
{

    //- Name of the element
    word name_;

    //- Number of atoms of this element in the specie
    label nAtoms_;


public:


    //- Construct null
    inline specieElement()
    {}

    //- Construct from components
    inline specieElement(const word& name, const label nAtoms)
    :
        name_(name),
        nAtoms_(nAtoms)
    {}

    //- Construct from Istream
    inline specieElement(Istream& is)
    :
        name_(is),
        nAtoms_(readLabel(is))
    {}


    // Member Functions

    //- Return the name of the element
    inline const word& name() const
    {
        return name_;
    }

    //- Return non-const access to the name of the element
    inline word& name()
    {
        return name_;
    }

    //- Return the number of atoms of this element in the specie
    inline label nAtoms() const
    {
        return nAtoms_;
    }

    //- Return non-const access to the number of atoms of this element
    //  in the specie
    inline label& nAtoms()
    {
        return nAtoms_;
    }


    // Member Operators

    //- Equality comparison
    inline bool operator==(const specieElement& se) const
    {
        return
        (
            nAtoms_ == se.nAtoms_
         && name_ == se.name_
        );
    }

    //- Inequality comparison
    inline bool operator!=(const specieElement& se) const
    {
        return !operator==(se);
    }


    // IOstream Operators

    inline friend Ostream& operator<<(Ostream& os, const specieElement& se)
    {
        os  << se.name() << token::SPACE << se.nAtoms();
        return os;
    }
};


} // End namespace CML


#endif
