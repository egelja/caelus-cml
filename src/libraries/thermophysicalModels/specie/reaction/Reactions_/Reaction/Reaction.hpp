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
    CML::Reaction

Description
    Simple extension of ReactionThermo to handle reaction kinetics in addition
    to the equilibrium thermodynamics already handled.

SourceFiles
    ReactionI.hpp
    Reaction.cpp

\*---------------------------------------------------------------------------*/

#ifndef Reaction_H
#define Reaction_H

#include "speciesTable.hpp"
#include "HashPtrTable.hpp"
#include "scalarField.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class ReactionThermo>
class Reaction;

template<class ReactionThermo>
inline Ostream& operator<<(Ostream&, const Reaction<ReactionThermo>&);


/*---------------------------------------------------------------------------*\
                           Class Reaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class Reaction
:
    public ReactionThermo
{

public:

    // Static data

        //- Number of un-named reactions
        static label nUnNamedReactions;


    // Public data types

        //- Class to hold the specie index and its coefficients in the
        //  reaction rate expression
        struct specieCoeffs
        {
            label index;
            scalar stoichCoeff;
            scalar exponent;

            specieCoeffs()
            :
                index(-1),
                stoichCoeff(0),
                exponent(1)
            {}

            specieCoeffs(const speciesTable& species, Istream& is);

            bool operator==(const specieCoeffs& sc) const
            {
                return index == sc.index;
            }

            bool operator!=(const specieCoeffs& sc) const
            {
                return index != sc.index;
            }

            friend Ostream& operator<<(Ostream& os, const specieCoeffs& sc)
            {
                os  << sc.index << token::SPACE
                    << sc.stoichCoeff << token::SPACE
                    << sc.exponent;
                return os;
            }
        };


private:

    // Private data

        //- Name of reaction
        const word name_;

        //- List of specie names present in reaction system
        const speciesTable& species_;

        //- Specie info for the left-hand-side of the reaction
        List<specieCoeffs> lhs_;

        //- Specie info for the right-hand-side of the reaction
        List<specieCoeffs> rhs_;


    // Private Member Functions

        //- Return string representation of reaction
        string reactionStr() const;

        //- Construct the left- and right-hand-side reaction coefficients
        void setLRhs(Istream&);

        //- Construct reaction thermo
        void setThermo(const HashPtrTable<ReactionThermo>& thermoDatabase);

        //- Disallow default bitwise assignment
        void operator=(const Reaction<ReactionThermo>&);

        //- Return new reaction ID for un-named reactions
        label getNewReactionID();


public:

    //- Runtime type information
    TypeName("Reaction");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            Reaction,
            Istream,
            (
                const speciesTable& species,
                const HashPtrTable<ReactionThermo>& thermoDatabase,
                Istream& is
            ),
            (species, thermoDatabase, is)
        );

        declareRunTimeSelectionTable
        (
            autoPtr,
            Reaction,
            dictionary,
            (
                const speciesTable& species,
                const HashPtrTable<ReactionThermo>& thermoDatabase,
                const dictionary& dict
            ),
            (species, thermoDatabase, dict)
        );


    // Public classes

        //- Class used for the read-construction of PtrLists of reaction
        class iNew
        {
            const speciesTable& species_;
            const HashPtrTable<ReactionThermo>& thermoDatabase_;

        public:

            iNew
            (
                const speciesTable& species,
                const HashPtrTable<ReactionThermo>& thermoDatabase
            )
            :
                species_(species),
                thermoDatabase_(thermoDatabase)
            {}

            autoPtr<Reaction> operator()(Istream& is) const
            {
                return autoPtr<Reaction>
                (
                    Reaction::New(species_, thermoDatabase_, is)
                );
            }
        };


    // Constructors

        //- Construct from components
        Reaction
        (
            const speciesTable& species,
            const List<specieCoeffs>& lhs,
            const List<specieCoeffs>& rhs,
            const HashPtrTable<ReactionThermo>& thermoDatabase
        );

        //- Construct as copy given new speciesTable
        Reaction(const Reaction<ReactionThermo>&, const speciesTable& species);

        //- Construct from Istream
        Reaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            Istream& is
        );

        //- Construct from dictionary
        Reaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const dictionary& dict
        );

        //- Construct and return a clone
        virtual autoPtr<Reaction<ReactionThermo> > clone() const
        {
            return autoPtr<Reaction<ReactionThermo> >
            (
                new Reaction<ReactionThermo>(*this)
            );
        }

        //- Construct and return a clone with new speciesTable
        virtual autoPtr<Reaction<ReactionThermo> > clone
        (
            const speciesTable& species
        ) const
        {
            return autoPtr<Reaction<ReactionThermo> >
            (
                new Reaction<ReactionThermo>(*this, species)
            );
        }


    // Selectors

        //- Return a pointer to new patchField created on freestore from input
        static autoPtr<Reaction<ReactionThermo> > New
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            Istream& is
        );

        //- Return a pointer to new patchField created on freestore from dict
        static autoPtr<Reaction<ReactionThermo> > New
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const dictionary& dict
        );


    //- Destructor
    virtual ~Reaction()
    {}


    // Member Functions

        // Access

            inline word& name();
            inline const word& name() const;

            inline const List<specieCoeffs>& lhs() const;
            inline const List<specieCoeffs>& rhs() const;


        // Reaction rate coefficients

            //- Forward rate constant
            virtual scalar kf
            (
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;

            //- Reverse rate constant from the given forward rate constant
            virtual scalar kr
            (
                const scalar kfwd,
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;

            //- Reverse rate constant.
            //  Note this evaluates the forward rate constant and divides by the
            //  equilibrium constant
            virtual scalar kr
            (
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;


        //- Write
        virtual void write(Ostream&) const;


    // Ostream Operator

        friend Ostream& operator<< <ReactionThermo>
        (
            Ostream&,
            const Reaction<ReactionThermo>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo>
inline word& Reaction<ReactionThermo>::name()
{
    return name_;
}


template<class ReactionThermo>
inline const word& Reaction<ReactionThermo>::name() const
{
    return name_;
}


template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::lhs() const
{
    return lhs_;
}


template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::rhs() const
{
    return rhs_;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class ReactionThermo>
inline Ostream& operator<<(Ostream& os, const Reaction<ReactionThermo>& r)
{
    os << r.type() << nl << "    ";

    forAll(r.lhs_, i)
    {
        const typename Reaction<ReactionThermo>::specieCoeffs& sc = r.lhs_[i];

        if (sc.stoichCoeff != 1)
        {
            os << sc.stoichCoeff;
        }

        os << r.species_[sc.index];

        if (sc.exponent != sc.stoichCoeff)
        {
            os << '^' << sc.exponent;
        }

        if (i < r.lhs_.size() - 1)
        {
            os << " + ";
        }
    }

    os << " = ";

    forAll(r.rhs_, i)
    {
        const typename Reaction<ReactionThermo>::specieCoeffs& sc = r.rhs_[i];

        if (sc.stoichCoeff != 1)
        {
            os << sc.stoichCoeff;
        }

        os << r.species_[sc.index];

        if (sc.exponent != sc.stoichCoeff)
        {
            os << '^' << sc.exponent;
        }

        if (i < r.rhs_.size() - 1)
        {
            os << " + ";
        }
    }

    os  << endl << "   ";

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * * Static Data * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::label CML::Reaction<ReactionThermo>::nUnNamedReactions = 0;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ReactionThermo>
CML::label CML::Reaction<ReactionThermo>::getNewReactionID()
{
    return nUnNamedReactions++;
}


template<class ReactionThermo>
CML::string CML::Reaction<ReactionThermo>::reactionStr() const
{
    OStringStream reaction;

    for (label i = 0; i < lhs_.size(); ++i)
    {
        if (i > 0)
        {
            reaction << " + ";
        }
        if (mag(lhs_[i].stoichCoeff - 1) > SMALL)
        {
            reaction << lhs_[i].stoichCoeff;
        }
        reaction << species_[lhs_[i].index];
        if (mag(lhs_[i].exponent - lhs_[i].stoichCoeff) > SMALL)
        {
            reaction << "^" << lhs_[i].exponent;
        }
    }

    reaction << " = ";

    for (label i = 0; i < rhs_.size(); ++i)
    {
        if (i > 0)
        {
            reaction << " + ";
        }
        if (mag(rhs_[i].stoichCoeff - 1) > SMALL)
        {
            reaction << rhs_[i].stoichCoeff;
        }
        reaction << species_[rhs_[i].index];
        if (mag(rhs_[i].exponent - rhs_[i].stoichCoeff) > SMALL)
        {
            reaction << "^" << rhs_[i].exponent;
        }
    }

    return reaction.str();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::setThermo
(
    const HashPtrTable<ReactionThermo>& thermoDatabase
)
{
    ReactionThermo::operator=
    (
        rhs_[0].stoichCoeff*(*thermoDatabase[species_[rhs_[0].index]])
    );

    for (label i=1; i<rhs_.size(); ++i)
    {
        this->operator+=
        (
            rhs_[i].stoichCoeff*(*thermoDatabase[species_[rhs_[i].index]])
        );
    }

    forAll(lhs_, i)
    {
        this->operator-=
        (
            lhs_[i].stoichCoeff*(*thermoDatabase[species_[lhs_[i].index]])
        );
    }
}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const speciesTable& species,
    const List<specieCoeffs>& lhs,
    const List<specieCoeffs>& rhs,
    const HashPtrTable<ReactionThermo>& thermoDatabase
)
:
    ReactionThermo(*thermoDatabase[species[0]]),
    name_("un-named-reaction-" + CML::name(getNewReactionID())),
    species_(species),
    lhs_(lhs),
    rhs_(rhs)
{
    setThermo(thermoDatabase);
}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const Reaction<ReactionThermo>& r,
    const speciesTable& species
)
:
    ReactionThermo(r),
    name_(r.name() + "Copy"),
    species_(species),
    lhs_(r.lhs_),
    rhs_(r.rhs_)
{}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::specieCoeffs::specieCoeffs
(
    const speciesTable& species,
    Istream& is
)
{
    token t(is);

    if (t.isNumber())
    {
        stoichCoeff = t.number();
        is >> t;
    }
    else
    {
        stoichCoeff = 1.0;
    }

    exponent = stoichCoeff;

    if (t.isWord())
    {
        word specieName = t.wordToken();

        size_t i = specieName.find('^');

        if (i != word::npos)
        {
            string exponentStr = specieName
            (
                i + 1,
                specieName.size() - i - 1
            );
            exponent = atof(exponentStr.c_str());
            specieName = specieName(0, i);
        }

        index = species[specieName];
    }
    else
    {
        FatalIOErrorIn("Reaction<ReactionThermo>::lrhs(Istream& is)", is)
            << "Expected a word but found " << t.info()
            << exit(FatalIOError);
    }
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::setLRhs(Istream& is)
{
    DynamicList<specieCoeffs> dlrhs;

    while (is)
    {
        dlrhs.append(specieCoeffs(species_, is));

        token t(is);

        if (t.isPunctuation())
        {
            if (t == token::ADD)
            {
            }
            else if (t == token::ASSIGN)
            {
                lhs_ = dlrhs.shrink();
                dlrhs.clear();
            }
            else
            {
                rhs_ = dlrhs.shrink();
                is.putBack(t);
                return;
            }
        }
        else
        {
            rhs_ = dlrhs.shrink();
            is.putBack(t);
            return;
        }
    }

    FatalIOErrorIn("Reaction<ReactionThermo>::setLRhs(Istream& is)", is)
        << "Cannot continue reading reaction data from stream"
        << exit(FatalIOError);
}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    Istream& is
)
:
    ReactionThermo(*thermoDatabase[species[0]]),
    name_("un-named-reaction" + CML::name(getNewReactionID())),
    species_(species)
{
    setLRhs(is);
    setThermo(thermoDatabase);
}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    ReactionThermo(*thermoDatabase[species[0]]),
    name_(dict.dictName()),
    species_(species)
{
    setLRhs(IStringStream(dict.lookup("reaction"))());
    setThermo(thermoDatabase);
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::autoPtr<CML::Reaction<ReactionThermo> >
CML::Reaction<ReactionThermo>::New
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    Istream& is
)
{
    if (is.eof())
    {
        FatalIOErrorIn
        (
            "Reaction<ReactionThermo>::New(const speciesTable&, "
            " const HashPtrTable<ReactionThermo>&, Istream&)",
            is
        )   << "Reaction type not specified" << nl << nl
            << "Valid Reaction types are :" << nl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word reactionTypeName(is);

    typename IstreamConstructorTable::iterator cstrIter
        = IstreamConstructorTablePtr_->find(reactionTypeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "Reaction<ReactionThermo>::New(const speciesTable&, "
            " const HashPtrTable<ReactionThermo>&, Istream&)",
            is
        )   << "Unknown reaction type "
            << reactionTypeName << nl << nl
            << "Valid reaction types are :" << nl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<Reaction<ReactionThermo> >
    (
        cstrIter()(species, thermoDatabase, is)
    );
}


template<class ReactionThermo>
CML::autoPtr<CML::Reaction<ReactionThermo> >
CML::Reaction<ReactionThermo>::New
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
{
    const word& reactionTypeName = dict.lookup("type");

    typename dictionaryConstructorTable::iterator cstrIter
        = dictionaryConstructorTablePtr_->find(reactionTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "Reaction<ReactionThermo>::New"
            "("
                "const speciesTable&, "
                "const HashPtrTable<ReactionThermo>&, "
                "const dictionary&"
            ")"
        )   << "Unknown reaction type "
            << reactionTypeName << nl << nl
            << "Valid reaction types are :" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<Reaction<ReactionThermo> >
    (
        cstrIter()(species, thermoDatabase, dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::write(Ostream& os) const
{
    os.writeKeyword("reaction") << reactionStr() << token::END_STATEMENT << nl;
}


template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::kf
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return 0.0;
}


template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::kr
(
    const scalar kfwd,
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return 0.0;
}


template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::kr
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return 0.0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
