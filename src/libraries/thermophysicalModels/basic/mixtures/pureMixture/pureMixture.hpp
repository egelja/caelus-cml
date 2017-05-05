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
    CML::pureMixture

Description
    CML::pureMixture

SourceFiles
    pureMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef pureMixture_H
#define pureMixture_H

#include "basicMixture.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class pureMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class pureMixture
:
    public basicMixture
{
    // Private data

        ThermoType mixture_;

        //- Construct as copy (not implemented)
        pureMixture(const pureMixture<ThermoType>&);


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Runtime type information
    TypeName("pureMixture");


    // Constructors

        //- Construct from dictionary and mesh
        pureMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~pureMixture();


    // Member functions

        const ThermoType& cellMixture(const label) const
        {
            return mixture_;
        }

        const ThermoType& patchFaceMixture
        (
            const label,
            const label
        ) const
        {
            return mixture_;
        }

        //- Read dictionary
        void read(const dictionary&);
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
pureMixture<ThermoType>::pureMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh
)
:
    basicMixture(thermoDict, mesh),
    mixture_(thermoDict.subDict("mixture"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ThermoType>
pureMixture<ThermoType>::~pureMixture()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void pureMixture<ThermoType>::read(const dictionary& thermoDict)
{
    mixture_ = ThermoType(thermoDict.subDict("mixture"));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
