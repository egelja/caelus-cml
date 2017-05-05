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

Class
    CML::caelusTableReader

Description
    Reads an interpolation table from a file - CAELUS-format


\*---------------------------------------------------------------------------*/

#ifndef caelusTableReader_H
#define caelusTableReader_H

#include "tableReader.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class caelusTableReader Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class caelusTableReader
:
    public tableReader<Type>
{

public:

    //- Runtime type information
    TypeName("caelus");


    // Constructors

        //- Construct from dictionary
        caelusTableReader(const dictionary &dict);

        //- Construct and return a copy
        virtual autoPtr<tableReader<Type> > clone() const
        {
            return autoPtr<tableReader<Type> >
            (
                new caelusTableReader<Type>
                (
                    *this
                )
            );
        }


    //- Destructor
    virtual ~caelusTableReader();


    // Member functions

        //- Read the table
        virtual void operator()(const fileName&, List<Tuple2<scalar, Type> > &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IFstream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::caelusTableReader<Type>::caelusTableReader(const dictionary& dict)
:
    tableReader<Type>(dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::caelusTableReader<Type>::~caelusTableReader()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::caelusTableReader<Type>::operator()
(
    const fileName& fName,
    List<Tuple2<scalar, Type> >& data
)
{
    // Read data from file
    IFstream(fName)() >> data;
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
