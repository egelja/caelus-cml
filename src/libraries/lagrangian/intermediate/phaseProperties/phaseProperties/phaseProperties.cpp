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

#include "phaseProperties.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* CML::NamedEnum
    <
        CML::phaseProperties::phaseType,
        4
    >::names[] =
    {
        "gas",
        "liquid",
        "solid",
        "unknown"
    };
}

const CML::NamedEnum<CML::phaseProperties::phaseType, 4>
    CML::phaseProperties::phaseTypeNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::phaseProperties::setGlobalIds(const wordList& globalNames)
{
    forAll(names_, i)
    {
        forAll(globalNames, j)
        {
            if (globalNames[j] == names_[i])
            {
                globalIds_[i] = j;
                break;
            }
        }
        if (globalIds_[i] == -1)
        {
            FatalErrorIn
            (
                "void CML::phaseProperties::setGlobalIds(const wordList&)"
            )   << "Could not find specie " << names_[i]
                << " in species list" <<  nl
                << "Available species are: " << nl << globalNames << nl
                << exit(FatalError);
        }
    }
}


void CML::phaseProperties::setGlobalCarrierIds
(
    const wordList& carrierNames
)
{
    globalCarrierIds_ = -1;

    forAll(names_, i)
    {
        forAll(carrierNames, j)
        {
            if (carrierNames[j] == names_[i])
            {
                globalCarrierIds_[i] = j;
                break;
            }
        }
        if (globalCarrierIds_[i] == -1)
        {
            FatalErrorIn
            (
                "void CML::phaseProperties::setGlobalCarrierIds"
                "("
                    "const wordList&"
                ")"
            )   << "Could not find carrier specie " << names_[i]
                << " in species list" <<  nl
                << "Available species are: " << nl << carrierNames << nl
                << exit(FatalError);
        }
    }
}


void CML::phaseProperties::checkTotalMassFraction() const
{
    scalar total = 0.0;
    forAll(Y_, cmptI)
    {
        total += Y_[cmptI];
    }

    if (Y_.size() != 0 && mag(total - 1.0) > SMALL)
    {
        FatalErrorIn
        (
            "void CML::phaseProperties::checkTotalMassFraction() const"
        )   << "Component fractions must total to unity for phase "
            << phaseTypeNames_[phase_] << nl
            << "Components: " << nl << names_ << nl << exit(FatalError);
    }
}


CML::word CML::phaseProperties::phaseToStateLabel(const phaseType pt) const
{
    word state = "(unknown)";
    switch (pt)
    {
        case GAS:
        {
            state = "(g)";
            break;
        }
        case LIQUID:
        {
            state = "(l)";
            break;
        }
        case SOLID:
        {
            state = "(s)";
            break;
        }
        default:
        {
            FatalErrorIn
            (
                "CML::phaseProperties::phaseToStateLabel(phaseType pt)"
            )   << "Invalid phase: " << phaseTypeNames_[pt] << nl
                << "    phase must be gas, liquid or solid" << nl
                << exit(FatalError);
        }
    }

    return state;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::phaseProperties::phaseProperties()
:
    phase_(UNKNOWN),
    stateLabel_("(unknown)"),
    names_(0),
    Y_(0),
    globalIds_(0),
    globalCarrierIds_(0)
{}


CML::phaseProperties::phaseProperties(const phaseProperties& pp)
:
    phase_(pp.phase_),
    stateLabel_(pp.stateLabel_),
    names_(pp.names_),
    Y_(pp.Y_),
    globalIds_(pp.globalIds_),
    globalCarrierIds_(pp.globalCarrierIds_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::phaseProperties::~phaseProperties()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::phaseProperties::initialiseGlobalIds
(
    const wordList& gasNames,
    const wordList& liquidNames,
    const wordList& solidNames
)
{
    // determine the addressing to map between components listed in the phase
    // with those given in the (main) thermo properties
    switch (phase_)
    {
        case GAS:
        {
            setGlobalIds(gasNames);
            forAll(globalCarrierIds_, i)
            {
                globalCarrierIds_[i] = globalIds_[i];
            }
            break;
        }
        case LIQUID:
        {
            setGlobalIds(liquidNames);
            setGlobalCarrierIds(gasNames);
            break;
        }
        case SOLID:
        {
            setGlobalIds(solidNames);
            WarningIn
            (
                "phaseProperties::initialiseGlobalIds(...)"
            )   << "Assuming no mapping between solid and carrier species"
                << endl;
//            setGlobalCarrierIds(gasNames);
            break;
        }
        default:
        {
            FatalErrorIn
            (
                "CML::phaseProperties::setGlobalIds"
                "("
                    "const PtrList<volScalarField>&, "
                    "const wordList&, "
                    "const wordList&"
                ")"
            )   << "Invalid phase: " << phaseTypeNames_[phase_] << nl
                << "    phase must be gas, liquid or solid" << nl
                << exit(FatalError);
        }
    }
}


CML::phaseProperties::phaseType CML::phaseProperties::phase() const
{
    return phase_;
}


const CML::word& CML::phaseProperties::stateLabel() const
{
    return stateLabel_;
}


CML::word CML::phaseProperties::phaseTypeName() const
{
    return phaseTypeNames_[phase_];
}


const CML::List<CML::word>& CML::phaseProperties::names() const
{
    return names_;
}


const CML::word& CML::phaseProperties::name(const label cmptI) const
{
    if (cmptI >= names_.size())
    {
        FatalErrorIn
        (
            "const CML::word& CML::phaseProperties::name"
            "("
                "const label"
            ") const"
        )   << "Requested component " << cmptI << "out of range" << nl
            << "Available phase components:" << nl << names_ << nl
            << exit(FatalError);
    }

    return names_[cmptI];
}


const CML::scalarField& CML::phaseProperties::Y() const
{
    return Y_;
}


CML::scalar& CML::phaseProperties::Y(const label cmptI)
{
    if (cmptI >= Y_.size())
    {
        FatalErrorIn
        (
            "const CML::scalar& CML::phaseProperties::Y"
            "("
                "const label"
            ") const"
        )   << "Requested component " << cmptI << "out of range" << nl
            << "Available phase components:" << nl << names_ << nl
            << exit(FatalError);
    }

    return Y_[cmptI];
}


CML::label CML::phaseProperties::globalId(const word& cmptName) const
{
    label id = this->id(cmptName);

    if (id < 0)
    {
        return id;
    }
    else
    {
        return globalIds_[id];
    }

}


const CML::labelList& CML::phaseProperties::globalIds() const
{
    return globalIds_;
}


const CML::labelList& CML::phaseProperties::globalCarrierIds() const
{
    return globalCarrierIds_;
}


CML::label CML::phaseProperties::id(const word& cmptName) const
{
    forAll(names_, cmptI)
    {
        if (names_[cmptI] == cmptName)
        {
            return cmptI;
        }
    }

    return -1;
}


// ************************************************************************* //

