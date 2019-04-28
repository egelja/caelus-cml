/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    LongList

Description
    A dynamic list is a 1-D vector of objects of type T which resizes
    itself as necessary to accept the new objects.  Internal storage
    is a 2-D graph with a fixed size of the chunks used to store the data.
    This way the data does not get copied every time array is resized, but
    only the pointers to the chunks of data.

Author: Franjo Juretic (franjo.juretic@c-fields.com)


\*---------------------------------------------------------------------------*/

#ifndef LongList_HPP
#define LongList_HPP

#include "label.hpp"
#include "bool.hpp"
#include "IOstreams.hpp"
#include "error.hpp"
#include "Ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class T, label Offset>
class LongList;

template<class T, label Offset>
Ostream& operator<<
(
    Ostream&,
    const LongList<T, Offset>&
);
template<class T, label Offset>
Istream& operator>>
(
    Istream&,
    LongList<T, Offset>&
);

/*---------------------------------------------------------------------------*\
                           Class LongList Declaration
\*---------------------------------------------------------------------------*/

template<class T, label Offset = 19>
class LongList
{
    // Private data
        //- number of allocated elements
        label N_;

        //- number of elements in the list
        label nextFree_;

        //- number of used blocks of data
        label numBlocks_;

        //- maximum number of blocks that can be allocated
        //- without reallocating the list containing pointers
        //- to the chunks of data
        label numAllocatedBlocks_;

        //- size of blocks is calculated by powers of 2
        //- and therefore the access can be done using shift and mask
        label shift_;
        label mask_;

        //- array of pointers to the blocks of data, each of the size WIDTH
        T** dataPtr_;

    // Private member functions
        //- check index
        void checkIndex(const label i) const;

        //- initialize width and mask
        void initializeParameters();

        //- Allocate memory for the list
        void allocateSize(const label);

        //- delete all elements
        void clearOut();

public:

    // Constructors

        //- Construct null
        inline LongList();

        //- Construct given size
        explicit inline LongList(const label size);

        //- Construct to given size and initialize
        explicit inline LongList(const label size, const T& t);

        //- Copy contructor
        inline LongList(const LongList<T, Offset>&);

    // Destructor

        inline ~LongList();

    // Member Functions

        // Access

            //- Size of the active part of the list.
            inline label size() const;

            //- Return the binary size in number of characters of the UList
            //  if the element is a primitive type
            //  i.e. contiguous<T>() == true
            inline label byteSize() const;

        // Edit

            //- Reset size of List.
            void setSize(const label);

            //- Clear the list, i.e. set next free to zero.
            //  Allocated size does not change
            void clear();

            //- Shrink the list to the number of elements used
            inline LongList<T, Offset>& shrink();

            //- transfer the list from another one without allocating it
            inline void transfer(LongList<T, Offset>&);


    // Member Operators

        //- Append an element at the end of the list
        inline void append(const T& e);

        //- Append an element at the end of the list if it is not yet
        //- present in the list (takes linear time)
        inline void appendIfNotIn(const T& e);

        //- check if the element is in the list (takes linear time)
        inline bool contains(const T& e) const;
        inline label containsAtPosition(const T& e) const;

        //- Return and remove the element
        inline T remove(const label i);
        inline T removeLastElement();

        //- get and set operators
        inline const T& operator[](const label i) const;
        inline T& operator[](const label i);

        //- Return non-const access to an element,
        //  resizing the list if necessary
        inline T& operator()(const label);

        //- return a non-const access to an element,
        // resize the list if necessary
        inline T& newElmt(const label);

        //- Assignment of all entries to the given value
        inline void operator=(const T&);

        //- Assignment operator
        inline void operator=(const LongList<T, Offset>&);


    // IOstream operators
        //- Read from stream and append to the current content
        void appendFromStream(Istream&);

        //- Write as a dictionary entry.
        void writeEntry(Ostream& os) const;

        //- Write as a dictionary entry with keyword.
        void writeEntry(const word& keyword, Ostream& os) const;

        // Write LongList to Ostream.
        friend Ostream& operator<< <T, Offset>
        (
            Ostream&,
            const LongList<T, Offset>&
        );

