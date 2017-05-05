/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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
    CML::swakRegistryProxySet

Description
    A sampledSet that is only a Proxy for one found in the
    Set-registry of swak

SourceFiles
    swakRegistryProxySet.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakRegistryProxySet_H
#define swakRegistryProxySet_H

#include "sampledSet.hpp"

#include "SetsRepository.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class swakRegistryProxySet Declaration
\*---------------------------------------------------------------------------*/

class swakRegistryProxySet
:
    public sampledSet
{
    // Private data

    word setName_;

    const sampledSet &realSet() const;

    sampledSet &realSet();

public:

    //- Runtime type information
    TypeName("swakRegistryProxy");


    // Constructors

        //- Construct from dictionary
        swakRegistryProxySet
        (
            const word& name,
            const polyMesh& mesh,
            const meshSearch& search,
            const dictionary& dict
        );


    // Destructor

        virtual ~swakRegistryProxySet();


    // Member Functions

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
