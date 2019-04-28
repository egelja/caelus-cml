/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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
    2011-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "dumpSwakGlobalVariableFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(dumpSwakGlobalVariableFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        dumpSwakGlobalVariableFunctionObject,
        dictionary
    );



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dumpSwakGlobalVariableFunctionObject::dumpSwakGlobalVariableFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    timelineFunctionObject(name,t,dict),
    globalScope_(dict.lookup("globalScope")),
    globalName_(dict.lookup("globalName"))
{
    const string warnSwitch="IKnowThatThisFunctionObjectMayWriteExcessiveAmountsOfData";
    if(!dict.lookupOrDefault<bool>(warnSwitch,false)) {
        WarningInFunction
            << "This functionObject may write huge amounts of data. "
            << "If you understand the risks set the switch " << warnSwitch
            << " to 'true' to get rid of this warning"
            << endl;
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

word dumpSwakGlobalVariableFunctionObject::dirName()
{
    return typeName;
}

wordList dumpSwakGlobalVariableFunctionObject::fileNames()
{
    return wordList(1,name());
}

stringList dumpSwakGlobalVariableFunctionObject::columnNames()
{
    return stringList(1,"No way to know how much data will follow");
}

void dumpSwakGlobalVariableFunctionObject::write()
{

    if(verbose()) {
        Info << "Global " << name() << " : ";
    }

    ExpressionResult value(
        GlobalVariablesRepository::getGlobalVariables(
                obr_
        ).get(
            globalName_,
            wordList(1,globalScope_)
        )
    );

    word rType(value.valueType());

    if(rType==pTraits<scalar>::typeName) {
        writeTheData<scalar>(value);
    } else if(rType==pTraits<vector>::typeName) {
        writeTheData<vector>(value);
    } else if(rType==pTraits<tensor>::typeName) {
        writeTheData<tensor>(value);
    } else if(rType==pTraits<symmTensor>::typeName) {
        writeTheData<symmTensor>(value);
    } else if(rType==pTraits<sphericalTensor>::typeName) {
        writeTheData<sphericalTensor>(value);
    } else {
        WarningInFunction
            << "Don't know how to handle type " << rType
            << endl;
    }

    if(verbose()) {
        Info << endl;
    }
}

} // namespace CML

// ************************************************************************* //