        //- Read from Istream, discarding contents of existing LongList.
        friend Istream& operator>> <T, Offset>
        (
            Istream&,
            LongList<T, Offset>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


template<class T, CML::label Offset>
void CML::LongList<T, Offset>::checkIndex(const label i) const
{
    if( (i < 0) || (i >= nextFree_) )
    {
        FatalErrorInFunction
            << "Index " << i << " is not in range " << 0
            << " and " << nextFree_ << abort(FatalError);
    }
}

template<class T, CML::label Offset>
void CML::LongList<T, Offset>::initializeParameters()
{
    unsigned int t = sizeof(T);
    label it(0);

    while( t > 1 )
    {
        t >>= 1;
        ++it;
    }

    shift_ = CML::max(10, Offset - it);
    mask_ = 1<<shift_;
    mask_ -= 1;
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::allocateSize(const label s)
{
    if( s == 0 )
    {
        clearOut();
        return;
    }
    else if( s < 0 )
    {
        FatalErrorInFunction
           << "Negative size requested." << abort(FatalError);
    }

    const label numblock1 = ((s-1)>>shift_) + 1;
    const label blockSize = 1<<shift_;

    if( numblock1 < numBlocks_ )
    {
        for(label i=numblock1;i<numBlocks_;++i)
            delete [] dataPtr_[i];
    }
    else if( numblock1 > numBlocks_ )
    {
        if( numblock1 >= numAllocatedBlocks_ )
        {
            do
            {
                numAllocatedBlocks_ += 64;
            } while( numblock1 > numAllocatedBlocks_ );

            T** dataptr1 = new T*[numAllocatedBlocks_];

            for(label i=0;i<numBlocks_;++i)
                dataptr1[i] = dataPtr_[i];

            if( dataPtr_ )
                delete [] dataPtr_;
            dataPtr_ = dataptr1;
        }

        for(label i=numBlocks_;i<numblock1;++i)
            dataPtr_[i] = new T[blockSize];
    }

    numBlocks_ = numblock1;
    N_ = numBlocks_ * blockSize;
}

template<class T, CML::label Offset>
void CML::LongList<T, Offset>::clearOut()
{
    for(label i=0;i<numBlocks_;++i)
        delete [] dataPtr_[i];

    if( dataPtr_ )
    {
        delete [] dataPtr_;
        dataPtr_ = nullptr;
    }

    N_ = 0;
    numBlocks_ = 0;
    numAllocatedBlocks_ = 0;
    nextFree_ = 0;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
template<class T, CML::label Offset>
inline CML::LongList<T, Offset>::LongList()
:
    N_(0),
    nextFree_(0),
    numBlocks_(0),
    numAllocatedBlocks_(0),
    shift_(),
    mask_(),
    dataPtr_(nullptr)
{
    initializeParameters();
}

//- Construct given size
template<class T, CML::label Offset>
inline CML::LongList<T, Offset>::LongList(const label s)
:
    N_(0),
    nextFree_(0),
    numBlocks_(0),
    numAllocatedBlocks_(0),
    shift_(),
    mask_(),
    dataPtr_(nullptr)
{
    initializeParameters();
    setSize(s);
}


//- Construct given size
template<class T, CML::label Offset>
inline CML::LongList<T, Offset>::LongList(const label s, const T& t)
:
    N_(0),
    nextFree_(0),
    numBlocks_(0),
    numAllocatedBlocks_(0),
    shift_(),
    mask_(),
    dataPtr_(nullptr)
{
    initializeParameters();
    setSize(s);
    *this = t;
}

template<class T, CML::label Offset>
inline CML::LongList<T, Offset>::LongList(const LongList<T, Offset>& ol)
:
    N_(0),
    nextFree_(0),
    numBlocks_(0),
    numAllocatedBlocks_(0),
    shift_(ol.shift_),
    mask_(ol.mask_),
    dataPtr_(nullptr)
{
    *this = ol;
}

template<class T, CML::label Offset>
inline CML::LongList<T, Offset>::~LongList()
{
    clearOut();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, CML::label Offset>
inline CML::label CML::LongList<T, Offset>::size() const
{
    return nextFree_;
}

template<class T, CML::label Offset>
inline CML::label CML::LongList<T, Offset>::byteSize() const
{
    if( !contiguous<T>() )
    {
        FatalErrorInFunction
            << "Cannot return the binary size of a list of "
               "non-primitive elements"
            << abort(FatalError);
    }

    return nextFree_*sizeof(T);
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::setSize(const label i)
{
    allocateSize(i);
    nextFree_ = i;
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::clear()
{
    nextFree_ = 0;
}


template<class T, CML::label Offset>
inline CML::LongList<T, Offset>&
CML::LongList<T, Offset>::shrink()
{
    setSize(nextFree_);
    return *this;
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::transfer(LongList<T, Offset>& ol)
{
    clearOut();
    dataPtr_ = ol.dataPtr_;
    N_ = ol.N_;
    nextFree_ = ol.nextFree_;
    numBlocks_ = ol.numBlocks_;
    numAllocatedBlocks_ = ol.numAllocatedBlocks_;
    shift_ = ol.shift_;
    mask_ = ol.mask_;

    ol.dataPtr_ = nullptr;
    ol.N_ = 0;
    ol.nextFree_ = 0;
    ol.numBlocks_ = 0;
    ol.numAllocatedBlocks_ = 0;
}


template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::append(const T& e)
{
    if( nextFree_ >= N_ )
    {
        allocateSize(nextFree_+1);
    }

    operator[](nextFree_++) = e;
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::appendIfNotIn(const T& e)
{
    if( !contains(e) )
         append(e);
}

template<class T, CML::label Offset>
inline bool CML::LongList<T, Offset>::contains(const T& e) const
{
    for(label i=0;i<nextFree_;++i)
        if( (*this)[i] == e )
            return true;

    return false;
}

template<class T, CML::label Offset>
inline CML::label CML::LongList<T, Offset>::containsAtPosition
(
    const T& e
) const
{
    for(label i=0;i<nextFree_;++i)
        if( (*this)[i] == e )
            return i;

    return -1;
}

template<class T, CML::label Offset>
inline T CML::LongList<T, Offset>::remove(const label i)
{
    if( nextFree_ == 0 )
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    T el = operator[](i);
    operator[](i) = operator[](nextFree_-1);
    --nextFree_;
    return el;
}

template<class T, CML::label Offset>
inline T CML::LongList<T, Offset>::removeLastElement()
{
    if( nextFree_ == 0 )
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    T lastEl = operator[](nextFree_-1);
    --nextFree_;
    return lastEl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, CML::label Offset>
inline const T& CML::LongList<T, Offset>::operator[](const label i) const
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif

    return dataPtr_[i>>shift_][i&mask_];
}

template<class T, CML::label Offset>
inline T& CML::LongList<T, Offset>::operator[](const label i)
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif

    return dataPtr_[i>>shift_][i&mask_];
}

template<class T, CML::label Offset>
inline T& CML::LongList<T, Offset>::operator()(const label i)
{
    if( i >= nextFree_ )
        setSize(i+1);

    return operator[](i);
}


template<class T, CML::label Offset>
inline T& CML::LongList<T, Offset>::newElmt(const label i)
{
    return operator()(i);
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::operator=(const T& t)
{
    for(label i=0;i<nextFree_;++i)
        operator[](i) = t;
}

template<class T, CML::label Offset>
inline void CML::LongList<T, Offset>::operator=(const LongList<T, Offset>& l)
{
    setSize(l.size());

    for(label i=0;i<l.nextFree_;++i)
        operator[](i) = l[i];
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, CML::label Offset>
void CML::LongList<T, Offset>::writeEntry(Ostream& os) const
{
    if
    (
        size() &&
        token::compound::isCompound
        (
            "LongList<" + word(pTraits<T>::typeName) + '>'
        )
    )
    {
        os  << word("LongList<" + word(pTraits<T>::typeName) + '>') << " ";
    }

    os << *this;
}

template<class T, CML::label Offset>
void CML::LongList<T, Offset>::writeEntry
(
    const word& keyword,
    Ostream& os
) const
{
    os.writeKeyword(keyword);
    writeEntry(os);
    os << token::END_STATEMENT << endl;
}

template<class T, CML::label Offset>
CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::LongList<T, Offset>& DL
)
{
    if( (os.format() == IOstream::ASCII) || !contiguous<T>() )
    {
        if( DL.size() < 15 )
        {
            // Write size of list and start contents delimiter
            os << DL.size() << token::BEGIN_LIST;

            // Write list contents
            forAll(DL, i)
            {
                if( i != 0 ) os << token::SPACE;
                os << DL[i];
            }

            // Write end of contents delimiter
            os << token::END_LIST;
        }
        else
        {
            // Write size of list and start contents delimiter
            os << nl << DL.size() << nl << token::BEGIN_LIST;

            // Write list contents
            forAll(DL, i)
            {
                os << nl << DL[i];
            }

            // Write end of contents delimiter
            os << nl << token::END_LIST << nl;
        }
    }
    else
    {
        os << nl << DL.nextFree_ << nl;
        if( DL.nextFree_ )
        {
            const label blockSize = 1<<DL.shift_;

            label currBlock(0);
            label currPos(0);

            while( currPos < DL.nextFree_ )
            {
                const label bs =
                    CML::min(DL.nextFree_ - currPos, blockSize);

                os.write
                (
                    reinterpret_cast<const char*>(DL.dataPtr_[currBlock]),
                    bs * sizeof(T)
                );

                currPos += bs;
                ++currBlock;
            }
        }
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const LongList&)");

    return os;
}


template<class T, CML::label Offset>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::LongList<T, Offset>& DL
)
{
    // Anull list
    DL.setSize(0);

    is.fatalCheck("operator>>(Istream&, LongList<T, Offset>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, LongList<T, Offset>&) : reading first token"
    );

    if( firstToken.isLabel() )
    {
        const label size = firstToken.labelToken();

        // Set list length to that read
        DL.setSize(size);

        // Read list contents depending on data format
        if( (is.format() == IOstream::ASCII) || !contiguous<T>() )
        {
            // Read beginning of contents
            char listDelimiter = is.readBeginList("List");

            if( size == 0 )
            {
                if( listDelimiter != token::BEGIN_LIST )
                {
                    WarningIn
                    (
                        "template<class T, CML::label Offset>"

                        "CML::Istream& CML::operator>>"
                        "("
                            "CML::Istream& ,"
                            "CML::LongList<T, Offset>& DL"
                        ")"
                    ) << "Missing ( after 0" << endl;

                    return is;
                }

                listDelimiter = is.readEndList("List");
                if( listDelimiter != token::END_LIST )
                {
                    WarningIn
                    (
                        "template<class T, CML::label Offset>"

                        "CML::Istream& CML::operator>>"
                        "("
                            "CML::Istream& ,"
                            "CML::LongList<T, Offset>& DL"
                        ")"
                    ) << "Missing ) after 0(" << endl;
                }

                return is;
            }

            if( listDelimiter == token::BEGIN_LIST )
            {
                for(label i=0;i<size;++i)
                {
                    is >> DL[i];

                    is.fatalCheck
                    (
                        "operator>>(Istream&, List<T>&) : reading entry"
                    );
                }
            }
            else
            {
                T element;
                is >> element;

                is.fatalCheck
                (
                    "operator>>(Istream&, List<T>&) : "
                    "reading the single entry"
                );

                for(label i=0;i<size;++i)
                {
                    DL[i] = element;
                }
            }

            // Read end of contents
            is.readEndList("List");
        }
        else
        {
            const label blockSize = (1<<DL.shift_);

            label currBlock(0);
            label currPos(0);

            while( currPos < size )
            {
                const label bs = CML::min(size - currPos, blockSize);

                is.read
                (
                    reinterpret_cast<char*>(DL.dataPtr_[currBlock]),
                    bs * sizeof(T)
                );

                currPos += bs;
                ++currBlock;
            }

            is.fatalCheck
            (
                "operator>>(Istream&, LongList<T, Offset>&)"
                ": reading the binary block"
            );
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int>, found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    return is;
}

template<class T, CML::label Offset>
void CML::LongList<T, Offset>::appendFromStream(Istream& is)
{
    is.fatalCheck("appendFromStream(Istream& is)");

    token firstToken(is);

    is.fatalCheck
    (
        "appendFromStream(Istream& is) : reading first token"
    );

    if( firstToken.isLabel() )
    {
        const label size = firstToken.labelToken();

        if( size == 0 )
        {
            Pout << "Appending empty stream" << endl;
            return;
        }

        label origSize(this->size());

        // Set list length to that read
        setSize(origSize+size);

        // Read list contents depending on data format
        if( (is.format() == IOstream::ASCII) || !contiguous<T>() )
        {
            // Read beginning of contents
            char listDelimiter = is.readBeginList("List");

            if( listDelimiter == token::BEGIN_LIST )
            {
                for(label i=0;i<size;++i)
                {
                    is >> this->operator[](origSize);
                    ++origSize;

                    is.fatalCheck
                    (
                        "appendFromStream(Istream& is) : reading entry"
                    );
                }
            }
            else
            {
                T element;
                is >> element;

                is.fatalCheck
                (
                    "appendFromStream(Istream& is) : "
                    "reading the single entry"
                );

                for(label i=0;i<size;++i)
                {
                    this->operator[](origSize) = element;
                    ++origSize;
                }
            }

            // Read end of contents
            is.readEndList("List");
        }
        else
        {
            List<T> buf(size);
            is.read(reinterpret_cast<char*>(buf.begin()), size * sizeof(T));

            forAll(buf, i)
                this->operator[](origSize++) = buf[i];

            is.fatalCheck
            (
                "appendFromStream(Istream& is)"
                ": reading the binary block"
            );
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int>, found "
            << firstToken.info()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
