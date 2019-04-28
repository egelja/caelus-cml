/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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

#include "turbulenceFields.hpp"
#include "dictionary.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(turbulenceFields, 0);

    template<>
    const char* NamedEnum<turbulenceFields::compressibleField, 6>::names[] =
    {
        "R",
        "devRhoReff",
        "mut",
        "muEff",
        "alphat",
        "alphaEff"
    };
    const NamedEnum<turbulenceFields::compressibleField, 6>
        turbulenceFields::compressibleFieldNames_;

    template<>
    const char* NamedEnum<turbulenceFields::incompressibleField, 4>::names[] =
    {
        "R",
        "devReff",
        "nut",
        "nuEff"
    };
    const NamedEnum<turbulenceFields::incompressibleField, 4>
        turbulenceFields::incompressibleFieldNames_;

    const word turbulenceFields::modelName = "turbulenceModel";
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

bool CML::turbulenceFields::compressible()
{
    if (obr_.foundObject<compressible::turbulenceModel>(modelName))
    {
        return true;
    }
    else if (obr_.foundObject<incompressible::turbulenceModel>(modelName))
    {
        return false;
    }
    else
    {
        WarningInFunction
            << "Turbulence model not found in database, deactivating";
        active_ = false;
    }

    return false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::turbulenceFields::turbulenceFields
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
    if (isA<fvMesh>(obr_))
    {
        read(dict);
    }
    else
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating " << name_
            << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::turbulenceFields::~turbulenceFields()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::turbulenceFields::read(const dictionary& dict)
{
    if (active_)
    {
        fieldSet_.insert(wordList(dict.lookup("fields")));

        Info<< type() << " " << name_ << ": ";
        if (fieldSet_.size())
        {
            Info<< "storing fields:" << nl;
            forAllConstIter(wordHashSet, fieldSet_, iter)
            {
                Info<< "    " << modelName << ':' << iter.key() << nl;
            }
            Info<< endl;
        }
        else
        {
            Info<< "no fields requested to be stored" << nl << endl;
        }
    }
}


void CML::turbulenceFields::execute()
{
    bool comp = compressible();

    if (!active_)
    {
        return;
    }

    if (comp)
    {
        const compressible::turbulenceModel& model =
            obr_.lookupObject<compressible::turbulenceModel>(modelName);

        forAllConstIter(wordHashSet, fieldSet_, iter)
        {
            const word& f = iter.key();
            switch (compressibleFieldNames_[f])
            {
                case cfR:
                {
                    processField<symmTensor>(f, model.R());
                    break;
                }
                case cfDevRhoReff:
                {
                    processField<symmTensor>(f, model.devRhoReff());
                    break;
                }
                case cfMut:
                {
                    processField<scalar>(f, model.mut());
                    break;
                }
                case cfMuEff:
                {
                    processField<scalar>(f, model.muEff());
                    break;
                }
                case cfAlphat:
                {
                    processField<scalar>(f, model.alphat());
                    break;
                }
                case cfAlphaEff:
                {
                    processField<scalar>(f, model.alphaEff());
                    break;
                }
                default:
                {
                    FatalErrorInFunction
                        << "Invalid field selection" << abort(FatalError);
                }
            }
        }
    }
    else
    {
        const incompressible::turbulenceModel& model =
            obr_.lookupObject<incompressible::turbulenceModel>(modelName);

        forAllConstIter(wordHashSet, fieldSet_, iter)
        {
            const word& f = iter.key();
            switch (incompressibleFieldNames_[f])
            {
                case ifR:
                {
                    processField<symmTensor>(f, model.R());
                    break;
                }
                case ifDevReff:
                {
                    processField<symmTensor>(f, model.devReff());
                    break;
                }
                case ifNut:
                {
                    processField<scalar>(f, model.nut());
                    break;
                }
                case ifNuEff:
                {
                    processField<scalar>(f, model.nuEff());
                    break;
                }
                default:
                {
                    FatalErrorInFunction
                        << "Invalid field selection" << abort(FatalError);
                }
            }
        }
    }
}


void CML::turbulenceFields::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::turbulenceFields::timeSet()
{
    // Do nothing
}


void CML::turbulenceFields::write()
{
    // Do nothing
}


// ************************************************************************* //
