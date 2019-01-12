/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::Histogram

Description
    Calculates the counts per bin of a list.


\*---------------------------------------------------------------------------*/

#ifndef Histogram_H
#define Histogram_H

#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


/*---------------------------------------------------------------------------*\
                          Class Histogram Declaration
\*---------------------------------------------------------------------------*/

template<class List>
class Histogram
{
    // Private data

        //- Counts per bin
        labelList counts_;

        //- Number of <= lowest bin
        label nLow_;

        //- Number of > highest bin
        label nHigh_;


    // Private Member Functions

        void count(const List& bins, const List& l);

        //- Disallow default bitwise copy construct
        Histogram(const Histogram&);

        //- Disallow default bitwise assignment
        void operator=(const Histogram&);


public:

    // Constructors

        //- Construct given bin values and input list
        Histogram(const List& bins, const List& l);

        //- Construct given min, max, number of bins and input list
        Histogram
        (
            const typename List::const_reference min,
            const typename List::const_reference max,
            const label nBins,
            const List& l
        );


        // Access

            //- Return the counts per bin
            inline const labelList& counts() const
            {
                return counts_;
            }

            //- Return the number of elements <= bins[0]
            //  (so inclusive lowest bin value)
            inline label nLow() const
            {
                return nLow_;
            }

            //- Return the number of elements > bins[bins.size()-1]
            //  (so exclusive highest bin value)
            inline label nHigh() const
            {
                return nHigh_;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#include "ListOps.hpp"


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class List>
void CML::Histogram<List>::count(const List& bins, const List& l)
{
    if (bins.size() < 2)
    {
        FatalErrorInFunction
            << "Should have at least two values in bins. Now:" << bins
            << exit(FatalError);
    }

    counts_.setSize(bins.size()-1);
    counts_ = 0;

    nLow_ = 0;
    nHigh_ = 0;

    forAll(l, i)
    {
        label index = findLower(bins, l[i]);

        if (index == -1)
        {
            nLow_++;
        }
        else if (index == bins.size()-1)
        {
            nHigh_++;
        }
        else
        {
            counts_[index]++;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class List>
CML::Histogram<List>::Histogram(const List& bins, const List& l)
{
    count(bins, l);
}


template<class List>
CML::Histogram<List>::Histogram
(
    const typename List::const_reference min,
    const typename List::const_reference max,
    const label nBins,
    const List& l
)
{
    List bins(nBins+1);

    typename List::value_type span = (max-min) / nBins;

    bins[0] = min;

    for (label i = 1; i < nBins; i++)
    {
        bins[i] = bins[i-1] + span;
    }

    // Set max directly to avoid truncation errors.
    bins[nBins] = max;

    count(bins, l);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
