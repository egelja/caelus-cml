/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::geometricOneField

Description
    A class representing the concept of a GeometricField of 1 used to avoid
    unnecessary manipulations for objects which are known to be one at
    compile-time.

    Used for example as the density argument to a function written for
    compressible to be used for incompressible flow.

\*---------------------------------------------------------------------------*/

#ifndef geometricOneField_H
#define geometricOneField_H

#include "oneFieldField.hpp"
#include "dimensionSet.hpp"
#include "scalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class geometricOneField Declaration
\*---------------------------------------------------------------------------*/

class geometricOneField
:
    public one
{

public:

    // Public typedefs

        typedef oneField DimesionedInternalField;
        typedef oneField InternalField;
        typedef oneField PatchFieldType;
        typedef oneFieldField GeometricBoundaryField;
        typedef one cmptType;

    // Constructors

        //- Construct null
        geometricOneField()
        {}


    // Member Operators

        inline const dimensionSet& dimensions() const;

        inline one operator[](const label) const;

        inline oneField field() const;

        inline oneField oldTime() const;

        inline InternalField internalField() const;

        inline DimesionedInternalField v() const;

        inline GeometricBoundaryField boundaryField() const;
};


inline const geometricOneField& operator*
(
    const geometricOneField&,
    const geometricOneField&
);

inline const geometricOneField::DimesionedInternalField& operator*
(
    const geometricOneField::DimesionedInternalField&,
    const geometricOneField&
);

inline const geometricOneField::DimesionedInternalField& operator*
(
    const geometricOneField&,
    const geometricOneField::DimesionedInternalField&
);

inline const geometricOneField& operator/
(
    const geometricOneField&,
    const geometricOneField&
);

inline const geometricOneField::DimesionedInternalField& operator/
(
    const geometricOneField::DimesionedInternalField&,
    const geometricOneField&
);

inline const geometricOneField::DimesionedInternalField& operator/
(
    const geometricOneField&,
    const geometricOneField::DimesionedInternalField&
);


} // End namespace CML


inline const CML::dimensionSet& CML::geometricOneField::dimensions() const
{
    return dimless;
}


inline CML::one CML::geometricOneField::operator[](const label) const
{
    return one();
}


inline CML::oneField CML::geometricOneField::field() const
{
    return oneField();
}


inline CML::oneField CML::geometricOneField::oldTime() const
{
    return oneField();
}


inline CML::geometricOneField::InternalField
CML::geometricOneField::internalField() const
{
    return InternalField();
}


inline CML::geometricOneField::DimesionedInternalField
CML::geometricOneField::v() const
{
    return DimesionedInternalField();
}


inline CML::geometricOneField::GeometricBoundaryField
CML::geometricOneField::boundaryField() const
{
    return GeometricBoundaryField();
}


inline const CML::geometricOneField& CML::operator*
(
    const geometricOneField& gof,
    const geometricOneField&
)
{
    return gof;
}


inline const CML::geometricOneField::DimesionedInternalField& CML::operator*
(
    const geometricOneField::DimesionedInternalField
& of,
    const geometricOneField&
)
{
    return of;
}


inline const CML::geometricOneField::DimesionedInternalField& CML::operator*
(
    const geometricOneField&,
    const geometricOneField::DimesionedInternalField& of
)
{
    return of;
}


inline const CML::geometricOneField& CML::operator/
(
    const geometricOneField& gof,
    const geometricOneField&
)
{
    return gof;
}


inline const CML::geometricOneField::DimesionedInternalField& CML::operator/
(
    const geometricOneField::DimesionedInternalField& of,
    const geometricOneField&
)
{
    return of;
}


inline const CML::geometricOneField::DimesionedInternalField& CML::operator/
(
    const geometricOneField&,
    const geometricOneField::DimesionedInternalField& of
)
{
    return of;
}


#endif
