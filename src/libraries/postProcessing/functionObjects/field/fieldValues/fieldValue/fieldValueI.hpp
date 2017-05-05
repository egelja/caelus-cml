/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "fieldValue.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::fieldValue::name() const
{
    return name_;
}


inline const CML::objectRegistry& CML::fieldValue::obr() const
{
    return obr_;
}


inline const CML::dictionary& CML::fieldValue::dict() const
{
    return dict_;
}


inline bool CML::fieldValue::active() const
{
    return active_;
}


inline const CML::Switch& CML::fieldValue::log() const
{
    return log_;
}


inline const CML::word& CML::fieldValue::sourceName() const
{
    return sourceName_;
}


inline const CML::wordList& CML::fieldValue::fields() const
{
    return fields_;
}


inline const CML::Switch& CML::fieldValue::valueOutput() const
{
    return valueOutput_;
}


inline const CML::fvMesh& CML::fieldValue::mesh() const
{
    return refCast<const fvMesh>(obr_);
}


inline const CML::dictionary& CML::fieldValue::resultDict() const
{
    return resultDict_;
}


// ************************************************************************* //
