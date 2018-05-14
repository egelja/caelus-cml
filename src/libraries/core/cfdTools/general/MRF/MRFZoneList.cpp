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
#include "fixedValueFvsPatchFields.hpp"

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


void CML::MRFZoneList::addAcceleration
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


void CML::MRFZoneList::addAcceleration(fvVectorMatrix& UEqn) const
{
    forAll(*this, i)
    {
        operator[](i).addCoriolis(UEqn);
    }
}


void CML::MRFZoneList::addAcceleration
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


CML::tmp<CML::volVectorField> CML::MRFZoneList::DDt
(
    const volVectorField& U
) const
{
    tmp<volVectorField> tacceleration
    (
        new volVectorField
        (
            IOobject
            (
                "MRFZoneList:acceleration",
                U.mesh().time().timeName(),
                U.mesh()
            ),
            U.mesh(),
            dimensionedVector("0", U.dimensions()/dimTime, Zero)
        )
    );
    volVectorField& acceleration = tacceleration();

    forAll(*this, i)
    {
        operator[](i).addCoriolis(U, acceleration);
    }

    return tacceleration;
}


CML::tmp<CML::volVectorField> CML::MRFZoneList::DDt
(
    const volScalarField& rho,
    const volVectorField& U
) const
{
    return rho*DDt(U);
}


void CML::MRFZoneList::makeRelative(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).makeRelative(U);
    }
}


void CML::MRFZoneList::makeRelative(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        operator[](i).makeRelative(phi);
    }
}


CML::tmp<CML::surfaceScalarField> CML::MRFZoneList::relative
(
    const tmp<surfaceScalarField>& tphi
) const
{
    if (size())
    {
        tmp<surfaceScalarField> rphi
        (
            reuseTmpGeometricField<scalar, scalar, fvsPatchField, surfaceMesh>
            ::New
            (
                tphi,
                "relative(" + tphi().name() + ')',
                tphi().dimensions()
            )
        );

        makeRelative(rphi());

        reuseTmpGeometricField<scalar, scalar, fvsPatchField, surfaceMesh>
        ::clear(tphi);

        return rphi;
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


CML::tmp<CML::FieldField<CML::fvsPatchField, CML::scalar> >
CML::MRFZoneList::relative
(
    const tmp<FieldField<fvsPatchField, scalar> >& tphi
) const
{
    if (size())
    {
        tmp<FieldField<fvsPatchField, scalar> > rphi
        (
            reuseTmpFieldField<fvsPatchField, scalar, scalar>::New(tphi)
        );

        forAll(*this, i)
        {
            operator[](i).makeRelative(rphi());
        }

        reuseTmpFieldField<fvsPatchField, scalar, scalar>::clear(tphi);

        return rphi;
    }
    else
    {
        return tmp<FieldField<fvsPatchField, scalar> >(tphi, true);
    }
}


CML::tmp<CML::Field<CML::scalar> >
CML::MRFZoneList::relative
(
    const tmp<Field<scalar> >& tphi,
    const label patchi
) const
{
    if (size())
    {
        tmp<Field<scalar> > rphi
        (
            reuseTmp<scalar, scalar>
            ::New(tphi)
        );

        forAll(*this, i)
        {
            operator[](i).makeRelative(rphi(), patchi);
        }

        tphi.clear();

        return rphi;
    }
    else
    {
        return tmp<Field<scalar> >(tphi, true);
    }
}


void CML::MRFZoneList::makeRelative
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        operator[](i).makeRelative(rho, phi);
    }
}


void CML::MRFZoneList::makeAbsolute(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).makeAbsolute(U);
    }
}


void CML::MRFZoneList::makeAbsolute(surfaceScalarField& phi) const
{
    forAll(*this, i)
    {
        operator[](i).makeAbsolute(phi);
    }
}


CML::tmp<CML::surfaceScalarField> CML::MRFZoneList::absolute
(
    const tmp<surfaceScalarField>& tphi
) const
{
    if (size())
    {
        tmp<surfaceScalarField> rphi      (
            reuseTmpGeometricField<scalar, scalar, fvsPatchField, surfaceMesh>
            ::New
            (
                tphi,
                "absolute(" + tphi().name() + ')',
                tphi().dimensions()
            )
        );

        makeAbsolute(rphi());

        reuseTmpGeometricField<scalar, scalar, fvsPatchField, surfaceMesh>
        ::clear(tphi);

        return rphi;
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


void CML::MRFZoneList::makeAbsolute
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    forAll(*this, i)
    {
        operator[](i).makeAbsolute(rho, phi);
    }
}


void CML::MRFZoneList::correctBoundaryVelocity(volVectorField& U) const
{
    forAll(*this, i)
    {
        operator[](i).correctBoundaryVelocity(U);
    }
}


void CML::MRFZoneList::correctBoundaryFlux
(
    const volVectorField& U,
    surfaceScalarField& phi
) const
{
    FieldField<fvsPatchField, scalar> Uf
    (
        relative(mesh_.Sf().boundaryField() & U.boundaryField())
    );


    surfaceScalarField::GeometricBoundaryField& phibf = phi.boundaryField();

    forAll(mesh_.boundary(), patchi)
    {
        if
        (
            isA<fixedValueFvsPatchScalarField>(phibf[patchi])
        )
        {
            phibf[patchi] == Uf[patchi];
        }
    }
}


void CML::MRFZoneList::update()
{
    if (mesh_.topoChanging())
    {
        forAll(*this, i)
        {
            operator[](i).update();
        }
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


