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
    CML::SurfacesRepository

Description

SourceFiles
    SurfacesRepositoryI.hpp
    SurfacesRepository.cpp
    SurfacesRepositoryIO.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SurfacesRepository_H
#define SurfacesRepository_H

#include "PtrMap.hpp"
#include "sampledSurface.hpp"
#include "RepositoryBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class SurfacesRepository Declaration
\*---------------------------------------------------------------------------*/

class SurfacesRepository
:
    public RepositoryBase
{
    // Private data

    //- the actual surfaces
    HashPtrTable<sampledSurface,word> surfaces_;

    //- names of the formats to be written
    HashTable<word,word> formatNames_;

    // Constructors

    //- Construct null
    SurfacesRepository(const IOobject &);

    //- The only instance of this repository
    static SurfacesRepository *repositoryInstance;

public:
     //- Runtime type information
    TypeName("SurfacesRepository");

    //- Destructor

    virtual ~SurfacesRepository();

    //- Get the Singleton of this class
    static SurfacesRepository &getRepository(const objectRegistry &mesh);

    //- get a surface using a dictionary (surface will be constructed if not existing)
    sampledSurface &getSurface(
        const dictionary &dict,
        const fvMesh &mesh
    );

    //- get a surface using its name (surface must exist)
    sampledSurface &getSurface(
        const word &name,
        const fvMesh &mesh
    );

    //- Make sure that this class is not abstract
    bool writeData(Ostream &) const;

    //- update repo at the start of a time-step
    void updateRepo();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
