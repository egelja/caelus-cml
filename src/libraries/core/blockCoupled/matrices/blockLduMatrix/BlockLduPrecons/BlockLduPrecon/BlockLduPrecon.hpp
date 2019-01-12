/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2015 H. Jasak
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
    BlockLduPrecon

Description
    Block LDU matrix preconditioner virtual base class

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef BlockLduPrecon_HPP
#define BlockLduPrecon_HPP

#include "BlockLduMatrix.hpp"
#include "BlockNoPrecon.hpp"
#include "runTimeSelectionTables.hpp"
#include "addToRunTimeSelectionTable.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class BlockLduPrecon Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockLduPrecon
{

    //- Disallow default bitwise copy construct
    BlockLduPrecon(const BlockLduPrecon&);

    //- Disallow default bitwise assignment
    void operator=(const BlockLduPrecon&);


protected:

    //- Matrix reference
    const BlockLduMatrix<Type>& matrix_;


public:

    //- Runtime type information
    TypeName("BlockLduPrecon");

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        BlockLduPrecon,
        dictionary,
        (
            const BlockLduMatrix<Type>& matrix,
            const dictionary& dict
         ),
        (
            matrix,
            dict
        )
    );

    //- Construct from matrix
    BlockLduPrecon(const BlockLduMatrix<Type>& matrix)
    :
        matrix_(matrix)
    {}

    // Selectors
    //- Select given matrix and dictionary
    static autoPtr<BlockLduPrecon<Type> > New
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict,
        const word keyword = word("preconditioner")
    );

    virtual ~BlockLduPrecon()
    {}

    // Member Functions
    //- Execute preconditioning
    virtual void precondition
    (
        Field<Type>& x,
        const Field<Type>& b
    ) const = 0;

    //- Execute preconditioning on a transposed matrix
    virtual void preconditionT
    (
        Field<Type>& xT,
        const Field<Type>& bT
    ) const
    {
        NotImplemented;
    }

};


} // End namespace CML


template<class Type>
class BlockNoPrecon;

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::BlockLduPrecon<Type> > CML::BlockLduPrecon<Type>::New
(
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict,
    const word keyword
)
{
    word preconName;

    // Handle primitive or dictionary entry
    const entry& e = dict.lookupEntry(keyword, false, false);
    if (e.isDict())
    {
        e.dict().lookup(keyword) >> preconName;
    }
    else
    {
        e.stream() >> preconName;
    }

    const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;

    if (matrix.diagonal())
    {
        // No preconditioning for the diagonal matrix
        return autoPtr<BlockLduPrecon<Type> >
        (
            new BlockNoPrecon<Type>
            (
                matrix,
                controls
            )
        );
    }
    else
    {
        typename dictionaryConstructorTable::iterator constructorIter =
            dictionaryConstructorTablePtr_->find(preconName);

        if (constructorIter == dictionaryConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << "Unknown matrix preconditioner " << preconName
                << endl << endl
                << "Valid matrix preconditioners are :" << endl
                << dictionaryConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<BlockLduPrecon<Type> >
        (
            constructorIter()
            (
                matrix,
                controls
            )
        );
    }
}


#define makeBlockPrecon(PreconType, typePreconType)                           \
                                                                              \
defineNamedTemplateTypeNameAndDebug(typePreconType, 0);                       \
                                                                              \
addToRunTimeSelectionTable(PreconType, typePreconType, dictionary);


#define makeBlockPreconsHyperVector(type, Type, args...)                      \
                                                                              \
typedef BlockLduPrecon<type > block##Type##Precon;                            \
defineNamedTemplateTypeNameAndDebug(block##Type##Precon, 0);                  \
defineTemplateRunTimeSelectionTable(block##Type##Precon, dictionary);         \
                                                                              \
typedef BlockNoPrecon<type > block##Type##NoPrecon;                           \
makeBlockPrecon(block##Type##Precon, block##Type##NoPrecon);                  \
                                                                              \
typedef BlockDiagonalPrecon<type > block##Type##DiagonalPrecon;               \
makeBlockPrecon(block##Type##Precon, block##Type##DiagonalPrecon);            \
                                                                              \
typedef BlockGaussSeidelPrecon<type > block##Type##GaussSeidelPrecon;         \
makeBlockPrecon(block##Type##Precon, block##Type##GaussSeidelPrecon);         \
                                                                              \
typedef BlockCholeskyPrecon<type > block##Type##CholeskyPrecon;               \
makeBlockPrecon(block##Type##Precon, block##Type##CholeskyPrecon);            \



#endif
