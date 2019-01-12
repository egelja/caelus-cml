/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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
    CML::circulator

Description
    Walks over a container as if it were circular. The container must have the
    following members defined:
        - value_type
        - size_type
        - difference_type
        - iterator
        - reference

    Examples

    \code
        face f(identity(5));

        // Construct circulator from the face
        circulator<face> circ(f);

        // First check that the circulator has a size to iterate over.
        // Then circulate around the list starting and finishing at the fulcrum.
        if (circ.size()) do
        {
            circ() += 1;

            Info<< "Iterate forwards over face : " << circ() << endl;

        } while (circ.circulate(CirculatorBase::CLOCKWISE));
    \endcode

SourceFiles
    circulatorI.H

\*---------------------------------------------------------------------------*/

#ifndef circulator_H
#define circulator_H

#include "CirculatorBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


/*---------------------------------------------------------------------------*\
                         Class circulator Declaration
\*---------------------------------------------------------------------------*/

template<class ContainerType>
class circulator
:
    public CirculatorBase
{

protected:

    // Protected data

        //- Iterator pointing to the beginning of the container
        typename ContainerType::iterator begin_;

        //- Iterator pointing to the end of the container
        typename ContainerType::iterator end_;

        //- Random access iterator for traversing ContainerType.
        typename ContainerType::iterator iter_;

        //- Iterator holding the location of the fulcrum (start and end) of
        //  the container. Used to decide when the iterator should stop
        //  circulating over the container
        typename ContainerType::iterator fulcrum_;


public:

    // STL type definitions

        //- Type of values ContainerType contains.
        typedef typename ContainerType::value_type      value_type;

        //- The type that can represent the size of ContainerType
        typedef typename ContainerType::size_type       size_type;

        //- The type that can represent the difference between any two
        //  iterator objects.
        typedef typename ContainerType::difference_type difference_type;

        //- Random access iterator for traversing ContainerType.
        typedef typename ContainerType::iterator        iterator;

        //- Type that can be used for storing into
        //  ContainerType::value_type objects.
        typedef typename ContainerType::reference       reference;


    // Constructors

        //- Construct null
        inline circulator();

        //- Construct from a container.
        inline explicit circulator(ContainerType& container);

        //- Construct from two iterators
        inline circulator(const iterator& begin, const iterator& end);

        //- Construct as copy
        inline circulator(const circulator<ContainerType>&);


    //- Destructor
    ~circulator();


    // Member Functions

        //- Return the range of the iterator
        inline size_type size() const;

        //- Circulate around the list in the given direction
        inline bool circulate(const CirculatorBase::direction dir = NONE);

        //- Set the fulcrum to the current position of the iterator
        inline void setFulcrumToIterator();

        //- Set the iterator to the current position of the fulcrum
        inline void setIteratorToFulcrum();

        //- Return the distance between the iterator and the fulcrum. This is
        //  equivalent to the number of rotations of the circulator.
        inline difference_type nRotations() const;

        //- Dereference the next iterator and return
        inline reference next() const;

        //- Dereference the previous iterator and return
        inline reference prev() const;


    // Member Operators

        //- Assignment operator for circulators that operate on the same
        //  container type
        inline void operator=(const circulator<ContainerType>&);

        //- Prefix increment. Increments the iterator.
        //  Sets the iterator to the beginning of the container if it reaches
        //  the end
        inline circulator<ContainerType>& operator++();

        //- Postfix increment. Increments the iterator.
        //  Sets the iterator to the beginning of the container if it reaches
        //  the end
        inline circulator<ContainerType> operator++(int);

        //- Prefix decrement. Decrements the iterator.
        //  Sets the iterator to the end of the container if it reaches
        //  the beginning
        inline circulator<ContainerType>& operator--();

        //- Postfix decrement. Decrements the iterator.
        //  Sets the iterator to the end of the container if it reaches
        //  the beginning
        inline circulator<ContainerType> operator--(int);

        //- Check for equality of this iterator with another iterator that
        //  operate on the same container type
        inline bool operator==(const circulator<ContainerType>& c) const;

        //- Check for inequality of this iterator with another iterator that
        //  operate on the same container type
        inline bool operator!=(const circulator<ContainerType>& c) const;

        //- Dereference the iterator and return
        inline reference operator*() const;

        //- Dereference the iterator and return
        inline reference operator()() const;

        //- Return the difference between this iterator and another iterator
        //  that operate on the same container type
        inline difference_type operator-
        (
            const circulator<ContainerType>& c
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ContainerType>
CML::circulator<ContainerType>::circulator()
:
    CirculatorBase(),
    begin_(0),
    end_(0),
    iter_(0),
    fulcrum_(0)
{}


template<class ContainerType>
CML::circulator<ContainerType>::circulator(ContainerType& container)
:
    CirculatorBase(),
    begin_(container.begin()),
    end_(container.end()),
    iter_(begin_),
    fulcrum_(begin_)
{}


template<class ContainerType>
CML::circulator<ContainerType>::circulator
(
    const iterator& begin,
    const iterator& end
)
:
    CirculatorBase(),
    begin_(begin),
    end_(end),
    iter_(begin),
    fulcrum_(begin)
{}


template<class ContainerType>
CML::circulator<ContainerType>::circulator
(
    const circulator<ContainerType>& rhs
)
:
    CirculatorBase(),
    begin_(rhs.begin_),
    end_(rhs.end_),
    iter_(rhs.iter_),
    fulcrum_(rhs.fulcrum_)
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

template<class ContainerType>
CML::circulator<ContainerType>::~circulator()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ContainerType>
typename CML::circulator<ContainerType>::size_type
CML::circulator<ContainerType>::size() const
{
    return end_ - begin_;
}


template<class ContainerType>
bool CML::circulator<ContainerType>::circulate
(
    const CirculatorBase::direction dir
)
{
    if (dir == CirculatorBase::CLOCKWISE)
    {
        operator++();
    }
    else if (dir == CirculatorBase::ANTICLOCKWISE)
    {
        operator--();
    }

    return !(iter_ == fulcrum_);
}


template<class ContainerType>
void CML::circulator<ContainerType>::setFulcrumToIterator()
{
    fulcrum_ = iter_;
}


template<class ContainerType>
void CML::circulator<ContainerType>::setIteratorToFulcrum()
{
    iter_ = fulcrum_;
}


template<class ContainerType>
typename CML::circulator<ContainerType>::difference_type
CML::circulator<ContainerType>::nRotations() const
{
    return (iter_ - fulcrum_);
}


template<class ContainerType>
typename CML::circulator<ContainerType>::reference
CML::circulator<ContainerType>::next() const
{
    if (iter_ == end_ - 1)
    {
        return *begin_;
    }

    return *(iter_ + 1);
}


template<class ContainerType>
typename CML::circulator<ContainerType>::reference
CML::circulator<ContainerType>::prev() const
{
    if (iter_ == begin_)
    {
        return *(end_ - 1);
    }

    return *(iter_ - 1);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class ContainerType>
void CML::circulator<ContainerType>::operator=
(
    const circulator<ContainerType>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    begin_ = rhs.begin_;
    end_ = rhs.end_;
    iter_ = rhs.iter_;
    fulcrum_ = rhs.fulcrum_;
}


template<class ContainerType>
CML::circulator<ContainerType>&
CML::circulator<ContainerType>::operator++()
{
    ++iter_;
    if (iter_ == end_)
    {
        iter_ = begin_;
    }

    return *this;
}


template<class ContainerType>
CML::circulator<ContainerType>
CML::circulator<ContainerType>::operator++(int)
{
    circulator<ContainerType> tmp = *this;
    ++(*this);
    return tmp;
}


template<class ContainerType>
CML::circulator<ContainerType>&
CML::circulator<ContainerType>::operator--()
{
    if (iter_ == begin_)
    {
        iter_ = end_;
    }
    --iter_;

    return *this;
}


template<class ContainerType>
CML::circulator<ContainerType>
CML::circulator<ContainerType>::operator--(int)
{
    circulator<ContainerType> tmp = *this;
    --(*this);
    return tmp;
}


template<class ContainerType>
bool CML::circulator<ContainerType>::operator==
(
    const circulator<ContainerType>& c
) const
{
    return
    (
        begin_ == c.begin_
     && end_ == c.end_
     && iter_ == c.iter_
     && fulcrum_ == c.fulcrum_
    );
}


template<class ContainerType>
bool CML::circulator<ContainerType>::operator!=
(
    const circulator<ContainerType>& c
) const
{
    return !(*this == c);
}


template<class ContainerType>
typename CML::circulator<ContainerType>::reference
CML::circulator<ContainerType>::operator*() const
{
    return *iter_;
}


template<class ContainerType>
typename CML::circulator<ContainerType>::reference
CML::circulator<ContainerType>::operator()() const
{
    return operator*();
}


template<class ContainerType>
typename CML::circulator<ContainerType>::difference_type
CML::circulator<ContainerType>::operator-
(
    const circulator<ContainerType>& c
) const
{
    return iter_ - c.iter_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
