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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "dynamicFunctionObjectListProxy.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(dynamicFunctionObjectListProxy, 0);
    addToRunTimeSelectionTable
    (
        functionObject,
        dynamicFunctionObjectListProxy,
        dictionary
    );

    typedef dynamicFunctionObjectListProxy::dynamicDictionaryProvider dynProvider;

    defineTypeNameAndDebug(dynProvider, 0);
    defineRunTimeSelectionTable(dynamicFunctionObjectListProxy::dynamicDictionaryProvider,dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dynamicFunctionObjectListProxy::dynamicFunctionObjectListProxy
(
    const word& name,
    const Time& t,
    const dictionary& dict,
    const char *providerNameStr
)
:
    functionObjectListProxy(
        name,
        t,
        dict,
        false
    )
{
    word providerName(providerNameStr);
    if(providerName.size()==0) {
        providerName=word(dict.lookup("dictionaryProvider"));
    }
    provider_=dynamicDictionaryProvider::New(
        providerName,
        dict,
        (*this)
    );

    if(
        readBool(dict.lookup("readDuringConstruction"))
    ) {
        if(writeDebug()) {
            Info << this->name() << " list initialized during construction" << endl;
        }
        read(dict);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void dynamicFunctionObjectListProxy::initFunctions()
{
    string text(provider_->getDictionaryText());
    if(Pstream::parRun()) {
        string localText=text;
        Pstream::scatter(text);
        if(text!=localText) {
            Pout << "WARNING: In dynamicFunctionObjectListProxy::initFunctions() "
                << "for " << name()
                << " the text of the dictionary is different from the master"
                << endl
                << " Overwritten local version with master";
        }
    }
    {
        fileName fName=obr_.time().path()/word(this->name()+".dictionaryText");
        OFstream o(fName);
        o << text.c_str();
    }

    IStringStream inStream(
        text
    );

    dynamicDict_.set(
        new dictionary(inStream)
    );

    {
        fileName fName=obr_.time().path()/word(this->name()+".dictionaryDump");
        OFstream o(fName);
        o << dynamicDict_();
    }

    if(!dynamicDict_->found("functions")) {
        FatalErrorInFunction
            << "Dictionary for" << this->name()
                << " does not have an entry 'functions'"
                << endl
                << exit(FatalError);

    }

    functions_.set(
        new functionObjectList(
            time(),
            dynamicDict_()
        )
    );

    if(writeDebug()) {
        Info << this->name() << " list initialized with "
            << functions_->size() << " FOs" << endl;
    }
}

dynamicFunctionObjectListProxy::dynamicDictionaryProvider::dynamicDictionaryProvider(
    const dictionary& dict,
    const dynamicFunctionObjectListProxy &owner
) :
    owner_(owner)
{
}

autoPtr<dynamicFunctionObjectListProxy::dynamicDictionaryProvider>
dynamicFunctionObjectListProxy::dynamicDictionaryProvider::New(
    const word& type,
    const dictionary& dict,
    const dynamicFunctionObjectListProxy &owner
){
    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(type);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown dynamicFunctionObjectListProxy::dynamicDictionaryProvider type " << type
            << endl << endl
            << "Valid types are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    if(debug) {
        Pout << "Creating dictionary provider of type " << type << endl;
    }

    return autoPtr<dynamicFunctionObjectListProxy::dynamicDictionaryProvider>
    (
        cstrIter()(dict,owner)
    );
}

} // namespace CML

// ************************************************************************* //
