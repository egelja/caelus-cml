/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::constIsoSolidTransport

Description
    Constant properties Transport package.
    Templated into a given thermodynamics package (needed for thermal
    conductivity).


\*---------------------------------------------------------------------------*/

#ifndef constIsoSolidTransport_HPP
#define constIsoSolidTransport_HPP

#include "vector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Thermo> class constIsoSolidTransport;

template<class Thermo>
inline constIsoSolidTransport<Thermo> operator*
(
    const scalar,
    const constIsoSolidTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const constIsoSolidTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                   Class constIsoSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class constIsoSolidTransport
:
    public Thermo
{

    //- Constant isotropic thermal conductivity
    scalar kappa_;


    //- Construct from components
    inline constIsoSolidTransport(const Thermo& t, const scalar kappa)
    :
        Thermo(t),
        kappa_(kappa)
    {}


public:


    //- Construct as named copy
    inline constIsoSolidTransport
    (
        const word& name,
        const constIsoSolidTransport& ct
    )
    :
        Thermo(name, ct),
        kappa_(ct.kappa_)
    {}

    //- Construct from dictionary
    constIsoSolidTransport(const dictionary& dict);

    // Selector from dictionary
    inline static autoPtr<constIsoSolidTransport> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<constIsoSolidTransport<Thermo> >
        (
            new constIsoSolidTransport<Thermo>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "constIso<" + Thermo::typeName() + '>';
    }

    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;

    //- Isotropic thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return kappa_;
    }

    //- Un-isotropic thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const
    {
        return vector(kappa_, kappa_, kappa_);
    }

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const
    {
        NotImplemented;
        return scalar(0);
    }

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const
    {
        return kappa_/this->Cp(p, T);
    }


    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const constIsoSolidTransport& ct)
    {
        Thermo::operator=(ct);
        kappa_ = ct.kappa_;
    }

    inline void operator+=(const constIsoSolidTransport& ct)
    {
        scalar Y1 = this->Y();
        Thermo::operator+=(ct);

        Y1 /= this->Y();
        scalar Y2 = ct.Y()/this->Y();

        kappa_ = Y1*kappa_ + Y2*ct.kappa_;
    }


    // Friend operators

    friend constIsoSolidTransport operator*
    (
        const scalar s,
        const constIsoSolidTransport& ct
    )
    {
        return constIsoSolidTransport<Thermo>
        (
            s*static_cast<const Thermo&>(ct),
            ct.kappa_
        );
    }


    // Ostream Operator

    friend Ostream& operator<< <Thermo>
    (
        Ostream&,
        const constIsoSolidTransport&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::constIsoSolidTransport<Thermo>::constIsoSolidTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    kappa_(readScalar(dict.subDict("transport").lookup("kappa")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void CML::constIsoSolidTransport<Thermo>::constIsoSolidTransport::write
(
    Ostream& os
) const
{
    Thermo::write(os);

    dictionary dict("transport");
    dict.add("kappa", kappa_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const constIsoSolidTransport<Thermo>& ct
)
{
    ct.write(os);
    return os;
}


#endif
