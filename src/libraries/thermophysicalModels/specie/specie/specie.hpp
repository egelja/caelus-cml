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
    CML::specie

Description
    Base class of the thermophysical property types.

SourceFiles
    specieI.hpp
    specie.cpp

\*---------------------------------------------------------------------------*/

#ifndef specie_HPP
#define specie_HPP

#include "word.hpp"
#include "scalar.hpp"
#include "dictionary.hpp"

#include "thermodynamicConstants.hpp"
using namespace CML::constant::thermodynamic;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class specie;

inline specie operator+(const specie&, const specie&);
inline specie operator*(const scalar, const specie&);
inline specie operator==(const specie&, const specie&);

Ostream& operator<<(Ostream&, const specie&);


/*---------------------------------------------------------------------------*\
                           Class specie Declaration
\*---------------------------------------------------------------------------*/

class specie
{

    //- Name of specie
    word name_;

    //- Number of moles of this component in the mixture
    scalar Y_;

    //- Molecular weight of specie [kg/kmol]
    scalar molWeight_;


public:

    //- Runtime type information
    ClassName("specie");

    //- Construct from components without name
    inline specie(const scalar Y, const scalar molWeight)
    :
        Y_(Y),
        molWeight_(molWeight)
    {}

    //- Construct from components with name
    inline specie
    (
        const word& name,
        const scalar Y,
        const scalar molWeight
    )
    :
        name_(name),
        Y_(Y),
        molWeight_(molWeight)
    {}

    //- Construct as named copy
    inline specie(const word& name, const specie& st)
    :
        name_(name),
        Y_(st.Y_),
        molWeight_(st.molWeight_)
    {}

    //- Construct from dictionary
    specie(const dictionary& dict);


    // Member Functions

    // Access

    //- Name
    inline const word& name() const
    {
        return name_;
    }

    //- Molecular weight [kg/kmol]
    inline scalar W() const
    {
        return molWeight_;
    }

    //- No of moles of this species in mixture
    inline scalar Y() const
    {
        return Y_;
    }

    //- Gas constant [J/(kg K)]
    inline scalar R() const
    {
        return RR/molWeight_;
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const specie& st)
    {
        Y_ = st.Y_;
        molWeight_ = st.molWeight_;
    }

    inline void operator+=(const specie& st)
    {
        const scalar sumY = Y_ + st.Y_;
        if (mag(sumY) > SMALL)
        {
            molWeight_ = sumY/(Y_/molWeight_ + st.Y_/st.molWeight_);
        }

        Y_ = sumY;
    }

    inline void operator*=(const scalar s)
    {
        Y_ *= s;
    }


    // Friend operators
    inline friend specie operator+(const specie& st1, const specie& st2)
    {
        const scalar sumY = max(st1.Y_ + st2.Y_, SMALL);

        if (mag(sumY) > SMALL)
        {
            return specie
            (
                sumY,
                sumY/(st1.Y_/st1.molWeight_ + st2.Y_/st2.molWeight_)
            );
        }
        else
        {
            return st1;
        }
    }

    inline friend specie operator*(const scalar s, const specie& st)
    {
        return specie
        (
            s*st.Y_,
            st.molWeight_
        );
    }

    inline friend specie operator==(const specie& st1, const specie& st2)
    {
        scalar diffY = st2.Y_ - st1.Y_;
        if (mag(diffY) < SMALL)
        {
            diffY = SMALL;
        }

        const scalar diffRW =
            st2.Y_/st2.molWeight_ - st1.Y_/st1.molWeight_;

        scalar molWeight = GREAT;
        if (mag(diffRW) > SMALL)
        {
            molWeight = diffY/diffRW;
        }

        return specie(diffY, molWeight);
    }


    // Ostream Operator
    friend Ostream& operator<<(Ostream&, const specie&);
};


} // End namespace CML


#endif
