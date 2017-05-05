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

\*---------------------------------------------------------------------------*/

#include "transformField.hpp"
#include "FieldM.hpp"
#include "diagTensor.hpp"

// * * * * * * * * * * * * * * * global functions  * * * * * * * * * * * * * //

void CML::transform
(
    vectorField& rtf,
    const quaternion& q,
    const vectorField& tf
)
{
    tensor t = q.R();
    TFOR_ALL_F_OP_FUNC_S_F(vector, rtf, =, transform, tensor, t, vector, tf)
}


CML::tmp<CML::vectorField> CML::transform
(
    const quaternion& q,
    const vectorField& tf
)
{
    tmp<vectorField > tranf(new vectorField(tf.size()));
    transform(tranf(), q, tf);
    return tranf;
}


CML::tmp<CML::vectorField> CML::transform
(
    const quaternion& q,
    const tmp<vectorField>& ttf
)
{
    tmp<vectorField > tranf = reuseTmp<vector, vector>::New(ttf);
    transform(tranf(), q, ttf());
    reuseTmp<vector, vector>::clear(ttf);
    return tranf;
}


void CML::transformPoints
(
    vectorField& rtf,
    const septernion& tr,
    const vectorField& tf
)
{
    vector T = tr.t();

    // Check if any translation
    if (mag(T) > VSMALL)
    {
        TFOR_ALL_F_OP_F_OP_S(vector, rtf, =, vector, tf, -, vector, T);
    }
    else
    {
        rtf = tf;
    }

    // Check if any rotation
    if (mag(tr.r().R() - I) > SMALL)
    {
        transform(rtf, tr.r(), rtf);
    }
}


CML::tmp<CML::vectorField> CML::transformPoints
(
    const septernion& tr,
    const vectorField& tf
)
{
    tmp<vectorField > tranf(new vectorField(tf.size()));
    transformPoints(tranf(), tr, tf);
    return tranf;
}


CML::tmp<CML::vectorField> CML::transformPoints
(
    const septernion& tr,
    const tmp<vectorField>& ttf
)
{
    tmp<vectorField > tranf = reuseTmp<vector, vector>::New(ttf);
    transformPoints(tranf(), tr, ttf());
    reuseTmp<vector, vector>::clear(ttf);
    return tranf;
}


// ************************************************************************* //
