/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::ChemistryCombustion

Description
    Chemistry model wrapper for combustion models


\*---------------------------------------------------------------------------*/

#ifndef ChemistryCombustion_HPP
#define ChemistryCombustion_HPP

#include "autoPtr.hpp"
#include "CombustionModel_.hpp"
#include "BasicChemistryModel_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    class ChemistryCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class ChemistryCombustion
:
    public CombustionModel<ReactionThermo>
{
protected:

    // Protected data

        //- Pointer to chemistry model
        autoPtr<BasicChemistryModel<ReactionThermo>> chemistryPtr_;


public:

    // Constructors

        //- Construct from components and thermo
        ChemistryCombustion
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressible::turbulenceModel& turb,
            const word& combustionProperties
        );


    //- Destructor
    virtual ~ChemistryCombustion();


    // Member Functions

        //- Return access to the thermo package
        virtual ReactionThermo& thermo();

        //- Return const access to the thermo package
        virtual const ReactionThermo& thermo() const;
};


} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::ChemistryCombustion<ReactionThermo>::ChemistryCombustion
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    CombustionModel<ReactionThermo>
    (
        modelType,
        thermo,
        turb,
        combustionProperties
    ),
    chemistryPtr_(BasicChemistryModel<ReactionThermo>::New(thermo))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::ChemistryCombustion<ReactionThermo>::
~ChemistryCombustion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
ReactionThermo&
CML::ChemistryCombustion<ReactionThermo>::thermo()
{
    return chemistryPtr_->thermo();
}


template<class ReactionThermo>
const ReactionThermo&
CML::ChemistryCombustion<ReactionThermo>::thermo() const
{
    return chemistryPtr_->thermo();
}


#endif
