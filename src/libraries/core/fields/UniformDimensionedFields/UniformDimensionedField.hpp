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
    CML::UniformDimensionedField

Description
    Dimensioned<Type> registered with the database as a registered IOobject
    which has the functionality of a uniform field and allows values from the
    top-level code to be passed to boundary conditions etc.


\*---------------------------------------------------------------------------*/

#ifndef UniformDimensionedField_H
#define UniformDimensionedField_H

#include "regIOobject.hpp"
#include "dimensionedType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class UniformDimensionedField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class UniformDimensionedField
:
    public regIOobject,
    public dimensioned<Type>
{

public:

    //- Runtime type information
    TypeName("UniformDimensionedField");


    // Constructors

        //- Construct from components
        UniformDimensionedField(const IOobject&, const dimensioned<Type>&);

        //- Construct as copy
        UniformDimensionedField(const UniformDimensionedField<Type>&);

        //- Construct from Istream
        UniformDimensionedField(const IOobject&);


    //- Destructor
    virtual ~UniformDimensionedField();


    // Member Functions

        //- Name function provided to resolve the ambiguity between the
        //  name functions in regIOobject and dimensioned<Type>
        virtual const word& name() const
        {
            return dimensioned<Type>::name();
        }

        bool writeData(Ostream&) const;


    // Member Operators

        void operator=(const UniformDimensionedField<Type>&);
        void operator=(const dimensioned<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::UniformDimensionedField<Type>::UniformDimensionedField
(
    const IOobject& io,
    const dimensioned<Type>& dt
)
:
    regIOobject(io),
    dimensioned<Type>(dt)
{}


template<class Type>
CML::UniformDimensionedField<Type>::UniformDimensionedField
(
    const UniformDimensionedField<Type>& rdt
)
:
    regIOobject(rdt),
    dimensioned<Type>(rdt)
{}


template<class Type>
CML::UniformDimensionedField<Type>::UniformDimensionedField
(
    const IOobject& io
)
:
    regIOobject(io),
    dimensioned<Type>(regIOobject::name(), dimless, pTraits<Type>::zero)
{
    dictionary dict(readStream(typeName));
    this->dimensions().reset(dict.lookup("dimensions"));
    dict.lookup("value") >> this->value();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::UniformDimensionedField<Type>::~UniformDimensionedField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool CML::UniformDimensionedField<Type>::writeData(Ostream& os) const
{
    os.writeKeyword("dimensions") << this->dimensions() << token::END_STATEMENT
        << nl;
    os.writeKeyword("value") << this->value() << token::END_STATEMENT
        << nl << nl;

    return (os.good());
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

template<class Type>
void CML::UniformDimensionedField<Type>::operator=
(
    const UniformDimensionedField<Type>& rhs
)
{
    dimensioned<Type>::operator=(rhs);
}


template<class Type>
void CML::UniformDimensionedField<Type>::operator=
(
    const dimensioned<Type>& rhs
)
{
    dimensioned<Type>::operator=(rhs);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
