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
    CML::distributionModels::general

Description
    general distribution model

SourceFiles
    general.cpp

\*---------------------------------------------------------------------------*/

#ifndef general_H
#define general_H

#include "distributionModel.hpp"
#include "Vector_.hpp"
#include "VectorSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{

/*---------------------------------------------------------------------------*\
                           Class general Declaration
\*---------------------------------------------------------------------------*/

class general
:
    public distributionModel
{
    // Private data

        typedef VectorSpace<Vector<scalar>, scalar, 2> pair;

        List<pair> xy_;

        label nEntries_;

        //- Min and max values of the distribution
        scalar minValue_;
        scalar maxValue_;

        scalar meanValue_;

        List<scalar> integral_;


public:

    //- Runtime type information
    TypeName("general");


    // Constructors

        //- Construct from components
        general(const dictionary& dict, Random& rndGen);

        //- Construct copy
        general(const general& p);

        //- Construct and return a clone
        virtual autoPtr<distributionModel> clone() const
        {
            return autoPtr<distributionModel>(new general(*this));
        }


    //- Destructor
    virtual ~general();


    // Member Functions

        //- Sample the distributionModel
        virtual scalar sample() const;

        //- Return the minimum value
        virtual scalar minValue() const;

        //- Return the maximum value
        virtual scalar maxValue() const;

        //- Return the mean value
        virtual scalar meanValue() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace distributionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
