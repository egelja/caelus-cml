/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
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

#include "waveTheory.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(waveTheory, 0);
defineRunTimeSelectionTable(waveTheory, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


waveTheory::waveTheory
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    IOdictionary
    (
        mesh_.thisDb().lookupObject<IOobject>("waveProperties")
    ),

    seaLevel_(readScalar(lookup("seaLevel"))),

    g_(uniformDimensionedVectorField
        (
            mesh_.thisDb().lookupObject<uniformDimensionedVectorField>("g")
        ).value()),

    direction_(g_/mag(g_)),

    coeffDict_(subDict(subDictName + "Coeffs")),

    PI_(M_PI),

    wind_( lookupOrDefault<vector>("wind", vector::zero ))
{
    {
        IOdictionary transProp
        (
            IOobject
            (
                "transportProperties",
                "constant",
                mesh_,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );
        dictionary sD(transProp.subDict("phase1"));
        rhoWater_ = (dimensionedScalar(sD.lookup("rho"))).value();
    }
}


waveTheory::~waveTheory()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void waveTheory::checkWaveDirection(const vector& k) const
{
    if (CML::mag(k & direction_) > SMALL)
    {
        FatalErrorInFunction
            << "The wave number " << k << " is not perpendicular to the \n"
            << "direction of the gravitational vector " << g_ << "\n"
            << endl << endl << exit(FatalError);
    }
}


void waveTheory::checkWaveDirection(const vectorField& k) const
{
    forAll (k, ki)
    {
        checkWaveDirection(k[ki]);
    }
}


scalarField waveTheory::eta
(
    const pointField& x,
    const scalar& time
) const
{
    scalarField temp(x.size(),0.0);

    forAll (x,ii)
    {
        temp[ii] = eta(x[ii],time);
    }

    return temp;
}


scalarField waveTheory::ddxPd
(
    const pointField& x,
    const scalar& time,
    const vectorField& unitVector
) const
{
    scalarField temp(x.size(),0.0);

    forAll (x,ii)
    {
        temp[ii] = ddxPd(x[ii],time, unitVector[ii]);
    }

    return temp;
}


vectorField waveTheory::U
(
    const pointField& x,
    const scalar& time
) const
{
    vectorField temp(x.size(),vector::zero);

    forAll (x,ii)
    {
        temp[ii] = U(x[ii],time);
    }

    return temp;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
