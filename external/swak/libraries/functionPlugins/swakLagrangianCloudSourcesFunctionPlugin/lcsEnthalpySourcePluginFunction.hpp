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
    CML::lcsEnthalpySourcePluginFunction

Description

SourceFiles
    lcsEnthalpySourcePluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef lcsEnthalpySourcePluginFunction_H
#define lcsEnthalpySourcePluginFunction_H

#include "LagrangianCloudSourcePluginFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class lcsEnthalpySourcePluginFunction Declaration
\*---------------------------------------------------------------------------*/

class lcsEnthalpySourcePluginFunction
:
    public LagrangianCloudSourcePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const lcsEnthalpySourcePluginFunction &);

    lcsEnthalpySourcePluginFunction(const lcsEnthalpySourcePluginFunction &);

protected:
    typedef DimensionedField<scalar,volMesh> dimScalarField;

    virtual autoPtr<dimScalarField> internalEvaluate();

public:

    lcsEnthalpySourcePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    );

    virtual ~lcsEnthalpySourcePluginFunction() {}

    TypeName("lcsEnthalpySourcePluginFunction");

    void doEvaluation();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
