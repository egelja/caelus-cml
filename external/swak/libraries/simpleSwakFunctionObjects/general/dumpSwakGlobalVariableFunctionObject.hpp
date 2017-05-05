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

Class
    dumpSwakGlobalVariableFunctionObject

Description
    Output a complete global variable into a line

SourceFiles
    dumpSwakGlobalVariableFunctionObject.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef dumpSwakGlobalVariableFunctionObject_H
#define dumpSwakGlobalVariableFunctionObject_H

#include "timelineFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class dumpSwakGlobalVariableFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class dumpSwakGlobalVariableFunctionObject
:
    public timelineFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    dumpSwakGlobalVariableFunctionObject(const dumpSwakGlobalVariableFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const dumpSwakGlobalVariableFunctionObject&);

    //- evaluate the expression
    template <class T>
    Field<T> evaluate(const word& fieldName,T preset) const;

    template<class Type>
    static void writeValue(Ostream &o,const Type &val,unsigned int &w);

    word globalScope_;

    word globalName_;

protected:

    word dirName();

    stringList columnNames();

    wordList fileNames();

    template<class T>
    void writeTheData(ExpressionResult &value);
public:

    //- Runtime type information
    TypeName("dumpSwakGlobalVariable");


    // Constructors

    //- Construct from components
    dumpSwakGlobalVariableFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    //- Do the writing
    virtual void write();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"
#include "GlobalVariablesRepository.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
void dumpSwakGlobalVariableFunctionObject::writeValue(
    Ostream &o,
    const scalar &val,
    unsigned int &w
)
{
    o << setw(w) << val;
}

template<class Type>
void dumpSwakGlobalVariableFunctionObject::writeValue(
    Ostream &o,
    const Type &val,
    unsigned int &w
)
{
    for(label j=0;j<Type::nComponents;j++) {
        o << setw(w) << val[j];
    }
}

template <class T>
void dumpSwakGlobalVariableFunctionObject::writeTheData(
    ExpressionResult &value
)
{
    Field<T> result(value.getResult<T>());

    if (Pstream::master()) {
        writeTime(name(),time().value());
        writeData(name(),result);
        endData(name());
    } else {
        Pout << "My data is lost because for dumpSwakGlobalVariableFunctionObject"
            << " only the masters data gets written" << endl;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
