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
    CML::coalCloudMassSourcePluginFunction

Description

SourceFiles
    coalCloudMassSourcePluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef coalCloudMassSourcePluginFunction_H
#define coalCloudMassSourcePluginFunction_H

#include "lcsMassSourcePluginFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class coalCloudMassSourcePluginFunction Declaration
\*---------------------------------------------------------------------------*/

class coalCloudMassSourcePluginFunction
:
    public lcsMassSourcePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const coalCloudMassSourcePluginFunction &);

    coalCloudMassSourcePluginFunction(const coalCloudMassSourcePluginFunction &);

protected:
    virtual autoPtr<dimScalarField> internalEvaluate();

public:

    coalCloudMassSourcePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    );

    virtual ~coalCloudMassSourcePluginFunction() {}

    TypeName("coalCloudMassSourcePluginFunction");

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
