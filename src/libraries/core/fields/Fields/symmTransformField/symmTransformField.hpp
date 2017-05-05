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

Typedef
    CML::symmTransformField

Description
    Spatial transformation functions for primitive fields.

\*---------------------------------------------------------------------------*/

#ifndef symmTransformField_H
#define symmTransformField_H

#include "symmTransform.hpp"
#include "symmTensorField.hpp"
#include "sphericalTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void transform(Field<Type>&, const symmTensorField&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const symmTensorField&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const symmTensorField&, const tmp<Field<Type> >&);

template<class Type>
tmp<Field<Type> > transform(const tmp<symmTensorField>&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform
(
    const tmp<symmTensorField>&,
    const tmp<Field<Type> >&
);


template<class Type>
void transform(Field<Type>&, const tensor&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensor&, const Field<Type>&);

template<class Type>
tmp<Field<Type> > transform(const tensor&, const tmp<Field<Type> >&);


template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
    const symmTensorField&
);

template<>
tmp<Field<sphericalTensor> > transformFieldMask<sphericalTensor>
(
    const tmp<symmTensorField>&
);


template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
    const symmTensorField&
);

template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
    const tmp<symmTensorField>&
);


template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
    const symmTensorField&
);

template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
    const tmp<symmTensorField>&
);


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
    const symmTensorField& trf,
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
            Type, rtf, =, transform, symmTensor, trf, Type, tf
        )
    }
}


template<class Type>
tmp<Field<Type> > transform
(
    const symmTensorField& trf,
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
    const symmTensorField& trf,
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
    const tmp<symmTensorField>& ttrf,
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
    const tmp<symmTensorField>& ttrf,
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
    const symmTensor& t,
    const Field<Type>& tf
)
{
    TFOR_ALL_F_OP_FUNC_S_F(Type, rtf, =, transform, tensor, t, Type, tf)
}


template<class Type>
tmp<Field<Type> > transform
(
    const symmTensor& t,
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
    const symmTensor& t,
    const tmp<Field<Type> >& ttf
)
{
    tmp<Field<Type> > tranf = reuseTmp<Type, Type>::New(ttf);
    transform(tranf(), t, ttf());
    reuseTmp<Type, Type>::clear(ttf);
    return tranf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
