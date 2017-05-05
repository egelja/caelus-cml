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
    CML::GeneralSetsPluginFunction

Description

SourceFiles
    GeneralSetsPluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef GeneralSetsPluginFunction_H
#define GeneralSetsPluginFunction_H

#include "FieldValuePluginFunction.hpp"

#include "runTimeSelectionTables.hpp"

#include "sampledSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class GeneralSetsPluginFunction Declaration
\*---------------------------------------------------------------------------*/
class GeneralSetsPluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const GeneralSetsPluginFunction &);

    GeneralSetsPluginFunction(const GeneralSetsPluginFunction &);

    word name_;

    autoPtr<labelList> meshCells_;

protected:

    inline const word &name() const {return name_; }

    const sampledSet &theSet() const;

public:
    GeneralSetsPluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &resultType,
        const string &arguments
    );

    virtual ~GeneralSetsPluginFunction() {}

    TypeName("GeneralSetsPluginFunction");

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
