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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::regIOobject::ownedByRegistry() const
{
    return ownedByRegistry_;
}


inline void CML::regIOobject::store()
{
    ownedByRegistry_ = true;
}


template<class Type>
inline Type& CML::regIOobject::store(Type* tPtr)
{
    if (!tPtr)
    {
        FatalErrorInFunction
            << "object deallocated"
            << abort(FatalError);
    }

    tPtr->regIOobject::ownedByRegistry_ = true;

    return *tPtr;
}


template<class Type>
inline Type& CML::regIOobject::store(autoPtr<Type>& atPtr)
{
    Type* tPtr = atPtr.ptr();

    if (!tPtr)
    {
        FatalErrorInFunction
            << "object deallocated"
            << abort(FatalError);
    }

    tPtr->regIOobject::ownedByRegistry_ = true;

    return *tPtr;
}


inline void CML::regIOobject::release()
{
    ownedByRegistry_ = false;
}


inline CML::label CML::regIOobject::eventNo() const
{
    return eventNo_;
}

inline CML::label& CML::regIOobject::eventNo()
{
    return eventNo_;
}


inline CML::label CML::regIOobject::watchIndex() const
{
    return watchIndex_;
}


inline CML::label& CML::regIOobject::watchIndex()
{
    return watchIndex_;
}


// ************************************************************************* //
