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
    CML::Reaction

Description
    Simple extension of ReactionThermo to handle reaction kinetics in addition
    to the equilibrium thermodynamics already handled.


\*---------------------------------------------------------------------------*/

#ifndef Reaction_HPP
#define Reaction_HPP

#include "speciesTable.hpp"
#include "HashPtrTable.hpp"
#include "scalarField.hpp"
#include "simpleMatrix.hpp"
#include "Tuple2.hpp"
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
    public ReactionThermo::thermoType
{
protected:

    //- Return string representation of the left of the reaction
    void reactionStrLeft(OStringStream& reaction) const;

    //- Return string representation of the right of the reaction
    void reactionStrRight(OStringStream& reaction) const;


public:

    // Static data

    //- Number of un-named reactions
    static label nUnNamedReactions;

    //- Default temperature limits of applicability of reaction rates
    static scalar TlowDefault, ThighDefault;

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

    //- Name of reaction
    const word name_;

    //- List of specie names present in reaction system
    const speciesTable& species_;

    //- Temperature limits of applicability of reaction rates
    scalar Tlow_, Thigh_;

    //- Specie info for the left-hand-side of the reaction
    List<specieCoeffs> lhs_;

    //- Specie info for the right-hand-side of the reaction
    List<specieCoeffs> rhs_;


    //- Return string representation of reaction
    string reactionStr(OStringStream& reaction) const;

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
        dictionary,
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const dictionary& dict
        ),
        (species, thermoDatabase, dict)
    );


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

    //- Construct from dictionary
    Reaction
    (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        const dictionary& dict
    );

    //- Construct and return a clone
    virtual autoPtr<Reaction<ReactionThermo> > clone() const = 0;

    //- Construct and return a clone with new speciesTable
    virtual autoPtr<Reaction<ReactionThermo> > clone
    (
        const speciesTable& species
    ) const = 0;

    //- Construct the left- and right-hand-side reaction coefficients
    void setLRhs
    (
        Istream&,
        const speciesTable&,
        List<specieCoeffs>& lhs,
        List<specieCoeffs>& rhs
    );


    // Selectors

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

    //- Return the name of the reaction
    inline const word& name() const
    {
        return name_;
    }

    //- Return the lower temperature limit for the reaction
    inline scalar Tlow() const
    {
        return Tlow_;
    }

    //- Return the upper temperature limit for the reaction
    inline scalar Thigh() const
    {
        return Thigh_;
    }

    //- Return the components of the left hand side
    inline const List<specieCoeffs>& lhs() const
    {
        return lhs_;
    }

    //- Return the components of the right hand side
    inline const List<specieCoeffs>& rhs() const
    {
        return rhs_;
    }

    //- Return the specie list
    const speciesTable& species() const;


    // Reaction rate coefficients

    //- Forward reaction rate
    void ddot
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& d
    ) const;

    //- Backward reaction rate
    void fdot
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& f
    ) const;

    //- Net reaction rate for individual species
    void omega
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcdt
    ) const;

    //- Net reaction rate
    scalar omega
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalar& pf,
        scalar& cf,
        label& lRef,
        scalar& pr,
        scalar& cr,
        label& rRef
    ) const;

    // Reaction rate coefficients

    //- Forward rate constant
    virtual scalar kf
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const = 0;

    //- Reverse rate constant from the given forward rate constant
    virtual scalar kr
    (
        const scalar kfwd,
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const = 0;

    //- Reverse rate constant
    virtual scalar kr
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const = 0;


    // Jacobian coefficients

    //- Derivative of the net reaction rate for each species involved
    //  w.r.t. the species concentration
    void dwdc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarSquareMatrix& J,
        scalarField& dcdt,
        scalar& omegaI,
        scalar& kfwd,
        scalar& kbwd,
        const bool reduced,
        const List<label>& c2s
    ) const;

    //- Derivative of the net reaction rate for each species involved
    //  w.r.t. the temperature
    void dwdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        const scalar omegaI,
        const scalar kfwd,
        const scalar kbwd,
        scalarSquareMatrix& J,
        const bool reduced,
        const List<label>& c2s,
        const label indexT
    ) const;

    //- Temperature derivative of forward rate
    virtual scalar dkfdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const = 0;

    //- Temperature derivative of reverse rate
    virtual scalar dkrdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        const scalar dkfdT,
        const scalar kr
    ) const = 0;

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    virtual const List<Tuple2<label, scalar> >& beta() const = 0;

    //- Species concentration derivative of the pressure dependent term
    virtual void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const = 0;

    //- Temperature derivative of the pressure dependent term
    virtual scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const = 0;


    //- Write
    virtual void write(Ostream&) const;


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const Reaction<ReactionThermo>& r
    )
    {
        r.write(os);
        return os;
    }

};


} // End namespace CML


