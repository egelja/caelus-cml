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
    CML::vectorTensorTransform

Description
    Vector-tensor class used to perform translations and rotations in
    3D space.

SourceFiles
    vectorTensorTransform.cpp

\*---------------------------------------------------------------------------*/

#ifndef vectorTensorTransform_H
#define vectorTensorTransform_H

#include "tensor.hpp"
#include "word.hpp"
#include "contiguous.hpp"
#include "pointField.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class vectorTensorTransform;
Istream& operator>>(Istream& is, vectorTensorTransform&);
Ostream& operator<<(Ostream& os, const vectorTensorTransform& C);


/*---------------------------------------------------------------------------*\
                    Class vectorTensorTransform Declaration
\*---------------------------------------------------------------------------*/

class vectorTensorTransform
{
    // private data

        //- Translation vector
        vector t_;

        //- Rotation tensor
        tensor R_;

        //- Recording if the transform has non-identity transform to
        //  allow its calculation to be skipped, which is the majority
        //  of the expected cases
        bool hasR_;


public:

    // Static data members

        static const char* const typeName;

        static const vectorTensorTransform zero;

        static const vectorTensorTransform I;


    // Constructors

        //- Construct null
        inline vectorTensorTransform();

        //- Construct given a translation vector, rotation tensor and
        //  hasR bool
        inline vectorTensorTransform
        (
            const vector& t,
            const tensor& R,
            bool hasR = true
        );

        //- Construct a pure translation vectorTensorTransform given a
        //  translation vector
        inline explicit vectorTensorTransform(const vector& t);

        //- Construct a pure rotation vectorTensorTransform given a
        //  rotation tensor
        inline explicit vectorTensorTransform(const tensor& R);

        //- Construct from Istream
        vectorTensorTransform(Istream&);


    // Member functions

        // Access

            inline const vector& t() const;

            inline const tensor& R() const;

            inline bool hasR() const;


        // Edit

            inline vector& t();

            inline tensor& R();


        // Transform

            //- Transform the given position
            inline vector transformPosition(const vector& v) const;

            //- Transform the given pointField
            inline pointField transformPosition(const pointField& pts) const;

            //- Inverse transform the given position
            inline vector invTransformPosition(const vector& v) const;

            //- Inverse transform the given pointField
            inline pointField invTransformPosition(const pointField& pts) const;

            //- Transform the given type
            template<class Type>
            Type transform(const Type&) const;

            //- Transform the given field
            template<class Type>
            tmp<Field<Type> > transform(const Field<Type>&) const;

            //- Inverse transform the given type
            template<class Type>
            Type invTransform(const Type&) const;

            //- Inverse transform the given field
            template<class Type>
            tmp<Field<Type> > invTransform(const Field<Type>&) const;


    // Member operators

        inline void operator=(const vectorTensorTransform&);
        inline void operator&=(const vectorTensorTransform&);

        inline void operator=(const vector&);
        inline void operator+=(const vector&);
        inline void operator-=(const vector&);

        inline void operator=(const tensor&);
        inline void operator&=(const tensor&);


    // IOstream operators

        friend Istream& operator>>(Istream& is, vectorTensorTransform&);

        friend Ostream& operator<<(Ostream& os, const vectorTensorTransform&);
};


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

//- Return the inverse of the given vectorTensorTransform
inline vectorTensorTransform inv(const vectorTensorTransform& tr);


//- Return a string representation of a vectorTensorTransform
word name(const vectorTensorTransform&);


//- Data associated with vectorTensorTransform type are contiguous
template<>
inline bool contiguous<vectorTensorTransform>() {return true;}

//- Template specialisations
template<>
tmp<Field<bool> > vectorTensorTransform::transform(const Field<bool>&) const;
template<>
tmp<Field<label> > vectorTensorTransform::transform(const Field<label>&) const;
template<>
tmp<Field<scalar> > vectorTensorTransform::transform(const Field<scalar>&)
const;


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
);


inline bool operator!=
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2

);


inline vectorTensorTransform operator+
(
    const vectorTensorTransform& tr,
    const vector& t
);


inline vectorTensorTransform operator+
(
    const vector& t,
    const vectorTensorTransform& tr
);


inline vectorTensorTransform operator-
(
    const vectorTensorTransform& tr,
    const vector& t
);


inline vectorTensorTransform operator&
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::vectorTensorTransform::vectorTensorTransform()
:
    t_(vector::zero),
    R_(sphericalTensor::I),
    hasR_(false)
{}


inline CML::vectorTensorTransform::vectorTensorTransform
(
    const vector& t,
    const tensor& R,
    bool hasR
)
:
    t_(t),
    R_(R),
    hasR_(hasR)
{}


inline CML::vectorTensorTransform::vectorTensorTransform(const vector& t)
:
    t_(t),
    R_(sphericalTensor::I),
    hasR_(false)
{}


