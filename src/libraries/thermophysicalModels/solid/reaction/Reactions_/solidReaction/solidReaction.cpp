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

\*---------------------------------------------------------------------------*/

#include "solidReaction.hpp"
#include "DynamicList.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(solidReaction, 0);
    defineRunTimeSelectionTable(solidReaction, Istream);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidReaction::solidReaction
(
    const speciesTable& components,
    const speciesTable& pyrolisisGases,
    const List<label>& slhs,
    const List<label>& srhs,
    const List<label>& grhs
)
:
    components_(components),
    pyrolisisGases_(pyrolisisGases),
    slhs_(slhs),
    srhs_(srhs),
    grhs_(grhs)
{}


CML::solidReaction::solidReaction
(
    const solidReaction& r,
    const speciesTable& components,
    const speciesTable& pyrolisisGases
)
:
    components_(components),
    pyrolisisGases_(pyrolisisGases),
    slhs_(r.slhs_),
    srhs_(r.srhs_),
    grhs_(r.grhs_)
{}


CML::solidReaction::solidReaction
(
    const speciesTable& components,
    Istream& is,
    const speciesTable& pyrolisisGases
)
:
    components_(components),
    pyrolisisGases_(pyrolisisGases)
{
    setLRhs(is);
}


CML::label CML::solidReaction::componentIndex
(
    bool& isGas,
    Istream& is
)
{
    token t(is);

    if (t.isWord())
    {
        word componentName = t.wordToken();

        size_t i = componentName.find('=');

        if (i != word::npos)
        {
            string exponentStr = componentName
            (
                i + 1,
                componentName.size() - i - 1
            );
            componentName = componentName(0, i);
        }
        if (components_.contains(componentName))
        {
            isGas = false;
            return (components_[componentName]);
        }
        else if (pyrolisisGases_.contains(componentName))
        {
            isGas = true;
            return (pyrolisisGases_[componentName]);
        }
        else
        {
            FatalIOErrorIn
            (
                "solidReaction::componentIndex(bool&, Istream& is)",
                is
            )
                << "Cannot find component" << componentName
                << "in tables :" << pyrolisisGases_ << " or "
                << components_
                << exit(FatalIOError);
            return -1;
        }

    }
    else
    {
        FatalIOErrorIn("solidReaction::componentIndex(bool&, Istream& is)", is)
            << "Expected a word but found " << t.info()
            << exit(FatalIOError);
        return -1;
    }
}


void CML::solidReaction::setLRhs(Istream& is)
{
    DynamicList<label> dlsrhs;

    label index = 0;

    while (is)
    {
        bool isGas = false;
        index = componentIndex(isGas, is);

        if (index != -1)
        {
            dlsrhs.append(index);

            token t(is);

            if (t.isPunctuation())
            {
                if (t == token::ADD)
                {
                    if(isGas)
                    {
                        grhs_ = dlsrhs.shrink();
                        dlsrhs.clear();
                        //is.putBack(t);
                        //return;
                    }
                    else
                    {
                        srhs_ = dlsrhs.shrink();
                        dlsrhs.clear(); //is.putBack(t);
                        //return;
                    }
                }
                else if (t == token::ASSIGN)
                {
                    if(isGas)
                    {
                        Info << "Pyrolysis Gases should appear on lhs of the"
                                "reaction" << endl;
                    }
                    else
                    {
                        slhs_ = dlsrhs.shrink();
                        dlsrhs.clear();
                    }
                }
                else if(isGas)
                {
                    grhs_ = dlsrhs.shrink();
                    is.putBack(t);
                    return;
                }
                else
                {
                    srhs_ = dlsrhs.shrink();
                    is.putBack(t);
                    return;
                }
            }
            else if(isGas)
            {
                grhs_ = dlsrhs.shrink();
                is.putBack(t);
                return;
            }
            else
            {
                srhs_ = dlsrhs.shrink();
                is.putBack(t);
                return;
            }
        }
        else
        {
            FatalIOErrorIn("solidReaction::lsrhs(Istream& is)", is)
                << "Cannot find component in tables"
                << exit(FatalIOError);
        }
    }

    FatalIOErrorIn("solidReaction::lsrhs(Istream& is)", is)
        << "Cannot continue reading reaction data from stream"
        << exit(FatalIOError);
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::solidReaction> CML::solidReaction::New
(
    const speciesTable& species,
    Istream& is,
    const speciesTable& pyrolisisGases
)
{
    if (is.eof())
    {
        FatalIOErrorIn
        (
            "solidReaction::New(const speciesTable& species,"
            " const HashPtrTable& thermoDatabase, Istream&)",
            is
        )   << "solidReaction type not specified" << nl << nl
            << "Valid solidReaction types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word reactionTypeName(is);

    IstreamConstructorTable::iterator cstrIter
        = IstreamConstructorTablePtr_->find(reactionTypeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "solidReaction::New(const speciesTable& species,"
            " const HashPtrTable& thermoDatabase, Istream&)",
            is
        )   << "Unknown reaction type "
            << reactionTypeName << nl << nl
            << "Valid reaction types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<solidReaction>
    (
        cstrIter()(species, is, pyrolisisGases)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::solidReaction::write(Ostream& os) const
{
    os << type() << nl << "    ";

    forAll(slhs_, i)
    {
        os << components_[slhs_[i]];
    }

    os << " = ";

    forAll(srhs_, i)
    {
        os << components_[srhs_[i]];
    }

    os  <<  " +  ";

    forAll(grhs_, i)
    {
        os << pyrolisisGases_[grhs_[i]];
    }

    os  << endl << "   ";
}


CML::scalar CML::solidReaction::kf
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return 0.0;
}


CML::scalar CML::solidReaction::nReact() const
{
    return 1.0;
}


// ************************************************************************* //
