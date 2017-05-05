/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fvOptionList.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(optionList, 0);
}
}


void CML::fv::optionList::checkApplied() const
{
    if (mesh_.time().timeIndex() == checkTimeIndex_)
    {
        forAll(*this, i)
        {
            const option& bs = this->operator[](i);
            bs.checkApplied();
        }
    }
}


CML::fv::optionList::optionList(const fvMesh& mesh, const dictionary& dict)
:
    PtrList<option>(),
    mesh_(mesh),
    checkTimeIndex_(mesh_.time().startTimeIndex() + 2)
{
    reset(dict);
}


CML::fv::optionList::optionList(const fvMesh& mesh)
:
    PtrList<option>(),
    mesh_(mesh),
    checkTimeIndex_(mesh_.time().startTimeIndex() + 2)
{}


void CML::fv::optionList::reset(const dictionary& dict)
{
    label count = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        // safety:
        if (iter().isDict())
        {
            count ++;
        }
    }

    this->setSize(count);
    label i = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        if (iter().isDict())
        {
            const word& name = iter().keyword();
            const dictionary& sourceDict = iter().dict();

            this->set
            (
                i++,
                option::New(name, sourceDict, mesh_)
            );
        }
    }
}


void CML::fv::optionList::relativeFlux(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        this->operator[](i).relativeFlux(phi);
    }
}


void CML::fv::optionList::relativeFlux
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        this->operator[](i).relativeFlux(rho, phi);
    }
}


void CML::fv::optionList::absoluteFlux(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        this->operator[](i).absoluteFlux(phi);
    }
}


void CML::fv::optionList::absoluteFlux
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        this->operator[](i).absoluteFlux(rho, phi);
    }
}


bool CML::fv::optionList::read(const dictionary& dict)
{
    checkTimeIndex_ = mesh_.time().timeIndex() + 2;

    bool allOk = true;
    forAll(*this, i)
    {
        option& bs = this->operator[](i);
        bool ok = bs.read(dict.subDict(bs.name()));
        allOk = (allOk && ok);
    }
    return allOk;
}


bool CML::fv::optionList::writeData(Ostream& os) const
{
    // Write list contents
    forAll(*this, i)
    {
        os  << nl;
        this->operator[](i).writeData(os);
    }

    // Check state of IOstream
    return os.good();
}


namespace CML
{
    Ostream& operator<<
    (
        Ostream& os,
        const fv::optionList& options
    )
    {
        options.writeData(os);
        return os;
    }
}



