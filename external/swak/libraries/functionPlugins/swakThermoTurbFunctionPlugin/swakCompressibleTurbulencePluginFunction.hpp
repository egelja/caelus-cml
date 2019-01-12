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
    CML::swakCompressibleTurbulencePluginFunction

Description

SourceFiles
    swakCompressibleTurbulencePluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakCompressibleTurbulencePluginFunction_H
#define swakCompressibleTurbulencePluginFunction_H

#include "swakThermophysicalPluginFunction.hpp"

#include "compressibleTurbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakCompressibleTurbulencePluginFunction Declaration
\*---------------------------------------------------------------------------*/
class swakCompressibleTurbulencePluginFunction
:
    public swakThermophysicalPluginFunction<swakFluidThermoType>
{
    //- Disallow default bitwise assignment
    void operator=(const swakCompressibleTurbulencePluginFunction &);

    swakCompressibleTurbulencePluginFunction(const swakCompressibleTurbulencePluginFunction &);

protected:
    static const compressible::turbulenceModel &turbInternal(const fvMesh &reg);

    const compressible::turbulenceModel &turb();

public:
    swakCompressibleTurbulencePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &returnValueType
    );

    virtual ~swakCompressibleTurbulencePluginFunction() {}

    TypeName("swakCompressibleTurbulencePluginFunction");

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
