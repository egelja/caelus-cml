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
    CML::chemistrySolver

Description
    An abstract base class for solving chemistry


\*---------------------------------------------------------------------------*/

#ifndef chemistrySolver_H
#define chemistrySolver_H

#include "ODEChemistryModel.hpp"
#include "IOdictionary.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class chemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class ODEChemistryType>
class chemistrySolver
:
    public ODEChemistryType
{
protected:

    // Protected data

        //- Name of the chemistry solver
        const word name_;


public:

        //- Runtime type information
        TypeName("chemistrySolver");


    // Constructors

        //- Construct from components
        chemistrySolver
        (
            const fvMesh& mesh,
            const word& ODEModelName,
            const word& thermoType
        );


    //- Destructor
    virtual ~chemistrySolver();


    // Member Functions

        //- Update the concentrations and return the chemical time
        virtual scalar solve
        (
            scalarField &c,
            const scalar T,
            const scalar p,
            const scalar t0,
            const scalar dt
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeChemistrySolver(ODEChem)                                          \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        chemistrySolver<ODEChem>,                                             \
        "chemistrySolver<"#ODEChem">",                                        \
        0                                                                     \
    );



#define makeChemistrySolverType(SS, ODEChem, Comp, Thermo)                    \
                                                                              \
    typedef SS<ODEChem<Comp, Thermo> > SS##ODEChem##Comp##Thermo;             \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        SS##ODEChem##Comp##Thermo,                                            \
        #SS"<"#ODEChem"<"#Comp","#Thermo">>",                                 \
        0                                                                     \
    );                                                                        \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        Comp,                                                                 \
        SS##ODEChem##Comp##Thermo,                                            \
        fvMesh                                                                \
    );


#define makeSolidChemistrySolverType(SS, ODEChem, Comp, SThermo, GThermo)     \
                                                                              \
    typedef SS<ODEChem<Comp, SThermo, GThermo> >                              \
        SS##ODEChem##Comp##SThermo##GThermo;                                  \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        SS##ODEChem##Comp##SThermo##GThermo,                                  \
        #SS"<"#ODEChem"<"#Comp","#SThermo","#GThermo">>",                     \
        0                                                                     \
    );                                                                        \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        Comp,                                                                 \
        SS##ODEChem##Comp##SThermo##GThermo,                                  \
        fvMesh                                                                \
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::chemistrySolver<ODEChemistryType>::chemistrySolver
(
    const fvMesh& mesh,
    const word& ODEModelName,
    const word& thermoType
)
:
    ODEChemistryType(mesh, ODEModelName, thermoType),
    name_(ODEModelName)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::chemistrySolver<ODEChemistryType>::~chemistrySolver()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
