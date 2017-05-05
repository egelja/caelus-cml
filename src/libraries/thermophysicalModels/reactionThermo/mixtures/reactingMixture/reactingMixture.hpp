/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::reactingMixture

Description
    CML::reactingMixture


\*---------------------------------------------------------------------------*/

#ifndef reactingMixture_H
#define reactingMixture_H

#include "speciesTable.hpp"
#include "chemistryReader.hpp"
#include "multiComponentMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class reactingMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class reactingMixture
:
    public speciesTable,
    public autoPtr<chemistryReader<ThermoType> >,
    public multiComponentMixture<ThermoType>,
    public PtrList<Reaction<ThermoType> >
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        reactingMixture(const reactingMixture&);

        //- Disallow default bitwise assignment
        void operator=(const reactingMixture&);


public:

    //- The type of thermo package this mixture is instantiated for
    typedef ThermoType thermoType;


    // Constructors

        //- Construct from dictionary and mesh
        reactingMixture
        (
            const dictionary&,
            const fvMesh&,
            const bool clearReader = true
        );


    //- Destructor
    virtual ~reactingMixture()
    {}


    // Member functions

        //- Read dictionary
        void read(const dictionary&);

        label size() const
        {
            return PtrList<Reaction<ThermoType> >::size();
        }

        Reaction<ThermoType>& operator [] (const label i)
        {
            return PtrList<Reaction<ThermoType> >::operator[](i);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::reactingMixture<ThermoType>::reactingMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const bool clearReader
)
:
    speciesTable(),
    autoPtr<chemistryReader<ThermoType> >
    (
        chemistryReader<ThermoType>::New(thermoDict, *this)
    ),
    multiComponentMixture<ThermoType>
    (
        thermoDict,
        *this,
        autoPtr<chemistryReader<ThermoType> >::operator()().speciesThermo(),
        mesh
    ),
    PtrList<Reaction<ThermoType> >
    (
        autoPtr<chemistryReader<ThermoType> >::operator()().reactions()
    )
{
    if (clearReader)
    {
        autoPtr<chemistryReader<ThermoType> >::clear();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void CML::reactingMixture<ThermoType>::read(const dictionary& thermoDict)
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
