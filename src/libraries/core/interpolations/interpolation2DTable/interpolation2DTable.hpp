/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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
    CML::interpolation2DTable

Description
    2D table interpolation. The data must be in ascending order in both
    dimensions x and y.

SourceFiles
    interpolation2DTable.C

\*---------------------------------------------------------------------------*/

#ifndef interpolation2DTable_HPP
#define interpolation2DTable_HPP

#include "List.hpp"
#include "Tuple2.hpp"
#include "tableReader.hpp"

namespace CML
{

template<class Type>
class interpolation2DTable
:
    public List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >
{
public:

    // Public data types

        //- Enumeration for handling out-of-bound values
        enum boundsHandling
        {
            ERROR,          /*!< Exit with a FatalError */
            WARN,           /*!< Issue warning and clamp value (default) */
            CLAMP           /*!< Clamp value to the start/end value */
        };

        //- Cconvenience typedef
        typedef List<Tuple2<scalar, List<Tuple2<scalar, Type> > > > table;


private:

    // Private data

        //- Enumeration for handling out-of-bound values
        boundsHandling boundsHandling_;

        //- File name
        fileName fileName_;

        //- the actual reader
        autoPtr<tableReader<Type> > reader_;


    // Private Member Functions

        //- Read the table of data from file
        void readTable();

        //- Return interpolated value in List
        Type interpolateValue
        (
            const List<Tuple2<scalar, Type> >& data,
            const scalar
        ) const;

        //- Return an X index from the matrix
        template<class BinaryOp>
        label Xi
        (
            const BinaryOp& bop,
            const scalar valueX,
            const bool reverse
        ) const;


public:

    // Constructors

        //- Construct null
        interpolation2DTable();

        //- Construct from components
        interpolation2DTable
        (
            const List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >& values,
            const boundsHandling bounds,
            const fileName& fName
        );

        //- Construct given the name of the file containing the table of data
        interpolation2DTable(const fileName& fName);

        //- Construct by reading the fileName and boundsHandling from dictionary
        interpolation2DTable(const dictionary& dict);

        //- Construct copy
        interpolation2DTable(const interpolation2DTable& interpTable);


    // Member Functions

        //- Return the out-of-bounds handling as a word
        word boundsHandlingToWord(const boundsHandling& bound) const;

        //- Return the out-of-bounds handling as an enumeration
        boundsHandling wordToBoundsHandling(const word& bound) const;

        //- Set the out-of-bounds handling from enum, return previous setting
        boundsHandling outOfBounds(const boundsHandling& bound);

        //- Check that list is monotonically increasing
        //  Exit with a FatalError if there is a problem
        void checkOrder() const;

        //- Write
        void write(Ostream& os) const;


    // Member Operators

        //- Return an element of constant Tuple2<scalar, Type>
        const List<Tuple2<scalar, Type> >& operator[](const label) const;

        //- Return an interpolated value
        Type operator()(const scalar, const scalar) const;
};


} // End namespace CML

#include "IFstream.hpp"
#include "caelusTableReader.hpp"

template<class Type>
void CML::interpolation2DTable<Type>::readTable()
{
    fileName fName(fileName_);
    fName.expand();

    // Read data from file
    reader_()(fName, *this);

    if (this->empty())
    {
        FatalErrorInFunction
            << "table read from " << fName << " is empty" << nl
            << exit(FatalError);
    }

    // Check that the data are in ascending order
    checkOrder();
}


template<class Type>
CML::interpolation2DTable<Type>::interpolation2DTable()
:
    List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >(),
    boundsHandling_(interpolation2DTable::WARN),
    fileName_("fileNameIsUndefined"),
    reader_(nullptr)
{}


template<class Type>
CML::interpolation2DTable<Type>::interpolation2DTable
(
    const List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >& values,
    const boundsHandling bounds,
    const fileName& fName
)
:
    List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >(values),
    boundsHandling_(bounds),
    fileName_(fName),
    reader_(nullptr)
{}


