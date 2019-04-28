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
    CML::chemistryTabulationMethod

Description
    An abstract class for chemistry tabulation.


\*---------------------------------------------------------------------------*/

#ifndef chemistryTabulationMethod_HPP
#define chemistryTabulationMethod_HPP

#include "IOdictionary.hpp"
#include "scalarField.hpp"
#include "Switch.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
class TDACChemistryModel;

/*---------------------------------------------------------------------------*\
                           Class chemistryTabulationMethod Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class chemistryTabulationMethod
{

protected:

    const dictionary& dict_;

    const dictionary coeffsDict_;

    //- Is tabulation active?
    Switch active_;

    //- Switch to select performance logging
    Switch log_;

    TDACChemistryModel<CompType, ThermoType>& chemistry_;

    scalar tolerance_;


public:

    //- Runtime type information
    TypeName("chemistryTabulationMethod");


    // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        chemistryTabulationMethod,
        dictionary,
        (
            const dictionary& dict,
            TDACChemistryModel<CompType, ThermoType>& chemistry
        ),
        (dict, chemistry)
    );


    //- Construct from components
    chemistryTabulationMethod
    (
        const dictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    // Selectors
    static autoPtr<chemistryTabulationMethod> New
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    //- Destructor
    virtual ~chemistryTabulationMethod()
    {}


    // Member Functions

    inline bool active()
    {
        return active_;
    }

    inline bool log()
    {
        return active_ && log_;
    }

    inline bool variableTimeStep()
    {
        return chemistry_.variableTimeStep();
    }

    inline scalar tolerance() const
    {
        return tolerance_;
    }

    virtual label size() = 0;

    virtual void writePerformance() = 0;

    // Retrieve function: (only virtual here)
    // Try to retrieve a stored point close enough (according to tolerance)
    // to a stored point. If successful, it returns true and store the
    // results in RphiQ, i.e. the result of the integration of phiQ
    virtual bool retrieve
    (
         const scalarField& phiQ,
         scalarField& RphiQ
    ) = 0;

    // Add function: (only virtual here)
    // Add information to the tabulation algorithm. Give the reference for
    // future retrieve (phiQ) and the corresponding result (RphiQ).
    virtual label add
    (
        const scalarField& phiQ,
        const scalarField& RphiQ,
        const scalar rho,
        const scalar deltaT
    ) = 0;

    // Update function: (only virtual here)
    // The underlying structure of the tabulation is updated/cleaned
    // to increase the performance of the retrieve
    virtual bool update() = 0;
};


} // End namespace CML


#include "TDACChemistryModel.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryTabulationMethod<CompType, ThermoType>::chemistryTabulationMethod
(
    const dictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    dict_(dict),
    coeffsDict_(dict.subDict("tabulation")),
    active_(coeffsDict_.lookupOrDefault<Switch>("active", false)),
    log_(coeffsDict_.lookupOrDefault<Switch>("log", false)),
    chemistry_(chemistry),
    tolerance_(coeffsDict_.lookupOrDefault<scalar>("tolerance", 1e-4))
{}


#include "Time.hpp"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::autoPtr<CML::chemistryTabulationMethod<CompType, ThermoType> >
CML::chemistryTabulationMethod<CompType, ThermoType>::New
(
    const IOdictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
{
    const dictionary& tabulationDict(dict.subDict("tabulation"));

    const word methodName(tabulationDict.lookup("method"));

    Info<< "Selecting chemistry tabulation method " << methodName << endl;

    const word methodTypeName =
        methodName + '<' + CompType::typeName + ',' + ThermoType::typeName()
        + '>';

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

    return autoPtr<chemistryTabulationMethod<CompType, ThermoType> >
    (
        cstrIter()(dict, chemistry)
    );

}


#endif
