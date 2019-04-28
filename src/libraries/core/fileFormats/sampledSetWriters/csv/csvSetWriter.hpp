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
    CML::csvSetWriter

Description
    Write set in csv format


\*---------------------------------------------------------------------------*/

#ifndef csvSetWriter_H
#define csvSetWriter_H

#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class csvSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class csvSetWriter
:
    public writer<Type>
{

    // Private Member Functions

        void writeCoordHeader(const coordSet&, Ostream&) const;

        void writeHeader(const coordSet&, const wordList&, Ostream&) const;


protected:

    virtual void writeSeparator(Ostream&) const;


public:

    //- Runtime type information
    TypeName("csv");


    // Constructors

        //- Construct null
        csvSetWriter();


    //- Destructor
    virtual ~csvSetWriter();


    // Member Functions

        virtual fileName getFileName
        (
            const coordSet&,
            const wordList&
        ) const;

        virtual void write
        (
            const coordSet&,
            const wordList&,
            const List<const Field<Type>*>&,
            Ostream&
        ) const;

        virtual void write
        (
            const bool writeTracks,
            const PtrList<coordSet>&,
            const wordList& valueSetNames,
            const List<List<Field<Type> > >&,
            Ostream&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "coordSet.hpp"
#include "fileName.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::csvSetWriter<Type>::csvSetWriter()
:
    writer<Type>()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::csvSetWriter<Type>::~csvSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::fileName CML::csvSetWriter<Type>::getFileName
(
    const coordSet& points,
    const wordList& valueSetNames
) const
{
    return this->getBaseName(points, valueSetNames) + ".csv";
}


template<class Type>
void CML::csvSetWriter<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<const Field<Type>*>& valueSets,
    Ostream& os
) const
{
    writeHeader(points,valueSetNames,os);

    // Collect sets into columns
    List<const List<Type>*> columns(valueSets.size());

    forAll(valueSets, i)
    {
        columns[i] = valueSets[i];
    }

    this->writeTable(points, columns, os);
}


template<class Type>
void CML::csvSetWriter<Type>::write
(
    const bool writeTracks,
    const PtrList<coordSet>& points,
    const wordList& valueSetNames,
    const List<List<Field<Type> > >& valueSets,
    Ostream& os
) const
{
    writeHeader(points[0],valueSetNames,os);

    if (valueSets.size() != valueSetNames.size())
    {
        FatalErrorInFunction
            << "Number of variables:" << valueSetNames.size() << endl
            << "Number of valueSets:" << valueSets.size()
            << exit(FatalError);
    }

    List<const List<Type>*> columns(valueSets.size());

    forAll(points, trackI)
    {
        // Collect sets into columns
        forAll(valueSets, i)
        {
            columns[i] = &valueSets[i][trackI];
        }

        this->writeTable(points[trackI], columns, os);
        os  << nl << nl;
    }
}


template<class Type>
void CML::csvSetWriter<Type>::writeSeparator(Ostream& os) const
{
    os << token::COMMA;
}


namespace CML
{
    // otherwise compiler complains about specialization
    template<>
    void csvSetWriter<scalar>::writeHeader
    (
        const coordSet& points,
        const wordList& valueSetNames,
        Ostream& os
    ) const
    {
        writeCoordHeader(points, os);

        forAll(valueSetNames, i)
        {
            if (i > 0)
            {
                writeSeparator(os);
            }
            os << valueSetNames[i];
        }

        os << nl;
    }
} // end namespace


template<class Type>
void CML::csvSetWriter<Type>::writeHeader
(
    const coordSet& points,
    const wordList& valueSetNames,
    Ostream& os
) const
{
    writeCoordHeader(points, os);

    forAll(valueSetNames, i)
    {
        for (label j=0; j<Type::nComponents; j++)
        {
            if (i>0 || j>0)
            {
                writeSeparator(os);
            }
            os << valueSetNames[i] << "_" << j;
        }
    }

    os << nl;
}


template<class Type>
void CML::csvSetWriter<Type>::writeCoordHeader
(
    const coordSet& points,
    Ostream& os
) const
{
    if (points.hasVectorAxis())
    {
        forAll(points, i)
        {
            os << points.axis()[i];
            writeSeparator(os);
        }
    }
    else
    {
        os << points.axis();
        writeSeparator(os);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
