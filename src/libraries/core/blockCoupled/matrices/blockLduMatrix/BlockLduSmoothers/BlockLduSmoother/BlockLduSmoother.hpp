/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2015 H. Jasak
Copyright (C) 2017-2018 Applied CCM Pty Ltd
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
    BlockLduSmoother

Description
    Block LDU matrix smoother virtual base class

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak
    Darrin Stephens

SourceFiles
    blockLduSmoothers.cpp

\*---------------------------------------------------------------------------*/

#ifndef BlockLduSmoother_HPP
#define BlockLduSmoother_HPP

#include "BlockLduMatrix.hpp"
#include "scalarBlockConstraint.hpp"
#include "runTimeSelectionTables.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class BlockLduSmoother Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockLduSmoother
{

    //- Disallow default bitwise copy construct
    BlockLduSmoother(const BlockLduSmoother&);

    //- Disallow default bitwise assignment
    void operator=(const BlockLduSmoother&);


protected:

    //- Matrix reference
    const BlockLduMatrix<Type>& matrix_;


public:

    //- Runtime type information
    TypeName("BlockLduSmoother");

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        BlockLduSmoother,
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
    BlockLduSmoother(const BlockLduMatrix<Type>& matrix)
    :
        matrix_(matrix)
    {}

    // Selectors
    //- Select given matrix and dictionary
    static autoPtr<BlockLduSmoother<Type> > New
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict,
        const word keyword = word("smoother")
    );

    virtual ~BlockLduSmoother()
    {}

    // Member Functions
    //- Execute smoothing
    virtual void smooth
    (
        Field<Type>& x,
        const Field<Type>& b,
        const label nSweeps
    ) const = 0;
};


} // End namespace CML


template<class Type>
class BlockNoSmoother;

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::BlockLduSmoother<Type> > CML::BlockLduSmoother<Type>::New
(
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict,
    const word keyword
)
{
    word smootherName;

    // Handle primitive or dictionary entry
    const entry& e = dict.lookupEntry(keyword, false, false);
    if (e.isDict())
    {
        e.dict().lookup(keyword) >> smootherName;
    }
    else
    {
        e.stream() >> smootherName;
    }

    const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;

    typename dictionaryConstructorTable::iterator constructorIter =
        dictionaryConstructorTablePtr_->find(smootherName);

    if (constructorIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown matrix smoother " << smootherName
            << endl << endl
            << "Valid matrix smoothers are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<BlockLduSmoother<Type> >
    (
        constructorIter()
        (
            matrix,
            controls
        )
    );
}


#define makeBlockSmoother(SmootherType, typeSmootherType)                     \
                                                                              \
defineNamedTemplateTypeNameAndDebug(typeSmootherType, 0);                     \
                                                                              \
addToRunTimeSelectionTable(SmootherType, typeSmootherType, dictionary);


#define makeBlockSmootherHyperVector(type, Type, args...)                     \
                                                                              \
typedef BlockLduSmoother<type > block##Type##Smoother;                        \
defineNamedTemplateTypeNameAndDebug(block##Type##Smoother, 0);                \
defineTemplateRunTimeSelectionTable(block##Type##Smoother, dictionary);       \
                                                                              \
typedef BlockGaussSeidelSmoother<type > block##Type##GaussSeidelSmoother;     \
makeBlockSmoother(block##Type##Smoother, block##Type##GaussSeidelSmoother);   \
                                                                              \
typedef BlockILUSmoother<type > block##Type##ILUSmoother;                     \
makeBlockSmoother(block##Type##Smoother, block##Type##ILUSmoother);           \
                                                                              \

#endif