#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * * Static Data * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::label CML::Reaction<ReactionThermo>::nUnNamedReactions(0);

template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::TlowDefault(0);

template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::ThighDefault(GREAT);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::reactionStrLeft
(
    OStringStream& reaction
) const
{
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
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::reactionStrRight
(
    OStringStream& reaction
) const
{
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
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ReactionThermo>
CML::label CML::Reaction<ReactionThermo>::getNewReactionID()
{
    return nUnNamedReactions++;
}


template<class ReactionThermo>
CML::string CML::Reaction<ReactionThermo>::reactionStr
(
    OStringStream& reaction
) const
{
    reactionStrLeft(reaction);
    reaction << " = ";
    reactionStrRight(reaction);
    return reaction.str();
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::setThermo
(
    const HashPtrTable<ReactionThermo>& thermoDatabase
)
{
    typename ReactionThermo::thermoType rhsThermo
    (
        rhs_[0].stoichCoeff
       *(*thermoDatabase[species_[rhs_[0].index]]).W()
       *(*thermoDatabase[species_[rhs_[0].index]])
    );

    for (label i=1; i<rhs_.size(); ++i)
    {
        rhsThermo +=
            rhs_[i].stoichCoeff
           *(*thermoDatabase[species_[rhs_[i].index]]).W()
           *(*thermoDatabase[species_[rhs_[i].index]]);
    }

    typename ReactionThermo::thermoType lhsThermo
    (
        lhs_[0].stoichCoeff
       *(*thermoDatabase[species_[lhs_[0].index]]).W()
       *(*thermoDatabase[species_[lhs_[0].index]])
    );

    for (label i=1; i<lhs_.size(); ++i)
    {
        lhsThermo +=
            lhs_[i].stoichCoeff
           *(*thermoDatabase[species_[lhs_[i].index]]).W()
           *(*thermoDatabase[species_[lhs_[i].index]]);
    }

    ReactionThermo::thermoType::operator=(lhsThermo == rhsThermo);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const speciesTable& species,
    const List<specieCoeffs>& lhs,
    const List<specieCoeffs>& rhs,
    const HashPtrTable<ReactionThermo>& thermoDatabase
)
:
    ReactionThermo::thermoType(*thermoDatabase[species[0]]),
    name_("un-named-reaction-" + CML::name(getNewReactionID())),
    species_(species),
    Tlow_(TlowDefault),
    Thigh_(ThighDefault),
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
    ReactionThermo::thermoType(r),
    name_(r.name() + "Copy"),
    species_(species),
    Tlow_(r.Tlow()),
    Thigh_(r.Thigh()),
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
        stoichCoeff = 1;
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

        if (species.contains(specieName))
        {
            index = species[specieName];
        }
        else
        {
            index = -1;
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "Expected a word but found " << t.info()
            << exit(FatalIOError);
    }
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::setLRhs
(
    Istream& is,
    const speciesTable& species,
    List<specieCoeffs>& lhs,
    List<specieCoeffs>& rhs
)
{
    DynamicList<specieCoeffs> dlrhs;

    while (is.good())
    {
        dlrhs.append(specieCoeffs(species, is));

        if (dlrhs.last().index != -1)
        {
            token t(is);
            if (t.isPunctuation())
            {
                if (t == token::ADD)
                {
                }
                else if (t == token::ASSIGN)
                {
                    lhs = dlrhs.shrink();
                    dlrhs.clear();
                }
                else
                {
                    rhs = dlrhs.shrink();
                    is.putBack(t);
                    return;
                }
            }
            else
            {
                rhs = dlrhs.shrink();
                is.putBack(t);
                return;
            }
        }
        else
        {
            dlrhs.remove();
            if (is.good())
            {
                token t(is);
                if (t.isPunctuation())
                {
                    if (t == token::ADD)
                    {
                    }
                    else if (t == token::ASSIGN)
                    {
                        lhs = dlrhs.shrink();
                        dlrhs.clear();
                    }
                    else
                    {
                        rhs = dlrhs.shrink();
                        is.putBack(t);
                        return;
                    }
                }
            }
            else
            {
                if (!dlrhs.empty())
                {
                    rhs = dlrhs.shrink();
                }
                return;
            }
        }
    }

    FatalIOErrorInFunction(is)
        << "Cannot continue reading reaction data from stream"
        << exit(FatalIOError);
}


template<class ReactionThermo>
CML::Reaction<ReactionThermo>::Reaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    ReactionThermo::thermoType(*thermoDatabase[species[0]]),
    name_(dict.dictName()),
    species_(species),
    Tlow_(dict.lookupOrDefault<scalar>("Tlow", TlowDefault)),
    Thigh_(dict.lookupOrDefault<scalar>("Thigh", ThighDefault))
{
    setLRhs
    (
        IStringStream(dict.lookup("reaction"))(),
        species_,
        lhs_,
        rhs_
    );
    setThermo(thermoDatabase);
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

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
        FatalErrorInFunction
            << "Unknown reaction type "
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
    OStringStream reaction;
    os.writeKeyword("reaction") << reactionStr(reaction)
        << token::END_STATEMENT << nl;
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::ddot
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarField& d
) const
{
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::fdot
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarField& f
) const
{
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::omega
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarField& dcdt
) const
{
    scalar pf, cf, pr, cr;
    label lRef, rRef;

    scalar omegaI = omega
    (
        p, T, c, pf, cf, lRef, pr, cr, rRef
    );

    forAll(lhs_, i)
    {
        const label si = lhs_[i].index;
        const scalar sl = lhs_[i].stoichCoeff;
        dcdt[si] -= sl*omegaI;
    }
    forAll(rhs_, i)
    {
        const label si = rhs_[i].index;
        const scalar sr = rhs_[i].stoichCoeff;
        dcdt[si] += sr*omegaI;
    }
}


template<class ReactionThermo>
CML::scalar CML::Reaction<ReactionThermo>::omega
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalar& pf,
    scalar& cf,
    label& lRef,
    scalar& pr,
    scalar& cr,
    label& rRef
) const
{

    scalar clippedT = min(max(T, this->Tlow()), this->Thigh());

    const scalar kf = this->kf(p, clippedT, c);
    const scalar kr = this->kr(kf, p, clippedT, c);

    pf = 1;
    pr = 1;

    const label Nl = lhs_.size();
    const label Nr = rhs_.size();

    label slRef = 0;
    lRef = lhs_[slRef].index;

    pf = kf;
    for (label s = 1; s < Nl; s++)
    {
        const label si = lhs_[s].index;

        if (c[si] < c[lRef])
        {
            const scalar exp = lhs_[slRef].exponent;
            pf *= pow(max(c[lRef], 0), exp);
            lRef = si;
            slRef = s;
        }
        else
        {
            const scalar exp = lhs_[s].exponent;
            pf *= pow(max(c[si], 0), exp);
        }
    }
    cf = max(c[lRef], 0);

    {
        const scalar exp = lhs_[slRef].exponent;
        if (exp < 1)
        {
            if (cf > SMALL)
            {
                pf *= pow(cf, exp - 1);
            }
            else
            {
                pf = 0;
            }
        }
        else
        {
            pf *= pow(cf, exp - 1);
        }
    }

    label srRef = 0;
    rRef = rhs_[srRef].index;

    // Find the matrix element and element position for the rhs
    pr = kr;
    for (label s = 1; s < Nr; s++)
    {
        const label si = rhs_[s].index;
        if (c[si] < c[rRef])
        {
            const scalar exp = rhs_[srRef].exponent;
            pr *= pow(max(c[rRef], 0), exp);
            rRef = si;
            srRef = s;
        }
        else
        {
            const scalar exp = rhs_[s].exponent;
            pr *= pow(max(c[si], 0), exp);
        }
    }
    cr = max(c[rRef], 0);

    {
        const scalar exp = rhs_[srRef].exponent;
        if (exp < 1)
        {
            if (cr > SMALL)
            {
                pr *= pow(cr, exp - 1);
            }
            else
            {
                pr = 0;
            }
        }
        else
        {
            pr *= pow(cr, exp - 1);
        }
    }

    return pf*cf - pr*cr;
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::dwdc
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarSquareMatrix& J,
    scalarField& dcdt,
    scalar& omegaI,
    scalar& kfwd,
    scalar& kbwd,
    const bool reduced,
    const List<label>& c2s
) const
{
    scalar pf, cf, pr, cr;
    label lRef, rRef;

    omegaI = omega(p, T, c, pf, cf, lRef, pr, cr, rRef);

    forAll(lhs_, i)
    {
        const label si = reduced ? c2s[lhs_[i].index] : lhs_[i].index;
        const scalar sl = lhs_[i].stoichCoeff;
        dcdt[si] -= sl*omegaI;
    }
    forAll(rhs_, i)
    {
        const label si = reduced ? c2s[rhs_[i].index] : rhs_[i].index;
        const scalar sr = rhs_[i].stoichCoeff;
        dcdt[si] += sr*omegaI;
    }

    kfwd = this->kf(p, T, c);
    kbwd = this->kr(kfwd, p, T, c);

    forAll(lhs_, j)
    {
        const label sj = reduced ? c2s[lhs_[j].index] : lhs_[j].index;
        scalar kf = kfwd;
        forAll(lhs_, i)
        {
            const label si = lhs_[i].index;
            const scalar el = lhs_[i].exponent;
            if (i == j)
            {
                if (el < 1)
                {
                    if (c[si] > SMALL)
                    {
                        kf *= el*pow(c[si] + VSMALL, el - 1);
                    }
                    else
                    {
                        kf = 0;
                    }
                }
                else
                {
                    kf *= el*pow(c[si], el - 1);
                }
            }
            else
            {
                kf *= pow(c[si], el);
            }
        }

        forAll(lhs_, i)
        {
            const label si = reduced ? c2s[lhs_[i].index] : lhs_[i].index;
            const scalar sl = lhs_[i].stoichCoeff;
            J(si, sj) -= sl*kf;
        }
        forAll(rhs_, i)
        {
            const label si = reduced ? c2s[rhs_[i].index] : rhs_[i].index;
            const scalar sr = rhs_[i].stoichCoeff;
            J(si, sj) += sr*kf;
        }
    }

    forAll(rhs_, j)
    {
        const label sj = reduced ? c2s[rhs_[j].index] : rhs_[j].index;
        scalar kr = kbwd;
        forAll(rhs_, i)
        {
            const label si = rhs_[i].index;
            const scalar er = rhs_[i].exponent;
            if (i == j)
            {
                if (er < 1)
                {
                    if (c[si] > SMALL)
                    {
                        kr *= er*pow(c[si] + VSMALL, er - 1);
                    }
                    else
                    {
                        kr = 0;
                    }
                }
                else
                {
                    kr *= er*pow(c[si], er - 1);
                }
            }
            else
            {
                kr *= pow(c[si], er);
            }
        }

        forAll(lhs_, i)
        {
            const label si = reduced ? c2s[lhs_[i].index] : lhs_[i].index;
            const scalar sl = lhs_[i].stoichCoeff;
            J(si, sj) += sl*kr;
        }
        forAll(rhs_, i)
        {
            const label si = reduced ? c2s[rhs_[i].index] : rhs_[i].index;
            const scalar sr = rhs_[i].stoichCoeff;
            J(si, sj) -= sr*kr;
        }
    }

    // When third-body species are involved, additional terms are added
    // beta function returns an empty list when third-body are not involved
    const List<Tuple2<label, scalar> >& beta = this->beta();
    if (notNull(beta))
    {
        // This temporary array needs to be cached for efficiency
        scalarField dcidc(beta.size());
        this->dcidc(p, T, c, dcidc);

        forAll(beta, j)
        {
            label sj = beta[j].first();
            sj = reduced ? c2s[sj] : sj;
            if (sj != -1)
            {
                forAll(lhs_, i)
                {
                    const label si =
                        reduced ? c2s[lhs_[i].index] : lhs_[i].index;
                    const scalar sl = lhs_[i].stoichCoeff;
                    J(si, sj) -= sl*dcidc[j]*omegaI;
                }
                forAll(rhs_, i)
                {
                    const label si =
                        reduced ? c2s[rhs_[i].index] : rhs_[i].index;
                    const scalar sr = rhs_[i].stoichCoeff;
                    J(si, sj) += sr*dcidc[j]*omegaI;
                }
            }
        }
    }
}


template<class ReactionThermo>
void CML::Reaction<ReactionThermo>::dwdT
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    const scalar omegaI,
    const scalar kfwd,
    const scalar kbwd,
    scalarSquareMatrix& J,
    const bool reduced,
    const List<label>& c2s,
    const label indexT
) const
{
    scalar kf = kfwd;
    scalar kr = kbwd;

    scalar dkfdT = this->dkfdT(p, T, c);
    scalar dkrdT = this->dkrdT(p, T, c, dkfdT, kr);

    scalar sumExp = 0.0;
    forAll(lhs_, i)
    {
        const label si = lhs_[i].index;
        const scalar el = lhs_[i].exponent;
        const scalar cExp = pow(c[si], el);
        dkfdT *= cExp;
        kf *= cExp;
        sumExp += el;
    }
    kf *= -sumExp/T;

    sumExp = 0.0;
    forAll(rhs_, i)
    {
        const label si = rhs_[i].index;
        const scalar er = rhs_[i].exponent;
        const scalar cExp = pow(c[si], er);
        dkrdT *= cExp;
        kr *= cExp;
        sumExp += er;
    }
    kr *= -sumExp/T;

    // dqidT includes the third-body (or pressure dependent) effect
    scalar dqidT = dkfdT - dkrdT + kf - kr;

    // For reactions including third-body efficiencies or pressure dependent
    // reaction, an additional term is needed
    scalar dcidT = this->dcidT(p, T, c);
    dcidT *= omegaI;

    // J(i, indexT) = sum_reactions nu_i dqdT
    forAll(lhs_, i)
    {
        const label si = reduced ? c2s[lhs_[i].index] : lhs_[i].index;
        const scalar sl = lhs_[i].stoichCoeff;
        J(si, indexT) -= sl*(dqidT + dcidT);
    }
    forAll(rhs_, i)
    {
        const label si = reduced ? c2s[rhs_[i].index] : rhs_[i].index;
        const scalar sr = rhs_[i].stoichCoeff;
        J(si, indexT) += sr*(dqidT + dcidT);
    }
}


template<class ReactionThermo>
const CML::speciesTable& CML::Reaction<ReactionThermo>::species() const
{
    return species_;
}

#endif
