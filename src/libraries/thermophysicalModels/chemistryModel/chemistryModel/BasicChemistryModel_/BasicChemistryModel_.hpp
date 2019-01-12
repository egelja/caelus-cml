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
    CML::BasicChemistryModel

Description
    Basic chemistry model templated on thermodynamics

SourceFiles
    BasicChemistryModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef BasicChemistryModel_HPP
#define BasicChemistryModel_HPP

#include "basicChemistryModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                     class BasicChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class BasicChemistryModel
:
    public basicChemistryModel
{
protected:

    //- Thermo
    ReactionThermo& thermo_;


public:

    //- Runtime type information
    TypeName("BasicChemistryModel");


    //- Thermo type
    typedef ReactionThermo reactionThermo;


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        BasicChemistryModel,
        thermo,
        (ReactionThermo& thermo),
        (thermo)
    );


    //- Construct from thermo
    BasicChemistryModel(ReactionThermo& thermo);


    //- Selector
    static autoPtr<BasicChemistryModel<ReactionThermo> > New
    (
        ReactionThermo& thermo
    );


    //- Destructor
    virtual ~BasicChemistryModel()
    {}


    // Member Functions

    //- Return access to the thermo package
    inline ReactionThermo& thermo()
    {
        return thermo_;
    }

    //- Return const access to the thermo package
    inline const ReactionThermo& thermo() const
    {
        return thermo_;
    }

};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::BasicChemistryModel<ReactionThermo>::BasicChemistryModel
(
    ReactionThermo& thermo
)
:
    basicChemistryModel(thermo),
    thermo_(thermo)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::autoPtr<CML::BasicChemistryModel<ReactionThermo> >
CML::BasicChemistryModel<ReactionThermo>::New(ReactionThermo& thermo)
{
    return basicChemistryModel::New<BasicChemistryModel<ReactionThermo> >
    (
        thermo
    );
}


#endif
