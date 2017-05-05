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
    CML::swakIncompressibleTurbulencePluginFunction

Description

SourceFiles
    swakIncompressibleTurbulencePluginFunction.C

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakIncompressibleTurbulencePluginFunction_H
#define swakIncompressibleTurbulencePluginFunction_H

#include "swakTransportModelsPluginFunction.hpp"

#include "turbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakIncompressibleTurbulencePluginFunction Declaration
\*---------------------------------------------------------------------------*/
class swakIncompressibleTurbulencePluginFunction
:
    public swakTransportModelsPluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const swakIncompressibleTurbulencePluginFunction &);

    swakIncompressibleTurbulencePluginFunction(const swakIncompressibleTurbulencePluginFunction &);

protected:
    static const incompressible::turbulenceModel &turbInternal(const fvMesh &reg);

    const incompressible::turbulenceModel &turb();

public:
    swakIncompressibleTurbulencePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &returnValueType
    );

    virtual ~swakIncompressibleTurbulencePluginFunction() {}

    TypeName("swakIncompressibleTurbulencePluginFunction");

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
