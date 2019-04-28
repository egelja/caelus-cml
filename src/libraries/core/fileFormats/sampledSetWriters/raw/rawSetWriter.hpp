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
    CML::rawSetWriter

Description


\*---------------------------------------------------------------------------*/

#ifndef rawSetWriter_H
#define rawSetWriter_H

#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class rawSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class rawSetWriter
:
    public writer<Type>
{

public:

    //- Runtime type information
    TypeName("raw");


    // Constructors

        //- Construct null
        rawSetWriter();


    //- Destructor
    virtual ~rawSetWriter();


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
CML::rawSetWriter<Type>::rawSetWriter()
:
    writer<Type>()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::rawSetWriter<Type>::~rawSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::fileName CML::rawSetWriter<Type>::getFileName
(
    const coordSet& points,
    const wordList& valueSetNames
) const
{
    return this->getBaseName(points, valueSetNames) + ".xy";
}


template<class Type>
void CML::rawSetWriter<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<const Field<Type>*>& valueSets,
    Ostream& os
) const
{
    // Collect sets into columns
    List<const List<Type>*> columns(valueSets.size());

    forAll(valueSets, i)
    {
        columns[i] = valueSets[i];
    }

    this->writeTable(points, columns, os);
}


template<class Type>
void CML::rawSetWriter<Type>::write
(
    const bool writeTracks,
    const PtrList<coordSet>& points,
    const wordList& valueSetNames,
    const List<List<Field<Type> > >& valueSets,
    Ostream& os
) const
{
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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
