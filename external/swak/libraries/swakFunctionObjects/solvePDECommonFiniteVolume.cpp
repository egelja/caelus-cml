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
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "solvePDECommonFiniteVolume.hpp"

#include "polyMesh.hpp"

#include "volFields.hpp"

#include "FieldValueExpressionDriver.hpp"

#include "fvScalarMatrix.hpp"

#include "fvm.hpp"

namespace CML {
    defineTypeNameAndDebug(solvePDECommonFiniteVolume,0);
}

CML::solvePDECommonFiniteVolume::solvePDECommonFiniteVolume
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    solvePDECommon(
        name,
        obr,
        dict,
        loadFromFiles
    )
{
    if (!isA<polyMesh>(obr))
    {
        active_=false;
        WarningInFunction
            << "Not a polyMesh. Nothing I can do"
            << endl;
    }
}

CML::solvePDECommonFiniteVolume::~solvePDECommonFiniteVolume()
{}

CML::volScalarField &CML::solvePDECommonFiniteVolume::theField()
{
    // either the field was created by someone else ... then it should be
    // in the registry. Or we created it.
    if(theField_.valid()) {
        return theField_();
    } else {
        return const_cast<volScalarField&>(
            obr_.lookupObject<volScalarField>(
                fieldName_
            )
        );
    }
}

void CML::solvePDECommonFiniteVolume::read(const dictionary& dict)
{
    solvePDECommon::read(dict);

    if(active_) {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        if(
            theField_.valid()
            &&
            fieldName_!=theField_->name()
        ) {
            WarningInFunction
                << "Throwing out field " << theField_->name()
                << " and loading " << fieldName_ << ". "
                << "This might lead to unpredicatable behaviour" << endl;
            theField_.clear();
        }
        if(!theField_.valid()) {
            if(obr_.foundObject<volScalarField>(fieldName_)) {
                if(!dict.found("useFieldFromMemory")) {
                    FatalErrorInFunction
                        << "Field " << fieldName_ << " alread in memory. "
                        << "Set 'useFieldFromMemory true;' to use it or "
                        << "use different name"
                        << endl
                        << exit(FatalError);

                }
                bool useFieldFromMemory=readBool(
                    dict.lookup("useFieldFromMemory")
                );
                if(!useFieldFromMemory) {
                    FatalErrorInFunction
                        << "Field " << fieldName_ << " alread in memory. "
                        << "Use different name"
                        << endl
                        << exit(FatalError);
                }
            } else {
                theField_.set(
                    new volScalarField(
                        IOobject (
                            fieldName_,
                            mesh.time().timeName(),
                            mesh,
                            IOobject::MUST_READ,
                            IOobject::AUTO_WRITE
                        ),
                        mesh
                    )
                );
            }
        }

        driver_.set(
            new FieldValueExpressionDriver(
                mesh.time().timeName(),
                mesh.time(),
                mesh,
                false, // no caching. No need
                true,  // search fields in memory
                false  // don't look up files on disc
            )
        );

        driver_->readVariablesAndTables(dict);

        driver_->createWriterAndRead(name_+"_"+fieldName_+"_"+type());
    }
}

void CML::solvePDECommonFiniteVolume::writeData()
{
    theField().write();

    driver_->tryWrite();
}

void CML::solvePDECommonFiniteVolume::writeNewField()
{
    theField().write();
}

void CML::solvePDECommonFiniteVolume::writeOldField()
{
    volScalarField temp(fieldName_+".presolve",theField());
    temp.write();
}

// ************************************************************************* //
