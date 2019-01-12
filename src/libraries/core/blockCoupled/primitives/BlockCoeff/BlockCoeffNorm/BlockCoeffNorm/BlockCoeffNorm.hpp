/*---------------------------------------------------------------------------*\
Copyright (C) 2013 K. Jareteg
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    BlockCoeffNorm

Description
    Abstract class for calculating norm of a BlockCoeff.
    
    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    The original file contained "For copyright notice see file Copyright",
    but no file named Copyright could be found in FOAM-Extend-4.0 repository.

    The copyright has therefore been attributed to the authour named in the
    original file, as shown below.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Klas Jareteg, 2013-01-28

\*---------------------------------------------------------------------------*/

#ifndef BlockCoeffNorm_HPP
#define BlockCoeffNorm_HPP

#include "blockCoeffs.hpp"
#include "coeffFields.hpp"
#include "runTimeSelectionTables.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //


/*---------------------------------------------------------------------------*\
                        Class BlockCoeffNorm Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCoeffNorm
{

    //- Control data dictionary
    dictionary dict_;


protected:

    //- Return dictionary
    const dictionary& dict() const
    {
        return dict_;
    }


public:

    //- Runtime type information
    TypeName("BlockCoeffNorm");

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        BlockCoeffNorm,
        dictionary,
        (
            const dictionary& dict
        ),
        (
            dict
        )
    );


    //- Construct from dictionary
    BlockCoeffNorm
    (
        const dictionary& dict
    );


    // Selectors

    //- Return a new norm calculator
    static autoPtr<BlockCoeffNorm<Type> > New
    (
        const dictionary& dict
    );


    virtual ~BlockCoeffNorm()
    {}


    // Member functions

    //- Solve
    virtual scalar normalize
    (
        const BlockCoeff<Type>& a
    ) = 0;

    virtual void coeffMag
    (
        const CoeffField<Type>& a,
        Field<scalar>& b
    ) = 0;
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
BlockCoeffNorm<Type>::BlockCoeffNorm
(
    const dictionary& dict
)
:
    dict_(dict)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
autoPtr<typename CML::BlockCoeffNorm<Type> >
BlockCoeffNorm<Type>::New
(
    const dictionary& dict
)
{
    word normName(dict.lookup("norm"));

    typename dictionaryConstructorTable::iterator constructorIter =
        dictionaryConstructorTablePtr_->find(normName);

    if (constructorIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown norm " << normName
            << endl << endl
            << "Valid matrix norms are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<BlockCoeffNorm<Type> >
    (
        constructorIter()
        (
            dict
        )
    );
}


} // End namespace CML

#define makeBlockCoeffNorm(BlockCoeffNormType, typeBlockCoeffNormType)        \
                                                                              \
defineNamedTemplateTypeNameAndDebug(typeBlockCoeffNormType, 0);               \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    BlockCoeffNormType,                                                       \
    typeBlockCoeffNormType,                                                   \
    dictionary                                                                \
);


#define makeBlockCoeffNormHyperVector(type, Type, args...)                    \
                                                                              \
typedef BlockCoeffNorm<type > block##Type##CoeffNorm;                         \
defineNamedTemplateTypeNameAndDebug(block##Type##CoeffNorm, 0);               \
defineTemplateRunTimeSelectionTable(block##Type##CoeffNorm, dictionary);      \
                                                                              \
typedef BlockCoeffTwoNorm<type > block##Type##CoeffTwoNorm;                   \
makeBlockCoeffNorm(block##Type##CoeffNorm, block##Type##CoeffTwoNorm);        \
                                                                              \
typedef BlockCoeffComponentNorm<type > block##Type##CoeffComponentNorm;       \
makeBlockCoeffNorm(block##Type##CoeffNorm, block##Type##CoeffComponentNorm);  \
                                                                              \
typedef BlockCoeffMaxNorm<type > block##Type##CoeffMaxNorm;                   \
makeBlockCoeffNorm(block##Type##CoeffNorm, block##Type##CoeffMaxNorm);        \
                                                                              \

#endif
