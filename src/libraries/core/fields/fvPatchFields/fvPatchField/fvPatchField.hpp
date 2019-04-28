/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2016 Applied CCM
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

Class
    CML::fvPatchField

Description
    Abstract base class with a fat-interface to all derived classes
    covering all possible ways in which they might be used.

    The first level of derivation is to basic patchFields which cover
    zero-gradient, fixed-gradient, fixed-value and mixed conditions.

    The next level of derivation covers all the specialised types with
    specific evaluation proceedures, particularly with respect to specific
    fields.

SourceFiles
    fvPatchField.cpp
    fvPatchFieldNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvPatchField_H
#define fvPatchField_H

#include "fvPatch.hpp"
#include "DimensionedField.hpp"
#include "volMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

class objectRegistry;
class dictionary;
class fvPatchFieldMapper;
class volMesh;


// Forward declaration of friend functions and operators

template<class Type>
class fvPatchField;

template<class Type>
class calculatedFvPatchField;

template<class Type>
class fvMatrix;

template<class Type>
Ostream& operator<<(Ostream&, const fvPatchField<Type>&);


/*---------------------------------------------------------------------------*\
                        Class fvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fvPatchField
:
    public Field<Type>
{
    // Private data

        //- Reference to patch
        const fvPatch& patch_;

        //- Reference to internal field
        const DimensionedField<Type, volMesh>& internalField_;

        //- Update index used so that updateCoeffs is called only once during
        //  the construction of the matrix
        bool updated_;

        //- Update index used so that manipulateMatrix is called only once
        //  during the construction of the matrix
        bool manipulatedMatrix_;

        //- Optional patch type, used to allow specified boundary conditions
        //  to be applied to constraint patches by providing the constraint
        //  patch type as 'patchType'
        word patchType_;


public:

    typedef fvPatch Patch;
    typedef calculatedFvPatchField<Type> Calculated;


    //- Runtime type information
    TypeName("fvPatchField");

    //- Debug switch to disallow the use of genericFvPatchField
    static int disallowGenericFvPatchField;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            fvPatchField,
            patch,
            (
                const fvPatch& p,
                const DimensionedField<Type, volMesh>& iF
            ),
            (p, iF)
        );

        declareRunTimeSelectionTable
        (
            tmp,
            fvPatchField,
            patchMapper,
            (
                const fvPatchField<Type>& ptf,
                const fvPatch& p,
                const DimensionedField<Type, volMesh>& iF,
                const fvPatchFieldMapper& m
            ),
            (dynamic_cast<const fvPatchFieldType&>(ptf), p, iF, m)
        );

        declareRunTimeSelectionTable
        (
            tmp,
            fvPatchField,
            dictionary,
            (
                const fvPatch& p,
                const DimensionedField<Type, volMesh>& iF,
                const dictionary& dict
            ),
            (p, iF, dict)
        );


    // Constructors

        //- Construct from patch and internal field
        fvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        fvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            Type const
        );

        //- Construct from patch and internal field and patch type
        fvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const word& patchType
        );

        //- Construct from patch and internal field and patch field
        fvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        fvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            const bool valueRequired=false
        );

        //- Construct by mapping the given fvPatchField onto a new patch
        fvPatchField
        (
            const fvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fvPatchField(const fvPatchField<Type>&);

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >(new fvPatchField<Type>(*this));
        }

        //- Construct as copy setting internal field reference
        fvPatchField
        (
            const fvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >(new fvPatchField<Type>(*this, iF));
        }


    // Selectors

        //- Return a pointer to a new patchField created on freestore given
        //  patch and internal field
        //  (does not set the patch field values)
        static tmp<fvPatchField<Type> > New
        (
            const word&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Return a pointer to a new patchField created on freestore given
        //  patch and internal field
        //  (does not set the patch field values).
        //  Allows override of constraint type
        static tmp<fvPatchField<Type> > New
        (
            const word&,
            const word& actualPatchType,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Return a pointer to a new patchField created on freestore from
        //  a given fvPatchField mapped onto a new patch
        static tmp<fvPatchField<Type> > New
        (
            const fvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Return a pointer to a new patchField created on freestore
        //  from dictionary
        static tmp<fvPatchField<Type> > New
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Return a pointer to a new calculatedFvPatchField created on
        //  freestore without setting patchField values
        static tmp<fvPatchField<Type> > NewCalculatedType
        (
            const fvPatch&
        );

        //- Return a pointer to a new calculatedFvPatchField created on
        //  freestore without setting patchField values
        template<class Type2>
        static tmp<fvPatchField<Type> > NewCalculatedType
        (
            const fvPatchField<Type2>&
        );


    //- Destructor
    virtual ~fvPatchField<Type>()
    {}


    // Member functions

        // Attributes

            //- Return the type of the calculated for of fvPatchField
            static const word& calculatedType();

            //- Return true if the value of the patch field
            //  is altered by assignment (the default)
            virtual bool assignable() const
            {
                return true;
            }

            //- Return true if this patch field fixes a value.
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations.
            virtual bool fixesValue() const
            {
                return false;
            }

            //- Return true if this patch field is coupled
            virtual bool coupled() const
            {
                return false;
            }

            //- Return true if this patch field fixes the gradient value
            virtual bool fixesGradient() const
            {
                return false;
            }

        // Access

            //- Return local objectRegistry
            const objectRegistry& db() const;

            //- Return patch
            const fvPatch& patch() const
            {
                return patch_;
            }

            //- Return dimensioned internal field reference
            const DimensionedField<Type, volMesh>&
            dimensionedInternalField() const
            {
                return internalField_;
            }

            //- Return internal field reference
            const Field<Type>& internalField() const
            {
                return internalField_;
            }

            //- Optional patch type
            const word& patchType() const
            {
                return patchType_;
            }

            //- Optional patch type
            word& patchType()
            {
                return patchType_;
            }

            //- Return true if the boundary condition has already been updated
            bool updated() const
            {
                return updated_;
            }

            //- Return true if the matrix has already been manipulated
            bool manipulatedMatrix() const
            {
                return manipulatedMatrix_;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>&,
                const labelList&
            );


        // Evaluation functions

            //- Return patch-normal gradient
            virtual tmp<Field<Type> > snGrad() const;

            //- Return patch-normal gradient for coupled-patches
            //  using the deltaCoeffs provided
            virtual tmp<Field<Type> > snGrad
            (
                const scalarField& deltaCoeffs
            ) const
            {
                NotImplemented;
                return *this;
            }

            //- Update the coefficients associated with the patch field
            //  Sets Updated to true
            virtual void updateCoeffs();

            //- Update the coefficients associated with the patch field
            //  and apply weight field
            //  Sets Updated to true
            virtual void updateCoeffs(const scalarField& weights);

            //- Return internal field next to patch as patch field
            virtual tmp<Field<Type> > patchInternalField() const;

            //- Return internal field next to patch as patch field
            virtual void patchInternalField(Field<Type>&) const;

            //- Return patchField on the opposite patch of a coupled patch
            virtual tmp<Field<Type> > patchNeighbourField() const
            {
                NotImplemented;
                return *this;
            }

            //- Initialise the evaluation of the patch field
            virtual void initEvaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            )
            {}

            //- Evaluate the patch field, sets Updated to false
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );


            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueInternalCoeffs
            (
                const tmp<Field<scalar> >&
            ) const
            {
                NotImplemented;
                return *this;
            }

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueBoundaryCoeffs
            (
                const tmp<Field<scalar> >&
            ) const
            {
                NotImplemented;
                return *this;
            }

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientInternalCoeffs() const
            {
                NotImplemented;
                return *this;
            }

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this coupled patchField
            //  using the deltaCoeffs provided
            virtual tmp<Field<Type> > gradientInternalCoeffs
            (
                const scalarField& deltaCoeffs
            ) const
            {
                NotImplemented;
                return *this;
            }

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientBoundaryCoeffs() const
            {
                NotImplemented;
                return *this;
            }

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this coupled patchField
            //  using the deltaCoeffs provided
            virtual tmp<Field<Type> > gradientBoundaryCoeffs
            (
                const scalarField& deltaCoeffs
            ) const
            {
                NotImplemented;
                return *this;
            }


            //- Manipulate matrix
            virtual void manipulateMatrix(fvMatrix<Type>& matrix);

            //- Manipulate matrix with given weights
            virtual void manipulateMatrix
            (
                fvMatrix<Type>& matrix,
                const scalarField& weights
            );


        // I-O

            //- Write
            virtual void write(Ostream&) const;

            //- Helper function to write the keyword and entry only if the
            //  values are not equal. The value is then output as value2
            template<class EntryType>
            void writeEntryIfDifferent
            (
                Ostream& os,
                const word& entryName,
                const EntryType& value1,
                const EntryType& value2
            ) const;


        // Check

            //- Check fvPatchField<Type> against given fvPatchField<Type>
            void check(const fvPatchField<Type>&) const;


    // Member operators

        virtual void operator=(const UList<Type>&);

        virtual void operator=(const fvPatchField<Type>&);
        virtual void operator+=(const fvPatchField<Type>&);
        virtual void operator-=(const fvPatchField<Type>&);
        virtual void operator*=(const fvPatchField<scalar>&);
        virtual void operator/=(const fvPatchField<scalar>&);

        virtual void operator+=(const Field<Type>&);
        virtual void operator-=(const Field<Type>&);

        virtual void operator*=(const Field<scalar>&);
        virtual void operator/=(const Field<scalar>&);

        virtual void operator=(const Type&);
        virtual void operator+=(const Type&);
        virtual void operator-=(const Type&);
        virtual void operator*=(const scalar);
        virtual void operator/=(const scalar);


        // Force an assignment irrespective of form of patch

        virtual void operator==(const fvPatchField<Type>&);
        virtual void operator==(const Field<Type>&);
        virtual void operator==(const Type&);


    // Ostream operator

        friend Ostream& operator<< <Type>(Ostream&, const fvPatchField<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOobject.hpp"
#include "dictionary.hpp"
#include "fvMesh.hpp"
#include "fvPatchFieldMapper.hpp"
//#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(word::null)
{}

template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    Type const val
)
:
    Field<Type>(p.size(),val),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(word::null)
{}

template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const word& patchType
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(patchType)
{}


template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& f
)
:
    Field<Type>(f),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(word::null)
{}


template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(ptf.patchType_)
{
    // For unmapped faces set to internal field value (zero-gradient)
    if (notNull(iF) && mapper.hasUnmapped())
    {
        fvPatchField<Type>::operator=(this->patchInternalField());
    }
    this->map(ptf, mapper);
}


template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict,
    const bool valueRequired
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(dict.lookupOrDefault<word>("patchType", word::null))
{
    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else if (!valueRequired)
    {
        fvPatchField<Type>::operator=(pTraits<Type>::zero);
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Essential entry 'value' missing"
            << exit(FatalIOError);
    }
}


template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(ptf.internalField_),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(ptf.patchType_)
{}


template<class Type>
CML::fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(iF),
    updated_(false),
    manipulatedMatrix_(false),
    patchType_(ptf.patchType_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const CML::objectRegistry& CML::fvPatchField<Type>::db() const
{
    return patch_.boundaryMesh().mesh();
}


template<class Type>
void CML::fvPatchField<Type>::check(const fvPatchField<Type>& ptf) const
{
    if (&patch_ != &(ptf.patch_))
    {
        FatalErrorInFunction
            << "different patches for fvPatchField<Type>s"
            << abort(FatalError);
    }
}


// Return gradient at boundary
template<class Type>
CML::tmp<CML::Field<Type> > CML::fvPatchField<Type>::snGrad() const
{
    return patch_.deltaCoeffs()*(*this - patchInternalField());
}


// Return internal field next to patch as patch field
template<class Type>
CML::tmp<CML::Field<Type> >
CML::fvPatchField<Type>::patchInternalField() const
{
    return patch_.patchInternalField(internalField_);
}


template<class Type>
void CML::fvPatchField<Type>::patchInternalField(Field<Type>& pif) const
{
    patch_.patchInternalField(internalField_, pif);
}


template<class Type>
void CML::fvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& mapper
)
{
    Field<Type>& f = *this;

    if (!this->size() && !mapper.distributed())
    {
        f.setSize(mapper.size());
        if (f.size())
        {
            f = this->patchInternalField();
        }
    }
    else
    {
        // Map all faces provided with mapping data
        Field<Type>::autoMap(mapper);

        // For unmapped faces set to internal field value (zero-gradient)
        if (mapper.hasUnmapped())
        {
            Field<Type> pif(this->patchInternalField());

            if
            (
                mapper.direct()
             && notNull(mapper.directAddressing())
             && mapper.directAddressing().size()
            )
            {
                const labelList& mapAddressing = mapper.directAddressing();

                forAll(mapAddressing, i)
                {
                    if (mapAddressing[i] < 0)
                    {
                        f[i] = pif[i];
                    }
                }
            }
            else if (!mapper.direct() && mapper.addressing().size())
            {
                const labelListList& mapAddressing = mapper.addressing();

                forAll(mapAddressing, i)
                {
                    const labelList& localAddrs = mapAddressing[i];

                    if (!localAddrs.size())
                    {
                        f[i] = pif[i];
                    }
                }
            }
        }
    }
}


template<class Type>
void CML::fvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    Field<Type>::rmap(ptf, addr);
}


template<class Type>
void CML::fvPatchField<Type>::updateCoeffs()
{
    updated_ = true;
}


template<class Type>
void CML::fvPatchField<Type>::updateCoeffs(const scalarField& weights)
{
    if (!updated_)
    {
        updateCoeffs();

        Field<Type>& fld = *this;
        fld *= weights;

        updated_ = true;
    }
}


template<class Type>
void CML::fvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!updated_)
    {
        updateCoeffs();
    }

    updated_ = false;
    manipulatedMatrix_ = false;
}


template<class Type>
void CML::fvPatchField<Type>::manipulateMatrix(fvMatrix<Type>& matrix)
{
    manipulatedMatrix_ = true;
}


template<class Type>
void CML::fvPatchField<Type>::manipulateMatrix
(
    fvMatrix<Type>& matrix,
    const scalarField& weights
)
{
    manipulatedMatrix_ = true;
}


template<class Type>
void CML::fvPatchField<Type>::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;

    if (patchType_.size())
    {
        os.writeKeyword("patchType") << patchType_
            << token::END_STATEMENT << nl;
    }
}


template<class Type>
template<class EntryType>
void CML::fvPatchField<Type>::writeEntryIfDifferent
(
    Ostream& os,
    const word& entryName,
    const EntryType& value1,
    const EntryType& value2
) const
{
    if (value1 != value2)
    {
        os.writeKeyword(entryName) << value2 << token::END_STATEMENT << nl;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::fvPatchField<Type>::operator=
(
    const UList<Type>& ul
)
{
    Field<Type>::operator=(ul);
}


template<class Type>
void CML::fvPatchField<Type>::operator=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator+=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator+=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator-=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator-=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator*=
(
    const fvPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorInFunction
            << "incompatible patches for patch fields"
            << abort(FatalError);
    }

    Field<Type>::operator*=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator/=
(
    const fvPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorInFunction
            << "    incompatible patches for patch fields"
            << abort(FatalError);
    }

    Field<Type>::operator/=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator+=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator+=(tf);
}


template<class Type>
void CML::fvPatchField<Type>::operator-=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator-=(tf);
}


template<class Type>
void CML::fvPatchField<Type>::operator*=
(
    const scalarField& tf
)
{
    Field<Type>::operator*=(tf);
}


template<class Type>
void CML::fvPatchField<Type>::operator/=
(
    const scalarField& tf
)
{
    Field<Type>::operator/=(tf);
}


template<class Type>
void CML::fvPatchField<Type>::operator=
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


template<class Type>
void CML::fvPatchField<Type>::operator+=
(
    const Type& t
)
{
    Field<Type>::operator+=(t);
}


template<class Type>
void CML::fvPatchField<Type>::operator-=
(
    const Type& t
)
{
    Field<Type>::operator-=(t);
}


template<class Type>
void CML::fvPatchField<Type>::operator*=
(
    const scalar s
)
{
    Field<Type>::operator*=(s);
}


template<class Type>
void CML::fvPatchField<Type>::operator/=
(
    const scalar s
)
{
    Field<Type>::operator/=(s);
}


// Force an assignment, overriding fixedValue status
template<class Type>
void CML::fvPatchField<Type>::operator==
(
    const fvPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void CML::fvPatchField<Type>::operator==
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void CML::fvPatchField<Type>::operator==
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<(Ostream& os, const fvPatchField<Type>& ptf)
{
    ptf.write(os);

    os.check("Ostream& operator<<(Ostream&, const fvPatchField<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::fvPatchField<Type> > CML::fvPatchField<Type>::New
(
    const word& patchFieldType,
    const word& actualPatchType,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
{
    if (debug)
    {
        Info<< "fvPatchField<Type>::New(const word&, const word&, "
               "const fvPatch&, const DimensionedField<Type, volMesh>&) :"
               " patchFieldType="
            << patchFieldType
            << " : " << p.type()
            << endl;
    }

    typename patchConstructorTable::iterator cstrIter =
        patchConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == patchConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown patchField type "
            << patchFieldType << nl << nl
            << "Valid patchField types are :" << endl
            << patchConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    typename patchConstructorTable::iterator patchTypeCstrIter =
        patchConstructorTablePtr_->find(p.type());

    if
    (
        actualPatchType == word::null
     || actualPatchType != p.type()
    )
    {
        if (patchTypeCstrIter != patchConstructorTablePtr_->end())
        {
            return patchTypeCstrIter()(p, iF);
        }
        else
        {
            return cstrIter()(p, iF);
        }
    }
    else
    {
        tmp<fvPatchField<Type> > tfvp = cstrIter()(p, iF);

        // Check if constraint type override and store patchType if so
        if ((patchTypeCstrIter != patchConstructorTablePtr_->end()))
        {
            tfvp().patchType() = actualPatchType;
        }
        return tfvp;
    }
}


template<class Type>
CML::tmp<CML::fvPatchField<Type> > CML::fvPatchField<Type>::New
(
    const word& patchFieldType,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
{
    return New(patchFieldType, word::null, p, iF);
}


template<class Type>
CML::tmp<CML::fvPatchField<Type> > CML::fvPatchField<Type>::New
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
{
    const word patchFieldType(dict.lookup("type"));

    if (debug)
    {
        Info<< "fvPatchField<Type>::New(const fvPatch&, "
               "const DimensionedField<Type, volMesh>&, "
               "const dictionary&) : patchFieldType="  << patchFieldType
            << endl;
    }

    typename dictionaryConstructorTable::iterator cstrIter
        = dictionaryConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        if (!disallowGenericFvPatchField)
        {
            cstrIter = dictionaryConstructorTablePtr_->find("generic");
        }

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << "Unknown patchField type " << patchFieldType
                << " for patch type " << p.type() << nl << nl
                << "Valid patchField types are :" << endl
                << dictionaryConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }
    }

    if
    (
       !dict.found("patchType")
     || word(dict.lookup("patchType")) != p.type()
    )
    {
        typename dictionaryConstructorTable::iterator patchTypeCstrIter
            = dictionaryConstructorTablePtr_->find(p.type());

        if
        (
            patchTypeCstrIter != dictionaryConstructorTablePtr_->end()
         && patchTypeCstrIter() != cstrIter()
        )
        {
            FatalIOErrorInFunction(dict)
                << "inconsistent patch and patchField types for \n"
                   "    patch type " << p.type()
                << " and patchField type " << patchFieldType
                << exit(FatalIOError);
        }
    }

    return cstrIter()(p, iF, dict);
}


template<class Type>
CML::tmp<CML::fvPatchField<Type> > CML::fvPatchField<Type>::New
(
    const fvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& pfMapper
)
{
    if (debug)
    {
        Info<< "fvPatchField<Type>::New(const fvPatchField<Type>&, "
               "const fvPatch&, const DimensionedField<Type, volMesh>&, "
               "const fvPatchFieldMapper&) : "
               "constructing fvPatchField<Type>"
            << endl;
    }

    typename patchMapperConstructorTable::iterator cstrIter =
        patchMapperConstructorTablePtr_->find(ptf.type());

    if (cstrIter == patchMapperConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown patchField type " << ptf.type() << nl << nl
            << "Valid patchField types are :" << endl
            << patchMapperConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(ptf, p, iF, pfMapper);
}



#   include "calculatedFvPatchField.hpp"

#define makeFvPatchField(fvPatchTypeField)                                     \
                                                                               \
defineNamedTemplateTypeNameAndDebug(fvPatchTypeField, 0);                      \
template<>                                                                     \
int fvPatchTypeField::disallowGenericFvPatchField                              \
(                                                                              \
    debug::debugSwitch("disallowGenericFvPatchField", 0)                       \
);                                                                             \
defineTemplateRunTimeSelectionTable(fvPatchTypeField, patch);                  \
defineTemplateRunTimeSelectionTable(fvPatchTypeField, patchMapper);            \
defineTemplateRunTimeSelectionTable(fvPatchTypeField, dictionary);


#define addToPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)    \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        patch                                                                  \
    );                                                                         \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        patchMapper                                                            \
    );                                                                         \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        dictionary                                                             \
    );


// use with caution
#define addRemovableToPatchFieldRunTimeSelection\
(PatchTypeField, typePatchTypeField)                                           \
                                                                               \
    addRemovableToRunTimeSelectionTable                                        \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        patch                                                                  \
    );                                                                         \
    addRemovableToRunTimeSelectionTable                                        \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        patchMapper                                                            \
    );                                                                         \
    addRemovableToRunTimeSelectionTable                                        \
    (                                                                          \
        PatchTypeField,                                                        \
        typePatchTypeField,                                                    \
        dictionary                                                             \
    );


// for non-templated patch fields
#define makePatchTypeField(PatchTypeField, typePatchTypeField)                 \
    defineTypeNameAndDebug(typePatchTypeField, 0);                             \
    addToPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)

// for non-templated patch fields - use with caution
#define makeRemovablePatchTypeField(PatchTypeField, typePatchTypeField)        \
    defineTypeNameAndDebug(typePatchTypeField, 0);                             \
    addRemovableToPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


// for templated patch fields
#define makeTemplatePatchTypeField(PatchTypeField, typePatchTypeField)         \
    defineNamedTemplateTypeNameAndDebug(typePatchTypeField, 0);                \
    addToPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


#define makePatchFields(type)                                                  \
    makeTemplatePatchTypeField                                                 \
    (                                                                          \
        fvPatchScalarField,                                                    \
        type##FvPatchScalarField                                               \
    );                                                                         \
    makeTemplatePatchTypeField                                                 \
    (                                                                          \
        fvPatchVectorField,                                                    \
        type##FvPatchVectorField                                               \
    );                                                                         \
    makeTemplatePatchTypeField                                                 \
    (                                                                          \
        fvPatchSphericalTensorField,                                           \
        type##FvPatchSphericalTensorField                                      \
    );                                                                         \
    makeTemplatePatchTypeField                                                 \
    (                                                                          \
        fvPatchSymmTensorField,                                                \
        type##FvPatchSymmTensorField                                           \
    );                                                                         \
    makeTemplatePatchTypeField                                                 \
    (                                                                          \
        fvPatchTensorField,                                                    \
        type##FvPatchTensorField                                               \
    );


#define makePatchFieldsTypeName(type)                                          \
    defineNamedTemplateTypeNameAndDebug(type##FvPatchScalarField, 0);          \
    defineNamedTemplateTypeNameAndDebug(type##FvPatchVectorField, 0);          \
    defineNamedTemplateTypeNameAndDebug(type##FvPatchSphericalTensorField, 0);\
    defineNamedTemplateTypeNameAndDebug(type##FvPatchSymmTensorField, 0);      \
    defineNamedTemplateTypeNameAndDebug(type##FvPatchTensorField, 0)


#define makePatchTypeFieldTypedefs(type)                                       \
    typedef type##FvPatchField<scalar> type##FvPatchScalarField;               \
    typedef type##FvPatchField<vector> type##FvPatchVectorField;               \
    typedef type##FvPatchField<sphericalTensor>                                \
        type##FvPatchSphericalTensorField;                                     \
    typedef type##FvPatchField<symmTensor> type##FvPatchSymmTensorField;       \
    typedef type##FvPatchField<tensor> type##FvPatchTensorField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
