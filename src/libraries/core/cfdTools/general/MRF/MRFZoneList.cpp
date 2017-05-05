/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "MRFZoneList.hpp"
#include "volFields.hpp"

CML::MRFZoneList::MRFZoneList
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    PtrList<MRFZone>(),
    mesh_(mesh)
{
    reset(dict);

    active(true);
}


CML::MRFZoneList::~MRFZoneList()
{}


bool CML::MRFZoneList::active(const bool warn) const
{
    bool a = false;
    forAll(*this, i)
    {
        a = a || this->operator[](i).active();
    }

    if (warn && this->size() && !a)
    {
        Info<< "    No MRF zones active" << endl;
    }

    return a;
}


void CML::MRFZoneList::reset(const dictionary& dict)
{
    label count = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        if (iter().isDict())
        {
            count++;
        }
    }

    this->setSize(count);
    label i = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        if (iter().isDict())
        {
            const word& name = iter().keyword();
            const dictionary& modelDict = iter().dict();

            Info<< "    creating MRF zone: " << name << endl;

            this->set
            (
                i++,
                new MRFZone(name, mesh_, modelDict)
            );
        }
    }
}


bool CML::MRFZoneList::read(const dictionary& dict)
{
    bool allOk = true;
    forAll(*this, i)
    {
        MRFZone& pm = this->operator[](i);
        bool ok = pm.read(dict.subDict(pm.name()));
        allOk = (allOk && ok);
    }
    return allOk;
}


bool CML::MRFZoneList::writeData(Ostream& os) const
{
    forAll(*this, i)
    {
        os  << nl;
        this->operator[](i).writeData(os);
    }

    return os.good();
}


void CML::MRFZoneList::addCoriolis
(
    const volVectorField& U,
    volVectorField& ddtU
) const
{
    forAll(*this, i)
    {
        operator[](i).addCoriolis(U, ddtU);
    }
}


void CML::MRFZoneList::addCoriolis(fvVectorMatrix& UEqn) const
{
    forAll(*this, i)
    {
        operator[](i).addCoriolis(UEqn);
    }
}


void CML::MRFZoneList::addCoriolis
(
    const volScalarField& rho,
    fvVectorMatrix& UEqn
) const
{
    forAll(*this, i)
    {
        operator[](i).addCoriolis(rho, UEqn);
    }
}


void CML::MRFZoneList::relativeVelocity(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).relativeVelocity(U);
    }
}


void CML::MRFZoneList::absoluteVelocity(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).absoluteVelocity(U);
    }
}


void CML::MRFZoneList::relativeFlux(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        operator[](i).relativeFlux(phi);
    }
}


void CML::MRFZoneList::relativeFlux
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        operator[](i).relativeFlux(rho, phi);
    }
}


void CML::MRFZoneList::absoluteFlux(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        operator[](i).absoluteFlux(phi);
    }
}


void CML::MRFZoneList::absoluteFlux
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        operator[](i).absoluteFlux(rho, phi);
    }
}


void CML::MRFZoneList::correctBoundaryVelocity(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).correctBoundaryVelocity(U);
    }
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const MRFZoneList& models
)
{
    models.writeData(os);
    return os;
}


