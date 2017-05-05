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
    CML::Euler

Description
    Euler-implicit integration

\*---------------------------------------------------------------------------*/

#ifndef Euler_H
#define Euler_H

#include "IntegrationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Euler Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Euler
:
    public IntegrationScheme<Type>
{
public:

    //- Runtime type information
    TypeName("Euler");


    // Constructors

        //- Construct from components
        Euler(const word& phiName, const dictionary& dict);

        //- Copy constructor
        Euler(const Euler& is);

        //- Construct and return clone
        virtual autoPtr<IntegrationScheme<Type> > clone() const
        {
            return autoPtr<IntegrationScheme<Type> >(new Euler<Type>(*this));
        }


    //- Destructor
    virtual ~Euler();


    // Member Functions

        //- Perform the integration
        virtual typename IntegrationScheme<Type>::integrationResult integrate
        (
            const Type& phi,
            const scalar dt,
            const Type& alphaBeta,
            const scalar beta
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::Euler<Type>::Euler
(
    const word& phiName,
    const dictionary& dict
)
:
    IntegrationScheme<Type>(phiName, dict)
{}


template<class Type>
CML::Euler<Type>::Euler(const Euler& is)
:
    IntegrationScheme<Type>(is)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::Euler<Type>::~Euler()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::IntegrationScheme<Type>::integrationResult
CML::Euler<Type>::integrate
(
    const Type& phi,
    const scalar dt,
    const Type& alphaBeta,
    const scalar beta
) const
{
    typename IntegrationScheme<Type>::integrationResult retValue;
    retValue.value() = (phi + alphaBeta*dt)/(1.0 + beta*dt);
    retValue.average() = 0.5*(phi + retValue.value());

    return retValue;
}


#endif

// ************************************************************************* //
