/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

#include "fvDOM.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "absorptionEmissionModel.hpp"
#include "scatterModel.hpp"
#include "constants.hpp"

using namespace CML::constant;
using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace radiation
    {
        defineTypeNameAndDebug(fvDOM, 0);

        addToRunTimeSelectionTable
        (
            radiationModel,
            fvDOM,
            dictionary
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::radiation::fvDOM::fvDOM(const volScalarField& T)
:
    radiationModel(typeName, T),
    G_
    (
        IOobject
        (
            "G",
            mesh_.time().timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("G", dimMass/pow3(dimTime), 0.0)
    ),
    Qr_
    (
        IOobject
        (
            "Qr",
            mesh_.time().timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("Qr", dimMass/pow3(dimTime), 0.0)
    ),
    Qem_
    (
        IOobject
        (
            "Qem",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("Qem", dimMass/pow3(dimTime), 0.0)
    ),
    Qin_
    (
        IOobject
        (
            "Qin",
            mesh_.time().timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("Qin", dimMass/pow3(dimTime), 0.0)
    ),
    a_
    (
        IOobject
        (
            "a",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("a", dimless/dimLength, 0.0)
    ),
    nTheta_(readLabel(coeffs_.lookup("nTheta"))),
    nPhi_(readLabel(coeffs_.lookup("nPhi"))),
    nRay_(0),
    nLambda_(absorptionEmission_->nBands()),
    aLambda_(nLambda_),
    blackBody_(nLambda_, T),
    IRay_(0),
    convergence_(coeffs_.lookupOrDefault<scalar>("convergence", 0.0)),
    maxIter_(coeffs_.lookupOrDefault<label>("maxIter", 50))
{
    if (mesh_.nSolutionD() == 3)    //3D
    {
        nRay_ = 4*nPhi_*nTheta_;
        IRay_.setSize(nRay_);
        scalar deltaPhi = pi/(2.0*nPhi_);
        scalar deltaTheta = pi/nTheta_;
        label i = 0;
        for (label n = 1; n <= nTheta_; n++)
        {
            for (label m = 1; m <= 4*nPhi_; m++)
            {
                scalar thetai = (2.0*n - 1.0)*deltaTheta/2.0;
                scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
                IRay_.set
                (
                    i,
                    new radiativeIntensityRay
                    (
                        *this,
                        mesh_,
                        phii,
                        thetai,
                        deltaPhi,
                        deltaTheta,
                        nLambda_,
                        absorptionEmission_,
                        blackBody_,
                        i
                    )
                );
                i++;
            }
        }
    }
    else
    {
        if (mesh_.nSolutionD() == 2)    //2D (X & Y)
        {
            scalar thetai = piByTwo;
            scalar deltaTheta = pi;
            nRay_ = 4*nPhi_;
            IRay_.setSize(nRay_);
            scalar deltaPhi = pi/(2.0*nPhi_);
            label i = 0;
            for (label m = 1; m <= 4*nPhi_; m++)
            {
                scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
                IRay_.set
                (
                    i,
                    new radiativeIntensityRay
                    (
                        *this,
                        mesh_,
                        phii,
                        thetai,
                        deltaPhi,
                        deltaTheta,
                        nLambda_,
                        absorptionEmission_,
                        blackBody_,
                        i
                    )
                );
                i++;
            }
        }
        else    //1D (X)
        {
            scalar thetai = piByTwo;
            scalar deltaTheta = pi;
            nRay_ = 2;
            IRay_.setSize(nRay_);
            scalar deltaPhi = pi;
            label i = 0;
            for (label m = 1; m <= 2; m++)
            {
                scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
                IRay_.set
                (
                    i,
                    new radiativeIntensityRay
                    (
                        *this,
                        mesh_,
                        phii,
                        thetai,
                        deltaPhi,
                        deltaTheta,
                        nLambda_,
                        absorptionEmission_,
                        blackBody_,
                        i
                    )
                );
                i++;
            }
        }
    }


    // Construct absorption field for each wavelength
    forAll(aLambda_, lambdaI)
    {
        aLambda_.set
        (
            lambdaI,
            new volScalarField
            (
                IOobject
                (
                    "aLambda_" + CML::name(lambdaI) ,
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                a_
            )
        );
    }

    Info<< "fvDOM : Allocated " << IRay_.size()
        << " rays with average orientation:" << nl;
    forAll(IRay_, i)
    {
        Info<< '\t' << IRay_[i].I().name()
            << '\t' << IRay_[i].dAve() << nl;
    }
    Info<< endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::radiation::fvDOM::~fvDOM()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::radiation::fvDOM::read()
{
    if (radiationModel::read())
    {
        // Only reading solution parameters - not changing ray geometry

        coeffs_.readIfPresent("convergence", convergence_);
        coeffs_.readIfPresent("maxIter", maxIter_);

        return true;
    }
    else
    {
        return false;
    }
}


void CML::radiation::fvDOM::calculate()
{
    absorptionEmission_->correct(a_, aLambda_);

    updateBlackBodyEmission();

    scalar maxResidual = 0.0;
    label radIter = 0;
    do
    {
        radIter++;
        forAll(IRay_, rayI)
        {
            maxResidual = 0.0;
            scalar maxBandResidual = IRay_[rayI].correct();
            maxResidual = max(maxBandResidual, maxResidual);
        }

        Info<< "Radiation solver iter: " << radIter << endl;

    } while (maxResidual > convergence_ && radIter < maxIter_);

    updateG();
}


CML::tmp<CML::volScalarField> CML::radiation::fvDOM::Rp() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Rp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            4.0*a_*physicoChemical::sigma //absorptionEmission_->a()
        )
    );
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::radiation::fvDOM::Ru() const
{

    const DimensionedField<scalar, volMesh>& G =
        G_.dimensionedInternalField();
    const DimensionedField<scalar, volMesh> E =
        absorptionEmission_->ECont()().dimensionedInternalField();
    const DimensionedField<scalar, volMesh> a =
        a_.dimensionedInternalField(); //absorptionEmission_->aCont()()

    return  a*G - E;
}


void CML::radiation::fvDOM::updateBlackBodyEmission()
{
    for (label j=0; j < nLambda_; j++)
    {
        blackBody_.correct(j, absorptionEmission_->bands(j));
    }
}


void CML::radiation::fvDOM::updateG()
{
    G_ = dimensionedScalar("zero",dimMass/pow3(dimTime), 0.0);
    Qr_ = dimensionedScalar("zero",dimMass/pow3(dimTime), 0.0);
    Qem_ = dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0);
    Qin_ = dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0);

    forAll(IRay_, rayI)
    {
        IRay_[rayI].addIntensity();
        G_ += IRay_[rayI].I()*IRay_[rayI].omega();
        Qr_.boundaryField() += IRay_[rayI].Qr().boundaryField();
        Qem_.boundaryField() += IRay_[rayI].Qem().boundaryField();
        Qin_.boundaryField() += IRay_[rayI].Qin().boundaryField();
    }
}


void CML::radiation::fvDOM::setRayIdLambdaId
(
    const word& name,
    label& rayId,
    label& lambdaId
) const
{
    // assuming name is in the form: CHARS_rayId_lambdaId
    size_type i1 = name.find_first_of("_");
    size_type i2 = name.find_last_of("_");

    rayId = readLabel(IStringStream(name.substr(i1+1, i2-1))());
    lambdaId = readLabel(IStringStream(name.substr(i2+1, name.size()-1))());
}


// ************************************************************************* //