template<class Type>
CML::interpolation2DTable<Type>::interpolation2DTable(const fileName& fName)
:
    List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >(),
    boundsHandling_(interpolation2DTable::WARN),
    fileName_(fName),
    reader_(new openCMLTableReader<Type>(dictionary()))
{
    readTable();
}


template<class Type>
CML::interpolation2DTable<Type>::interpolation2DTable(const dictionary& dict)
:
    List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >(),
    boundsHandling_(wordToBoundsHandling(dict.lookup("outOfBounds"))),
    fileName_(dict.lookup("fileName")),
    reader_(tableReader<Type>::New(dict))
{
    readTable();
}


template<class Type>
CML::interpolation2DTable<Type>::interpolation2DTable
(
     const interpolation2DTable& interpTable
)
:
    List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >(interpTable),
    boundsHandling_(interpTable.boundsHandling_),
    fileName_(interpTable.fileName_),
    reader_(interpTable.reader_)    // note: steals reader. Used in write().
{}



template<class Type>
Type CML::interpolation2DTable<Type>::interpolateValue
(
    const List<Tuple2<scalar, Type> >& data,
    const scalar lookupValue
) const
{
    label n = data.size();

    scalar minLimit = data.first().first();
    scalar maxLimit = data.last().first();

    if (lookupValue < minLimit)
    {
        switch (boundsHandling_)
        {
            case interpolation2DTable::ERROR:
            {
                FatalErrorInFunction
                    << "value (" << lookupValue << ") less than lower "
                    << "bound (" << minLimit << ")" << nl
                    << exit(FatalError);
                break;
            }
            case interpolation2DTable::WARN:
            {
                WarningInFunction
                    << "value (" << lookupValue << ") less than lower "
                    << "bound (" << minLimit << ")" << nl
                    << "    Continuing with the first entry"
                    << endl;
                // fall-through to 'CLAMP'
                [[fallthrough]];
            }
            case interpolation2DTable::CLAMP:
            {
                return data.first().second();
                break;
            }
        }
    }
    else if (lookupValue >= maxLimit)
    {
        switch (boundsHandling_)
        {
            case interpolation2DTable::ERROR:
            {
                FatalErrorInFunction
                    << "value (" << lookupValue << ") greater than upper "
                    << "bound (" << maxLimit << ")" << nl
                    << exit(FatalError);
                break;
            }
            case interpolation2DTable::WARN:
            {
                WarningInFunction
                    << "value (" << lookupValue << ") greater than upper "
                    << "bound (" << maxLimit << ")" << nl
                    << "    Continuing with the last entry"
                    << endl;
                // fall-through to 'CLAMP'
                [[fallthrough]];
            }
            case interpolation2DTable::CLAMP:
            {
                return data.last().second();
                break;
            }
        }
    }

    // look for the correct range in X
    label lo = 0;
    label hi = 0;

    for (label i = 0; i < n; ++i)
    {
        if (lookupValue >= data[i].first())
        {
            lo = hi = i;
        }
        else
        {
            hi = i;
            break;
        }
    }

    if (lo == hi)
    {
        return data[lo].second();
    }
    else
    {
        Type m =
            (data[hi].second() - data[lo].second())
           /(data[hi].first() - data[lo].first());

        // normal interpolation
        return data[lo].second() + m*(lookupValue - data[lo].first());
    }
}


template<class Type>
template<class BinaryOp>
CML::label CML::interpolation2DTable<Type>::Xi
(
    const BinaryOp& bop,
    const scalar valueX,
    const bool reverse
) const
{
    const table& t = *this;

    label limitI = 0;
    if (reverse)
    {
        limitI = t.size() - 1;
    }

    if (bop(valueX, t[limitI].first()))
    {
        switch (boundsHandling_)
        {
            case interpolation2DTable::ERROR:
            {
                FatalErrorInFunction
                    << "value (" << valueX << ") out of bounds"
                    << exit(FatalError);
                break;
            }
            case interpolation2DTable::WARN:
            {
                WarningInFunction
                    << "value (" << valueX << ") out of bounds"
                    << endl;
                // fall-through to 'CLAMP'
                [[fallthrough]];
            }
            case interpolation2DTable::CLAMP:
            {
                return limitI;
            }
            default:
            {
                FatalErrorInFunction
                    << "Un-handled enumeration " << boundsHandling_
                    << abort(FatalError);
            }
        }
    }

    label i = 0;
    if (reverse)
    {
        label nX = t.size();
        i = 0;
        while ((i < nX) && (valueX > t[i].first()))
        {
            i++;
        }
    }
    else
    {
        i = t.size() - 1;
        while ((i > 0) && (valueX < t[i].first()))
        {
            i--;
        }
    }

    return i;
}


