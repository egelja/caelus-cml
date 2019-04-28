/*---------------------------------------------------------------------------*\
Copyright (C) 2014 V. Vukcevic
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::BlockLduSystem

Description
    BlockLduSystem is a wrapper for BlockLduMatrix with source field. This is
    the return type of implicit div and grad operators needed for implicitly
    coupled systems (namely p-U coupling).

    This file was orginally obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Vuko Vukcevic, FMENA Zagreb.

\*---------------------------------------------------------------------------*/

#ifndef BlockLduSystem_HPP
#define BlockLduSystem_HPP

#include "BlockLduMatrix.hpp"
#include "scalarBlockConstraint.hpp"
#include "hyperVectorTensorFieldsFwd.hpp"
#include "ExpandHyperTensorField.hpp"
#include "hyperVectorFieldTypes.hpp"
#include "volHyperVectorFields.hpp"
#include "IOstreams.hpp"


namespace CML
{

// Forward declaration of friend functions and operators

template<class blockType, class sourceType>
class BlockLduSystem;

template<class blockType, class sourceType>
Ostream& operator<<(Ostream&, const BlockLduSystem<blockType, sourceType>&);


/*---------------------------------------------------------------------------*\
                        Class BlockLduSystem Declaration
\*---------------------------------------------------------------------------*/

template<class blockType, class sourceType>
class BlockLduSystem
:
    public BlockLduMatrix<blockType>
{

    //- Source term
    Field<sourceType> source_;

public:

    //- Construct given addressing
    explicit BlockLduSystem(const lduMesh&);

    //- Construct given addressing and source field
    BlockLduSystem(const lduMesh&, const Field<sourceType>&);

    //- Construct from components
    BlockLduSystem
    (
        const BlockLduMatrix<blockType>&,
        const Field<sourceType>&
    );

    //- Construct as copy
    BlockLduSystem(const BlockLduSystem<blockType, sourceType>&);

    virtual ~BlockLduSystem()
    {}

    // Member functions
    Field<sourceType>& source()
    {
        return source_;
    }

    const Field<sourceType>& source() const
    {
        return source_;
    }

    // Member operators
    void operator=(const BlockLduSystem<blockType, sourceType>&);

    void negate();

    void operator+=(const BlockLduSystem<blockType, sourceType>&);
    void operator-=(const BlockLduSystem<blockType, sourceType>&);

    void operator*=(const scalarField&);
    void operator*=(const scalar);

    // Ostream operator
    friend Ostream& operator<< <blockType, sourceType>
    (
        Ostream&,
        const BlockLduSystem<blockType, sourceType>&
    );
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class blockType, class sourceType>
BlockLduSystem<blockType, sourceType>::BlockLduSystem
(
    const lduMesh& ldu
)
:
    BlockLduMatrix<blockType>(ldu),
    source_(ldu.lduAddr().size(), pTraits<sourceType>::zero)
{}


template<class blockType, class sourceType>
BlockLduSystem<blockType, sourceType>::BlockLduSystem
(
    const lduMesh& ldu,
    const Field<sourceType>& s
)
:
    BlockLduMatrix<blockType>(ldu),
    source_(s)
{
    if (ldu.lduAddr().size() != s.size())
    {
        FatalErrorInFunction
            << "Sizes of ldu addressing and source field are not the same."
            << abort(FatalError);
    }
}


template<class blockType, class sourceType>
BlockLduSystem<blockType, sourceType>::BlockLduSystem
(
    const BlockLduMatrix<blockType>& bm,
    const Field<sourceType>& s
)
:
    BlockLduMatrix<blockType>(bm),
    source_(s)
{
    if (this->lduAddr().size() != s.size())
    {
        FatalErrorInFunction
            << "Sizes of block matrix and source field are not the same."
            << abort(FatalError);
    }
}


template<class blockType, class sourceType>
BlockLduSystem<blockType, sourceType>::BlockLduSystem
(
    const BlockLduSystem<blockType, sourceType>& bs
)
:
    BlockLduMatrix<blockType>(bs),
    source_(bs.source())
{}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class blockType, class sourceType>
Ostream& operator<<
(
    Ostream& os,
    const BlockLduSystem<blockType, sourceType>& bs
)
{
    os  << static_cast<const BlockLduMatrix<blockType>&>(bs) << nl
        << bs.source() << endl;

    return os;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::negate()
{
    BlockLduMatrix<blockType>::negate();
    source_.negate();
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::operator=
(
    const BlockLduSystem<blockType, sourceType>& bs
)
{
    if (this == &bs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    BlockLduMatrix<blockType>::operator=(bs);
    source_ = bs.source();
}


template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::operator+=
(
    const BlockLduSystem<blockType, sourceType>& bs
)
{
    BlockLduMatrix<blockType>::operator+=(bs);
    source_ += bs.source();
}


template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::operator-=
(
    const BlockLduSystem<blockType, sourceType>& bs
)
{
    BlockLduMatrix<blockType>::operator-=(bs);
    source_ -= bs.source();
}


template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::operator*=
(
    const scalarField& sf
)
{
    BlockLduMatrix<blockType>::operator*=(sf);
    source_ *= sf;
}


template<class blockType, class sourceType>
void BlockLduSystem<blockType, sourceType>::operator*=
(
    const scalar s
)
{
    BlockLduMatrix<blockType>::operator*=(s);
    source_ *= s;
}


} // End namespace CML
#endif
