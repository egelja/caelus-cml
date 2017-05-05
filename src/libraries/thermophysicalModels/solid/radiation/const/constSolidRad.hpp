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
    CML::constSolidRad

Description
    Constant radiative properties


\*---------------------------------------------------------------------------*/

#ifndef constSolidRad_H
#define constSolidRad_H

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class thermo> class constSolidRad;

template<class thermo>
Ostream& operator<<
(
    Ostream&,
    const constSolidRad<thermo>&
);

/*---------------------------------------------------------------------------*\
                      Class constSolidRad Declaration
\*---------------------------------------------------------------------------*/

template<class thermo>
class constSolidRad
:
    public  thermo
{
    // Private data

        //- Absorption coefficient
        scalar kappa_;

        //- Scattering coefficient
        scalar sigmaS_;

        //- Emissivity coefficient
        scalar emissivity_;


         //- Construct from components
        inline constSolidRad
        (
            const thermo& t,
            const scalar kappa,
            const scalar sigmaS,
            const scalar emissivity
        );


public:

    // Constructors

        //- Construct from Istream
        constSolidRad(const dictionary&);

        //- Construct as named copy
        inline constSolidRad(const word& name, const constSolidRad&);

        //- Construct and return a clone
        inline autoPtr<constSolidRad> clone() const;

        // Selector from Istream
        inline static autoPtr<constSolidRad> New(Istream& is);


    // Member functions

        //- Return absorption coefficient [1/m]
        inline scalar kappa(scalar T) const;

        //- Return scattering [1/m]
        inline scalar sigmaS(scalar T) const;

        //- Return  emissivity[]
        inline scalar emissivity(scalar T) const;


    // Ostream Operator

        friend Ostream& operator<< <thermo>
        (
            Ostream&,
            const constSolidRad&
        );
};



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class thermo>
inline constSolidRad<thermo>::constSolidRad
(
    const thermo& t,
    const scalar kappa,
    const scalar sigmaS,
    const scalar emissivity
)
:
    thermo(t),
    kappa_(kappa),
    sigmaS_(sigmaS),
    emissivity_(emissivity)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class thermo>
inline constSolidRad<thermo>::constSolidRad
(
    const word& name,
    const constSolidRad& pg
)
:
    thermo(name, pg),
    kappa_(pg.kappa_),
    sigmaS_(pg.sigmaS_),
    emissivity_(pg.emissivity_)
{}


template<class thermo>
inline autoPtr<constSolidRad<thermo> > constSolidRad<thermo>::clone() const
{
    return autoPtr<constSolidRad<thermo> >(new constSolidRad<thermo>(*this));
}


template<class thermo>
inline autoPtr<constSolidRad<thermo> > constSolidRad<thermo>::New(Istream& is)
{
    return autoPtr<constSolidRad<thermo> >(new constSolidRad<thermo>(is));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class thermo>
inline scalar constSolidRad<thermo>::kappa(scalar) const
{
    return kappa_;
}


template<class thermo>
inline scalar constSolidRad<thermo>::sigmaS(scalar) const
{
    return sigmaS_;
}


template<class thermo>
inline scalar constSolidRad<thermo>::emissivity(scalar) const
{
    return emissivity_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class thermo>
constSolidRad<thermo>::constSolidRad(const dictionary& dict)
:
    thermo(dict),
    kappa_(readScalar(dict.subDict("radiation").lookup("kappa"))),
    sigmaS_(readScalar(dict.subDict("radiation").lookup("sigmaS"))),
    emissivity_(readScalar(dict.subDict("radiation").lookup("emissivity")))
{}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class thermo>
Ostream& operator<<(Ostream& os, const constSolidRad<thermo>& pg)
{
    os  << static_cast<const thermo&>(pg);
    os << tab << pg.kappa_ << tab << pg.sigmaS_ << tab << pg.emissivity_;

    os.check("Ostream& operator<<(Ostream& os, const constSolidRad& st)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
