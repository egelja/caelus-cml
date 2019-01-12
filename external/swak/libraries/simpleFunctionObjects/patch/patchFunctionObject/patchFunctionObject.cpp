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
    2008-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "patchFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"
#include "OStringStream.hpp"
#include "wordReList.hpp"
#include "stringListOps.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(patchFunctionObject, 0);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

patchFunctionObject::patchFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    timelineFunctionObject(name,t,dict),
    patchNames_(0),
    patchIndizes_(0)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool patchFunctionObject::start()
{
    wordList oldPatchNames(patchNames_);
    wordReList newPatches(dict_.lookup("patches"));
    HashSet<word> patchNamesNew;
    bool allowCoupled(dict_.lookupOrDefault<bool>("allowCoupled",false));

    const fvMesh &mesh=refCast<const fvMesh>(obr_);
    wordList allPatches(mesh.boundaryMesh().names());
    forAll(newPatches,i) {
        labelList IDs=findStrings(newPatches[i],allPatches);
        forAll(IDs,j) {
            const word &name=allPatches[IDs[j]];
            bool add=true;
            if(!allowCoupled) {
                label patchI=mesh.boundaryMesh().findPatchID(name);
                const polyPatch &thePatch=mesh.boundaryMesh()[patchI];
                if(thePatch.coupled()) {
                    WarningInFunction
                        << "Patch " << name << " in " << dict_.name() << " is coupled." << nl
                            << "Disabling. If you want it enabled set 'allowCoupled true;'"
                            << endl;
                    add=false;
                }
            }
            if(add) {
                patchNamesNew.insert(name);
            }
        }
    }

    patchNames_=patchNamesNew.toc();

    // the patches changed
    if(patchNames_!=oldPatchNames) {
        closeAllFiles();
    }
    timelineFunctionObject::start();

    patchIndizes_.setSize(patchNames_.size());

    forAll(patchNames_,i) {
        const word &name=patchNames_[i];
        patchIndizes_[i]=mesh.boundaryMesh().findPatchID(name);
        if(patchIndizes_[i]<0) {
            WarningInFunction
                << " Patch " << name << " does not exist in patches: "
                    << mesh.boundaryMesh().names() << endl;
        }
    }
    if(debug>1) {
        Pout << "Patch names: " << patchNames_ << nl
            << " Index: " << patchIndizes_ << endl;
    }

    if (Pstream::master())
    {
        if (debug)
        {
            Pout<< "Probing patches:" << patchNames_ << nl
                << endl;
        }
    }

    return true;
}

stringList patchFunctionObject::columnNames()
{
    stringList result(patchNames_.size());
    forAll(result,i) {
        result[i]=patchNames_[i];
    }
    return result;
}

} // namespace CML

// ************************************************************************* //
