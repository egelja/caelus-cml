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
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "foreignMeshesFollowTime.hpp"

#include "CommonValueExpressionDriver.hpp"

namespace CML {
    defineTypeNameAndDebug(foreignMeshesFollowTime,0);
}

CML::foreignMeshesFollowTime::foreignMeshesFollowTime
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    obr_(obr)
{
    read(dict);
    execute();
}

CML::foreignMeshesFollowTime::~foreignMeshesFollowTime()
{}

void CML::foreignMeshesFollowTime::followCurrentTime()
{
    if(followingMeshes_.size()<=0) {
        return;
    }
    scalar t=obr_.time().value();
    label index=obr_.time().timeIndex();
    Info << "Setting meshes to t=" << t << endl;
    forAll(followingMeshes_,i) {
        const word &name=followingMeshes_[i];
        Info << "  " << name << " : " << flush;
        scalar usedTime=MeshesRepository::getRepository().setTime(
            name,
            t,
            index
        );
        Info << " actual time: t=" << usedTime << endl;
    }
}

void CML::foreignMeshesFollowTime::timeSet()
{
    // Do nothing
}

void CML::foreignMeshesFollowTime::read(const dictionary& dict)
{
    followingMeshes_=wordList(dict.lookup("followingMeshes"));
    followCurrentTime();
}

void CML::foreignMeshesFollowTime::execute()
{
    //    followCurrentTime();
}


void CML::foreignMeshesFollowTime::end()
{
    followCurrentTime();
}

void CML::foreignMeshesFollowTime::write()
{
    followCurrentTime();
}

void CML::foreignMeshesFollowTime::clearData()
{
}

// ************************************************************************* //
