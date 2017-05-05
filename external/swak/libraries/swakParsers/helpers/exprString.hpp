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
    CML::exprString

Description
    Subclass of the regular strings. Basically there to make sure
    that dictionary variables get expanded

SourceFiles
    exprString.C

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef exprString_H
#define exprString_H

#include "string.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class exprString Declaration
\*---------------------------------------------------------------------------*/

class CommonValueExpressionDriver;
class CommonPluginFunction;

class exprString
:
    public string
{
    // Private data

    bool isValid();

    // do not implement to make sure that exprString are explicitly constructed
    //    exprString(const string &);
    //    exprString(const std::string &);

protected:

    // this is to avoid to only let people convert this that know what they're doing
   static exprString toExpr(const string &);

public:

    exprString();

    exprString(const exprString &);

    exprString(const char *);

    exprString(
        Istream &,
        const dictionary &dict
    );

    exprString(
        const string &,
        const dictionary &
    );

   ~exprString();

    exprString &operator=(const string &s);

    friend class CommonValueExpressionDriver;
    friend class CommonPluginFunction;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
