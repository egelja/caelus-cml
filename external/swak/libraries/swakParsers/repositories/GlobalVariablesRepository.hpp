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
    CML::GlobalVariablesRepository

Description

SourceFiles
    GlobalVariablesRepository.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef GlobalVariablesRepository_H
#define GlobalVariablesRepository_H

#include "ExpressionResult.hpp"

#include "HashPtrTable.hpp"

#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class GlobalVariablesRepository Declaration
\*---------------------------------------------------------------------------*/

class GlobalVariablesRepository
:
    public regIOobject
{
public:
    class ResultTable
    :
    public HashPtrTable<ExpressionResult,word>
    {
    public:
        ResultTable(const ResultTable &r);
        ResultTable(Istream &);
        ResultTable();
    };
private:
   // Private data

    typedef HashTable<ResultTable,word> ResultTableTable;

    ResultTableTable globalVariables_;

    // Constructors

    //- Construct null
    GlobalVariablesRepository(const objectRegistry &obr);

    //- The only instance of this repository
    static GlobalVariablesRepository *repositoryInstance;

    static ExpressionResult zero_;

    ResultTable &getScope(const word &name);

    label lastTimeIndex_;

public:
     //- Runtime type information
    TypeName("GlobalVariablesRepository");

    //- Destructor

    virtual ~GlobalVariablesRepository();

    //- Get the Singleton of this class
    static GlobalVariablesRepository &getGlobalVariables(
        const objectRegistry &obr
    );

    //- get a global variable (if there is one)
    const ExpressionResult &get(
        const word &name,
        const wordList &scopes
    );

    ExpressionResult& addValue(
        const word &name,
        const word &scope,
        const ExpressionResult &value,
        const bool overwrite=true
    );

    bool removeValue(
        const word &name,
        const word &scope
    );

    ExpressionResult& addValue(
        const word &name,
        const word &scope,
        autoPtr<ExpressionResult> value,
        const bool overwrite=true
    );

    ExpressionResult& addValue(
        const dictionary &dict,
        const word scope="",
        const bool overwrite=true
    );

    //- get a global namespace
    ResultTable &getNamespace(const word &name);

    virtual bool writeData(Ostream &) const ;

    virtual bool readData(Istream &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
