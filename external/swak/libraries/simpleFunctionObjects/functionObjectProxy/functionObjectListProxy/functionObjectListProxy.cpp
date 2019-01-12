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

#include "functionObjectListProxy.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(functionObjectListProxy, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        functionObjectListProxy,
        dictionary
    );


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjectListProxy::functionObjectListProxy
(
    const word& name,
    const Time& t,
    const dictionary& dict,
    bool allowReadingDuringConstruction
)
:
    simpleFunctionObject(
        name,
        t,
        dict
    )
{
    if(
        allowReadingDuringConstruction
        &&
        !dict.found("functions")
    ) {
        FatalErrorInFunction
            << "No entry 'functions' in dictionary of " << name << endl
                << exit(FatalError);
    }
    if(
        allowReadingDuringConstruction
        &&
        readBool(dict.lookup("readDuringConstruction"))
    ) {
        if(writeDebug()) {
            Info << this->name() << " list initialized during construction" << endl;
        }
        //        initFunctions();
        read(dict);
    } else if(
        !allowReadingDuringConstruction
        &&
        readBool(dict.lookup("readDuringConstruction"))
    ) {
        WarningInFunction
            << "For " << name << " the 'readDuringConstruction'-variable "
                << " is ignored because this concrete class does not allow "
                << " constructing the functionObjects during construction"
                << endl;

    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void functionObjectListProxy::initFunctions()
{
    functions_.set(
        new functionObjectList(
            time(),
            dict_
        )
    );

    if(writeDebug()) {
        Info << this->name() << " list initialized with "
            << functions_->size() << " FOs" << endl;
    }
}

functionObjectList &functionObjectListProxy::functions()
{
    if(!functions_.valid()) {
        if(writeDebug()) {
            Info << this->name() << " list initialized on demand" << endl;
        }
        initFunctions();
    }

    return functions_();
}

bool functionObjectListProxy::execute(const bool forceWrite)
{
    if(writeDebug()) {
        Info << this->name() << " functionObjectListProxy::execute()" << endl;
    }

    return functions().execute(forceWrite);
}

bool functionObjectListProxy::start()
{
    if(writeDebug()) {
        Info << this->name() << " functionObjectListProxy::start()" << endl;
    }

    return functions().start();
}

bool functionObjectListProxy::end()
{
    if(writeDebug()) {
        Info << this->name() << " functionObjectListProxy::end()" << endl;
    }

    return functions().end();
}

bool functionObjectListProxy::read(const dictionary& dict)
{
    if(writeDebug()) {
        Info << this->name() << " functionObjectListProxy::read()" << endl;
    }

    return functions().read();
}

void functionObjectListProxy::write()
{
    // Don't want to be abstract
}

} // namespace CML

// ************************************************************************* //
