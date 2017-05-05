/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::SetSubsetValueExpressionDriver

Description

SourceFiles
    SetSubsetValueExpressionDriverI.hpp
    SetSubsetValueExpressionDriver.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SetSubsetValueExpressionDriver_H
#define SetSubsetValueExpressionDriver_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "SubsetValueExpressionDriver.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class SetSubsetValueExpressionDriver Declaration
\*---------------------------------------------------------------------------*/

class SetSubsetValueExpressionDriver
:
    public SubsetValueExpressionDriver
{
    // Private Member Functions

protected:

    const word id_;

    SetOrigin origin_;

public:

     //- Runtime type information
    TypeName("SetSubsetValueExpressionDriver");

    // Constructors

    //- Construct from components
    SetSubsetValueExpressionDriver(
        const word &id,
        const SetOrigin origin,
        bool autoInterpolate=false,
        bool warnAutoInterpolate=true
    );
    
    //- Copy construct
    SetSubsetValueExpressionDriver(const SetSubsetValueExpressionDriver&);

    //- Copy from a dictionary
    SetSubsetValueExpressionDriver(
        const dictionary&,
        const word &id,
        const SetOrigin origin
    );

    // Destructor

    ~SetSubsetValueExpressionDriver();


    // Member Functions


};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
