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
    CML::reactingMixture


\*---------------------------------------------------------------------------*/

#ifndef reactingMixture_HPP
#define reactingMixture_HPP

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

    //- Table of species composition
    speciesCompositionTable speciesComposition_;


    //- Disallow default bitwise copy construct
    reactingMixture(const reactingMixture&);

    //- Disallow default bitwise assignment
    void operator=(const reactingMixture&);


public:

    //- The type of thermo package this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    reactingMixture(const dictionary&, const fvMesh&, const word&);


    //- Destructor
    virtual ~reactingMixture()
    {}


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "reactingMixture<" + ThermoType::typeName() + '>';
    }

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

    const Reaction<ThermoType>& operator[](const label i) const
    {
        return PtrList<Reaction<ThermoType> >::operator[](i);
    }

    //- Table of species composition
    const speciesCompositionTable& specieComposition() const
    {
        return speciesComposition_;
     }

};


} // End namespace CML

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::reactingMixture<ThermoType>::reactingMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
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
        mesh,
        phaseName
    ),
    PtrList<Reaction<ThermoType> >
    (
        autoPtr<chemistryReader<ThermoType> >::operator()().reactions()
    ),
    speciesComposition_
    (
        autoPtr<chemistryReader<ThermoType> >::operator()().specieComposition()
    )
{
    autoPtr<chemistryReader<ThermoType> >::clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void CML::reactingMixture<ThermoType>::read(const dictionary& thermoDict)
{}


#endif
