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

#include "fromFileDictionaryProvider.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "IFstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(fromFileDictionaryProvider,0);

    // to keep the macro happy
    typedef dynamicFunctionObjectListProxy::dynamicDictionaryProvider dynamicFunctionObjectListProxydynamicDictionaryProvider;

    addToRunTimeSelectionTable 
    (
        dynamicFunctionObjectListProxydynamicDictionaryProvider,
        fromFileDictionaryProvider,
        dictionary
    );

    fromFileDictionaryProvider::fromFileDictionaryProvider(
        const dictionary& dict,
        const dynamicFunctionObjectListProxy &owner
    ):
        dynamicFunctionObjectListProxy::dynamicDictionaryProvider(
            dict,
            owner
        ),
        fileName_(dict.lookup("dictionaryFile"))
    {}
        

    string fromFileDictionaryProvider::getDictionaryText() {
        fileName fn=fileName_.expand();
        std::ifstream in(fn.c_str());
        if(!in.good()) {
            FatalErrorInFunction
                << "The file " << fn << " does not exist"
                    << endl
                    << exit(FatalError);

        }

        std::stringstream buffer;

        buffer << in.rdbuf();

        return buffer.str();
    }

} // namespace CML

// ************************************************************************* //
