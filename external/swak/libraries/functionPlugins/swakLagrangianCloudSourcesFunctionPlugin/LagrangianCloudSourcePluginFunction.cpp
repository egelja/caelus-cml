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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "LagrangianCloudSourcePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "cloud_.hpp"
#include "IOmanip.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

LagrangianCloudSourcePluginFunction::LagrangianCloudSourcePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType,
    const string &additionalArgs
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        returnValueType,
        string("cloudName primitive word")+ (
            additionalArgs==""
            ?
            additionalArgs
            :
            string(string(",")+additionalArgs)
        )
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Ostream& LagrangianCloudSourcePluginFunction::listAvailableClouds(Ostream &o)
{
    o << nl << nl << "Available clouds in " << mesh().name() << endl;
    typedef HashTable<const cloud *> cloudTable;
    cloudTable clouds=mesh().lookupClass<cloud>();
    if(clouds.size()==0) {
        o << " No clouds available\n" << endl;
    }
    const label nameWidth=20;
    o << setw(nameWidth) << "Name" << " | " << "Type" << endl;
    o << "-------------------------------------------------------------" << endl;
    forAllConstIter(cloudTable,clouds,it) {
        o << setw(nameWidth) << it.key() << " | "
            << (*(*it)).type() << endl;
    }
    o << "-------------------------------------------------------------" << endl;
    if(mesh().foundObject<cloud>(cloudName())) {
        o << "Cloud " << cloudName() << " has type "
            << mesh().lookupObject<cloud>(cloudName()).type() << " typeid:"
            << typeid(mesh().lookupObject<cloud>(cloudName())).name()
            << endl;
    }
    return o;
}


void LagrangianCloudSourcePluginFunction::setArgument(
    label index,
    const word &name
)
{
    assert(index==0);
    cloudName_=name;
}


// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace

// ************************************************************************* //
