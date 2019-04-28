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
    CML::ConcretePluginFunction

Description

SourceFiles
    ConcretePluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef ConcretePluginFunction_H
#define ConcretePluginFunction_H

#include "CommonPluginFunction.hpp"

#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class ConcretePluginFunction Declaration
\*---------------------------------------------------------------------------*/
template<class DriverType>
class ConcretePluginFunction
:
    public CommonPluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const ConcretePluginFunction &);

    ConcretePluginFunction(const ConcretePluginFunction &);

public:
    ConcretePluginFunction(
        const DriverType &parentDriver,
        const word &name,
        const word &returnType,
        const string &argumentSpecification
    );

    virtual ~ConcretePluginFunction() {}

    TypeName("ConcretePluginFunction");

    typedef DriverType driverType;

    declareRunTimeSelectionTable
    (
        autoPtr,
        ConcretePluginFunction,
        name,
        (
            const DriverType& driver,
            const word &name
        ),
        (driver,name)
    );

    static bool exists(
        const DriverType& driver,
        const word &name
    );

    static autoPtr<ConcretePluginFunction> New
    (
        const DriverType& driver,
        const word &name
    );
};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ConcretePluginFunction.hpp"

namespace CML {


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class DriverType>
ConcretePluginFunction<DriverType>::ConcretePluginFunction(
    const DriverType &parentDriver,
    const word &name,
    const word &returnType,
    const string &argumentSpecification
):
    CommonPluginFunction(
        parentDriver,
        name,
        returnType,
        argumentSpecification
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class DriverType>
autoPtr<ConcretePluginFunction<DriverType> > ConcretePluginFunction<DriverType>::New (
    const DriverType& driver,
    const word &name
)
{
    if(debug) {
        Info << "ConcretePluginFunction::New looking for "
            << name << " in "
            << nameConstructorTablePtr_->sortedToc()
            << endl;
    }
    if(nameConstructorTablePtr_==nullptr) {
        FatalErrorInFunction
            << "Constructor table of plugin functions for "
                << DriverType::typeName << " is not initialized"
                << endl
                << exit(FatalError);
        }
    typename nameConstructorTable::iterator cstrIter =
        nameConstructorTablePtr_->find(name);
    if(cstrIter==nameConstructorTablePtr_->end()) {
        FatalErrorInFunction
            << "Unknow plugin function " << name << endl
            << " Available functions are "
            << nameConstructorTablePtr_->sortedToc()
            << endl
            << exit(FatalError);
    }
    return autoPtr<ConcretePluginFunction>
        (
            cstrIter()(driver,name)
        );
}

template<class DriverType>
bool ConcretePluginFunction<DriverType>::exists (
    const DriverType& driver,
    const word &name
)
{
    static bool firstCall=true;
    if(firstCall) {
        firstCall=false;

        if(nameConstructorTablePtr_==nullptr) {
            WarningInFunction
                << "Constructor table of plugin functions for "
                << DriverType::typeName << " is not initialized"
                << endl;
            return false;
        }
        if(nameConstructorTablePtr_->size()>0) {
            Info<< endl << "Loaded plugin functions for '"+
                DriverType::typeName+"':" << endl;
            wordList names(
                nameConstructorTablePtr_->sortedToc()
            );
            forAll(names,nameI)
            {
                const word &theName=names[nameI];
                typename nameConstructorTable::iterator iter =
                    nameConstructorTablePtr_->find(theName);
                Info << "  " << theName << ":" << endl
                    << "    " << iter()(driver,theName)->helpText() << endl;
            }

            Info << endl;
        }
    }

    typename nameConstructorTable::iterator cstrIter =
        nameConstructorTablePtr_->find(name);

    return cstrIter!=nameConstructorTablePtr_->end();
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
