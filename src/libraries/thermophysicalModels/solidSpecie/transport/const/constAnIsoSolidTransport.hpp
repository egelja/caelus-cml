/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::constAnIsoSolidTransport

Description
    Constant properties Transport package.
    Templated into a given Thermodynamics package (needed for thermal
    conductivity).


\*---------------------------------------------------------------------------*/

#ifndef constAnIsoSolidTransport_HPP
#define constAnIsoSolidTransport_HPP
#include "vector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Thermo> class constAnIsoSolidTransport;

template<class Thermo>
inline constAnIsoSolidTransport<Thermo> operator*
(
    const scalar,
    const constAnIsoSolidTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const constAnIsoSolidTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                   Class constAnIsoSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class constAnIsoSolidTransport
:
    public Thermo
{

    //- Constant anisotropic thermal conductivity.
    vector kappa_;


    //- Construct from components
    inline constAnIsoSolidTransport(const Thermo& t, const vector kappa)
    :
        Thermo(t),
        kappa_(kappa)
    {}


public:


    //- Construct as named copy
    inline constAnIsoSolidTransport
    (
        const word& name,
        const constAnIsoSolidTransport& ct
    )
    :
        Thermo(name, ct),
        kappa_(ct.kappa_)
    {}

    //- Construct from dictionary
    constAnIsoSolidTransport(const dictionary&);


    // Selector from dictionary
    inline static autoPtr<constAnIsoSolidTransport> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<constAnIsoSolidTransport<Thermo> >
        (
            new constAnIsoSolidTransport<Thermo>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "constAnIso<" + Thermo::typeName() + '>';
    }

    //- Is the thermal conductivity isotropic
    static const bool isotropic = false;

    //- Isotropic thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return mag(kappa_);
    }

    //- Un-isotropic thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const
    {
        return kappa_;
    }

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const
    {
        NotImplemented;
        return scalar(0);
    }

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline vector alphah(const scalar p, const scalar T) const
    {
        return kappa_/this->Cp(p, T);
    }


    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const constAnIsoSolidTransport& ct)
    {
        kappa_ = ct.kappa_;
    }

    inline void operator+=(const constAnIsoSolidTransport& ct)
    {
        scalar Y1 = this->Y();

        Y1 /= this->Y();
        scalar Y2 = ct.Y()/this->Y();

        kappa_ = Y1*kappa_ + Y2*ct.kappa_;
    }


    // Friend operators

    friend constAnIsoSolidTransport operator*
    (
        const scalar s,
        const constAnIsoSolidTransport& ct
    )
    {
        return constAnIsoSolidTransport<Thermo>
        (
            s*static_cast<const Thermo&>(ct),
            ct.kappa_
        );
    }


    // Ostream Operator

    friend Ostream& operator<< <Thermo>
    (
        Ostream&,
        const constAnIsoSolidTransport&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    kappa_(dict.subDict("transport").lookup("kappa"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void CML::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport::write
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
    const constAnIsoSolidTransport<Thermo>& ct
)
{
    ct.write(os);
    return os;
}


#endif
