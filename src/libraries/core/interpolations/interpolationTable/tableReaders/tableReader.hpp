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
    CML::tableReader

Description
    Base class to read table data for the interpolationTable


\*---------------------------------------------------------------------------*/

#ifndef tableReader_H
#define tableReader_H

#include "fileName.hpp"
#include "wordList.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "autoPtr.hpp"
#include "dictionary.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class tableReader Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class tableReader
{

public:

    //- Runtime type information
    TypeName("tableReader");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            tableReader,
            dictionary,
            (const dictionary& dict),
            (dict)
        );


    // Constructors

        //- Construct from dictionary
        tableReader(const dictionary& dict);

        //- Construct and return a clone
        virtual autoPtr<tableReader<Type> > clone() const = 0;


    // Selectors

        //- Return a reference to the selected tableReader
        static autoPtr<tableReader> New(const dictionary& spec);


    //- Destructor
    virtual ~tableReader();


    // Member functions

        //- Read the table
        virtual void operator()
        (
            const fileName&,
            List<Tuple2<scalar, Type> >&
        ) = 0;

        //- Write additional information
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::tableReader<Type> > CML::tableReader<Type>::New
(
    const dictionary& spec
)
{
    const word readerType = spec.lookupOrDefault<word>
    (
        "readerType",
        "caelus"
    );

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_
            ->find(readerType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown reader type " << readerType
            << nl << nl
            << "Valid reader types : " << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<tableReader<Type> >(cstrIter()(spec));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::tableReader<Type>::tableReader(const dictionary&)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::tableReader<Type>::~tableReader()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::tableReader<Type>::write(Ostream& os) const
{
    if (this->type() != "caelus")
    {
        os.writeKeyword("readerType")
            << this->type() << token::END_STATEMENT << nl;
    }
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
