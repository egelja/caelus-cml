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
    CML::reactingSolidMixture

Description
    CML::reactingSolidMixture

SourceFiles
    reactingSolidMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef reactingSolidMixture_H
#define reactingSolidMixture_H

#include "multiComponentSolidMixture.hpp"
#include "solidReaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class reactingSolidMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoSolidType>
class reactingSolidMixture
:
    public multiComponentSolidMixture<ThermoSolidType>,
    public PtrList<solidReaction>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        reactingSolidMixture(const reactingSolidMixture&);

        //- Disallow default bitwise assignment
        void operator=(const reactingSolidMixture&);


public:

    //- The type of thermo package this mixture is instantiated for
    typedef ThermoSolidType thermoType;


    // Constructors

        //- Construct from dictionary and mesh
        reactingSolidMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~reactingSolidMixture()
    {}


    // Member functions

        //- Read dictionary
        void read(const dictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoSolidType>
CML::reactingSolidMixture<ThermoSolidType>::reactingSolidMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh
)
:
    multiComponentSolidMixture<ThermoSolidType>
    (
        thermoDict,
        mesh
    ),
    PtrList<solidReaction>
    (
        mesh.lookupObject<dictionary>
            ("chemistryProperties").lookup("reactions"),
        solidReaction::iNew
        (
            this->components_,
            mesh.lookupObject<dictionary>
                ("chemistryProperties").lookup("species")
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoSolidType>
void CML::reactingSolidMixture<ThermoSolidType>::read
(
  const dictionary& thermoDict
)
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
