/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::pointPatchField

Description
    Abstract base class for point-mesh patch fields.

    The base-field does not store values as they are part of the
    "internal field".  There are derived classes to store constraint values
    e.g. fixedValuePointPatchField derived from the generic
    valuePointPatchField which ensures the values in the "internal field"
    are reset to the fixed-values by applying the stored values.

SourceFiles
    pointPatchField.cpp
    newPointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointPatchField_H
#define pointPatchField_H

#include "pointPatch.hpp"
#include "DimensionedField.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

class objectRegistry;
class dictionary;
class pointPatchFieldMapper;
class pointMesh;

// Forward declaration of friend functions and operators

template<class Type>
class pointPatchField;

template<class Type>
class calculatedPointPatchField;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const pointPatchField<Type>&
);


/*---------------------------------------------------------------------------*\
                       Class pointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class pointPatchField
{
    // Private data

        //- Reference to patch
        const pointPatch& patch_;

        //- Reference to internal field
        const DimensionedField<Type, pointMesh>& internalField_;

        //- Update index used so that updateCoeffs is called only once during
        //  the construction of the matrix
        bool updated_;

        //- Optional patch type, used to allow specified boundary conditions
        //  to be applied to constraint patches by providing the constraint
        //  patch type as 'patchType'
        word patchType_;


public:

    typedef pointPatch Patch;
    typedef calculatedPointPatchField<Type> Calculated;


    //- Runtime type information
    TypeName("pointPatchField");

    //- Debug switch to disallow the use of genericPointPatchField
    static int disallowGenericPointPatchField;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            pointPatchField,
            pointPatch,
            (
                const pointPatch& p,
                const DimensionedField<Type, pointMesh>& iF
            ),
            (p, iF)
        );

        declareRunTimeSelectionTable
        (
            autoPtr,
            pointPatchField,
            patchMapper,
            (
                const pointPatchField<Type>& ptf,
                const pointPatch& p,
                const DimensionedField<Type, pointMesh>& iF,
                const pointPatchFieldMapper& m
            ),
            (dynamic_cast<const pointPatchFieldType&>(ptf), p, iF, m)
        );

        declareRunTimeSelectionTable
        (
            autoPtr,
            pointPatchField,
            dictionary,
            (
                const pointPatch& p,
                const DimensionedField<Type, pointMesh>& iF,
                const dictionary& dict
            ),
            (p, iF, dict)
        );


    // Constructors

        //- Construct from patch and internal field
        pointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        pointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct as copy
        pointPatchField(const pointPatchField<Type>&);

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const = 0;

        //- Construct as copy setting internal field reference
        pointPatchField
        (
            const pointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const = 0;


    // Selectors

        //- Return a pointer to a new patchField created on freestore given
        //  patch and internal field
        //  (does not set the patch field values)
        static autoPtr<pointPatchField<Type> > New
        (
            const word&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Return a pointer to a new patchField created on freestore given
        //  patch and internal field
        //  (does not set the patch field values).
        //  Allows override of constraint type
        static autoPtr<pointPatchField<Type> > New
        (
            const word&,
            const word& actualPatchType,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Return a pointer to a new patchField created on freestore from
        //  a given pointPatchField mapped onto a new patch
        static autoPtr<pointPatchField<Type> > New
        (
            const pointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Return a pointer to a new patchField created on freestore
        //  from dictionary
        static autoPtr<pointPatchField<Type> > New
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );


        //- Return a pointer to a new CalculatedpointPatchField created on
        //  freestore without setting patchField values
        template<class Type2>
        static autoPtr<pointPatchField<Type> >
        NewCalculatedType
        (
            const pointPatchField<Type2>&
        );


    //- Destructor
    virtual ~pointPatchField<Type>()
    {}


    // Member functions

        // Access

            //- Return local objectRegistry
            const objectRegistry& db() const;

            //- Return size
            label size() const
            {
                return patch().size();
            }

            //- Return patch
            const pointPatch& patch() const
            {
                return patch_;
            }

            //- Return dimensioned internal field reference
            const DimensionedField<Type, pointMesh>&
            dimensionedInternalField() const
            {
                return internalField_;
            }

            //- Return internal field reference
            const Field<Type>& internalField() const
            {
                return internalField_;
            }

            //- Return true if this patch field fixes a value
            virtual bool fixesValue() const
            {
                return false;
            }

            //- Return true if this patch field is coupled
            virtual bool coupled() const
            {
                return false;
            }

            //- Return true if the boundary condition has already been updated
            bool updated() const
            {
                return updated_;
            }

            //- Return field created from appropriate internal field values
            tmp<Field<Type> > patchInternalField() const;

            //- Return field created from appropriate internal field values
            //  given internal field reference
            template<class Type1>
            tmp<Field<Type1> > patchInternalField
            (
                const Field<Type1>& iF
            ) const;

            //- Return field created from selected internal field values
            //  given internal field reference
            template<class Type1>
            tmp<Field<Type1> > patchInternalField
            (
                const Field<Type1>& iF,
                const labelList& meshPoints
            ) const;

            //- Given the internal field and a patch field,
            //  add the patch field to the internal field
            template<class Type1>
            void addToInternalField
            (
                Field<Type1>& iF,
                const Field<Type1>& pF
            ) const;

            //- Given the internal field and a patch field,
            //  add selected elements of the patch field to the internal field
            template<class Type1>
            void addToInternalField
            (
                Field<Type1>& iF,
                const Field<Type1>& pF,
                const labelList& points
            ) const;

            //- Given the internal field and a patch field,
            //  set the patch field in the internal field
            template<class Type1>
            void setInInternalField
            (
                Field<Type1>& iF,
                const Field<Type1>& pF,
                const labelList& meshPoints
            ) const;

            //- Given the internal field and a patch field,
            //  set the patch field in the internal field
            template<class Type1>
            void setInInternalField
            (
                Field<Type1>& iF,
                const Field<Type1>& pF
            ) const;

            //- Return the type of the calculated form of pointPatchField
            static const word& calculatedType();

            //- Return the constraint type this pointPatchField implements.
            virtual const word& constraintType() const
            {
                return word::null;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const pointPatchFieldMapper&
            )
            {}

            //- Reverse map the given pointPatchField onto this pointPatchField
            virtual void rmap
            (
                const pointPatchField<Type>&,
                const labelList&
            )
            {}


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            //  Sets Updated to true
            virtual void updateCoeffs()
            {
                updated_ = true;
            }

            //- Initialise evaluation of the patch field (do nothing)
            virtual void initEvaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            )
            {}

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );


        //- Write
        virtual void write(Ostream&) const;


    // Member operators

        virtual void operator=
        (
            const pointPatchField<Type>&
        )
        {}

        virtual void operator+=
        (
            const pointPatchField<Type>&
        )
        {}

        virtual void operator-=
        (
            const pointPatchField<Type>&
        )
        {}

        virtual void operator*=
        (
            const pointPatchField<scalar>&
        )
        {}

        virtual void operator/=
        (
            const pointPatchField<scalar>&
        )
        {}

        virtual void operator=(const Field<Type>&){}
        virtual void operator+=(const Field<Type>&){}
        virtual void operator-=(const Field<Type>&){}

        virtual void operator*=(const Field<scalar>&){}
        virtual void operator/=(const Field<scalar>&){}

        virtual void operator=(const Type&){}
        virtual void operator+=(const Type&){}
        virtual void operator-=(const Type&){}
        virtual void operator*=(const scalar){}
        virtual void operator/=(const scalar){}


        // Force an assignment irrespective of form of patch
        // By generic these do nothing unless the patch actually has boundary
        // values

            virtual void operator==
            (
                const pointPatchField<Type>&
            )
            {}

            virtual void operator==(const Field<Type>&){}
            virtual void operator==(const Type&){}


    // Ostream operator

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const pointPatchField<Type>&
        );
};


// This function is added as a hack to enable simple backward compatability
// with verions using referenceLevel in GeometicField
template<class Type>
const pointPatchField<Type>& operator+
(
    const pointPatchField<Type>& ppf,
    const Type&
)
{
    return ppf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pointPatchFieldFunctions.hpp"

#include "pointMesh.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
pointPatchField<Type>::pointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    patch_(p),
    internalField_(iF),
    updated_(false),
    patchType_(word::null)
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    patch_(p),
    internalField_(iF),
    updated_(false),
    patchType_(dict.lookupOrDefault<word>("patchType", word::null))
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
    const pointPatchField<Type>& ptf
)
:
    patch_(ptf.patch_),
    internalField_(ptf.internalField_),
    updated_(false),
    patchType_(ptf.patchType_)
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
    const pointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    patch_(ptf.patch_),
    internalField_(iF),
    updated_(false),
    patchType_(ptf.patchType_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const objectRegistry& pointPatchField<Type>::db() const
{
    return patch_.boundaryMesh().mesh()();
}


template<class Type>
void pointPatchField<Type>::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;

    if (patchType_.size())
    {
        os.writeKeyword("patchType") << patchType_
            << token::END_STATEMENT << nl;
    }
}


template<class Type>
tmp<Field<Type> > pointPatchField<Type>::patchInternalField() const
{
    return patchInternalField(internalField());
}


template<class Type>
template<class Type1>
tmp<Field<Type1> > pointPatchField<Type>::patchInternalField
(
    const Field<Type1>& iF,
    const labelList& meshPoints
) const
{
    // Check size
    if (iF.size() != internalField().size())
    {
        FatalErrorInFunction
            << "given internal field does not correspond to the mesh. "
            << "Field size: " << iF.size()
            << " mesh size: " << internalField().size()
            << abort(FatalError);
    }

    return tmp<Field<Type1> >(new Field<Type1>(iF, meshPoints));
}


template<class Type>
template<class Type1>
tmp<Field<Type1> > pointPatchField<Type>::patchInternalField
(
    const Field<Type1>& iF
) const
{
    return patchInternalField(iF, patch().meshPoints());
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::addToInternalField
(
    Field<Type1>& iF,
    const Field<Type1>& pF
) const
{
    // Check size
    if (iF.size() != internalField().size())
    {
        FatalErrorInFunction
            << "given internal field does not correspond to the mesh. "
            << "Field size: " << iF.size()
            << " mesh size: " << internalField().size()
            << abort(FatalError);
    }

    if (pF.size() != size())
    {
        FatalErrorInFunction
            << "given patch field does not correspond to the mesh. "
            << "Field size: " << pF.size()
            << " mesh size: " << size()
            << abort(FatalError);
    }

    // Get the addressing
    const labelList& mp = patch().meshPoints();

    forAll(mp, pointI)
    {
        iF[mp[pointI]] += pF[pointI];
    }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::addToInternalField
(
    Field<Type1>& iF,
    const Field<Type1>& pF,
    const labelList& points
) const
{
    // Check size
    if (iF.size() != internalField().size())
    {
        FatalErrorInFunction
            << "given internal field does not correspond to the mesh. "
            << "Field size: " << iF.size()
            << " mesh size: " << internalField().size()
            << abort(FatalError);
    }

    if (pF.size() != size())
    {
        FatalErrorInFunction
            << "given patch field does not correspond to the mesh. "
            << "Field size: " << pF.size()
            << " mesh size: " << size()
            << abort(FatalError);
    }

    // Get the addressing
    const labelList& mp = patch().meshPoints();

    forAll(points, i)
    {
        label pointI = points[i];
        iF[mp[pointI]] += pF[pointI];
    }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::setInInternalField
(
    Field<Type1>& iF,
    const Field<Type1>& pF,
    const labelList& meshPoints
) const
{
    // Check size
    if (iF.size() != internalField().size())
    {
        FatalErrorInFunction
            << "given internal field does not correspond to the mesh. "
            << "Field size: " << iF.size()
            << " mesh size: " << internalField().size()
            << abort(FatalError);
    }

    if (pF.size() != meshPoints.size())
    {
        FatalErrorInFunction
            << "given patch field does not correspond to the meshPoints. "
            << "Field size: " << pF.size()
            << " meshPoints size: " << size()
            << abort(FatalError);
    }

    forAll(meshPoints, pointI)
    {
        iF[meshPoints[pointI]] = pF[pointI];
    }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::setInInternalField
(
    Field<Type1>& iF,
    const Field<Type1>& pF
) const
{
    setInInternalField(iF, pF, patch().meshPoints());
}


template<class Type>
void pointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!updated_)
    {
        updateCoeffs();
    }

    updated_ = false;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<
(
    Ostream& os,
    const pointPatchField<Type>& ptf
)
{
    ptf.write(os);

    os.check("Ostream& operator<<(Ostream&, const pointPatchField<Type>&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::pointPatchField<Type> > CML::pointPatchField<Type>::New
(
    const word& patchFieldType,
    const word& actualPatchType,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
{
    if (debug)
    {
        Info<< "PointPatchField<Type>::"
               "New(const word&, const word&"
                ", const pointPatch&, const Field<Type>&) : "
               "constructing pointPatchField<Type>"
            << endl;
    }

    typename pointPatchConstructorTable::iterator cstrIter =
        pointPatchConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == pointPatchConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown patchFieldType type "
            << patchFieldType << nl << nl
            << "Valid patchField types are :" << endl
            << pointPatchConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    autoPtr<pointPatchField<Type> > pfPtr(cstrIter()(p, iF));

    if
    (
        actualPatchType == word::null
     || actualPatchType != p.type()
    )
    {
        if (pfPtr().constraintType() != p.constraintType())
        {
            // Use default constraint type
            typename pointPatchConstructorTable::iterator patchTypeCstrIter =
                pointPatchConstructorTablePtr_->find(p.type());

            if (patchTypeCstrIter == pointPatchConstructorTablePtr_->end())
            {
                FatalErrorInFunction
                    << "inconsistent patch and patchField types for \n"
                    << "    patch type " << p.type()
                    << " and patchField type " << patchFieldType
                    << exit(FatalError);
            }

            return patchTypeCstrIter()(p, iF);
        }
    }

    return pfPtr;
}


template<class Type>
CML::autoPtr<CML::pointPatchField<Type> > CML::pointPatchField<Type>::New
(
    const word& patchFieldType,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
{
    return New(patchFieldType, word::null, p, iF);
}


template<class Type>
CML::autoPtr<CML::pointPatchField<Type> > CML::pointPatchField<Type>::New
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
{
    if (debug)
    {
        Info<< "PointPatchField<Type>::"
               "New(const pointPatch&, const Field<Type>&, const dictionary&)"
               " : constructing pointPatchField<Type>"
            << endl;
    }

    word patchFieldType(dict.lookup("type"));

    typename dictionaryConstructorTable::iterator cstrIter
        = dictionaryConstructorTablePtr_->find(patchFieldType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        if (!disallowGenericPointPatchField)
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

    // Construct (but not necessarily returned)
    autoPtr<pointPatchField<Type> > pfPtr(cstrIter()(p, iF, dict));

    if
    (
       !dict.found("patchType")
     || word(dict.lookup("patchType")) != p.type()
    )
    {
        if (pfPtr().constraintType() == p.constraintType())
        {
            // Compatible (constraint-wise) with the patch type
            return pfPtr;
        }
        else
        {
            // Use default constraint type
            typename dictionaryConstructorTable::iterator patchTypeCstrIter
                = dictionaryConstructorTablePtr_->find(p.type());

            if (patchTypeCstrIter == dictionaryConstructorTablePtr_->end())
            {
                FatalIOErrorInFunction(dict)
                    << "inconsistent patch and patchField types for \n"
                    << "    patch type " << p.type()
                    << " and patchField type " << patchFieldType
                    << exit(FatalIOError);
            }

            return patchTypeCstrIter()(p, iF, dict);
        }
    }

    return cstrIter()(p, iF, dict);
}


// Return a pointer to a new patch created on freestore from
// a given pointPatchField<Type> mapped onto a new patch
template<class Type>
CML::autoPtr<CML::pointPatchField<Type> > CML::pointPatchField<Type>::New
(
    const pointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& pfMapper
)
{
    if (debug)
    {
        Info<< "PointPatchField<Type>::"
               "New(const pointPatchField<Type>&,"
               " const pointPatch&, const Field<Type>&, "
               "const pointPatchFieldMapper&) : "
               "constructing pointPatchField<Type>"
            << endl;
    }

    typename patchMapperConstructorTable::iterator cstrIter =
        patchMapperConstructorTablePtr_->find(ptf.type());

    if (cstrIter == patchMapperConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown patchField type "
            << ptf.type() << nl << nl
            << "Valid patchField types are :" << endl
            << patchMapperConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(ptf, p, iF, pfMapper);
}



#   include "calculatedPointPatchField.hpp"



#define addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)      \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        PatchTypeField,                                                       \
        typePatchTypeField,                                                   \
        pointPatch                                                            \
    );                                                                        \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        PatchTypeField,                                                       \
        typePatchTypeField,                                                   \
        patchMapper                                                           \
    );                                                                        \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        PatchTypeField,                                                       \
        typePatchTypeField,                                                   \
        dictionary                                                            \
    );


// for non-templated patch fields
#define makePointPatchTypeField(PatchTypeField,typePatchTypeField)            \
    defineTypeNameAndDebug(typePatchTypeField, 0);                            \
    addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


// for templated patch fields
#define makeTemplatePointPatchTypeField(PatchTypeField, typePatchTypeField)   \
    defineNamedTemplateTypeNameAndDebug(typePatchTypeField, 0);               \
    addToPointPatchFieldRunTimeSelection(PatchTypeField, typePatchTypeField)


#define makePointPatchFields(type)                                            \
    makeTemplatePointPatchTypeField                                           \
    (                                                                         \
        pointPatchScalarField,                                                \
        type##PointPatchScalarField                                           \
    );                                                                        \
    makeTemplatePointPatchTypeField                                           \
    (                                                                         \
        pointPatchVectorField,                                                \
        type##PointPatchVectorField                                           \
    );                                                                        \
    makeTemplatePointPatchTypeField                                           \
    (                                                                         \
        pointPatchSphericalTensorField,                                       \
        type##PointPatchSphericalTensorField                                  \
    );                                                                        \
    makeTemplatePointPatchTypeField                                           \
    (                                                                         \
        pointPatchSymmTensorField,                                            \
        type##PointPatchSymmTensorField                                       \
    );                                                                        \
    makeTemplatePointPatchTypeField                                           \
    (                                                                         \
        pointPatchTensorField,                                                \
        type##PointPatchTensorField                                           \
);


#define makePointPatchFieldsTypeName(type)                                    \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchScalarField, 0);      \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchVectorField, 0);      \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        type##PointPatchSphericalTensorField, 0                               \
    );                                                                        \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchSymmTensorField, 0);  \
    defineNamedTemplateTypeNameAndDebug(type##PointPatchTensorField, 0)


#define makePointPatchFieldTypedefs(type)                                     \
    typedef type##PointPatchField<scalar> type##PointPatchScalarField;        \
    typedef type##PointPatchField<vector> type##PointPatchVectorField;        \
    typedef type##PointPatchField<sphericalTensor>                            \
        type##PointPatchSphericalTensorField;                                 \
    typedef type##PointPatchField<symmTensor> type##PointPatchSymmTensorField;\
    typedef type##PointPatchField<tensor> type##PointPatchTensorField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
