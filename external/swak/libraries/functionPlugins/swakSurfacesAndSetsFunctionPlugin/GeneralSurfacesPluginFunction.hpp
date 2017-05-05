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
    CML::GeneralSurfacesPluginFunction

Description

SourceFiles
    GeneralSurfacesPluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef GeneralSurfacesPluginFunction_H
#define GeneralSurfacesPluginFunction_H

#include "FieldValuePluginFunction.hpp"

#include "runTimeSelectionTables.hpp"

#include "sampledSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class GeneralSurfacesPluginFunction Declaration
\*---------------------------------------------------------------------------*/
class GeneralSurfacesPluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const GeneralSurfacesPluginFunction &);

    GeneralSurfacesPluginFunction(const GeneralSurfacesPluginFunction &);

    word name_;

    autoPtr<labelList> meshCells_;

protected:

    inline const word &name() const {return name_; }

    const sampledSurface &theSurface() const;

    const labelList &meshCells() ;

public:
    GeneralSurfacesPluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &resultType,
        const string &arguments
    );

    virtual ~GeneralSurfacesPluginFunction() {}

    TypeName("GeneralSurfacesPluginFunction");

    void setArgument(
        label index,
        const word &content
    );

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
