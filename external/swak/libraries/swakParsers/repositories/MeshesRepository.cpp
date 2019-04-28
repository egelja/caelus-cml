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
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "MeshesRepository.hpp"

#include "polyMesh.hpp"
#include "meshSearch.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

defineTypeNameAndDebug(MeshesRepository, 0);

MeshesRepository *MeshesRepository::repositoryInstance(nullptr);

MeshesRepository::MeshesRepository()
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

MeshesRepository::~MeshesRepository()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

MeshesRepository &MeshesRepository::getRepository()
{
    MeshesRepository*  ptr=repositoryInstance;

    if(debug) {
        Pout << "MeshesRepository: asking for Singleton" << endl;
    }

    if(ptr==nullptr) {
        Pout << "swak: Allocating new repository for sampledMeshes\n";

        ptr=new MeshesRepository();
    }

    repositoryInstance=ptr;

    return *repositoryInstance;
}

fvMesh &MeshesRepository::addMesh(
    const word &name,
    const fileName &caseN,
    const word &region
)
{
    fileName usedN=caseN;
    usedN.expand();

    if(debug) {
        Pout << "MeshesRepository: Adding" << name
            << " (assuming it does not exist)" << endl;
        Pout << "Location: " << usedN << " Region " << region << endl;
    }

    typedef  HashPtrTable<Time,word> tableIterator;

    forAllIter(
        tableIterator,
        times_,
        iter
    ) {
        const Time &time=(*iter());
        fileName meshPath=time.rootPath()/time.caseName();
        word meshRegion=time.dbDir();
        if(meshRegion=="") {
            meshRegion=polyMesh::defaultRegion;
        }
        if(debug) {
            Info << iter.key() << ": " << meshPath << " region: "
                << time.dbDir() << endl;
        }
        if(
            usedN==meshPath
            &&
            region==meshRegion
        ) {
            if(iter.key()==name) {
                if(debug) {
                    Info << "Mesh already present under the name"
                        << name << endl;
                }
                return *meshes_[name];
            }
            FatalErrorInFunction
                << "Mesh of case " << usedN << " region " << region
                    << " is already in the repository under the name "
                    << iter.key()
                    << endl
                    << exit(FatalError);

        }
    }

    if(meshes_.found(name)) {
        FatalErrorInFunction
            << "There is already an entry " << name
                << " in the repository"
                << endl
                << exit(FatalError);

    }

    times_.insert(
        name,
        new Time(
            Time::controlDictName,
            usedN.path(),
            fileName(usedN.name())
        )
    );
    meshes_.insert(
        name,
        new fvMesh(
            IOobject(
                region,
                (*times_[name]).timeName(),
                (*times_[name]),
                CML::IOobject::MUST_READ
            )
        )
    );

    return *meshes_[name];
}

fvMesh &MeshesRepository::addCoupledMesh(
    const word &name,
    const word &masterName,
    const word &region
)
{
    if(meshes_.found(name)) {
        FatalErrorInFunction
            << "There is already an entry " << name
                << " in the repository"
                << endl
                << exit(FatalError);
    }
    if(!meshes_.found(masterName)) {
        FatalErrorInFunction
            << "There is no entry " << masterName
                << " in the repository"
                << endl
                << "Available keys:" << nl << meshes_.sortedToc()
                << exit(FatalError);
    }

    fvMesh &master=*meshes_[masterName];
    Time &time=const_cast<Time&>(master.time());

    meshes_.insert(
        name,
        new fvMesh(
            IOobject(
                region,
                time.timeName(),
                time,
                CML::IOobject::MUST_READ
            )
        )
    );

    return *meshes_[name];
}

fvMesh &MeshesRepository::getMesh(
    const word &name
)
{
    return *meshes_[name];
}

bool MeshesRepository::hasMesh(
    const word &name
)
{
    return meshes_.found(name);
}

meshToMesh &MeshesRepository::getMeshToMesh(
    const word &name,
    const fvMesh &mesh
)
{
    if(!meshInterpolations_.found(name)) {
         meshInterpolations_.insert(
             name,
             new meshToMesh(
                 *meshes_[name],
                 mesh
                 ,getInterpolationOrder(name)
             )
         );
    }

    return *meshInterpolations_[name];
}

scalar MeshesRepository::setTime(
    const word &name,
    const string &time,
    label timeIndex
) {
    IStringStream tStream(time);

    return setTime(
        name,
        readScalar(tStream),
        timeIndex
    );
}

scalar MeshesRepository::setTime(
    const word &name,
    scalar time,
    label timeI
) {
    if(debug) {
        Info << "Setting mesh " << name << " to t=" << time << endl;
    }
    fvMesh &mesh=getMesh(name);

    instant iTime=mesh.time().findClosestTime(time);
    if(debug) {
        Info << "Found time " << iTime << " as an approximation for t="
            << time << endl;
    }

    // const_cast<Time&>(mesh.time()).setTime(
    //     iTime,
    //     timeI
    // );

    Time &theTime=*(times_[name]);
    word oldTime=theTime.timeName();

    theTime.setTime(
        iTime,
        timeI
    );

    if(oldTime!=theTime.timeName()) {
        mesh.readUpdate();
        mesh.readModifiedObjects();

        typedef HashTable<regIOobject*> regIOTable;

        regIOTable content(mesh.lookupClass<regIOobject>());

        forAllIter(regIOTable,content,iter) {
            word newInstance=theTime.findInstance(
                (*iter)->local(),
                iter.key(),
                IOobject::READ_IF_PRESENT
            );
            if(newInstance==theTime.timeName()) {
                // Deregister the field. Don't know a better way to ensure it is properly reread
                if(debug) {
                    Info << "Deregistering " << iter.key() << endl;
                }
                regIOobject &obj=const_cast<regIOobject&>(*(*iter));
                mesh.polyMesh::checkOut(obj);
            }
        }
   }
    return mesh.time().value();
}

void MeshesRepository::setInterpolationOrder(
    const word &name,
    meshToMeshOrder val
)
{
    interpolationOrder_.set(name,val);
}

meshToMeshOrder MeshesRepository::getInterpolationOrder(
    const word &name
)
{
    if(interpolationOrder_.found(name)) {
        return interpolationOrder_[name];
    } else {
        return meshToMesh::imCellVolumeWeight;
    }
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
