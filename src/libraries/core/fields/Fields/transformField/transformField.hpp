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

InNamespace
    CML

Description
    Spatial transformation functions for primitive fields.


\*---------------------------------------------------------------------------*/

#ifndef transformField_H
#define transformField_H

#include "transform.hpp"
#include "quaternion.hpp"
#include "septernion.hpp"
#include "vectorField.hpp"
#include "tensorField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void transform(Field<Type>&, const tensorField&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensorField&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensorField&, const tmp<Field<Type> >&);

template<class Type>
tmp<Field<Type> > transform(const tmp<tensorField>&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tmp<tensorField>&, const tmp<Field<Type> >&);


template<class Type>
void transform(Field<Type>&, const tensor&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensor&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensor&, const tmp<Field<Type> >&);


template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const Field<Type2>&);

template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const tmp<Field<Type2> >&);


template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
    const tensorField&
);

template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
    const tmp<tensorField>&
);


template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
    const tensorField&
);

template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
    const tmp<tensorField>&
);


//- Rotate given vectorField with the given quaternion
void transform(vectorField&, const quaternion&, const vectorField&);

//- Rotate given vectorField with the given quaternion
tmp<vectorField> transform(const quaternion&, const vectorField&);

//- Rotate given tmp<vectorField> with the given quaternion
tmp<vectorField> transform(const quaternion&, const tmp<vectorField>&);


//- Transform given vectorField of coordinates with the given septernion
void transformPoints(vectorField&, const septernion&, const vectorField&);

//- Transform given vectorField of coordinates with the given septernion
tmp<vectorField> transformPoints(const septernion&, const vectorField&);

//- Transform given tmp<vectorField> of coordinates with the given septernion
tmp<vectorField> transformPoints(const septernion&, const tmp<vectorField>&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FieldM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * global functions  * * * * * * * * * * * * * //

template<class Type>
void transform
(
    Field<Type>& rtf,
    const tensorField& trf,
    const Field<Type>& tf
)
{
    if (trf.size() == 1)
    {
        return transform(rtf, trf[0], tf);
    }
    else
    {
        TFOR_ALL_F_OP_FUNC_F_F
        (
            Type, rtf, =, transform, tensor, trf, Type, tf
        )
    }
}


template<class Type>
tmp<Field<Type> > transform
(
    const tensorField& trf,
    const Field<Type>& tf
)
{
    tmp<Field<Type> > tranf(new Field<Type> (tf.size()));
    transform(tranf(), trf, tf);
    return tranf;
}


template<class Type>
tmp<Field<Type> > transform
(
    const tensorField& trf,
    const tmp<Field<Type> >& ttf
)
{
    tmp<Field<Type> > tranf = reuseTmp<Type, Type>::New(ttf);
    transform(tranf(), trf, ttf());
    reuseTmp<Type, Type>::clear(ttf);
    return tranf;
}


template<class Type>
tmp<Field<Type> > transform
(
    const tmp<tensorField>& ttrf,
    const Field<Type>& tf
)
{
    tmp<Field<Type> > tranf(new Field<Type> (tf.size()));
    transform(tranf(), ttrf(), tf);
    ttrf.clear();
    return tranf;
}


template<class Type>
tmp<Field<Type> > transform
(
    const tmp<tensorField>& ttrf,
    const tmp<Field<Type> >& ttf
)
{
    tmp<Field<Type> > tranf = reuseTmp<Type, Type>::New(ttf);
    transform(tranf(), ttrf(), ttf());
    reuseTmp<Type, Type>::clear(ttf);
    ttrf.clear();
    return tranf;
}


template<class Type>
void transform
(
    Field<Type>& rtf,
    const tensor& t,
    const Field<Type>& tf
)
{
    TFOR_ALL_F_OP_FUNC_S_F(Type, rtf, =, transform, tensor, t, Type, tf)
}


template<class Type>
tmp<Field<Type> > transform
(
    const tensor& t,
    const Field<Type>& tf
)
{
    tmp<Field<Type> > tranf(new Field<Type>(tf.size()));
    transform(tranf(), t, tf);
    return tranf;
}


template<class Type>
tmp<Field<Type> > transform
(
    const tensor& t,
    const tmp<Field<Type> >& ttf
)
{
    tmp<Field<Type> > tranf = reuseTmp<Type, Type>::New(ttf);
    transform(tranf(), t, ttf());
    reuseTmp<Type, Type>::clear(ttf);
    return tranf;
}


template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const Field<Type2>& f)
{
    return f;
}

template<class Type1, class Type2>
tmp<Field<Type1> > transformFieldMask(const tmp<Field<Type2> >& tf)
{
    return tmp<Field<Type1> >(tf.ptr());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
