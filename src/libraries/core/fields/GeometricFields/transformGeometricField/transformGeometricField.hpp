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

InClass
    CML::transformGeometricField

Description
    Spatial transformation functions for FieldFields.

SourceFiles
    transformGeometricField.cpp

\*---------------------------------------------------------------------------*/

#ifndef transformGeometricField_H
#define transformGeometricField_H

#include "transform.hpp"
#include "GeometricField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
    GeometricField<Type, PatchField, GeoMesh>&,
    const GeometricField<tensor, PatchField, GeoMesh>&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const GeometricField<tensor, PatchField, GeoMesh>&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const GeometricField<tensor, PatchField, GeoMesh>&,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const tmp<GeometricField<tensor, PatchField, GeoMesh> >&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const tmp<GeometricField<tensor, PatchField, GeoMesh> >&,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);


template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
    GeometricField<Type, PatchField, GeoMesh>&,
    const dimensionedTensor&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const dimensionedTensor&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const dimensionedTensor&,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "transformField.hpp"
#include "transformFieldField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * global functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
    GeometricField<Type, PatchField, GeoMesh>& rtf,
    const GeometricField<tensor, PatchField, GeoMesh>& trf,
    const GeometricField<Type, PatchField, GeoMesh>& tf
)
{
    transform(rtf.internalField(), trf.internalField(), tf.internalField());
    transform(rtf.boundaryField(), trf.boundaryField(), tf.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const GeometricField<tensor, PatchField, GeoMesh>& trf,
    const GeometricField<Type, PatchField, GeoMesh>& tf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf
    (
        new GeometricField<Type, PatchField, GeoMesh>
        (
            IOobject
            (
                "transform(" + trf.name() + ',' + tf.name() + ')',
                tf.instance(),
                tf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            tf.mesh(),
            tf.dimensions()
        )
    );

    transform(tranf(), trf, tf);

    return tranf;
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const GeometricField<tensor, PatchField, GeoMesh>& trf,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& ttf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf =
        transform(trf, ttf());
    ttf.clear();
    return tranf;
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const tmp<GeometricField<tensor, PatchField, GeoMesh> >& ttrf,
    const GeometricField<Type, PatchField, GeoMesh>& tf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf =
        transform(ttrf(), tf);
    ttrf.clear();
    return tranf;
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const tmp<GeometricField<tensor, PatchField, GeoMesh> >& ttrf,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& ttf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf =
        transform(ttrf(), ttf());
    ttf.clear();
    ttrf.clear();
    return tranf;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
    GeometricField<Type, PatchField, GeoMesh>& rtf,
    const dimensionedTensor& t,
    const GeometricField<Type, PatchField, GeoMesh>& tf
)
{
    transform(rtf.internalField(), t.value(), tf.internalField());
    transform(rtf.boundaryField(), t.value(), tf.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const dimensionedTensor& t,
    const GeometricField<Type, PatchField, GeoMesh>& tf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf
    (
        new GeometricField<vector, PatchField, GeoMesh>
        (
            IOobject
            (
                "transform(" + t.name() + ',' + tf.name() + ')',
                tf.instance(),
                tf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            tf.mesh(),
            tf.dimensions()
        )
    );

    transform(tranf(), t, tf);

    return tranf;
}


template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
    const dimensionedTensor& t,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& ttf
)
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > tranf =
        transform(t, ttf());
    ttf.clear();
    return tranf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
