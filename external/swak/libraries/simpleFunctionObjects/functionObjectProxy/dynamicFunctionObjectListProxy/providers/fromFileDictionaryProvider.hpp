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
    fromFileDictionaryProvider

Description
    Reads text from a file and uses it as the dictionary text

SourceFiles
    fromFileDictionaryProvider.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef fromFileDictionaryProvider_H
#define fromFileDictionaryProvider_H

#include "dynamicFunctionObjectListProxy.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class fromFileDictionaryProvider Declaration
\*---------------------------------------------------------------------------*/

class fromFileDictionaryProvider
:
    public dynamicFunctionObjectListProxy::dynamicDictionaryProvider
{
    
    fileName fileName_;

public:
    
    TypeName("fromFileDictionaryProvider");

    fromFileDictionaryProvider(
        const dictionary&,
        const dynamicFunctionObjectListProxy &
    );

    string getDictionaryText();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
