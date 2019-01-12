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
    CML::interpolationLookUpTable

Description
    A list of lists. Interpolates based on the first dimension.
    The values must be positive and monotonically increasing in each dimension

Note
    - Accessing an empty list results in an error.
    - Accessing a list with a single element always returns the same value.


\*---------------------------------------------------------------------------*/

#ifndef interpolationLookUpTable_H
#define interpolationLookUpTable_H

#include "List.hpp"
#include "ListOps.hpp"
#include "scalarField.hpp"
#include "HashTable.hpp"
#include "IOdictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                  Class interpolationLookUpTable Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationLookUpTable
:
    public List<scalarField>
{
private:

    // Private data

        //- File name
        fileName fileName_;

        //- Table dimensions
        List<label> dim_;

        //- Min on each dimension
        List<scalar> min_;

        //- Deltas on each dimension
        List<scalar> delta_;

        //- Maximum on each dimension
        List<scalar> max_;

        //- Dictionary entries
        List<dictionary> entries_;

        //- Output dictionaries
        List<dictionary> output_;

        //- Input indices from the lookup table
        List<label> entryIndices_;

        //- Output indices from the lookup Table
        List<label> outputIndices_;

        //- Field names and indices
        HashTable<label> fieldIndices_;

        //- Output list containing input and interpolation values of outputs
        List<scalar> interpOutput_;


    // Private Member Functions

        //- Read the table of data from file
        void readTable(const word& instance, const objectRegistry&);

        //- Dimension table from dictionaries input and output
        void dimensionTable();

        //- Find table index by scalarList
        label index(const List<scalar>&, const bool lastDim=true) const;

        //- Find table index by scalar
        label index(const scalar) const;

        //- Check range of lookup value
        bool checkRange(const scalar, const label) const;

        //- Interpolate function returning a scalar
        scalar interpolate
        (
            const label lo,
            const label hi,
            const scalar lookUpValue,
            const label ofield,
            const label interfield
        ) const;

        // Check list is monotonically increasing
        void check() const;

        // find hi index, interpolate and populate interpOutput_
        void findHi(const label lo, const scalar retvals);


public:

    // Constructors

        //- Construct null
        interpolationLookUpTable();

        //- Construct given the name of the file containing the table of data
        interpolationLookUpTable
        (
            const fileName&,
            const word& instance,
            const objectRegistry&
        );

         //- Construct from dictionary
        interpolationLookUpTable(const dictionary&);

        //- Construct copy
        interpolationLookUpTable(const interpolationLookUpTable&);


    // Member Functions

        //- Return true if the field exists in the table
        bool found(const word& fieldName) const;

        //- Return the output list given a single input scalar
        const List<scalar>& lookUp(const scalar);

        //- Write lookup table to filename.
        void write
        (
            Ostream&,
            const fileName&,
            const word& instance,
            const objectRegistry&
        ) const;


    // Access

        //- Return the index of a field by name
        inline label findFieldIndex(const word& fieldName) const;

        //- Return const access to the output dictionaries
        inline const List<dictionary>& output() const;

        //- Return const access tp the dictionary entries
        inline const List<dictionary>& entries() const;

        //- Return const access to the list of min dimensions
        inline const List<scalar>& min() const;

        //- Return const access to the list of dimensions
        inline const List<label>& dim() const;

        //- Return const access to the deltas in each dimension
        inline const List<scalar>& delta() const;

        //- Return const access to the list of max dimensions
        inline const List<scalar>& max() const;

        //- Return const access to the table name
        inline word tableName() const;


     // Member Operators

        //- Return an element of constant List<scalar, Type>
        const scalarField& operator[](const label) const;

        //- Return an element of List<scalar, Type>
        scalarField& operator[](const label);

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
inline CML::label
CML::interpolationLookUpTable<Type>::findFieldIndex
(
    const word& fieldName
) const
{
    return fieldIndices_[fieldName];
}


template<class Type>
inline const CML::List<CML::dictionary>&
CML::interpolationLookUpTable<Type>::output() const
{
    return output_;
}


template<class Type>
inline const CML::List<CML::dictionary>&
CML::interpolationLookUpTable<Type>::entries() const
{
    return entries_;
}


template<class Type>
inline const CML::List<CML::scalar>&
CML::interpolationLookUpTable<Type>::min() const
{
    return min_;
}


template<class Type>
inline const CML::List<CML::label>&
CML::interpolationLookUpTable<Type>::dim() const
{
    return dim_;
}


template<class Type>
inline const CML::List<CML::scalar>&
CML::interpolationLookUpTable<Type>::delta() const
{
    return delta_;
}


template<class Type>
inline const CML::List<CML::scalar>&
CML::interpolationLookUpTable<Type>::max() const
{
    return max_;
}


template<class Type>
inline CML::word CML::interpolationLookUpTable<Type>::tableName() const
{
    return fileName_.name();
}



// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template <class Type>
CML::label CML::interpolationLookUpTable<Type>::index
(
    const List<scalar>& indices,
    const bool lastDim
) const
{
    label totalIndex = 0;

    forAll(dim_, i)
    {
        label dim = 1;
        for (int j = i + 1; j < dim_.size(); j++)
        {
            dim *= dim_[j] + 1;
        }

        totalIndex +=
            dim
           *min
            (
                max(label((indices[i] - min_[i])/delta_[i]), 0),
                dim_[i]
            );
    }

    if (lastDim)
    {
        label iLastdim = dim_.size() - 1;
        totalIndex += CML::min
        (
            max
            (
                label((indices[iLastdim] - min_[iLastdim])/delta_[iLastdim]),
                0
            ),
            dim_[iLastdim]
        );
    }

    return totalIndex;
}


template <class Type>
CML::label CML::interpolationLookUpTable<Type>::index
(
    const scalar indice
) const
{
    label i = 0;
    label totalIndex =
        CML::min
        (
            CML::max
            (
                label((indice - min_[i])/delta_[i]),
                0
            ),
            dim_[i]
        );

    return totalIndex;
}


template<class Type>
bool CML::interpolationLookUpTable<Type>::checkRange
(
    const scalar lookUpValue,
    const label interfield
) const
{
    return lookUpValue >= min_[interfield] && lookUpValue <= max_[interfield];
}


template<class Type>
CML::scalar CML::interpolationLookUpTable<Type>::interpolate
(
    const label lo,
    const label hi,
    const scalar lookUpValue,
    const label ofield,
    const label interfield
) const
{
        if
        (
            List<scalarField>::operator[](interfield).operator[](hi)
         != List<scalarField>::operator[](interfield).operator[](lo)
        )
        {
            scalar output
            (
                List<scalarField>::operator[](ofield).operator[](lo)
              + (
                    List<scalarField>::operator[](ofield).operator[](hi)
                  - List<scalarField>::operator[](ofield).operator[](lo)
                )
               *(
                    lookUpValue
                  - List<scalarField>::operator[](interfield).operator[](lo)
                )
               /(
                    List<scalarField>::operator[](interfield).operator[](hi)
                  - List<scalarField>::operator[](interfield).operator[](lo)
                )
            );
            return output;
        }
        else
        {
            return List<scalarField>::operator[](ofield).operator[](lo);
        }
}


template<class Type>
void CML::interpolationLookUpTable<Type>::dimensionTable()
{
    min_.setSize(entries_.size());
    dim_.setSize(entries_.size());
    delta_.setSize(entries_.size());
    max_.setSize(entries_.size());
    entryIndices_.setSize(entries_.size());
    outputIndices_.setSize(output_.size());
    label index = 0;
    label tableDim = 1;

    forAll(entries_,i)
    {
        dim_[i] = readLabel(entries_[i].lookup("N"));
        max_[i] = readScalar(entries_[i].lookup("max"));
        min_[i] = readScalar(entries_[i].lookup("min"));
        delta_[i] = (max_[i] - min_[i])/dim_[i];
        tableDim *= dim_[i] + 1;
        fieldIndices_.insert(entries_[i].lookup("name"), index);
        entryIndices_[i] = index;
        index++;
    }

    forAll(output_,i)
    {
        fieldIndices_.insert(output_[i].lookup("name"), index);
        outputIndices_[i] = index;
        index++;
    }

    List<scalarField>& internal = *this;

    internal.setSize(entries_.size() + output_.size());

    interpOutput_.setSize(entries_.size() + output_.size());

    forAll(internal, i)
    {
        internal[i].setSize(tableDim);
    }
}


template<class Type>
void CML::interpolationLookUpTable<Type>::readTable
(
    const word& instance,
    const objectRegistry& obr
)
{
    IOdictionary control
    (
        IOobject
        (
            fileName_,
            instance,
            obr,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );

    control.lookup("fields") >> entries_;
    control.lookup("output") >> output_;
    control.lookup("values") >> *this;

    dimensionTable();

    check();

    if (this->size() == 0)
    {
        FatalErrorInFunction
            << "table is empty" << nl << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::interpolationLookUpTable<Type>::interpolationLookUpTable()
:
    List<scalarField>(),
    fileName_("fileNameIsUndefined")
{}


template<class Type>
CML::interpolationLookUpTable<Type>::interpolationLookUpTable
(
    const fileName& fn,
    const word& instance,
    const objectRegistry& obr
)
:
    List<scalarField>(),
    fileName_(fn),
    dim_(0),
    min_(0),
    delta_(0.0),
    max_(0.0),
    entries_(0),
    output_(0),
    entryIndices_(0),
    outputIndices_(0),
    interpOutput_(0)
{
    readTable(instance, obr);
}


template<class Type>
CML::interpolationLookUpTable<Type>::interpolationLookUpTable
(
     const interpolationLookUpTable& interpTable
)
:
    List<scalarField>(interpTable),
    fileName_(interpTable.fileName_),
    entryIndices_(interpTable.entryIndices_),
    outputIndices_(interpTable.outputIndices_),
    dim_(interpTable.dim_),
    min_(interpTable.min_),
    delta_(interpTable.delta_),
    max_(interpTable.max_),
    entries_(0),
    output_(0),
    interpOutput_(interpTable.interpOutput_)
{}


template<class Type>
CML::interpolationLookUpTable<Type>::interpolationLookUpTable
(
    const dictionary& dict
)
:
    List<scalarField>(),
    fileName_(fileName(dict.lookup("fileName")).expand()),
    dim_(0),
    min_(0.0),
    delta_(0.0),
    max_(0.0),
    entries_(dict.lookup("fields")),
    output_(dict.lookup("output")),
    entryIndices_(0),
    outputIndices_(0),
    fieldIndices_(0),
    interpOutput_(0)
{
    dimensionTable();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::interpolationLookUpTable<Type>::check() const
{
    // check order in the first dimension.
    scalar prevValue = List<scalarField>::operator[](0).operator[](0);
    label dim = 1;
    for (int j = 1; j < dim_.size(); j++)
    {
        dim *= dim_[j] + 1;
    }

    for (label i = 1; i < dim_[0]; i++)
    {
        label index = i*dim;
        const scalar currValue =
            List<scalarField>::operator[](0).operator[](index);

        // avoid duplicate values (divide-by-zero error)
        if (currValue <= prevValue)
        {
            FatalErrorInFunction
                << "out-of-order value: " << currValue
                << " at index " << index << nl << exit(FatalError);
        }
        prevValue = currValue;
    }
}


template<class Type>
void CML::interpolationLookUpTable<Type>::write
(
    Ostream& os,
    const fileName& fn,
    const word& instance,
    const objectRegistry& obr
) const
{
    IOdictionary control
    (
        IOobject
        (
            fn,
            instance,
            obr,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    );

    control.writeHeader(os);

    os.writeKeyword("fields")
        << entries_ << token::END_STATEMENT << nl;

    os.writeKeyword("output")
        << output_ << token::END_STATEMENT << nl;

    if (this->size() == 0)
    {
        FatalErrorInFunction
            << "table is empty" << nl << exit(FatalError);
    }
    os.writeKeyword("values")
        << *this << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
CML::scalarField&
CML::interpolationLookUpTable<Type>::operator[](const label i)
{
    const label n = this->size();

    if (n <= 1)
    {
        FatalErrorInFunction
            << "table has (" << n << ") columns" << nl << exit(FatalError);
    }
    else if (i < 0)
    {
        FatalErrorInFunction
            << "index (" << i << ") underflow" << nl << exit(FatalError);
    }
    else if (i >= n)
    {
        FatalErrorInFunction
            << "index (" << i << ") overflow" << nl << exit(FatalError);
    }

    return List<scalarField>::operator[](i);
}


template<class Type>
const CML::scalarField&
CML::interpolationLookUpTable<Type>::operator[](const label i) const
{
    const label n = this->size();

    if (n <= 1)
    {
        FatalErrorInFunction
            << "table has (" << n << ") columns" << nl << exit(FatalError);
    }
    else if (i < 0)
    {
        FatalErrorInFunction
            << "index (" << i << ") underflow" << nl << exit(FatalError);
    }
    else if (i >= n)
    {
        FatalErrorInFunction
            << "index (" << i << ") overflow" << nl
            << exit(FatalError);
    }

    return List<scalarField>::operator[](i);
}


template<class Type>
bool CML::interpolationLookUpTable<Type>::found(const word& fieldName) const
{
    return fieldIndices_.found(fieldName);
}


template<class Type>
const CML::scalarList&
CML::interpolationLookUpTable<Type>::lookUp(const scalar retvals)
{
    const label lo = index(retvals);
    findHi(lo, retvals);
    return interpOutput_;
}


template<class Type>
void CML::interpolationLookUpTable<Type>::findHi
(
    const label lo,
    const scalar retvals
)
{
    forAll(outputIndices_,j)
    {
        scalar tmp = 0;
        label ofield = outputIndices_[j];
        scalar baseValue = List<scalarField>::operator[](ofield).operator[](lo);

        forAll(entryIndices_,i)
        {
            if (checkRange(retvals, entryIndices_[i]))
            {
                label dim = 1;

                label hi = CML::min(lo + dim, (*this)[0].size() - 1);

                tmp += interpolate(lo, hi, retvals, ofield, entryIndices_[i])
                     - baseValue;
            }
            interpOutput_[entryIndices_[i]] = retvals;
        }

        tmp += baseValue;
        interpOutput_[outputIndices_[j]] = tmp;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
