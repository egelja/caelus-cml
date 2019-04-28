/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::basicSolidChemistryModel

Description
    Chemistry model for solid thermodynamics

SourceFiles
    basicSolidChemistryModel.cpp
    newChemistrySolidModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef basicSolidChemistryModel_H
#define basicSolidChemistryModel_H

#include "basicChemistryModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "solidReactionThermo.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                     class basicSolidChemistryModel Declaration
\*---------------------------------------------------------------------------*/

class basicSolidChemistryModel
:
    public basicChemistryModel
{
    // Private Member Functions

    //- Construct as copy (not implemented)
    basicSolidChemistryModel(const basicSolidChemistryModel&);

    //- Disallow default bitwise assignment
    void operator=(const basicSolidChemistryModel&);


protected:

    // Protected data

    //- Solid thermo
    solidReactionThermo& solidThermo_;


public:

    //- Runtime type information
    TypeName("basicSolidChemistryModel");


    //- Thermo type
    typedef solidReactionThermo reactionThermo;


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        basicSolidChemistryModel,
        thermo,
        (solidReactionThermo& thermo),
        (thermo)
    );


    //- Construct from thermo
    basicSolidChemistryModel(solidReactionThermo& thermo);


    //- Selector
    static autoPtr<basicSolidChemistryModel> New(solidReactionThermo& thermo);


    //- Destructor
    virtual ~basicSolidChemistryModel()
    {}


    // Member Functions

    //- Return access to the solid thermo package
    inline solidReactionThermo& solidThermo()
    {
        return solidThermo_;
    }

    //- Return const access to the solid thermo package
    inline const solidReactionThermo& solidThermo() const
    {
        return solidThermo_;
    }

    //- Return total gases mass source term [kg/m3/s]
    virtual tmp<DimensionedField<scalar, volMesh> > RRg() const = 0;

    //- Return total solids mass source term [kg/m3/s]
    virtual tmp<DimensionedField<scalar, volMesh> > RRs() const = 0;

    //- Return chemical source terms for solids [kg/m3/s]
    virtual const DimensionedField<scalar, volMesh>& RRs
    (
        const label i
    ) const = 0;

    //- Return chemical source terms for gases [kg/m3/s]
    virtual const DimensionedField<scalar, volMesh>& RRg
    (
        const label i
    ) const = 0;

    //- Returns the reaction rate of the speciei in reactionI
    virtual tmp<DimensionedField<scalar, volMesh> > calculateRR
    (
        const label reactionI,
        const label speciei
    ) const;

    //- Return sensible enthalpy for gas i [J/Kg]
    virtual tmp<volScalarField> gasHs
    (
        const volScalarField& p,
        const volScalarField& T,
        const label i
    ) const = 0;

    //- Return specie Table for gases
    virtual const speciesTable& gasTable() const = 0;

    //- Set reacting status of cell, celli
    virtual void setCellReacting(const label celli, const bool active) = 0;

    //- Calculates the reaction rates
    virtual void calculate() = 0;

    //- Return const access to the total source terms
    virtual const DimensionedField<scalar, volMesh>& RR
    (
        const label i
    ) const;

    //- Return non-const access to the total source terms
    virtual DimensionedField<scalar, volMesh>& RR(const label i);
};


} // End namespace CML

#endif
