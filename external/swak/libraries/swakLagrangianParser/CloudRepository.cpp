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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "CloudRepository.hpp"

#include "polyMesh.hpp"
#include "meshSearch.hpp"

#include "ReaderParticleCloud.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

defineTypeNameAndDebug(CloudRepository, 0);

CloudRepository *CloudRepository::repositoryInstance(nullptr);

CloudRepository::CloudRepository(const IOobject &o)
    :
    RepositoryBase(o)
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CloudRepository::~CloudRepository()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CloudRepository &CloudRepository::getRepository(const objectRegistry &obr)
{
    CloudRepository*  ptr=repositoryInstance;

    if(debug) {
        Pout << "CloudRepository: asking for Singleton" << endl;
    }

    if(ptr==nullptr) {
        Pout << "swak: Allocating new repository for sampledSets\n";

        ptr=new CloudRepository(
            IOobject(
                "swakClouds",
                obr.time().timeName(),
                "cloudRepository",
                obr.time(),
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            )
        );
    }

    repositoryInstance=ptr;

    return *repositoryInstance;
}

bool CloudRepository::writeData(Ostream &f) const
{
    if(debug) {
        Info << "CloudRepository::write()" << endl;
    }

    f << "Non-updateable " << clouds_.toc() << endl;

    f << "Updateable " << updateableClouds_.toc() << endl;

    return true;
}

void CloudRepository::addUpdateableCloud(
    autoPtr<ReaderParticleCloud> c
) {
    const word &name=c->name();

    if(clouds_.found(name)) {
        FatalErrorInFunction
            << "There is already a cloud " << name
            << " in the non-updateable clouds. I guess there is a mistake"
            << endl
            << exit(FatalError);
    }

    if(updateableClouds_.found(name)) {
        FatalErrorInFunction
            << "Repository of updateable clouds already has an entry "
            << name << ". This can't be right"
            << endl
            << exit(FatalError);
    } else {
        updateableClouds_.insert(
            name,
            c.ptr()
        );
    }
}

void CloudRepository::addCloud(
    autoPtr<cloud> c
) {
    const word &name=c->name();

    if(updateableClouds_.found(name)) {
        FatalErrorInFunction
            << "There is already a cloud " << name
            << " in the updateable clouds. I guess there is a mistake"
            << endl
            << exit(FatalError);
    }

    if(clouds_.found(name)) {
        WarningInFunction
            << "Repository of clouds already has an entry "
            << name <<". Overwriting. Expect strange behaviour"
            << endl;
        clouds_.set(
            name,
            c.ptr()
        );
    } else {
        clouds_.insert(
            name,
            c.ptr()
        );
    }
}

void CloudRepository::updateRepo()
{
    clouds_.clear();

    typedef HashPtrTable<ReaderParticleCloud,word> updateTable;

    forAllIter(updateTable,updateableClouds_,it)
    {
        //	(*it)->clear();
        //        Info << "Updating " << it.key() << endl;
        (*it)->clearData();
        ReaderParticle::readFields(*(*it));
    }

}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
