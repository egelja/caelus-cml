/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::ContributionScalarPluginFunction

Description

SourceFiles
    ContributionScalarPluginFunction.C

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef ContributionScalarPluginFunction_H
#define ContributionScalarPluginFunction_H

#include "FieldValuePluginFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ContributionScalarPluginFunction Declaration
\*---------------------------------------------------------------------------*/

template <class Driver,class PluginType>
class ContributionScalarPluginFunction
:
    public PluginType
{
    //- Disallow default bitwise assignment
    void operator=(const ContributionScalarPluginFunction &);

    ContributionScalarPluginFunction(const ContributionScalarPluginFunction &);

    autoPtr<scalarField> values_;

    autoPtr<vectorField> positions_;

    string unitName_;

protected:

    void setArgument(
        label index,
        const string &content,
        const CommonValueExpressionDriver &driver
    );

    void setArgument(
        label index,
        const word &value
    );

    word getID(
        label index
    );

public:
    ContributionScalarPluginFunction(
        const typename PluginType::driverType &parentDriver,
        const word &name
    );

    virtual ~ContributionScalarPluginFunction() {}

    TypeName("ContributionScalarPluginFunction");

    void doEvaluation();

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
