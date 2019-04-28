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
    CML::distributionModels::RosinRammler

Description
    Rosin-Rammler distributionModel

   \f[
       cumulative model =
           (1.0 - exp( -(( x - d0)/d)^n )
         / (1.0 - exp( -((d1 - d0)/d)^n )
   \f]


SourceFiles
    RosinRammler.cpp

\*---------------------------------------------------------------------------*/

#ifndef RosinRammler_H
#define RosinRammler_H

#include "distributionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{

/*---------------------------------------------------------------------------*\
                       Class RosinRammler Declaration
\*---------------------------------------------------------------------------*/

class RosinRammler
:
    public distributionModel
{
    // Private data

        //- Distribution minimum
        scalar minValue_;

        //- Distribution maximum
        scalar maxValue_;

        // Model coefficients

            scalar d_;
            scalar n_;


public:

    //- Runtime type information
    TypeName("RosinRammler");


    // Constructors

        //- Construct from components
        RosinRammler(const dictionary& dict, Random& rndGen);

        //- Construct copy
        RosinRammler(const RosinRammler& p);

        //- Construct and return a clone
        virtual autoPtr<distributionModel> clone() const
        {
            return autoPtr<distributionModel>(new RosinRammler(*this));
        }


    //- Destructor
    virtual ~RosinRammler();


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
