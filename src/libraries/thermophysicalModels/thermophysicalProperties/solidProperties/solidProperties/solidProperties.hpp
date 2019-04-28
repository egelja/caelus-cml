/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::solidProperties

Description
    The thermophysical properties of a solid

SourceFiles
    solidProperties.cpp

\*---------------------------------------------------------------------------*/

#ifndef solidProperties_H
#define solidProperties_H

#include "typeInfo.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "dictionary.hpp"
#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class solidProperties Declaration
\*---------------------------------------------------------------------------*/

class solidProperties
{
    // Private data
    //- Density [kg/m3]
    scalar rho_;

    //- Specific heat capacity [J/(kg.K)]
    scalar Cp_;

    //- Thermal conductivity [W/(m.K)]
    scalar kappa_;

    //- Heat of formation [J/kg]
    scalar Hf_;

    //- Emissivity
    scalar emissivity_;


public:

    //- Runtime type information
    TypeName("solid");


    // Declare run-time constructor selection tables

    declareRunTimeSelectionTable
    (
        autoPtr,
        solidProperties,
        ,
        (),
        ()
    );

    declareRunTimeSelectionTable
    (
        autoPtr,
        solidProperties,
        dictionary,
        (const dictionary& dict),
        (dict)
    );


    //- Construct from components
    solidProperties
    (
        scalar rho,
        scalar Cp,
        scalar kappa,
        scalar Hf,
        scalar emissivity
    );

    //- Construct from dictionary
    solidProperties(const dictionary& dict);

    //- Construct and return clone
    virtual autoPtr<solidProperties> clone() const
    {
        return autoPtr<solidProperties>(new solidProperties(*this));
    }


    // Selectors

    //- Return a pointer to a new solidProperties created from name
    static autoPtr<solidProperties> New(const word& name);

    //- Return a pointer to a new solidProperties created from dictionary
    static autoPtr<solidProperties> New(const dictionary& dict);


    //- Destructor
    virtual ~solidProperties()
    {}


    // Member Functions

    // Physical constants which define the solidProperties

    //- Density [kg/m3]
    inline scalar rho() const
    {
        return rho_;
    }

    //- Specific heat capacity [J/(kg.K)]
    inline scalar Cp() const
    {
        return Cp_;
    }

    //- Thermal conductivity [W/(m.K)]
    inline scalar kappa() const
    {
        return kappa_;
    }

    //- Heat of formation [J/kg]
    inline scalar Hf() const
    {
        return Hf_;
    }

    //- Sensible enthalpy - reference to Tstd [J/kg]
    inline scalar Hs(const scalar T) const
    {
        return Cp_*(T - Tstd);
    }

    //- Emissivity []
    inline scalar emissivity() const
    {
        return emissivity_;
    }


    // I-O

    //- Read and set the properties present it the given dictionary
    void readIfPresent(const dictionary& dict);

    //- Write the solidProperties properties
    virtual void writeData(Ostream& os) const;

    //- Ostream Operator
    friend Ostream& operator<<(Ostream& os, const solidProperties& s);
};


Ostream& operator<<(Ostream&, const solidProperties&);


} // End namespace CML


#endif
