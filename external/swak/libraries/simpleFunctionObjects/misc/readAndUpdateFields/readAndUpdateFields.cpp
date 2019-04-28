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

#include "readAndUpdateFields.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(readAndUpdateFields, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::readAndUpdateFields::readAndUpdateFields
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true),
    fieldSet_()
{
    // Check if the available mesh is an fvMesh otherise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating."
            << endl;
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::readAndUpdateFields::~readAndUpdateFields()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::readAndUpdateFields::timeSet()
{
    // Do nothing
}

void CML::readAndUpdateFields::read(const dictionary& dict)
{
    if (active_)
    {
        dict.lookup("fields") >> fieldSet_;

        //Info<< type() << " " << name_ << ":" << nl;

        // Clear out any previously loaded fields 
        vsf_.clear();
        vvf_.clear();
        vSpheretf_.clear();
        vSymmtf_.clear();
        vtf_.clear();
        
        psf_.clear();
        pvf_.clear();
        pSpheretf_.clear();
        pSymmtf_.clear();
        ptf_.clear();
        
        forAll(fieldSet_, fieldI)
        {
            bool found = loadField<scalar>(fieldSet_[fieldI], vsf_, psf_);
            found = found || loadField<vector>(fieldSet_[fieldI], vvf_, pvf_);
            found = found || loadField<sphericalTensor>(fieldSet_[fieldI], vSpheretf_, pSpheretf_);
            found = found || loadField<symmTensor>(fieldSet_[fieldI], vSymmtf_, pSymmtf_);
            found = found || loadField<tensor>(fieldSet_[fieldI], vtf_, ptf_);
        
            if(!found)
            { 
                FatalErrorInFunction
                    << "Field " << fieldSet_[fieldI] << " does not exist"
                        << endl
                        << exit(FatalError);
                
            }
        }
    }
}

const CML::pointMesh &CML::readAndUpdateFields::pMesh(const polyMesh &mesh)
{
    if(!pMesh_.valid()) {
        pMesh_.set(new pointMesh(mesh));
    }
    return pMesh_();
}

void CML::readAndUpdateFields::execute()
{
    if(active_) {
        correctBoundaryConditions(vsf_);
        correctBoundaryConditions(vvf_);
        correctBoundaryConditions(vtf_);
        correctBoundaryConditions(vSymmtf_);
        correctBoundaryConditions(vSpheretf_);

        correctBoundaryConditions(psf_);
        correctBoundaryConditions(pvf_);
        correctBoundaryConditions(ptf_);
        correctBoundaryConditions(pSymmtf_);
        correctBoundaryConditions(pSpheretf_);
    }
}


void CML::readAndUpdateFields::end()
{
    execute();
}


void CML::readAndUpdateFields::write()
{
    // Do nothing
}


// ************************************************************************* //
