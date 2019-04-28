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

#include "stdoutFromCommandProvider.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "IFstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(stdoutFromCommandProvider,0);

    // to keep the macro happy
    typedef dynamicFunctionObjectListProxy::dynamicDictionaryProvider dynamicFunctionObjectListProxydynamicDictionaryProvider;

    addToRunTimeSelectionTable 
    (
        dynamicFunctionObjectListProxydynamicDictionaryProvider,
        stdoutFromCommandProvider,
        dictionary
    );

    stdoutFromCommandProvider::stdoutFromCommandProvider(
        const dictionary& dict,
        const dynamicFunctionObjectListProxy &owner
    ):
        dynamicFunctionObjectListProxy::dynamicDictionaryProvider(
            dict,
            owner
        ),
        theCommand_(dict.lookup("theCommand"))
    {}
        

    string stdoutFromCommandProvider::getDictionaryText() {
        string cmd=theCommand_.expand();

        string buffer;
        FILE *output = popen(cmd.c_str(), "r");
        if(!output) {
            FatalErrorInFunction
                << "Problem executing " << cmd
                    << endl
                    << exit(FatalError);

        }
        int c=fgetc(output);
        while(c!=EOF) {
            buffer+=string(char(c));
            c=fgetc(output);
        }
        if(ferror(output)) {
            FatalErrorInFunction
                << "Problem while executing" << cmd
                    << endl
                    << exit(FatalError);

        }
        return buffer;
    }

} // namespace CML

// ************************************************************************* //
