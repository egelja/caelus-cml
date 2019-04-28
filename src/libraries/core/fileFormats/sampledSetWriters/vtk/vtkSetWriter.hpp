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
    CML::vtkSetWriter

Description


\*---------------------------------------------------------------------------*/

#ifndef vtkSetWriter_H
#define vtkSetWriter_H

#include "writer.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class vtkSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class vtkSetWriter
:
    public writer<Type>
{

public:

    //- Runtime type information
    TypeName("vtk");


    // Constructors

        //- Construct null
        vtkSetWriter();


    //- Destructor
    virtual ~vtkSetWriter();


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
CML::vtkSetWriter<Type>::vtkSetWriter()
:
    writer<Type>()
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::vtkSetWriter<Type>::~vtkSetWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::fileName CML::vtkSetWriter<Type>::getFileName
(
    const coordSet& points,
    const wordList& valueSetNames
) const
{
    return this->getBaseName(points, valueSetNames) + ".vtk";
}


template<class Type>
void CML::vtkSetWriter<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<const Field<Type>*>& valueSets,
    Ostream& os
) const
{
    os  << "# vtk DataFile Version 2.0" << nl
        << points.name() << nl
        << "ASCII" << nl
        << "DATASET POLYDATA" << nl
        << "POINTS " << points.size() << " float" << nl;

    forAll(points, i)
    {
        const vector& pt = points[i];
        os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }


    os  << "POINT_DATA " << points.size() << nl
        << " FIELD attributes " << valueSetNames.size() << nl;

    forAll(valueSetNames, setI)
    {
        os  << valueSetNames[setI] << ' ' << pTraits<Type>::nComponents << ' '
            << points.size() << " float" << nl;

        const Field<Type>& fld = *valueSets[setI];

        forAll(fld, pointI)
        {
            if (pointI != 0)
            {
                os  << ' ';
            }
            writer<Type>::write(fld[pointI], os);
        }
        os  << nl;
    }
}


template<class Type>
void CML::vtkSetWriter<Type>::write
(
    const bool writeTracks,
    const PtrList<coordSet>& tracks,
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

    label nTracks = tracks.size();
    label nPoints = 0;
    forAll(tracks, i)
    {
        nPoints += tracks[i].size();
    }

    os  << "# vtk DataFile Version 2.0" << nl
        << tracks[0].name() << nl
        << "ASCII" << nl
        << "DATASET POLYDATA" << nl
        << "POINTS " << nPoints << " float" << nl;

    forAll(tracks, trackI)
    {
        const coordSet& points = tracks[trackI];
        forAll(points, i)
        {
            const vector& pt = points[i];
            os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
        }
    }

    if (writeTracks)
    {
        os  << "LINES " << nTracks << ' ' << nPoints+nTracks << nl;

        // Write ids of track points to file
        label globalPtI = 0;
        forAll(tracks, trackI)
        {
            const coordSet& points = tracks[trackI];

            os  << points.size();
            forAll(points, i)
            {
                os  << ' ' << globalPtI;
                globalPtI++;
            }
            os << nl;
        }
    }

    os  << "POINT_DATA " << nPoints << nl
        << " FIELD attributes " << valueSetNames.size() << nl;

    forAll(valueSetNames, setI)
    {
        os  << valueSetNames[setI] << ' ' << pTraits<Type>::nComponents << ' '
            << nPoints << " float" << nl;

        const List<Field<Type> >& fieldVals = valueSets[setI];

        forAll(fieldVals, i)
        {
            const Field<Type>& vals = fieldVals[i];

            forAll(vals, j)
            {
                if (j != 0)
                {
                    os  << ' ';
                }
                writer<Type>::write(vals[j], os);
            }
            os  << nl;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
