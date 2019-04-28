/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "cellLimitedGrad.hpp"
#include "BJGradientLimiter.hpp"
#include "DLGradientLimiter.hpp"
#include "MGGradientLimiter.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeNamedFvLimitedGradTypeScheme(SS, Type, Limiter, Name)              \
    typedef CML::fv::SS<CML::Type, CML::fv::gradientLimiters::Limiter>      \
        SS##Type##Limiter##_;                                                  \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        SS##Type##Limiter##_,                                                  \
        Name,                                                                  \
        0                                                                      \
    );                                                                         \
                                                                               \
    namespace CML                                                             \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            gradScheme<Type>::addIstreamConstructorToTable                     \
            <                                                                  \
                SS<Type, gradientLimiters::Limiter>                            \
            > add##SS##Type##Limiter##IstreamConstructorToTable_;              \
        }                                                                      \
    }

#define makeFvLimitedGradTypeScheme(SS, Type, Limiter)                         \
    makeNamedFvLimitedGradTypeScheme(SS##Grad, Type, Limiter, #SS"<"#Limiter">")

#define makeFvLimitedGradScheme(SS, Limiter)                                   \
                                                                               \
    makeFvLimitedGradTypeScheme(SS, scalar, Limiter)                           \
    makeFvLimitedGradTypeScheme(SS, vector, Limiter)


// Default limiter in BJ specified without the limiter name
// for backward compatibility
makeNamedFvLimitedGradTypeScheme(cellLimitedGrad, scalar, BJ, "cellLimited")
makeNamedFvLimitedGradTypeScheme(cellLimitedGrad, vector, BJ, "cellLimited")

makeFvLimitedGradScheme(cellLimited, DL)
makeFvLimitedGradScheme(cellLimited, MG)

// ************************************************************************* //
