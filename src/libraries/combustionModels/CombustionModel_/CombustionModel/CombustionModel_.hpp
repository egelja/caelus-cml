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
    CML::CombustionModel

Description
    Combustion models for templated thermodynamics


\*---------------------------------------------------------------------------*/

#ifndef CombustionModel_HPP
#define CombustionModel_HPP

#include "combustionModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     class CombustionModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class CombustionModel
:
    public combustionModel
{
public:

    //- Thermo type
    typedef ReactionThermo reactionThermo;


    //- Runtime type information
    TypeName("CombustionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        CombustionModel,
        dictionary,
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressible::turbulenceModel& turb,
            const word& combustionProperties
        ),
        (modelType, thermo, turb, combustionProperties)
    );


    //- Construct from components
    CombustionModel
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Selector
    static autoPtr<CombustionModel> New
    (
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties=combustionPropertiesName
    );


    //- Destructor
    virtual ~CombustionModel();


    // Member Functions

    //- Return access to the thermo package
    virtual ReactionThermo& thermo() = 0;

    //- Return const access to the thermo package
    virtual const ReactionThermo& thermo() const = 0;


    // IO

    //- Update properties from given dictionary
    virtual bool read();

};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::CombustionModel<ReactionThermo>::CombustionModel
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    combustionModel(modelType, thermo, turb, combustionProperties)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::autoPtr<CML::CombustionModel<ReactionThermo>>
CML::CombustionModel<ReactionThermo>::New
(
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
{
    return
        combustionModel::New<CombustionModel<ReactionThermo>>
        (
            thermo,
            turb,
            combustionProperties
        );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::CombustionModel<ReactionThermo>::~CombustionModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
bool CML::CombustionModel<ReactionThermo>::read()
{
    if (combustionModel::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}


#endif
