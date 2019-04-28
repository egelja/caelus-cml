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
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "solveTransportPDE.hpp"

#include "polyMesh.hpp"

#include "volFields.hpp"

#include "FieldValueExpressionDriver.hpp"

#include "fvScalarMatrix.hpp"

#include "fvm.hpp"

namespace CML {
    defineTypeNameAndDebug(solveTransportPDE,0);
}

CML::solveTransportPDE::solveTransportPDE
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    solvePDECommonFiniteVolume(
        name,
        obr,
        dict,
        loadFromFiles
    ),
    rhoDimension_(dimless),
    diffusionDimension_(dimless),
    phiDimension_(dimless),
    sourceDimension_(dimless),
    sourceImplicitDimension_(dimless)
{
    if (!isA<polyMesh>(obr))
    {
        active_=false;
        WarningInFunction
            << "Not a polyMesh. Nothing I can do"
            << endl;
    }

    read(dict);

    if(solveAt_==saStartup) {
        solveWrapper();
    }
}

CML::solveTransportPDE::~solveTransportPDE()
{}

void CML::solveTransportPDE::read(const dictionary& dict)
{
    solvePDECommonFiniteVolume::read(dict);

    if(active_) {
        if(!steady_) {
            readExpressionAndDimension(
                dict,
                "rho",
                rhoExpression_,
                rhoDimension_
            );
        }
        readExpressionAndDimension(
            dict,
            "diffusion",
            diffusionExpression_,
            diffusionDimension_
        );
        readExpressionAndDimension(
            dict,
            "source",
            sourceExpression_,
            sourceDimension_
        );
        if(dict.found("sourceImplicit")) {
            readExpressionAndDimension(
                dict,
                "sourceImplicit",
                sourceImplicitExpression_,
                sourceImplicitDimension_
            );
        } else {
            if(sourceExpression_!="0") {
                WarningInFunction
                    << "Source expression " << sourceExpression_ << " set. "
                    << "Consider factoring out parts to 'sourceImplicit'\n"
                    << endl;
            }
        }
        readExpressionAndDimension(
            dict,
            "phi",
            phiExpression_,
            phiDimension_
        );
    }
}

void CML::solveTransportPDE::solve()
{
    if(active_) {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);
        dictionary sol=mesh.solutionDict().subDict(fieldName_+"TransportPDE");

        FieldValueExpressionDriver &driver=driver_();

        int nCorr=sol.lookupOrDefault<int>("nCorrectors", 0);
        if(
            nCorr==0
            &&
            steady_
        ) {
            WarningInFunction
                << name_ << " is steady. It is recommended to have in "
                << sol.name() << " a nCorrectors>0"
                << endl;
        }

        for (int corr=0; corr<=nCorr; corr++) {

            driver.clearVariables();

            driver.parse(diffusionExpression_);
            if(!driver.resultIsTyp<volScalarField>()) {
                FatalErrorInFunction
                    << diffusionExpression_ << " does not evaluate to a scalar"
                    << endl
                    << exit(FatalError);
            }
            volScalarField diffusionField(driver.getResult<volScalarField>());
            diffusionField.dimensions().reset(diffusionDimension_);

            driver.parse(sourceExpression_);
            if(!driver.resultIsTyp<volScalarField>()) {
                FatalErrorInFunction
                    << sourceExpression_ << " does not evaluate to a scalar"
                    << endl
                    << exit(FatalError);
            }
            volScalarField sourceField(driver.getResult<volScalarField>());
            sourceField.dimensions().reset(sourceDimension_);

            driver.parse(phiExpression_);
            if(!driver.resultIsTyp<surfaceScalarField>()) {
                FatalErrorInFunction
                    << phiExpression_ << " does not evaluate to a surface scalar"
                    << endl
                    << exit(FatalError);
            }
            surfaceScalarField phiField(driver.getResult<surfaceScalarField>());
            phiField.dimensions().reset(phiDimension_);

            volScalarField &f=theField();

            fvMatrix<scalar> eq(
                fvm::div(phiField,f)
                -fvm::laplacian(diffusionField,f,"laplacian(diffusion,"+f.name()+")")
                ==
                sourceField
                + fvOptions()(f)
            );

            if(!steady_) {
                driver.parse(rhoExpression_);
                if(!driver.resultIsTyp<volScalarField>()) {
                    FatalErrorInFunction
                        << rhoExpression_ << " does not evaluate to a scalar"
                        << endl
                        << exit(FatalError);
                }
                volScalarField rhoField(driver.getResult<volScalarField>());
                rhoField.dimensions().reset(rhoDimension_);

                fvMatrix<scalar> ddtMatrix(fvm::ddt(f));
                if(
                    !ddtMatrix.diagonal()
                    &&
                    !ddtMatrix.symmetric()
                    &&
                    !ddtMatrix.asymmetric()
                ) {
                    // Adding would fail
                } else {
                    eq+=rhoField*ddtMatrix;
                }
            }

            if(sourceImplicitExpression_!="") {
                driver.parse(sourceImplicitExpression_);
                if(!driver.resultIsTyp<volScalarField>()) {
                    FatalErrorInFunction
                        << sourceImplicitExpression_ << " does not evaluate to a scalar"
                        << endl
                        << exit(FatalError);
                }
                volScalarField sourceImplicitField(driver.getResult<volScalarField>());
                sourceImplicitField.dimensions().reset(sourceImplicitDimension_);

                eq-=fvm::Sp(sourceImplicitField,f);
            }

            if(doRelax(corr==nCorr)) {
                eq.relax();
            }

            fvOptions().constrain(eq);

            int nNonOrthCorr=sol.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);
            for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
            {
                eq.solve();
            }

            fvOptions().correct(f);
        }
    }
}

// ************************************************************************* //
