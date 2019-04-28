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
    CML::thirdBodyEfficiencies

Description
    Third body efficiencies


\*---------------------------------------------------------------------------*/

#ifndef thirdBodyEfficiencies_HPP
#define thirdBodyEfficiencies_HPP

#include "scalarList.hpp"
#include "speciesTable.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class thirdBodyEfficiencies;
Ostream& operator<<(Ostream&, const thirdBodyEfficiencies&);


/*---------------------------------------------------------------------------*\
                  Class thirdBodyEfficiencies Declaration
\*---------------------------------------------------------------------------*/

class thirdBodyEfficiencies
:
    public scalarList
{

    const speciesTable& species_;


public:


    //- Construct from components
    inline thirdBodyEfficiencies
    (
        const speciesTable& species,
        const scalarList& efficiencies
    )
    :
        scalarList(efficiencies),
        species_(species)
    {
        if (size() != species_.size())
        {
            FatalErrorInFunction
                << "number of efficiencies = " << size()
                << " is not equal to the number of species " << species_.size()
                << exit(FatalError);
        }
    }

    //- Construct from dictionary
    inline thirdBodyEfficiencies
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        scalarList(species.size()),
        species_(species)
    {
        if (dict.found("coeffs"))
        {
            List<Tuple2<word, scalar> > coeffs(dict.lookup("coeffs"));
            if (coeffs.size() != species_.size())
            {
                FatalErrorInFunction
                    << "number of efficiencies = " << coeffs.size()
                    << " is not equat to the number of species " << species_.size()
                    << exit(FatalIOError);
            }

            forAll(coeffs, i)
            {
                operator[](species[coeffs[i].first()]) = coeffs[i].second();
            }
        }
        else
        {
            scalar defaultEff = readScalar(dict.lookup("defaultEfficiency"));
            scalarList::operator=(defaultEff);
        }
    }


    // Member functions

    //- Calculate and return M, the concentration of the third-bodies
    inline scalar M(const scalarList& c) const
    {
        scalar M = 0;
        forAll(*this, i)
        {
            M += operator[](i)*c[i];
        }

        return M;
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        List<Tuple2<word, scalar> > coeffs(species_.size());
        forAll(coeffs, i)
        {
            coeffs[i].first() = species_[i];
            coeffs[i].second() = operator[](i);
        }

        os.writeKeyword("coeffs") << coeffs << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const thirdBodyEfficiencies& tbes
    )
    {
        tbes.write(os);
        return os;
    }

};


} // End namespace CML


#endif
