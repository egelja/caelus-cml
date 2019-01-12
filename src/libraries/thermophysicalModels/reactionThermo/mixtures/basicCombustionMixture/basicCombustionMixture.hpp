/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::basicCombustionMixture

Description
    Specialization of the basicSpecieMixture for combustion.

SourceFiles
    basicCombustionMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef basicCombustionMixture_HPP
#define basicCombustionMixture_HPP

#include "basicSpecieMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class basicCombustionMixture Declaration
\*---------------------------------------------------------------------------*/

class basicCombustionMixture
:
    public basicSpecieMixture
{

public:

    //- Run time type information
    TypeName("basicCombustionMixture");


    //- The base class of the mixture
    typedef basicCombustionMixture basicMixtureType;


    //- Construct from dictionary, specie names, mesh and phase name
    basicCombustionMixture
    (
        const dictionary&,
        const wordList& specieNames,
        const fvMesh&,
        const word&
    );


    //- Destructor
    virtual ~basicCombustionMixture()
    {}


    // Member functions

    inline scalar fres(const scalar ft, const scalar stoicRatio) const
    {
        return max(ft - (scalar(1) - ft)/stoicRatio, scalar(0));
    }

    inline tmp<volScalarField> fres
    (
        const volScalarField& ft,
        const dimensionedScalar& stoicRatio
    ) const
    {
        return max(ft - (scalar(1) - ft)/stoicRatio.value(), scalar(0));
    }
};


} // End namespace CML


#endif
