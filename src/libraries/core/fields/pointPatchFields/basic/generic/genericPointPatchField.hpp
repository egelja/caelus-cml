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

Class
    CML::genericPointPatchField

Description
    A generic version of calculatedPointPatchField, useful as a fallback for
    handling unknown patch types.


\*---------------------------------------------------------------------------*/

#ifndef genericPointPatchField_H
#define genericPointPatchField_H

#include "calculatedPointPatchField.hpp"
#include "HashPtrTable.hpp"
#include "pointPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class genericPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class genericPointPatchField
:
    public calculatedPointPatchField<Type>
{
    // Private data

        word actualTypeName_;
        dictionary dict_;

        HashPtrTable<scalarField> scalarFields_;
        HashPtrTable<vectorField> vectorFields_;
        HashPtrTable<sphericalTensorField> sphericalTensorFields_;
        HashPtrTable<symmTensorField> symmTensorFields_;
        HashPtrTable<tensorField> tensorFields_;


public:

    //- Runtime type information
    TypeName("generic");


    // Constructors

        //- Construct from patch and internal field
        genericPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        genericPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        genericPointPatchField
        (
            const genericPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new genericPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        genericPointPatchField
        (
            const genericPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new genericPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const pointPatchFieldMapper&
            );

            //- Reverse map the given pointPatchField onto this pointPatchField
            virtual void rmap
            (
                const pointPatchField<Type>&,
                const labelList&
            );


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    calculatedPointPatchField<Type>(p, iF)
{
    NotImplemented;
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    calculatedPointPatchField<Type>(p, iF, dict),
    actualTypeName_(dict.lookup("type")),
    dict_(dict)
{
    forAllConstIter(dictionary, dict_, iter)
    {
        if (iter().keyword() != "type")
        {
            if
            (
                iter().isStream()
             && iter().stream().size()
            )
            {
                ITstream& is = iter().stream();

                // Read first token
                token firstToken(is);

                if
                (
                    firstToken.isWord()
                 && firstToken.wordToken() == "nonuniform"
                )
                {
                    token fieldToken(is);

                    if (!fieldToken.isCompound())
                    {
                        if
                        (
                            fieldToken.isLabel()
                         && fieldToken.labelToken() == 0
                        )
                        {
                            scalarFields_.insert
                            (
                                iter().keyword(),
                                new scalarField(0)
                            );
                        }
                        else
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    token following 'nonuniform' "
                                  "is not a compound"
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                            << exit(FatalIOError);
                        }
                    }
                    else if
                    (
                        fieldToken.compoundToken().type()
                     == token::Compound<List<scalar> >::typeName
                    )
                    {
                        scalarField* fPtr = new scalarField;
                        fPtr->transfer
                        (
                            dynamicCast<token::Compound<List<scalar> > >
                            (
                                fieldToken.transferCompoundToken(is)
                            )
                        );

                        if (fPtr->size() != this->size())
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    size of field " << iter().keyword()
                                << " (" << fPtr->size() << ')'
                                << " is not the same size as the patch ("
                                << this->size() << ')'
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                                << exit(FatalIOError);
                        }

                        scalarFields_.insert(iter().keyword(), fPtr);
                    }
                    else if
                    (
                        fieldToken.compoundToken().type()
                     == token::Compound<List<vector> >::typeName
                    )
                    {
                        vectorField* fPtr = new vectorField;
                        fPtr->transfer
                        (
                            dynamicCast<token::Compound<List<vector> > >
                            (
                                fieldToken.transferCompoundToken(is)
                            )
                        );

                        if (fPtr->size() != this->size())
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    size of field " << iter().keyword()
                                << " (" << fPtr->size() << ')'
                                << " is not the same size as the patch ("
                                << this->size() << ')'
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                                << exit(FatalIOError);
                        }

                        vectorFields_.insert(iter().keyword(), fPtr);
                    }
                    else if
                    (
                        fieldToken.compoundToken().type()
                     == token::Compound<List<sphericalTensor> >::typeName
                    )
                    {
                        sphericalTensorField* fPtr = new sphericalTensorField;
                        fPtr->transfer
                        (
                            dynamicCast
                            <
                                token::Compound<List<sphericalTensor> >
                            >
                            (
                                fieldToken.transferCompoundToken(is)
                            )
                        );

                        if (fPtr->size() != this->size())
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    size of field " << iter().keyword()
                                << " (" << fPtr->size() << ')'
                                << " is not the same size as the patch ("
                                << this->size() << ')'
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                                << exit(FatalIOError);
                        }

                        sphericalTensorFields_.insert(iter().keyword(), fPtr);
                    }
                    else if
                    (
                        fieldToken.compoundToken().type()
                     == token::Compound<List<symmTensor> >::typeName
                    )
                    {
                        symmTensorField* fPtr = new symmTensorField;
                        fPtr->transfer
                        (
                            dynamicCast
                            <
                                token::Compound<List<symmTensor> >
                            >
                            (
                                fieldToken.transferCompoundToken(is)
                            )
                        );

                        if (fPtr->size() != this->size())
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    size of field " << iter().keyword()
                                << " (" << fPtr->size() << ')'
                                << " is not the same size as the patch ("
                                << this->size() << ')'
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                                << exit(FatalIOError);
                        }

                        symmTensorFields_.insert(iter().keyword(), fPtr);
                    }
                    else if
                    (
                        fieldToken.compoundToken().type()
                     == token::Compound<List<tensor> >::typeName
                    )
                    {
                        tensorField* fPtr = new tensorField;
                        fPtr->transfer
                        (
                            dynamicCast<token::Compound<List<tensor> > >
                            (
                                fieldToken.transferCompoundToken(is)
                            )
                        );

                        if (fPtr->size() != this->size())
                        {
                            FatalIOErrorInFunction(dict)
                                << "\n    size of field " << iter().keyword()
                                << " (" << fPtr->size() << ')'
                                << " is not the same size as the patch ("
                                << this->size() << ')'
                                << "\n    on patch " << this->patch().name()
                                << " of field "
                                << this->dimensionedInternalField().name()
                                << " in file "
                                << this->dimensionedInternalField().objectPath()
                                << exit(FatalIOError);
                        }

                        tensorFields_.insert(iter().keyword(), fPtr);
                    }
                    else
                    {
                        FatalIOErrorInFunction(dict)
                            << "\n    compound " << fieldToken.compoundToken()
                            << " not supported"
                            << "\n    on patch " << this->patch().name()
                            << " of field "
                            << this->dimensionedInternalField().name()
                            << " in file "
                            << this->dimensionedInternalField().objectPath()
                            << exit(FatalIOError);
                    }
                }
            }
        }
    }
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
    const genericPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    calculatedPointPatchField<Type>(ptf, p, iF, mapper),
    actualTypeName_(ptf.actualTypeName_),
    dict_(ptf.dict_)
{
    forAllConstIter
    (
        HashPtrTable<scalarField>,
        ptf.scalarFields_,
        iter
    )
    {
        scalarFields_.insert
        (
            iter.key(),
            new scalarField(*iter(), mapper)
        );
    }

    forAllConstIter
    (
        HashPtrTable<vectorField>,
        ptf.vectorFields_,
        iter
    )
    {
        vectorFields_.insert
        (
            iter.key(),
            new vectorField(*iter(), mapper)
        );
    }

    forAllConstIter
    (
        HashPtrTable<sphericalTensorField>,
        ptf.sphericalTensorFields_,
        iter
    )
    {
        sphericalTensorFields_.insert
        (
            iter.key(),
            new sphericalTensorField(*iter(), mapper)
        );
    }

    forAllConstIter
    (
        HashPtrTable<symmTensorField>,
        ptf.symmTensorFields_,
        iter
    )
    {
        symmTensorFields_.insert
        (
            iter.key(),
            new symmTensorField(*iter(), mapper)
        );
    }

    forAllConstIter
    (
        HashPtrTable<tensorField>,
        ptf.tensorFields_,
        iter
    )
    {
        tensorFields_.insert
        (
            iter.key(),
            new tensorField(*iter(), mapper)
        );
    }
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
    const genericPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    calculatedPointPatchField<Type>(ptf, iF),
    actualTypeName_(ptf.actualTypeName_),
    dict_(ptf.dict_),
    scalarFields_(ptf.scalarFields_),
    vectorFields_(ptf.vectorFields_),
    sphericalTensorFields_(ptf.sphericalTensorFields_),
    symmTensorFields_(ptf.symmTensorFields_),
    tensorFields_(ptf.tensorFields_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void genericPointPatchField<Type>::autoMap
(
    const pointPatchFieldMapper& m
)
{
    forAllIter
    (
        HashPtrTable<scalarField>,
        scalarFields_,
        iter
    )
    {
        iter()->autoMap(m);
    }

    forAllIter
    (
        HashPtrTable<vectorField>,
        vectorFields_,
        iter
    )
    {
        iter()->autoMap(m);
    }

    forAllIter
    (
        HashPtrTable<sphericalTensorField>,
        sphericalTensorFields_,
        iter
    )
    {
        iter()->autoMap(m);
    }

    forAllIter
    (
        HashPtrTable<symmTensorField>,
        symmTensorFields_,
        iter
    )
    {
        iter()->autoMap(m);
    }

    forAllIter
    (
        HashPtrTable<tensorField>,
        tensorFields_,
        iter
    )
    {
        iter()->autoMap(m);
    }
}


template<class Type>
void genericPointPatchField<Type>::rmap
(
    const pointPatchField<Type>& ptf,
    const labelList& addr
)
{
    const genericPointPatchField<Type>& dptf =
        refCast<const genericPointPatchField<Type> >(ptf);

    forAllIter
    (
        HashPtrTable<scalarField>,
        scalarFields_,
        iter
    )
    {
        HashPtrTable<scalarField>::const_iterator dptfIter =
            dptf.scalarFields_.find(iter.key());

        if (dptfIter != scalarFields_.end())
        {
            iter()->rmap(*dptfIter(), addr);
        }
    }

    forAllIter
    (
        HashPtrTable<vectorField>,
        vectorFields_,
        iter
    )
    {
        HashPtrTable<vectorField>::const_iterator dptfIter =
            dptf.vectorFields_.find(iter.key());

        if (dptfIter != vectorFields_.end())
        {
            iter()->rmap(*dptfIter(), addr);
        }
    }

    forAllIter
    (
        HashPtrTable<sphericalTensorField>,
        sphericalTensorFields_,
        iter
    )
    {
        HashPtrTable<sphericalTensorField>::const_iterator dptfIter =
            dptf.sphericalTensorFields_.find(iter.key());

        if (dptfIter != sphericalTensorFields_.end())
        {
            iter()->rmap(*dptfIter(), addr);
        }
    }

    forAllIter
    (
        HashPtrTable<symmTensorField>,
        symmTensorFields_,
        iter
    )
    {
        HashPtrTable<symmTensorField>::const_iterator dptfIter =
            dptf.symmTensorFields_.find(iter.key());

        if (dptfIter != symmTensorFields_.end())
        {
            iter()->rmap(*dptfIter(), addr);
        }
    }

    forAllIter
    (
        HashPtrTable<tensorField>,
        tensorFields_,
        iter
    )
    {
        HashPtrTable<tensorField>::const_iterator dptfIter =
            dptf.tensorFields_.find(iter.key());

        if (dptfIter != tensorFields_.end())
        {
            iter()->rmap(*dptfIter(), addr);
        }
    }
}


template<class Type>
void genericPointPatchField<Type>::write(Ostream& os) const
{
    os.writeKeyword("type") << actualTypeName_ << token::END_STATEMENT << nl;

    forAllConstIter(dictionary, dict_, iter)
    {
        if (iter().keyword() != "type")
        {
            if
            (
                iter().isStream()
             && iter().stream().size()
             && iter().stream()[0].isWord()
             && iter().stream()[0].wordToken() == "nonuniform"
            )
            {
                if (scalarFields_.found(iter().keyword()))
                {
                    scalarFields_.find(iter().keyword())()
                        ->writeEntry(iter().keyword(), os);
                }
                else if (vectorFields_.found(iter().keyword()))
                {
                    vectorFields_.find(iter().keyword())()
                        ->writeEntry(iter().keyword(), os);
                }
                else if (sphericalTensorFields_.found(iter().keyword()))
                {
                    sphericalTensorFields_.find(iter().keyword())()
                        ->writeEntry(iter().keyword(), os);
                }
                else if (symmTensorFields_.found(iter().keyword()))
                {
                    symmTensorFields_.find(iter().keyword())()
                        ->writeEntry(iter().keyword(), os);
                }
                else if (tensorFields_.found(iter().keyword()))
                {
                    tensorFields_.find(iter().keyword())()
                        ->writeEntry(iter().keyword(), os);
                }
            }
            else
            {
               iter().write(os);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#endif

// ************************************************************************* //
