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

Class
    CML::exponentialSolidTransport

Description
    Exponential properties for solid heat transport
    Templated into a given thermodynamics package.

\*---------------------------------------------------------------------------*/

#ifndef exponentialSolidTransport_HPP
#define exponentialSolidTransport_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Thermo> class exponentialSolidTransport;

template<class Thermo>
inline exponentialSolidTransport<Thermo> operator*
(
    const scalar,
    const exponentialSolidTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const exponentialSolidTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                        Class exponentialSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class exponentialSolidTransport
:
    public Thermo
{
    //- Constant thermal coefficient.
    scalar kappa0_;

    //- Exponent coefficient
    scalar n0_;

    //- Reference temperature
    scalar Tref_;


    //- Construct from components
    inline exponentialSolidTransport
    (
        const Thermo& t,
        const scalar kappa0,
        const scalar n0,
        const scalar Tref
    )
    :
        Thermo(t),
        kappa0_(kappa0),
        n0_(n0),
        Tref_(Tref)
    {}


public:

    // Constructors

    //- Construct as named copy
    inline exponentialSolidTransport
    (
        const word& name,
        const exponentialSolidTransport& ct
    )
    :
        Thermo(name, ct),
        kappa0_(ct.kappa0_),
        n0_(ct.n0_),
        Tref_(ct.Tref_)
    {}


    //- Construct from dictionary
    exponentialSolidTransport(const dictionary&);

    // Selector from dictionary
    inline static autoPtr<exponentialSolidTransport> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<exponentialSolidTransport<Thermo> >
        (
            new exponentialSolidTransport<Thermo>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "exponential<" + Thermo::typeName() + '>';
    }

    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;

    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return (kappa0_*pow(T/Tref_, n0_));
    }

    //- Thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const
    {
        const scalar kappa(kappa0_*pow(T/Tref_, n0_));
        return vector(kappa, kappa, kappa);
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
        return kappa(p, T)/this->Cp(p, T);
    }

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators
    inline void operator=(const exponentialSolidTransport&);
    inline void operator+=(const exponentialSolidTransport&);


    // Friend operators
    friend exponentialSolidTransport operator* <Thermo>
    (
        const scalar,
        const exponentialSolidTransport&
    );

    // Ostream Operator

    friend Ostream& operator<< <Thermo>
    (
        Ostream&,
        const exponentialSolidTransport&
    );

};


} // End namespace CML


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


template<class Thermo>
inline void CML::exponentialSolidTransport<Thermo>::operator=
(
    const exponentialSolidTransport<Thermo>& ct
)
{
    kappa0_ = ct.kappa0_;
    n0_ = ct.n0_;
    Tref_ = ct.Tref_;
}


template<class Thermo>
inline void CML::exponentialSolidTransport<Thermo>::operator+=
(
    const exponentialSolidTransport<Thermo>& ct
)
{
    scalar Y1 = this->Y();

    Y1 /= this->Y();
    scalar Y2 = ct.Y()/this->Y();

    kappa0_ = Y1*kappa0_ + Y2*ct.kappa0_;
    n0_ = (Y1*n0_ + Y2*ct.n0_);
    Tref_ = (Y1*Tref_ + Y2*ct.Tref_);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::exponentialSolidTransport<Thermo> CML::operator*
(
    const scalar s,
    const exponentialSolidTransport<Thermo>& ct
)
{
    return exponentialSolidTransport<Thermo>
    (
        s*static_cast<const Thermo&>(ct),
        ct.kappa0_,
        ct.n0_,
        ct.Tref_
    );
}


#include "IOstreams.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    kappa0_(0.0),
    n0_(0.0),
    Tref_(0.0)
{
    const dictionary& subDict = dict.subDict("transport");
    kappa0_ = readScalar(subDict.lookup("kappa0"));
    n0_ = readScalar(subDict.lookup("n0"));
    Tref_ = readScalar(subDict.lookup("Tref"));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void CML::exponentialSolidTransport<Thermo>::exponentialSolidTransport::write
(
    Ostream& os
) const
{
    Thermo::write(os);

    dictionary dict("transport");
    dict.add("kappa0", kappa0_);
    dict.add("n0", n0_);
    dict.add("Tref", Tref_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<
(
    Ostream& os, const exponentialSolidTransport<Thermo>& et
)
{
    et.write(os);
    return os;
}


#endif
