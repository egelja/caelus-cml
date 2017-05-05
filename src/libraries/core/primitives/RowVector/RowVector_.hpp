/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::RowVector

Description
    Templated 3D row-vector derived from MatrixSpace adding construction from
    3 components and element access using x(), y() and z().

SourceFiles
    RowVectorI.hpp

\*---------------------------------------------------------------------------*/

#ifndef RowVector_HPP
#define RowVector_HPP

#include "MatrixSpace_.hpp"
#include "Vector_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class RowVector Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt>
class RowVector
:
    public MatrixSpace<RowVector<Cmpt>, Cmpt, 1, 3>
{

public:

    //- Equivalent type of labels used for valid component indexing
    typedef RowVector<label> labelType;


    //- Component labeling enumeration
    enum components { X, Y, Z };


    // Constructors

        //- Construct null
        inline RowVector();

        //- Construct initialized to zero
        inline RowVector(const CML::zero);

        //- Construct given VectorSpace of the same rank
        template<class Cmpt2>
        inline RowVector(const MatrixSpace<RowVector<Cmpt2>, Cmpt2, 1, 3>&);

        //- Construct given three components
        inline RowVector(const Cmpt& rvx, const Cmpt& rvy, const Cmpt& rvz);

        //- Construct from Istream
        inline RowVector(Istream&);


    // Member Functions

        // Access

            inline const Cmpt& x() const;
            inline const Cmpt& y() const;
            inline const Cmpt& z() const;

            inline Cmpt& x();
            inline Cmpt& y();
            inline Cmpt& z();
};


template<class Cmpt>
class typeOfTranspose<Cmpt, Vector<Cmpt>>
{
public:

    typedef RowVector<Cmpt> type;
};


template<class Cmpt>
class typeOfTranspose<Cmpt, RowVector<Cmpt>>
{
public:

    typedef Vector<Cmpt> type;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "RowVectorI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