inline CML::vectorTensorTransform::vectorTensorTransform(const tensor& R)
:
    t_(vector::zero),
    R_(R),
    hasR_(true)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::vector& CML::vectorTensorTransform::t() const
{
    return t_;
}


inline const CML::tensor& CML::vectorTensorTransform::R() const
{
    return R_;
}


inline bool CML::vectorTensorTransform::hasR() const
{
    return hasR_;
}


inline CML::vector& CML::vectorTensorTransform::t()
{
    return t_;
}


inline CML::tensor& CML::vectorTensorTransform::R()
{
    // Assume that non-const access to R changes it from I, so set
    // hasR to true

    hasR_ = true;

    return R_;
}


inline CML::vector CML::vectorTensorTransform::transformPosition
(
    const vector& v
) const
{
    if (hasR_)
    {
        return t() + (R() & v);
    }
    else
    {
        return t() + v;
    }
}


inline CML::pointField CML::vectorTensorTransform::transformPosition
(
    const pointField& pts
) const
{
    tmp<pointField> tfld;

    if (hasR_)
    {
        tfld = t() + (R() & pts);
    }
    else
    {
        tfld = t() + pts;
    }
    return tfld();
}


inline CML::vector CML::vectorTensorTransform::invTransformPosition
(
    const vector& v
) const
{
    if (hasR_)
    {
        return (R().T() & (v - t()));
    }
    else
    {
        return v - t();
    }
}


inline CML::pointField CML::vectorTensorTransform::invTransformPosition
(
    const pointField& pts
) const
{
    tmp<pointField> tfld;

    if (hasR_)
    {
        tfld = (R().T() & (pts - t()));
    }
    else
    {
        tfld = pts - t();
    }
    return tfld();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::vectorTensorTransform::operator=
(
    const vectorTensorTransform& tr
)
{
    t_ = tr.t_;
    R_ = tr.R_;
    hasR_ = tr.hasR_;
}


inline void CML::vectorTensorTransform::operator&=
(
    const vectorTensorTransform& tr
)
{
    t_ += tr.t_;
    R_ = tr.R_ & R_;

    // If either of the two objects has hasR_ as true, then inherit
    // it, otherwise, these should both be I tensors.
    hasR_ = tr.hasR_ || hasR_;
}


inline void CML::vectorTensorTransform::operator=(const vector& t)
{
    t_ = t;
}


inline void CML::vectorTensorTransform::operator+=(const vector& t)
{
    t_ += t;
}


inline void CML::vectorTensorTransform::operator-=(const vector& t)
{
    t_ -= t;
}


inline void CML::vectorTensorTransform::operator=(const tensor& R)
{
    hasR_ = true;

    R_ = R;
}


inline void CML::vectorTensorTransform::operator&=(const tensor& R)
{
    hasR_ = true;

    R_ = R & R_;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

inline CML::vectorTensorTransform CML::inv(const vectorTensorTransform& tr)
{
    return vectorTensorTransform(-tr.t(), tr.R().T(), tr.hasR());
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool CML::operator==
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return (tr1.t() == tr2.t() && tr1.R() == tr2.R());
}


inline bool CML::operator!=
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return !operator==(tr1, tr2);
}


inline CML::vectorTensorTransform CML::operator+
(
    const vectorTensorTransform& tr,
    const vector& t
)
{
    return vectorTensorTransform(tr.t() + t, tr.R(), tr.hasR());
}


inline CML::vectorTensorTransform CML::operator+
(
    const vector& t,
    const vectorTensorTransform& tr
)
{
    return vectorTensorTransform(t + tr.t(), tr.R(), tr.hasR());
}


inline CML::vectorTensorTransform CML::operator-
(
    const vectorTensorTransform& tr,
    const vector& t
)
{
    return vectorTensorTransform(tr.t() - t, tr.R(), tr.hasR());
}


inline CML::vectorTensorTransform CML::operator&
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return vectorTensorTransform
    (
        tr1.t() + tr2.t(),
        tr1.R() & tr2.R(),
        (tr1.hasR() || tr2.hasR())
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class Type>
Type CML::vectorTensorTransform::transform(const Type& x) const
{
    if (hasR_)
    {
        return CML::transform(R(), x);
    }
    else
    {
        return x;
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::vectorTensorTransform::transform
(
    const Field<Type>& fld
) const
{
    if (hasR_)
    {
        return CML::transform(R(), fld);
    }
    else
    {
        return fld;
    }
}


template<class Type>
Type CML::vectorTensorTransform::invTransform(const Type& x) const
{
    if (hasR_)
    {
        return CML::transform(R().T(), x);
    }
    else
    {
        return x;
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::vectorTensorTransform::invTransform
(
    const Field<Type>& fld
) const
{
    if (hasR_)
    {
        return CML::transform(R().T(), fld);
    }
    else
    {
        return fld;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
