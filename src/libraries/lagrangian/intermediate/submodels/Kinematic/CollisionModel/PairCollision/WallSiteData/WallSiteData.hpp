/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::WallSiteData

Description
    Stores the patch ID and templated data to represent a collision
    with a wall to be passed to the wall model.


\*---------------------------------------------------------------------------*/

#ifndef WallSiteData_H
#define WallSiteData_H

#include "label.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class WallSiteData;

template<class Type>
Istream& operator>>(Istream&, WallSiteData<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const WallSiteData<Type>&);


/*---------------------------------------------------------------------------*\
                         Class WallSiteData Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class WallSiteData
{
    // Private data

        //- Index of originating patch
        label patchi_;

        //- Wall data
        Type wallData_;


public:

    // Constructors

        //- Construct null
        WallSiteData();

        //- Construct from components
        WallSiteData
        (
            label patchi,
            const Type& wallData
        );


    //- Destructor
    ~WallSiteData();


    // Member Functions

            //- Return access to the patch index
            inline label patchIndex() const;

            //- Return non-const access to the patch index
            inline label& patchIndex();

            //- Return access to wall data
            inline const Type& wallData() const;

            //- Return non-const access to wall data
            inline Type& wallData();


    // Member Operators

        bool operator==(const WallSiteData<Type>&) const;
        bool operator!=(const WallSiteData<Type>&) const;


    // IOstream Operators

        friend Istream& operator>> <Type>
        (Istream&, WallSiteData<Type>&);

        friend Ostream& operator<< <Type>
        (Ostream&, const WallSiteData<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::label CML::WallSiteData<Type>::patchIndex() const
{
    return patchi_;
}


template<class Type>
CML::label& CML::WallSiteData<Type>::patchIndex()
{
    return patchi_;
}


template<class Type>
const Type& CML::WallSiteData<Type>::wallData() const
{
    return wallData_;
}


template<class Type>
Type& CML::WallSiteData<Type>::wallData()
{
    return wallData_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::WallSiteData<Type>::WallSiteData()
:
    patchi_(),
    wallData_()
{}


template<class Type>
CML::WallSiteData<Type>::WallSiteData
(
    label patchi,
    const Type& wallData
)
:
    patchi_(patchi),
    wallData_(wallData)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::WallSiteData<Type>::~WallSiteData()
{}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

template<class Type>
bool CML::WallSiteData<Type>::operator==
(
    const WallSiteData<Type>& rhs
) const
{
    return patchi_ == rhs.patchi_ && wallData_ == rhs.wallData_;
}


template<class Type>
bool CML::WallSiteData<Type>::operator!=
(
    const WallSiteData<Type>& rhs
) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Istream& CML::operator>>
(
    Istream& is,
    WallSiteData<Type>& wIS
)
{
    is  >> wIS.patchi_ >> wIS.wallData_;

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>"
        "(CML::Istream&, CML::WallSiteData<Type>&)"
    );

    return is;
}


template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const WallSiteData<Type>& wIS
)
{
    os  << wIS.patchi_ << token::SPACE << wIS.wallData_;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<"
        "(Ostream&, const WallSiteData<Type>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
