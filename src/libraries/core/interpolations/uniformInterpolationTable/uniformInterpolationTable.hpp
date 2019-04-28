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
    CML::uniformInterpolationTable

Description
    Table with uniform interval in independent variable, with linear
    interpolation

    Example usage (scalar): values specified within a dictionary:

    \verbatim
    {
        x0          0;          // lower limit
        dx          0.2;        // fixed interval
        log10       true;       // take log(10) when interpolating?
        data                    // list of dependent data values
        (
            7870                // value at x0
            7870                // value at x0 + dx
            ...
            7870                // value at x0 + n*dx
        );
    }
    \endverbatim

SourceFiles
    uniformInterpolationTable.cpp

\*---------------------------------------------------------------------------*/

#ifndef uniformInterpolationTable_H
#define uniformInterpolationTable_H

#include "List.hpp"
#include "Switch.hpp"
#include "IOobject.hpp"
#include "objectRegistry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class uniformInterpolationTable Declaration
\*---------------------------------------------------------------------------*/

template <class Type>
class uniformInterpolationTable
:
    public IOobject,
    public List<Type>
{
    // Private data

        // Control parameters

            //- Lower limit
            scalar x0_;

            //- Fixed interval
            scalar dx_;

            //- Flag to indicate that x data are given in log10(x) form
            Switch log10_;

            //- Bound x values
            Switch bound_;


    // Private Member Functions

        //- Check that the table is valid
        void checkTable() const;

        //- Disallow default bitwise assignment
        void operator=(const uniformInterpolationTable&);


public:

    // Constructors

        //- Construct from IOobject and readFields flag.
        //  Creates a null object if readFields = false
        uniformInterpolationTable(const IOobject&, const bool readFields);

        //- Construct from name, objectRegistry and dictionary.
        //  If initialiseOnly flag is set, control parameters are read from
        //  the dictionary, but not the data table
        uniformInterpolationTable
        (
            const word& tableName,
            const objectRegistry&,
            const dictionary&,
            const bool initialiseOnly = false
        );

        //- Construct as copy
        uniformInterpolationTable(const uniformInterpolationTable&);


    //- Destructor
    ~uniformInterpolationTable();


    // Member Functions

        // Access

            //- Return the lower limit
            inline scalar x0() const;

            //- Return the fixed interval
            inline scalar dx() const;

            //- Return the log10(x) flag
            inline const Switch& log10() const;

            //- Return the bound flag
            inline const Switch& bound() const;


        // Edit

            //- Return the lower limit
            inline scalar& x0();

            //- Return the fixed interval
            inline scalar& dx();

            //- Return the log10(x) flag
            inline Switch& log10();

            //- Return the bound flag
            inline Switch& bound();


        // Evaluation

            //- Return the minimum x value
            inline scalar xMin() const;

            //- Return the maximum x value
            inline scalar xMax() const;

            //- Interpolate
            Type interpolate(scalar x) const;

            //- Interpolate - takes log10 flag into account
            Type interpolateLog10(scalar x) const;


        // Override ancestor size() function and [] operator

            //- Return the size of the table
            using List<Type>::size;

            //- Use List[] operator for read/write access
            using List<Type>::operator[];


        // I-O

            //- Write
            void write() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template <class Type>
CML::scalar CML::uniformInterpolationTable<Type>::x0() const
{
    return x0_;
}


template <class Type>
CML::scalar CML::uniformInterpolationTable<Type>::dx() const
{
    return dx_;
}


template <class Type>
const CML::Switch& CML::uniformInterpolationTable<Type>::log10() const
{
    return log10_;
}


template <class Type>
const CML::Switch& CML::uniformInterpolationTable<Type>::bound() const
{
    return bound_;
}


template <class Type>
CML::scalar& CML::uniformInterpolationTable<Type>::x0()
{
    return x0_;
}


template <class Type>
CML::scalar& CML::uniformInterpolationTable<Type>::dx()
{
    return dx_;
}


template <class Type>
CML::Switch& CML::uniformInterpolationTable<Type>::log10()
{
    return log10_;
}


template <class Type>
CML::Switch& CML::uniformInterpolationTable<Type>::bound()
{
    return bound_;
}


template <class Type>
CML::scalar CML::uniformInterpolationTable<Type>::xMin() const
{
    return x0_;
}


template <class Type>
CML::scalar CML::uniformInterpolationTable<Type>::xMax() const
{
    return x0_ + dx_*(size() - 1);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"

// * * * * * * * * * * * *  Private Member Functions * * * * * * * * * * * * //

template <class Type>
void CML::uniformInterpolationTable<Type>::checkTable() const
{
    if (size() < 2)
    {
        FatalErrorInFunction
            << "Table " << name() << ": must have at least 2 values." << nl
            << "Table size = " << size() << nl
            << "    min, interval width = " << x0_ << ", " << dx_ << nl
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class Type>
CML::uniformInterpolationTable<Type>::uniformInterpolationTable
(
    const IOobject& io,
    bool readFields
)
:
    IOobject(io),
    List<scalar>(2, 0.0),
    x0_(0.0),
    dx_(1.0),
    log10_(false),
    bound_(false)
{
    if (readFields)
    {
        IOdictionary dict(io);

        dict.lookup("data") >> *this;
        dict.lookup("x0") >> x0_;
        dict.lookup("dx") >> dx_;
        dict.readIfPresent("log10", log10_);
        dict.readIfPresent("bound", bound_);
    }

    checkTable();
}


template <class Type>
CML::uniformInterpolationTable<Type>::uniformInterpolationTable
(
    const word& tableName,
    const objectRegistry& db,
    const dictionary& dict,
    const bool initialiseOnly
)
:
    IOobject
    (
        tableName,
        db.time().constant(),
        db,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false // if used in BCs, could be used by multiple patches
    ),
    List<scalar>(2, 0.0),
    x0_(readScalar(dict.lookup("x0"))),
    dx_(readScalar(dict.lookup("dx"))),
    log10_(dict.lookupOrDefault<Switch>("log10", false)),
    bound_(dict.lookupOrDefault<Switch>("bound", false))
{
    if (initialiseOnly)
    {
        const scalar xMax = readScalar(dict.lookup("xMax"));
        const label nIntervals = static_cast<label>(xMax - x0_)/dx_ + 1;
        this->setSize(nIntervals);
    }
    else
    {
        dict.lookup("data") >> *this;
    }

    checkTable();
}


template <class Type>
CML::uniformInterpolationTable<Type>::uniformInterpolationTable
(
    const uniformInterpolationTable& uit
)
:
    IOobject(uit),
    List<scalar>(uit),
    x0_(uit.x0_),
    dx_(uit.dx_),
    log10_(uit.log10_),
    bound_(uit.bound_)
{
    checkTable();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template <class Type>
CML::uniformInterpolationTable<Type>::~uniformInterpolationTable()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template <class Type>
Type CML::uniformInterpolationTable<Type>::interpolate(scalar x) const
{
    if (bound_)
    {
        x = max(min(xMax() - SMALL*dx_, x), x0_);
    }
    else
    {
        if (x < x0_)
        {
            FatalErrorInFunction
                << "Supplied value is less than minimum table value:" << nl
                << "xMin=" << x0_ << ", xMax=" << xMax() << ", x=" << x << nl
                << exit(FatalError);
        }

        if (x > xMax())
        {
            FatalErrorInFunction
                << "Supplied value is greater than maximum table value:" << nl
                << "xMin=" << x0_ << ", xMax=" << xMax() << ", x=" << x << nl
                << exit(FatalError);
        }
    }

    const label i = static_cast<label>((x - x0_)/dx_);

    const scalar xLo = x0_ + i*dx_;

    Type fx = (x - xLo)/dx_*(operator[](i+1) - operator[](i)) + operator[](i);

    if (debug)
    {
        Info<< "Table: " << name() << ", x=" << x
            << ", x_lo=" << xLo << ", x_hi=" << xLo + dx_
            << ", f(x_lo)=" << operator[](i) << ", f(x_hi)=" << operator[](i+1)
            << ", f(x)=" << fx << endl;
    }

    return fx;
}


template <class Type>
Type CML::uniformInterpolationTable<Type>::interpolateLog10
(
    scalar x
) const
{
    if (log10_)
    {
        if (x > 0)
        {
            x = ::log10(x);
        }
        else if (bound_ && (x <= 0))
        {
            x = x0_;
        }
        else
        {
            FatalErrorInFunction
                << "Table " << name() << nl
                << "Supplied value must be greater than 0 when in log10 mode"
                << nl << "x=" << x << nl << exit(FatalError);
        }
    }

    return interpolate(x);
}


template <class Type>
void CML::uniformInterpolationTable<Type>::write() const
{
    IOdictionary dict(*this);

    dict.add("data", static_cast<const List<scalar>&>(*this));
    dict.add("x0", x0_);
    dict.add("dx", dx_);
    if (log10_)
    {
        dict.add("log10", log10_);
    }
    if (bound_)
    {
        dict.add("bound", bound_);
    }

    dict.regIOobject::write();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
