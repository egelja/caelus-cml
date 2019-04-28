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

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "exprString.hpp"

#include "CommonValueExpressionDriver.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

exprString::exprString()
:
    string()
{
    isValid();
}

exprString::exprString(const exprString &o)
:
    string(o)
{
    isValid();
}

// exprString::exprString(const string &o)
// :
//     string(o)
// {
//     isValid();
// }

// exprString::exprString(const std::string &o)
// :
//     string(o)
// {
//     isValid();
// }

exprString::exprString(const char *o)
:
    string(o)
{
    isValid();
}

exprString::exprString(
    Istream & in,
    const dictionary &dict
)
    :
    string(in)
{
    (*this)=CommonValueExpressionDriver::expandDictVariables(
        (*this),
        dict
    );
    isValid();
}

exprString::exprString(
    const string &in,
    const dictionary &dict
)
    :
    string(in)
{
    (*this)=CommonValueExpressionDriver::expandDictVariables(
        (*this),
        dict
    );
    isValid();
}

exprString::~exprString() {
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

exprString exprString::toExpr(const string &o)
{
    exprString e;

    e.string::operator=(o);

    return e;
}

exprString &exprString::operator=(const string &o)
{
    string::operator=(o);
    isValid();
    return *this;
}

bool exprString::isValid() {
    if(find('$')!=std::string::npos) {
        FatalErrorInFunction
            << "There is a '$' in " << *this
            << endl
            << exit(FatalError);

        return false;
    }
    return true;
}

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
