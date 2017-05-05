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
    CML::Analytical

Description
    Analytical integration

\*---------------------------------------------------------------------------*/

#ifndef Analytical_H
#define Analytical_H

#include "IntegrationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class Analytical Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Analytical
:
    public IntegrationScheme<Type>
{
public:

    //- Runtime type information
    TypeName("analytical");


    // Constructors

        //- Construct from components
        Analytical(const word& phiName, const dictionary& dict);

        //- Copy constructor
        Analytical(const Analytical& is);

        //- Construct and return clone
        virtual autoPtr<IntegrationScheme<Type> > clone() const
        {
            return autoPtr<IntegrationScheme<Type> >
            (
                new Analytical<Type>(*this)
            );
        }


    //- Destructor
    virtual ~Analytical();


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
CML::Analytical<Type>::Analytical
(
    const word& phiName,
    const dictionary& dict
)
:
    IntegrationScheme<Type>(phiName, dict)
{}


template<class Type>
CML::Analytical<Type>::Analytical(const Analytical& is)
:
    IntegrationScheme<Type>(is)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::Analytical<Type>::~Analytical()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::IntegrationScheme<Type>::integrationResult
CML::Analytical<Type>::integrate
(
    const Type& phi,
    const scalar dt,
    const Type& alphaBeta,
    const scalar beta
) const
{
    typename IntegrationScheme<Type>::integrationResult retValue;

    const scalar expTerm = exp(min(50, -beta*dt));

    if (beta > ROOTVSMALL)
    {
        const Type alpha = alphaBeta/beta;
        retValue.average() = alpha + (phi - alpha)*(1 - expTerm)/(beta*dt);
        retValue.value() =  alpha + (phi - alpha)*expTerm;
    }
    else
    {
        retValue.average() = phi;
        retValue.value() =  phi;
    }


    return retValue;
}


#endif

// ************************************************************************* //
