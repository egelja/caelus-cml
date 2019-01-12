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
    CML::distributionModels::fixedValue

Description
    Returns a fixed value

SourceFiles
    fixedValue.cpp

\*---------------------------------------------------------------------------*/

#ifndef distributionModelFixedValue_H
#define distributionModelFixedValue_H

#include "distributionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{
/*---------------------------------------------------------------------------*\
                       Class fixedValue Declaration
\*---------------------------------------------------------------------------*/

class fixedValue
:
    public distributionModel
{
    // Private data

        //- Fixed value
        scalar value_;


public:

    //- Runtime type information
    TypeName("fixedValue");


    // Constructors

        //- Construct from components
        fixedValue(const dictionary& dict, Random& rndGen);

        //- Construct copy
        fixedValue(const fixedValue& p);

        //- Construct and return a clone
        virtual autoPtr<distributionModel> clone() const
        {
            return autoPtr<distributionModel>(new fixedValue(*this));
        }


    //- Destructor
    virtual ~fixedValue();


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
