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
    CML::gnuplotSetWriter

Description


\*---------------------------------------------------------------------------*/

#ifndef gnuplotSetWriter_H
#define gnuplotSetWriter_H

#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class gnuplotSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class gnuplotSetWriter
:
    public writer<Type>
{

public:

    //- Runtime type information
    TypeName("gnuplot");


    // Constructors

        //- Construct null
        gnuplotSetWriter();


    //- Destructor
    virtual ~gnuplotSetWriter();


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
#include "addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::gnuplotSetWriter<Type>::gnuplotSetWriter()
:
    writer<Type>()
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::gnuplotSetWriter<Type>::~gnuplotSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::fileName CML::gnuplotSetWriter<Type>::getFileName
(
    const coordSet& points,
    const wordList& valueSetNames
) const
{
    return this->getBaseName(points, valueSetNames) + ".gplt";
}


template<class Type>
void CML::gnuplotSetWriter<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<const Field<Type>*>& valueSets,
    Ostream& os
) const
{
    os  << "set term postscript color" << nl
        << "set output \"" << points.name() << ".ps\"" << nl
        << "plot";

    forAll(valueSets, i)
    {
        if (i != 0)
        {
            os << ',';
        }

        os  << " \"-\" title \"" << valueSetNames[i] << "\" with lines";
    }
    os  << nl;


    forAll(valueSets, i)
    {
        this->writeTable(points, *valueSets[i], os);
        os  << "e" << nl;
    }
}


template<class Type>
void CML::gnuplotSetWriter<Type>::write
(
    const bool writeTracks,
    const PtrList<coordSet>& trackPoints,
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
    if (trackPoints.size() > 0)
    {
        os  << "set term postscript color" << nl
            << "set output \"" << trackPoints[0].name() << ".ps\"" << nl;

        forAll(trackPoints, trackI)
        {
            os  << "plot";

            forAll(valueSets, i)
            {
                if (i != 0)
                {
                    os << ',';
                }

                os  << " \"-\" title \"" << valueSetNames[i] << "\" with lines";
            }
            os << nl;

            forAll(valueSets, i)
            {
                this->writeTable(trackPoints[trackI], valueSets[i][trackI], os);
                os  << "e" << nl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
