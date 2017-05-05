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

#include "liquidProperties.hpp"
#include "HashTable.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(liquidProperties, 0);
    defineRunTimeSelectionTable(liquidProperties,);
    defineRunTimeSelectionTable(liquidProperties, Istream);
    defineRunTimeSelectionTable(liquidProperties, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::liquidProperties::liquidProperties
(
    scalar W,
    scalar Tc,
    scalar Pc,
    scalar Vc,
    scalar Zc,
    scalar Tt,
    scalar Pt,
    scalar Tb,
    scalar dipm,
    scalar omega,
    scalar delta
)
:
    W_(W),
    Tc_(Tc),
    Pc_(Pc),
    Vc_(Vc),
    Zc_(Zc),
    Tt_(Tt),
    Pt_(Pt),
    Tb_(Tb),
    dipm_(dipm),
    omega_(omega),
    delta_(delta)
{}


CML::liquidProperties::liquidProperties(Istream& is)
:
    W_(readScalar(is)),
    Tc_(readScalar(is)),
    Pc_(readScalar(is)),
    Vc_(readScalar(is)),
    Zc_(readScalar(is)),
    Tt_(readScalar(is)),
    Pt_(readScalar(is)),
    Tb_(readScalar(is)),
    dipm_(readScalar(is)),
    omega_(readScalar(is)),
    delta_(readScalar(is))
{}


CML::liquidProperties::liquidProperties(const dictionary& dict)
:
    W_(readScalar(dict.lookup("W"))),
    Tc_(readScalar(dict.lookup("Tc"))),
    Pc_(readScalar(dict.lookup("Pc"))),
    Vc_(readScalar(dict.lookup("Vc"))),
    Zc_(readScalar(dict.lookup("Zc"))),
    Tt_(readScalar(dict.lookup("Tt"))),
    Pt_(readScalar(dict.lookup("Pt"))),
    Tb_(readScalar(dict.lookup("Tb"))),
    dipm_(readScalar(dict.lookup("dipm"))),
    omega_(readScalar(dict.lookup("omega"))),
    delta_(readScalar(dict.lookup("delta")))
{}


CML::liquidProperties::liquidProperties(const liquidProperties& liq)
:
    W_(liq.W_),
    Tc_(liq.Tc_),
    Pc_(liq.Pc_),
    Vc_(liq.Vc_),
    Zc_(liq.Zc_),
    Tt_(liq.Tt_),
    Pt_(liq.Pt_),
    Tb_(liq.Tb_),
    dipm_(liq.dipm_),
    omega_(liq.omega_),
    delta_(liq.delta_)
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

CML::autoPtr<CML::liquidProperties> CML::liquidProperties::New(Istream& is)
{
    if (debug)
    {
        Info<< "liquidProperties::New(Istream&): "
            << "constructing liquidProperties" << endl;
    }

    const word liquidPropertiesType(is);
    const word coeffs(is);

    if (coeffs == "defaultCoeffs")
    {
        ConstructorTable::iterator cstrIter =
            ConstructorTablePtr_->find(liquidPropertiesType);

        if (cstrIter == ConstructorTablePtr_->end())
        {
            FatalErrorIn("liquidProperties::New(Istream&)")
                << "Unknown liquidProperties type "
                << liquidPropertiesType << nl << nl
                << "Valid liquidProperties types are:" << nl
                << ConstructorTablePtr_->sortedToc()
                << abort(FatalError);
        }

        return autoPtr<liquidProperties>(cstrIter()());
    }
    else if (coeffs == "coeffs")
    {
        IstreamConstructorTable::iterator cstrIter =
            IstreamConstructorTablePtr_->find(liquidPropertiesType);

        if (cstrIter == IstreamConstructorTablePtr_->end())
        {
            FatalErrorIn("liquidProperties::New(Istream&)")
                << "Unknown liquidProperties type "
                << liquidPropertiesType << nl << nl
                << "Valid liquidProperties types are:" << nl
                << IstreamConstructorTablePtr_->sortedToc()
                << abort(FatalError);
        }

        return autoPtr<liquidProperties>(cstrIter()(is));
    }
    else
    {
        FatalErrorIn("liquidProperties::New(Istream&)")
            << "liquidProperties type " << liquidPropertiesType
            << ", option " << coeffs << " given"
            << ", should be coeffs or defaultCoeffs"
            << abort(FatalError);

        return autoPtr<liquidProperties>(NULL);
    }
}


CML::autoPtr<CML::liquidProperties> CML::liquidProperties::New
(
    const dictionary& dict
)
{
    if (debug)
    {
        Info<< "liquidProperties::New(const dictionary&):"
            << "constructing liquidProperties" << endl;
    }

    const word& liquidPropertiesTypeName = dict.dictName();

    const Switch defaultCoeffs(dict.lookup("defaultCoeffs"));

    if (defaultCoeffs)
    {
        ConstructorTable::iterator cstrIter =
            ConstructorTablePtr_->find(liquidPropertiesTypeName);

        if (cstrIter == ConstructorTablePtr_->end())
        {
            FatalErrorIn
            (
                "liquidProperties::New(const dictionary&, const word&)"
            )   << "Unknown liquidProperties type "
                << liquidPropertiesTypeName << nl << nl
                << "Valid liquidProperties types are:" << nl
                << ConstructorTablePtr_->sortedToc()
                << abort(FatalError);
        }

        return autoPtr<liquidProperties>(cstrIter()());
    }
    else
    {
        dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->find(liquidPropertiesTypeName);

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalErrorIn
            (
                "liquidProperties::New(const dictionary&, const word&)"
            )   << "Unknown liquidProperties type "
                << liquidPropertiesTypeName << nl << nl
                << "Valid liquidProperties types are:" << nl
                << dictionaryConstructorTablePtr_->sortedToc()
                << abort(FatalError);
        }

        return autoPtr<liquidProperties>
        (
            cstrIter()(dict.subDict(liquidPropertiesTypeName + "Coeffs"))
        );
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::liquidProperties::rho(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::rho(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::pv(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::pv(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::hl(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::hl(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::Cp(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::Cp(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::h(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::h(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::Cpg(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::Cpg(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::mu(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::mu(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::mug(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::mug(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::K(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::K(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::Kg(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::Kg(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::sigma(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::sigms(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::D(scalar p, scalar T) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::D(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::D(scalar p, scalar T, scalar Wb) const
{
    notImplemented
    (
        "CML::scalar CML::liquidProperties::D(scalar, scalar) const"
    );
    return 0.0;
}


CML::scalar CML::liquidProperties::pvInvert(scalar p) const
{
    scalar T = Tc_;
    scalar deltaT = 10.0;
    scalar dp0 = pv(p, T) - p;


    label n = 0;

    while ((n < 200) && (mag(deltaT) > 1.0e-3))
    {
        n++;
        scalar pBoil = pv(p, T);
        scalar dp = pBoil - p;

        if ((dp > 0.0) && (dp0 > 0.0))
        {
            T -= deltaT;
        }
        else
        {
            if ((dp < 0.0) && (dp0 < 0.0))
            {
                T += deltaT;
            }
            else
            {
                deltaT *= 0.5;
                if ((dp > 0.0) && (dp0 < 0.0))
                {
                    T -= deltaT;
                }
                else
                {
                    T += deltaT;
                }
            }
        }

        dp0 = dp;
    }

    return T;
}


void CML::liquidProperties::writeData(Ostream& os) const
{

    os  << W_ << token::SPACE
        << Tc_ << token::SPACE
        << Pc_ << token::SPACE
        << Vc_ << token::SPACE
        << Zc_ << token::SPACE
        << Tt_ << token::SPACE
        << Pt_ << token::SPACE
        << Tb_ << token::SPACE
        << dipm_ << token::SPACE
        << omega_<< token::SPACE
        << delta_;
}


// ************************************************************************* //
