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
    CML::writer

Description
    Base class for graphics format writing. Entry points are
    - write(..). \n
      Write to an Ostream a table of points with corresponding values.
    - write(scalar/vector/sphericalTensor/symmTensor/tensor). \n
      Write single scalar/vector/sphericalTensor/symmTensor/tensor.
      Default is to write space separated components.

    Example:
    \verbatim
        // Construct writer of xmgr type
        autoPtr<writer<scalar> > scalarFormatter(writer<scalar>::New("xmgr"));

        // Output list of points and corresponding values
        scalarFormatter().write
        (
            coordSet(...)
            "U.component(0)",   // name of values
            vals                // values
        );
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef writer_H
#define writer_H

#include "fileName.hpp"
#include "wordList.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "autoPtr.hpp"
#include "Field.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class coordSet;

/*---------------------------------------------------------------------------*\
                           Class writer Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class writer
{

protected:

    //- Generates filename from coordSet and sampled fields
    fileName getBaseName(const coordSet&, const wordList&) const;

    void writeCoord(const coordSet&, const label sampleI, Ostream&) const;

    //- Writes single-column ascii write. Column 1 is coordSet coordinate,
    //  columns 2 is the value. Uses write() function
    //  to write coordinate in correct format.
    void writeTable(const coordSet&, const List<Type>&, Ostream&) const;

    //- Writes multi-column ascii write. Column 1 is coordSet coordinate,
    //  columns 2..n are the values. Uses write() function
    //  to write coordinate in correct format.
    void writeTable
    (
        const coordSet&,
        const List<const List<Type>*>&,
        Ostream& os
    ) const;

    //- Writes a separator. Used by write functions.
    virtual void writeSeparator(Ostream& os) const;


public:

    //- Runtime type information
    TypeName("writer");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            writer,
            word,
            (),
            ()
        );


    // Selectors

        //- Return a reference to the selected writer
        static autoPtr<writer> New(const word& writeFormat);


    // Constructors

        //- Construct null
        writer();


    //- Destructor
    virtual ~writer() = 0;


    // Member Functions

        //- Generate file name with correct extension
        virtual fileName getFileName
        (
            const coordSet&,
            const wordList&
        ) const = 0;

        //- General entry point for writing.
        //  The data is organized in a set of point with one or more values
        //  per point
        virtual void write
        (
            const coordSet&,
            const wordList&,
            const List<const Field<Type>*>&,
            Ostream&
        ) const = 0;

        //- General entry point for writing.
        //  The data is organized in a set of point with one or more values
        //  per point
        virtual void write
        (
            const coordSet&,
            const wordList&,
            const List<Field<Type> >&,
            Ostream&
        ) const;

        //- General entry point for writing of multiple coordSets.
        //  Each coordSet (track) has same data variables.
        //  The data is per variable, per track, per point of track.
        //  If writeTracks adds connecting lines (wherever applicable)
        virtual void write
        (
            const bool writeTracks,
            const PtrList<coordSet>&,
            const wordList& valueSetNames,
            const List<List<Field<Type> > >&,
            Ostream&
        ) const = 0;

        //- Write scalar as ascii
        virtual Ostream& write(const scalar, Ostream&) const;

        template<class VSType>
        Ostream& writeVS(const VSType&, Ostream&) const;

        //- Write vector. Tab separated ascii
        virtual Ostream& write(const vector&, Ostream&) const;

        //- Write sphericalTensor. Tab separated ascii
        virtual Ostream& write(const sphericalTensor&, Ostream&) const;

        //- Write symmTensor. Tab separated ascii
        virtual Ostream& write(const symmTensor&, Ostream&) const;

        //- Write tensor. Tab separated ascii
        virtual Ostream& write(const tensor&, Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "coordSet.hpp"
#include "OFstream.hpp"
#include "OSspecific.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr< CML::writer<Type> > CML::writer<Type>::New
(
    const word& writeType
)
{
    typename wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(writeType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown write type "
            << writeType << nl << nl
            << "Valid write types : " << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<writer<Type> >(cstrIter()());
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
CML::fileName CML::writer<Type>::getBaseName
(
    const coordSet& points,
    const wordList& valueSets
) const
{
    fileName fName(points.name());

    forAll(valueSets, i)
    {
        fName += '_' + valueSets[i];
    }

    return fName;
}


template<class Type>
void CML::writer<Type>::writeCoord
(
    const coordSet& points,
    const label pointI,
    Ostream& os
) const
{
    if (points.hasVectorAxis())
    {
        write(points.vectorCoord(pointI), os);
    }
    else
    {
        write(points.scalarCoord(pointI), os);
    }
}


template<class Type>
void CML::writer<Type>::writeTable
(
    const coordSet& points,
    const List<Type>& values,
    Ostream& os
) const
{
    forAll(points, pointI)
    {
        writeCoord(points, pointI, os);
        writeSeparator(os);
        write(values[pointI], os);
        os << nl;
    }
}


template<class Type>
void CML::writer<Type>::writeTable
(
    const coordSet& points,
    const List<const List<Type>*>& valuesPtrList,
    Ostream& os
) const
{
    forAll(points, pointI)
    {
        writeCoord(points, pointI, os);

        forAll(valuesPtrList, i)
        {
            writeSeparator(os);

            const List<Type>& values = *valuesPtrList[i];
            write(values[pointI], os);
        }
        os << nl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::writer<Type>::writer()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::writer<Type>::~writer()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::writer<Type>::write
(
    const coordSet& points,
    const wordList& valueSetNames,
    const List<Field<Type> >& valueSets,
    Ostream& os
) const
{
    List<const Field<Type>*> valueSetPtrs(valueSets.size());
    forAll(valueSetPtrs, i)
    {
        valueSetPtrs[i] = &valueSets[i];
    }
    write(points, valueSetNames, valueSetPtrs, os);
}


template<class Type>
CML::Ostream& CML::writer<Type>::write
(
    const scalar value,
    Ostream& os
) const
{
    return os << value;
}


template<class Type>
template<class VSType>
CML::Ostream& CML::writer<Type>::writeVS
(
    const VSType& value,
    Ostream& os
) const
{
    for (direction d=0; d<VSType::nComponents; d++)
    {
        if (d > 0)
        {
            writeSeparator(os);
        }

        os << value.component(d);
    }
    return os;
}


template<class Type>
void CML::writer<Type>::writeSeparator
(
    Ostream& os
) const
{
    os << token::SPACE << token::TAB;
}


template<class Type>
CML::Ostream& CML::writer<Type>::write
(
    const vector& value,
    Ostream& os
) const
{
    return writeVS(value, os);
}


template<class Type>
CML::Ostream& CML::writer<Type>::write
(
    const sphericalTensor& value,
    Ostream& os
) const
{
    return writeVS(value, os);
}


template<class Type>
CML::Ostream& CML::writer<Type>::write
(
    const symmTensor& value,
    Ostream& os
) const
{
    return writeVS(value, os);
}


template<class Type>
CML::Ostream& CML::writer<Type>::write
(
    const tensor& value,
    Ostream& os
) const
{
    return writeVS(value, os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