template<class Type>
Type CML::interpolation2DTable<Type>::operator()
(
    const scalar valueX,
    const scalar valueY
) const
{
    // Considers all of the list in Y being equal
    label nX = this->size();

    const table& t = *this;

    if (nX == 0)
    {
        WarningInFunction
            << "cannot interpolate a zero-sized table - returning zero" << endl;

        return pTraits<Type>::zero;
    }
    else if (nX == 1)
    {
        // only 1 column (in X) - interpolate to find Y value
        return interpolateValue(t.first().second(), valueY);
    }
    else
    {
        // have 2-D data, interpolate

        // find low and high indices in the X range that bound valueX
        label x0i = Xi(lessOp<scalar>(), valueX, false);
        label x1i = Xi(greaterOp<scalar>(), valueX, true);

        if (x0i == x1i)
        {
            return interpolateValue(t[x0i].second(), valueY);
        }
        else
        {
            Type y0(interpolateValue(t[x0i].second(), valueY));
            Type y1(interpolateValue(t[x1i].second(), valueY));

            // gradient in X
            scalar x0 = t[x0i].first();
            scalar x1 = t[x1i].first();
            Type mX = (y1 - y0)/(x1 - x0);

            // interpolate
            return y0 + mX*(valueX - x0);
        }
    }
}


template<class Type>
CML::word CML::interpolation2DTable<Type>::boundsHandlingToWord
(
     const boundsHandling& bound
) const
{
    word enumName("warn");

    switch (bound)
    {
        case interpolation2DTable::ERROR:
        {
            enumName = "error";
            break;
        }
        case interpolation2DTable::WARN:
        {
            enumName = "warn";
            break;
        }
        case interpolation2DTable::CLAMP:
        {
            enumName = "clamp";
            break;
        }
    }

    return enumName;
}


template<class Type>
typename CML::interpolation2DTable<Type>::boundsHandling
CML::interpolation2DTable<Type>::wordToBoundsHandling
(
    const word& bound
) const
{
    if (bound == "error")
    {
        return interpolation2DTable::ERROR;
    }
    else if (bound == "warn")
    {
        return interpolation2DTable::WARN;
    }
    else if (bound == "clamp")
    {
        return interpolation2DTable::CLAMP;
    }
    else
    {
        WarningInFunction
            << "bad outOfBounds specifier " << bound << " using 'warn'" << endl;

        return interpolation2DTable::WARN;
    }
}


template<class Type>
typename CML::interpolation2DTable<Type>::boundsHandling
CML::interpolation2DTable<Type>::outOfBounds
(
    const boundsHandling& bound
)
{
    boundsHandling prev = boundsHandling_;
    boundsHandling_ = bound;
    return prev;
}


template<class Type>
void CML::interpolation2DTable<Type>::checkOrder() const
{
    label n = this->size();
    const table& t = *this;

    scalar prevValue = t[0].first();

    for (label i=1; i<n; ++i)
    {
        const scalar currValue = t[i].first();

        // avoid duplicate values (divide-by-zero error)
        if (currValue <= prevValue)
        {
            FatalErrorInFunction
                << "out-of-order value: "
                << currValue << " at index " << i << nl
                << exit(FatalError);
        }
        prevValue = currValue;
    }
}


template<class Type>
void CML::interpolation2DTable<Type>::write(Ostream& os) const
{
    os.writeKeyword("fileName")
        << fileName_ << token::END_STATEMENT << nl;
    os.writeKeyword("outOfBounds")
        << boundsHandlingToWord(boundsHandling_) << token::END_STATEMENT << nl;

    *this >> os;
}


#endif


