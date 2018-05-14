/*---------------------------------------------------------------------------*\
Copyright (C) 2017 OpenFOAM Foundation
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
    CML::DataEntryTypes::ramp

Description
    Ramp function base class for the set of scalar functions starting from 0 and
    increasing monotonically to 1 from \c start over the \c duration and
    remaining at 1 thereafter.

    Usage:
    \verbatim
        <entryName> <rampFunction>;
        <entryName>Coeffs
        {
            start     10;
            duration  20;
        }
    \endverbatim
    or
    \verbatim
        <entryName>
        {
            type      <rampFunction>;
            start     10;
            duration  20;
        }
    \endverbatim

    Where:
    \table
        Property | Description  | Required | Default value
        start    | Start time   | no       | 0
        duration | Duration     | yes      |
    \endtable

See also
    CML::DataEntry

SourceFiles
    ramp.cpp

\*---------------------------------------------------------------------------*/

#ifndef ramp_HPP
#define ramp_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class ramp Declaration
\*---------------------------------------------------------------------------*/

class ramp
:
    public DataEntry<scalar>
{
protected:

    // Protected data

        //- Start-time of the ramp function
        scalar start_;

        //- Duration of the ramp function
        scalar duration_;

        //- Simple linear ramp function
        //  which form the basis of many more complex ramp functions
        inline scalar linearRamp(const scalar t) const
        {
            return max(min((t - start_)/duration_, 1), 0);
        }


private:

    // Private Member Functions

        //- Read the coefficients from the given dictionary
        void read(const dictionary& coeffs);

        //- Disallow default bitwise assignment
        void operator=(const ramp&);


public:

    // Constructors

        //- Construct from entry name and dictionary
        ramp
        (
            const word& entryName,
            const dictionary& dict
        );


    //- Destructor
    virtual ~ramp();


    // Member Functions

        //- Return value for time t
        scalar value(const scalar t) const = 0;

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
