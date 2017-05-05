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
    CML::CloudsRepository

Description
    Store clouds that nobody else wants

SourceFiles
    CloudRepository.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudRepository_H
#define CloudRepository_H

#include "PtrMap.hpp"
#include "cloud_.hpp"
#include "RepositoryBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

    class ReaderParticleCloud;

/*---------------------------------------------------------------------------*\
                           Class CloudRepository Declaration
\*---------------------------------------------------------------------------*/

class CloudRepository
:
    public RepositoryBase
{
    // Private data

    //- the actual sets
    HashPtrTable<cloud,word> clouds_;

    //- Clouds that can be updated
    HashPtrTable<ReaderParticleCloud,word> updateableClouds_;

   // Constructors

    //- Construct null
    CloudRepository(const IOobject &);

    //- The only instance of this repository
    static CloudRepository *repositoryInstance;

public:
     //- Runtime type information
    TypeName("CloudRepository");

    //- Destructor

    virtual ~CloudRepository();

    //- Get the Singleton of this class
    static CloudRepository &getRepository(const objectRegistry &mesh);

    void addCloud(
        autoPtr<cloud> c
    );

    void addUpdateableCloud(
        autoPtr<ReaderParticleCloud> c
    );

    //- Make sure that this class is not abstract. Write sets
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
