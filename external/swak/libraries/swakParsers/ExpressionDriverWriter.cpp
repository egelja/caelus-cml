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
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "ExpressionDriverWriter.hpp"

#include "CommonValueExpressionDriver.hpp"

namespace CML {
 
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

defineTypeNameAndDebug(ExpressionDriverWriter, 0);

ExpressionDriverWriter::ExpressionDriverWriter(
        const word &name,
        CommonValueExpressionDriver &driver
):
    regIOobject(
        IOobject(
            name,
            driver.mesh().time().timeName(),
            "swak",
            driver.mesh().time(),
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        )
    ),
    driver_(driver)
{
    if(debug) {
        Pout << "ExpressionDriverWriter at " << objectPath() << " created" << endl;
    }

    if(headerOk()) {
        if(debug) {
            Pout << "Found a file " <<  objectPath() << endl;
        }

        readData(readStream("ExpressionDriverWriter"));
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

ExpressionDriverWriter::~ExpressionDriverWriter()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool ExpressionDriverWriter::writeData(Ostream &os) const 
{
    if(debug) {
        Pout << "ExpressionDriverWriter at " << objectPath() 
            << " writing" << endl;
    }

    dictionary dict;

    driver_.prepareData(dict);

    dict.write(os,false);

    if(debug) {
        Pout << "written " << dict << endl;
    }

    return os.good();    
}

bool ExpressionDriverWriter::readData(Istream &is)
{
    if(debug) {
        Pout << "ExpressionDriverWriter at " << objectPath() 
            << " reading" << endl;
    }

    const dictionary dict(is);

    if(debug) {
        Pout << "reading " << dict << endl;
    }

    driver_.getData(dict);

    return !is.bad();
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
