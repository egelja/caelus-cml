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
    CML::RepositoryBase

Description
    Base for the repositories. Manly here to generate a list so that
    all repositories can be updated in one go

SourceFiles
    RepositoryBase.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef RepositoryBase_H
#define RepositoryBase_H

#include "DynamicList.hpp"
#include "objectRegistry.hpp"

#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class RepositoryBase Declaration
\*---------------------------------------------------------------------------*/

class RepositoryBase
:
    public regIOobject
{
    // Private data

    // Constructors

    //- List of the repositories
    static DynamicList<RepositoryBase *> allRepos_;

protected:

    //- Construct null
    RepositoryBase(const IOobject &);

public:
     //- Runtime type information
    TypeName("RepositoryBase");

    //- Destructor

    virtual ~RepositoryBase();

    //- Update all the repositories
    static void updateRepos();

    //- method to update this repo for a new timestep
    virtual void updateRepo() = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
