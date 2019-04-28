/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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
    CML::chemistryReductionMethod

Description
    An abstract class for methods of chemical mechanism reduction


\*---------------------------------------------------------------------------*/

#ifndef chemistryReductionMethod_HPP
#define chemistryReductionMethod_HPP

#include "IOdictionary.hpp"
#include "Switch.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class CompType, class ThermoType>
class TDACChemistryModel;

/*---------------------------------------------------------------------------*\
                           Class chemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class chemistryReductionMethod
{

protected:

    const IOdictionary& dict_;

    //- Dictionary that store the algorithm data
    const dictionary coeffsDict_;

    //- Is mechanism reduction active?
    Switch active_;

    //- Switch to select performance logging
    Switch log_;

    TDACChemistryModel<CompType, ThermoType>& chemistry_;

    //- List of active species (active = true)
    List<bool> activeSpecies_;

    //- Number of active species
    label NsSimp_;

    //- Number of species
    const label nSpecie_;

    //- Tolerance for the mechanism reduction algorithm
    scalar tolerance_;


public:

    //- Runtime type information
    TypeName("chemistryReductionMethod");


    // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        chemistryReductionMethod,
        dictionary,
        (
            const IOdictionary& dict,
            TDACChemistryModel<CompType, ThermoType>& chemistry
        ),
        (dict, chemistry)
    );


    //- Construct from components
    chemistryReductionMethod
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    // Selector
    static autoPtr<chemistryReductionMethod<CompType, ThermoType> > New
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    //- Destructor
    virtual ~chemistryReductionMethod()
    {}


    // Member Functions

    //- Is mechanism reduction active?
    inline bool active() const
    {
        return active_;
    }

    //- Is performance data logging enabled?
    inline bool log() const
    {
        return active_ && log_;
    }

    //- Return the active species
    inline const List<bool>& activeSpecies() const
    {
        return activeSpecies_;
    }

    //- Return the number of active species
    inline label NsSimp()
    {
        return NsSimp_;
    }

    //- Return the initial number of species
    inline label nSpecie()
    {
        return nSpecie_;
    }

    //- Return the tolerance
    inline scalar tolerance() const
    {
        return tolerance_;
    }

    //- Reduce the mechanism
    virtual void reduceMechanism
    (
        const scalarField &c,
        const scalar T,
        const scalar p
    ) = 0;
};


} // End namespace CML

#include "TDACChemistryModel.hpp"
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryReductionMethod<CompType, ThermoType>::chemistryReductionMethod
(
    const CML::IOdictionary& dict,
    CML::TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    dict_(dict),
    coeffsDict_(dict.subDict("reduction")),
    active_(coeffsDict_.lookupOrDefault<Switch>("active", false)),
    log_(coeffsDict_.lookupOrDefault<Switch>("log", false)),
    chemistry_(chemistry),
    activeSpecies_(chemistry.nSpecie(), false),
    NsSimp_(chemistry.nSpecie()),
    nSpecie_(chemistry.nSpecie()),
    tolerance_(coeffsDict_.lookupOrDefault<scalar>("tolerance", 1e-4))
{}


#include "Time.hpp"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::autoPtr<CML::chemistryReductionMethod<CompType, ThermoType> >
CML::chemistryReductionMethod<CompType, ThermoType>::New
(
    const IOdictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
{
    const dictionary& reductionDict(dict.subDict("reduction"));

    const word methodName(reductionDict.lookup("method"));

    Info<< "Selecting chemistry reduction method " << methodName << endl;

    const word methodTypeName =
        methodName
      + '<' + CompType::typeName + ',' + ThermoType::typeName() + '>';

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(methodTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown " << typeName_() << " type " << methodName << endl
            << endl;

        const wordList names(dictionaryConstructorTablePtr_->toc());

        wordList thisCmpts;
        thisCmpts.append(word::null);
        thisCmpts.append(CompType::typeName);
        thisCmpts.append
        (
            basicThermo::splitThermoName(ThermoType::typeName(), 5)
        );

        wordList validNames;
        forAll(names, i)
        {
            const wordList cmpts(basicThermo::splitThermoName(names[i], 7));

            bool isValid = true;
            for (label i = 1; i < cmpts.size() && isValid; ++ i)
            {
                isValid = isValid && cmpts[i] == thisCmpts[i];
            }

            if (isValid)
            {
                validNames.append(cmpts[0]);
            }
        }

        FatalErrorInFunction
            << "Valid " << typeName_() << " types for this thermodynamic model "
            << "are:" << endl << validNames << endl;

        List<wordList> validCmpts;
        validCmpts.append(wordList(7, word::null));
        validCmpts[0][0] = typeName_();
        validCmpts[0][1] = "reactionThermo";
        validCmpts[0][2] = "transport";
        validCmpts[0][3] = "thermo";
        validCmpts[0][4] = "equationOfState";
        validCmpts[0][5] = "specie";
        validCmpts[0][6] = "energy";
        forAll(names, i)
        {
            validCmpts.append(basicThermo::splitThermoName(names[i], 7));
        }

        FatalErrorInFunction
            << "All " << validCmpts[0][0] << '/' << validCmpts[0][1]
            << "/thermoPhysics combinations are:" << endl << endl;
        printTable(validCmpts, FatalErrorInFunction);

        FatalErrorInFunction << exit(FatalError);
    }

    return autoPtr<chemistryReductionMethod<CompType, ThermoType> >
    (
        cstrIter()(dict, chemistry)
    );
}


#endif
