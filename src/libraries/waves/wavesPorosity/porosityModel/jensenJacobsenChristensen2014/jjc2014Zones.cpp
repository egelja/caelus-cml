/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen, Deltares.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

\*---------------------------------------------------------------------------*/

#include "jjc2014Zones.hpp"
#include "Time.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTemplateTypeNameAndDebug(IOPtrList<jjc2014Zone>, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::jjc2014Zones::jjc2014Zones
(
    const fvMesh& mesh
)
:
    IOPtrList<jjc2014Zone>
    (
        IOobject
        (
            "porosityZones",
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        jjc2014Zone::iNew(mesh)
    ),
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<volScalarField> CML::jjc2014Zones::porosity() const
{
    // Create the return field
    tmp<volScalarField> tporosity
    (
        new volScalarField
        (
            IOobject
            (
                "porosity_tmp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("null", dimless, 1.0),
            "zeroGradient"
        )
    );

    // Make reference to the return field (stripping tmp-nature)
    volScalarField& poro(tporosity());


    // Loop over all zones
    forAll(*this, i)
    {
        operator[](i).porosity(poro);
    }

    return tporosity;
}


void CML::jjc2014Zones::addResistance(fvVectorMatrix& UEqn) const
{
    forAll(*this, i)
    {
        operator[](i).addResistance(UEqn);
    }
}

void CML::jjc2014Zones::addResistance
(
    const fvVectorMatrix& UEqn,
    volTensorField& AU
) const
{
    // addResistance for each zone, delaying the correction of the
    // precessor BCs of AU
    forAll(*this, i)
    {
        operator[](i).addResistance(UEqn, AU, false);
    }

    // Correct the boundary conditions of the tensorial diagonal to ensure
    // processor bounaries are correctly handled when AU^-1 is interpolated
    // for the pressure equation.
    AU.correctBoundaryConditions();
}


bool CML::jjc2014Zones::readData(Istream& is)
{
    clear();

    IOPtrList<jjc2014Zone> newLst
    (
        IOobject
        (
            "porosityZones",
            mesh_.time().constant(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false     // Don't re-register new zones with objectRegistry
        ),
        jjc2014Zone::iNew(mesh_)
    );

    transfer(newLst);

    return is.good();
}


bool CML::jjc2014Zones::writeData(Ostream& os, bool subDict) const
{
    // Write size of list
    os << nl << size();

    // Write beginning of contents
    os << nl << token::BEGIN_LIST;

    // Write list contents
    forAll(*this, i)
    {
        os << nl;
        operator[](i).writeDict(os, subDict);
    }

    // Write end of contents
    os << token::END_LIST << nl;

    // Check state of IOstream
    return os.good();
}


// ************************************************************************* //
