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
    CML::xmgraceSetWriter

Description


\*---------------------------------------------------------------------------*/

#ifndef xmgraceSetWriter_H
#define xmgraceSetWriter_H

#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class xmgraceSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class xmgraceSetWriter
:
    public writer<Type>
{

public:

    //- Runtime type information
    TypeName("xmgr");


    // Constructors

        //- Construct null
        xmgraceSetWriter();


    //- Destructor
    virtual ~xmgraceSetWriter();


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
CML::xmgraceSetWriter<Type>::xmgraceSetWriter()
:
    writer<Type>()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::xmgraceSetWriter<Type>::~xmgraceSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::fileName CML::xmgraceSetWriter<Type>::getFileName
(
    const coordSet& points,
    const wordList& valueSetNames
) const
{
    return this->getBaseName(points, valueSetNames) + ".agr";
}


template<class Type>
void CML::xmgraceSetWriter<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<const Field<Type>*>& valueSets,
    Ostream& os
) const
{
    os  << "@g0 on" << nl
        << "@with g0" << nl
        << "@    title \"" << points.name() << '"' << nl
        << "@    xaxis label " << '"' << points.axis() << '"' << nl;

    forAll(valueSets, i)
    {
        os  << "@    s" << i << " legend " << '"'
            << valueSetNames[i] << '"' << nl
            << "@target G0.S" << i << nl;

        this->writeTable(points, *valueSets[i], os);

        os  << '&' << nl;
    }
}


template<class Type>
void CML::xmgraceSetWriter<Type>::write
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
        os  << "@g0 on" << nl
            << "@with g0" << nl
            << "@    title \"" << trackPoints[0].name() << '"' << nl
            << "@    xaxis label " << '"' << trackPoints[0].axis() << '"' << nl;

        // Data index.
        label sI = 0;

        forAll(trackPoints, trackI)
        {
            forAll(valueSets, i)
            {
                os  << "@    s" << sI << " legend " << '"'
                    << valueSetNames[i] << "_track" << i << '"' << nl
                    << "@target G0.S" << sI << nl;
                this->writeTable(trackPoints[trackI], valueSets[i][trackI], os);
                os  << '&' << nl;

                sI++;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
