/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::chemistryReductionMethods::DRGEP

Description
    The DRGEP algorithm [1] is based on

            |sum_i=1->Nr vAi wi dBi|
    rAB = --------------------------- ,
                max(PA, CA)

    PA = sum_i=1->Nr (max (0, vAi wi))  -> production of species A

    CA = sum_i=1->Nr (max (0, -vAi wi)) -> consumption of species A

    where i is the reaction index, Nr the number of reactions, vAi is the net
    stoechiometric coefficient of species A in the ith reaction (vAi = v''-v')
    , wi is the progress variable of reaction i and dBi equals 1 if reaction i
    involves B and O otherwise.
    rAB show the error introduced to the production rates of A when B and all
    the reactions including it are removed. It is computed as in [2] so that
    the algorithm is O(Nr).

    DAC uses a initial set of species that represents the major parts of the
    combustion mechanism, i.e. H2/O2, fuel decomposition and CO2 production.
    Usually, it includes the fuel, HO2 and CO. Then it computes the dependence
    of these set to the other species. This is done by introducing R-value
    defined by

    R_V0 (V) = max_SP(product(rij)) ,

    where SP is the set of all possible paths leading from V0 to V and
    product(rij) is the chain product of the weights of the edges along the
    given path. The R-value for the initial set species is 1.

    When the R-value of a species is larger than a user-defined tolerance
    then the species is included in the simplified mechanism. Otherwise,
    the species is removed along with all the reactions including it.

    During this process, instead of looking over all species like described
    in [1], the algorithm implemented here creates dynamic list to retain
    the initialized edges only (see [2]).

    To avoid using the target species when they are not contributing yet or
    anymore to the system, a coefficient based on the exchange of element is
    introduced:

                NTa |PT - CT|
    alphaTa = ----------------
                    Pa

    Pa = sum_speciesS NSa max(0, PS-CS)

    where 'a' refers to different elements present in the system
    (namely C, H, O and N for conventionail hydrocarbon combustion),
    NTa is the number of element a in species T.
    When this coefficient alpha is below the specified threshold, the species is
    removed from the search initiating set. In the original paper from
    Pepiot-Desjardins et al.[2], this coefficient is further transformed to
    compute a global normalized scaling coefficient but here as it is
    dynamically computed, alpha is not introduced in the calculation of R.

    References:
    \verbatim
        [1] Pepiot-Desjardins, P., & Pitsch, H. (2008).
        An efficient error-propagation-based reduction method for large
        chemical kinetic mechanisms.
        Combustion and Flame, 154(1), 67-81.

        [2] Lu, T., & Law, C. K. (2006).
        Linear time reduction of large kinetic mechanisms with directed
        relation graph: n-Heptane and iso-octane.
        Combustion and Flame, 144(1), 24-36.
    \endverbatim

SourceFiles
    DRGEP.C

\*---------------------------------------------------------------------------*/

#ifndef DRGEP_HPP
#define DRGEP_HPP

#include "chemistryReductionMethod.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace chemistryReductionMethods
{

/*---------------------------------------------------------------------------*\
                           Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class DRGEP
:
    public chemistryReductionMethod<CompType, ThermoType>
{

    //- List of label for the search initiating set
    labelList searchInitSet_;

    List<label> sC_,sH_,sO_,sN_;
    label NGroupBased_;

public:

    //- Runtime type information
    TypeName("DRGEP");


    //- Construct from components
    DRGEP
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    //- Destructor
    virtual ~DRGEP()
    {}


    // Member Functions

    //- Reduce the mechanism
    virtual void reduceMechanism
    (
        const scalarField &c,
        const scalar T,
        const scalar p
    );
};


} // End namespace chemistryReductionMethods
} // End namespace CML


#include "SortableListDRGEP.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryReductionMethods::DRGEP<CompType, ThermoType>::DRGEP
(
    const IOdictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    chemistryReductionMethod<CompType, ThermoType>(dict, chemistry),
    searchInitSet_(this->coeffsDict_.subDict("initialSet").size()),
    sC_(this->nSpecie_,0),
    sH_(this->nSpecie_,0),
    sO_(this->nSpecie_,0),
    sN_(this->nSpecie_,0),
    NGroupBased_(50)
{
    label j=0;
    dictionary initSet = this->coeffsDict_.subDict("initialSet");
    for (label i=0; i<chemistry.nSpecie(); i++)
    {
        if (initSet.found(chemistry.Y()[i].member()))
        {
            searchInitSet_[j++]=i;
        }
    }
    if (j<searchInitSet_.size())
    {
        FatalErrorInFunction
            << searchInitSet_.size()-j
            << " species in the initial set is not in the mechanism "
            << initSet
            << exit(FatalError);
    }

    if (this->coeffsDict_.found("NGroupBased"))
    {
        NGroupBased_ = readLabel(this->coeffsDict_.lookup("NGroupBased"));
    }

    const List<List<specieElement> >& specieComposition =
        this->chemistry_.specieComp();
    for (label i=0; i<this->nSpecie_; i++)
    {
        const List<specieElement>& curSpecieComposition =
            specieComposition[i];
        // for all elements in the current species
        forAll(curSpecieComposition, j)
        {
            const specieElement& curElement =
                curSpecieComposition[j];
            if (curElement.name() == "C")
            {
                sC_[i] = curElement.nAtoms();
            }
            else if (curElement.name() == "H")
            {
                sH_[i] = curElement.nAtoms();
            }
            else if (curElement.name() == "O")
            {
                sO_[i] = curElement.nAtoms();
            }
            else if (curElement.name() == "N")
            {
                sN_[i] = curElement.nAtoms();
            }
            else
            {
                Info<< "element not considered"<< endl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
void CML::chemistryReductionMethods::DRGEP<CompType, ThermoType>::
reduceMechanism
(
    const scalarField &c,
    const scalar T,
    const scalar p
)
{
    scalarField& completeC(this->chemistry_.completeC());
    scalarField c1(this->chemistry_.nEqns(), 0.0);

    for (label i=0; i<this->nSpecie_; i++)
    {
        c1[i] = c[i];
        completeC[i] = c[i];
    }

    c1[this->nSpecie_] = T;
    c1[this->nSpecie_+1] = p;

    // Compute the rAB matrix
    RectangularMatrix<scalar> rABNum(this->nSpecie_,this->nSpecie_,0.0);
    scalarField PA(this->nSpecie_,0.0);
    scalarField CA(this->nSpecie_,0.0);

    // Number of initialized rAB for each lines
    Field<label> NbrABInit(this->nSpecie_,0);
    // Position of the initialized rAB, -1 when not initialized
    RectangularMatrix<label> rABPos(this->nSpecie_, this->nSpecie_, -1);
    // Index of the other species involved in the rABNum
    RectangularMatrix<label> rABOtherSpec(this->nSpecie_, this->nSpecie_, -1);

    scalar pf, cf, pr, cr;
    label lRef, rRef;
    scalarField omegaV(this->chemistry_.reactions().size());
    forAll(this->chemistry_.reactions(), i)
    {
        const Reaction<ThermoType>& R = this->chemistry_.reactions()[i];
        // for each reaction compute omegai
        scalar omegai = this->chemistry_.omega
        (
         R, c1, T, p, pf, cf, lRef, pr, cr, rRef
         );
        omegaV[i] = omegai;

        // then for each pair of species composing this reaction,
        // compute the rAB matrix (separate the numerator and
        // denominator)

        DynamicList<scalar> wA(R.lhs().size()+R.rhs().size());
        DynamicList<label> wAID(R.lhs().size()+R.rhs().size());
        forAll(R.lhs(), s)// compute rAB for all species in the left hand side
        {
            label ss = R.lhs()[s].index;
            scalar sl = -R.lhs()[s].stoichCoeff; // vAi = v''-v' => here -v'
            List<bool> deltaBi(this->nSpecie_, false);
            FIFOStack<label> usedIndex;
            forAll(R.lhs(), j)
            {
                label sj = R.lhs()[j].index;
                usedIndex.push(sj);
                deltaBi[sj] = true;
            }
            forAll(R.rhs(), j)
            {
                label sj = R.rhs()[j].index;
                usedIndex.push(sj);
                deltaBi[sj] = true;
            }

            // Disable for self reference (by definition rAA=0)
            deltaBi[ss] = false;

            while(!usedIndex.empty())
            {
                label curIndex = usedIndex.pop();
                if (deltaBi[curIndex])
                {
                    // disable to avoid counting it more than once
                    deltaBi[curIndex] = false;
                    // test if this rAB is not initialized
                    if (rABPos(ss, curIndex)==-1)
                    {
                        rABPos(ss, curIndex) = NbrABInit[ss];
                        NbrABInit[ss]++;
                        rABNum(ss, rABPos(ss, curIndex)) = sl*omegai;
                        rABOtherSpec(ss, rABPos(ss, curIndex)) = curIndex;
                    }
                    else
                    {
                        rABNum(ss, rABPos(ss, curIndex)) += sl*omegai;
                    }
                }
            }
            bool found(false);
            forAll(wAID, id)
            {
                if (ss==wAID[id])
                {
                    wA[id] += sl*omegai;
                    found = true;
                }
            }
            if (!found)
            {
                wA.append(sl*omegai);
                wAID.append(ss);
            }
        }

        // Compute rAB for all species in the right hand side
        forAll(R.rhs(), s)
        {
            label ss = R.rhs()[s].index;
            scalar sl = R.rhs()[s].stoichCoeff; // vAi = v''-v' => here v''
            List<bool> deltaBi(this->nSpecie_, false);
            FIFOStack<label> usedIndex;
            forAll(R.lhs(), j)
            {
                label sj = R.lhs()[j].index;
                usedIndex.push(sj);
                deltaBi[sj] = true;
            }
            forAll(R.rhs(), j)
            {
                label sj = R.rhs()[j].index;
                usedIndex.push(sj);
                deltaBi[sj] = true;
            }

            // Disable for self reference (by definition rAA=0)
            deltaBi[ss] = false;

            while(!usedIndex.empty())
            {
                label curIndex = usedIndex.pop();
                if (deltaBi[curIndex])
                {
                    // disable to avoid counting it more than once
                    deltaBi[curIndex] = false;
                    // test if this rAB is not initialized
                    if (rABPos(ss, curIndex)==-1)
                    {
                        rABPos(ss, curIndex) = NbrABInit[ss];
                        NbrABInit[ss]++;
                        rABNum(ss, rABPos(ss, curIndex)) = sl*omegai;
                        rABOtherSpec(ss, rABPos(ss, curIndex)) = curIndex;
                    }
                    else
                    {
                        rABNum(ss, rABPos(ss, curIndex)) += sl*omegai;
                    }
                }
            }

            bool found(false);
            forAll(wAID, id)
            {
                if (ss==wAID[id])
                {
                    wA[id] += sl*omegai;
                    found = true;
                }
            }
            if (!found)
            {
                wA.append(sl*omegai);
                wAID.append(ss);
            }
        }

        wAID.shrink();
        // Now that every species of the reactions has been visited, we can
        // compute the production and consumption rate. This way, it avoids
        // getting wrong results when species are present in both lhs and rhs
        forAll(wAID, id)
        {
            if (wA[id] > 0.0)
            {
                if (PA[wAID[id]] == 0.0)
                {
                    PA[wAID[id]] = wA[id];
                }
                else
                {
                    PA[wAID[id]] += wA[id];
                }
            }
            else
            {
                if (CA[wAID[id]] == 0.0)
                {
                    CA[wAID[id]] = -wA[id];
                }
                else
                {
                    CA[wAID[id]] += -wA[id];
                }
            }
        }
    }
    // rii = 0.0 by definition

    // Compute the production rate of each element Pa
    label nElements = 4; // 4 main elements (C, H, O, N)
    scalarList Pa(nElements,0.0);
    scalarList Ca(nElements,0.0);

    // for (label q=0; q<SIS.size(); q++)
    for (label i=0; i<this->nSpecie_; i++)
    {
        Pa[0] += sC_[i]*max(0.0,(PA[i]-CA[i]));
        Ca[0] += sC_[i]*max(0.0,-(PA[i]-CA[i]));
        Pa[1] += sH_[i]*max(0.0,(PA[i]-CA[i]));
        Ca[1] += sH_[i]*max(0.0,-(PA[i]-CA[i]));
        Pa[2] += sO_[i]*max(0.0,(PA[i]-CA[i]));
        Ca[2] += sO_[i]*max(0.0,-(PA[i]-CA[i]));
        Pa[3] += sN_[i]*max(0.0,(PA[i]-CA[i]));
        Ca[3] += sN_[i]*max(0.0,-(PA[i]-CA[i]));
    }

    // Using the rAB matrix (numerator and denominator separated)
    // compute the R value according to the search initiating set
    scalarField Rvalue(this->nSpecie_,0.0);
    label speciesNumber = 0;
    List<bool> disabledSpecies(this->nSpecie_,false);

    // set all species to inactive and activate them according
    // to rAB and initial set
    for (label i=0; i<this->nSpecie_; i++)
    {
        this->activeSpecies_[i] = false;
    }
    // Initialize the FIFOStack for search set
    FIFOStack<label> Q;
    const labelList& SIS(this->searchInitSet_);
    DynamicList<label> QStart(SIS.size());
    DynamicList<scalar> alphaQ(SIS.size());

    // Compute the alpha coefficient and initialize the R value of the species
    // in the SIS
    for (label i=0; i<SIS.size(); i++)
    {
        label q = SIS[i];
        // compute alpha
        scalar alphaA(0.0);
        // C
        if (Pa[0] > VSMALL)
        {
            scalar alphaTmp = (sC_[q]*mag(PA[q]-CA[q])/Pa[0]);
            if (alphaTmp > alphaA)
            {
                alphaA = alphaTmp;
            }
        }
        // H
        if (Pa[1] > VSMALL)
        {
            scalar alphaTmp = (sH_[q]*mag(PA[q]-CA[q])/Pa[1]);
            if (alphaTmp > alphaA)
            {
                alphaA = alphaTmp;
            }
        }
        // O
        if (Pa[2] > VSMALL)
        {
            scalar alphaTmp = (sO_[q]*mag(PA[q]-CA[q])/Pa[2]);
            if (alphaTmp > alphaA)
            {
                alphaA = alphaTmp;
            }
        }
        // N
        if (Pa[3] > VSMALL)
        {
            scalar alphaTmp = (sN_[q]*mag(PA[q]-CA[q])/Pa[3]);
            if (alphaTmp > alphaA)
            {
                alphaA = alphaTmp;
            }
        }
        if (alphaA > this->tolerance())
        {
            this->activeSpecies_[q] = true;
            speciesNumber++;
            Q.push(q);
            QStart.append(q);
            alphaQ.append(1.0);
            Rvalue[q] = 1.0;
        }
        else
        {
            Rvalue[q] = alphaA;
        }
    }

    // if all species from the SIS has been removed
    // force the use of the species with maximum Rvalue
    if (Q.empty())
    {
        scalar Rmax=0.0;
        label specID=-1;
        forAll(SIS, i)
        {
            if (Rvalue[SIS[i]] > Rmax)
            {
                Rmax = Rvalue[SIS[i]];
                specID=SIS[i];
            }
        }
        Q.push(specID);
        QStart.append(specID);
        alphaQ.append(1.0);
        speciesNumber++;
        Rvalue[specID] = 1.0;
        this->activeSpecies_[specID] = true;
    }

    // Execute the main loop for R-value
    while (!Q.empty())
    {
        label u = Q.pop();
        scalar Den = max(PA[u],CA[u]);
        if (Den > VSMALL)
        {
            for (label v=0; v<NbrABInit[u]; v++)
            {
                label otherSpec = rABOtherSpec(u, v);
                scalar rAB = mag(rABNum(u, v))/Den;
                if (rAB > 1)
                {
                    rAB = 1;
                }

                scalar Rtemp = Rvalue[u]*rAB;
                // a link analysed previously is stronger
                if (Rvalue[otherSpec] < Rtemp)
                {
                    Rvalue[otherSpec] = Rtemp;
                    // the (composed) link is stronger than the tolerance
                    if (Rtemp >= this->tolerance())
                    {
                        Q.push(otherSpec);
                        if (!this->activeSpecies_[otherSpec])
                        {
                            this->activeSpecies_[otherSpec] = true;
                            speciesNumber++;
                        }
                    }
                }
            }
        }
    }

    // Group-based reduction
    // number of species disabled in the first step
    label NDisabledSpecies(this->nSpecie_-speciesNumber);

    // while the number of removed species is greater than NGroupBased, the rAB
    // are reevaluated according to the group based definition for each loop the
    // temporary disabled species (in the first reduction) are sorted to disable
    // definitely the NGroupBased species with lower R then these R value a
    // reevaluated taking into account these disabled species
    while(NDisabledSpecies > NGroupBased_)
    {
        // keep track of disabled species using sortablelist to extract only
        // NGroupBased lower R value
        SortableListDRGEP<scalar> Rdisabled(NDisabledSpecies);
        labelList Rindex(NDisabledSpecies);
        label nD = 0;
        forAll(disabledSpecies, i)
        {
            // if just disabled and not in a previous loop
            if (!this->activeSpecies_[i] && !disabledSpecies[i])
            {
                // Note: non-reached species will be removed first (Rvalue=0)
                Rdisabled[nD] = Rvalue[i];
                Rindex[nD++] = i;
            }
        }
        // sort the Rvalue to obtain the NGroupBased lower R value
        Rdisabled.partialSort(NGroupBased_);
        labelList tmpIndex(Rdisabled.indices());

        // disable definitely NGroupBased species in this loop
        for (label i=0; i<NGroupBased_; i++)
        {
            disabledSpecies[Rindex[tmpIndex[i]]] = true;
        }
        NDisabledSpecies -= NGroupBased_;

        // reevaluate the rAB according to the group-based definition rAB{S} [1]
        // only update the numerator
        forAll(NbrABInit, i)
        {
            for (label v=0; v<NbrABInit[i]; v++)
            {
                rABNum(i, v) = 0.0;
            }
        }
        forAll(this->chemistry_.reactions(), i)
        {
            const Reaction<ThermoType>& R = this->chemistry_.reactions()[i];

            scalar omegai = omegaV[i];

            forAll(R.lhs(), s)
            {
                label ss = R.lhs()[s].index;
                scalar sl = -R.lhs()[s].stoichCoeff; // vAi = v''-v' => here -v'
                List<bool> deltaBi(this->nSpecie_, false);
                bool alreadyDisabled(false);
                FIFOStack<label> usedIndex;
                forAll(R.lhs(), j)
                {
                    label sj = R.lhs()[j].index;
                    usedIndex.push(sj);
                    deltaBi[sj] = true;
                    if (disabledSpecies[sj])
                    {
                        alreadyDisabled=true;
                    }
                }
                forAll(R.rhs(), j)
                {
                    label sj = R.rhs()[j].index;
                    usedIndex.push(sj);
                    deltaBi[sj] = true;
                    if (disabledSpecies[sj])
                    {
                        alreadyDisabled=true;
                    }
                }

                deltaBi[ss] = false;

                if (alreadyDisabled)
                {
                    // if one of the species in this reaction is disabled, all
                    // species connected to species ss are modified
                    for (label v=0; v<NbrABInit[ss]; v++)
                    {
                        rABNum(ss, v) += sl*omegai;
                    }
                }
                else
                {
                    while(!usedIndex.empty())
                    {
                        label curIndex = usedIndex.pop();
                        if (deltaBi[curIndex])
                        {
                            // disable to avoid counting it more than once
                            deltaBi[curIndex] = false;
                            rABNum(ss, rABPos(ss, curIndex)) += sl*omegai;
                        }
                    }
                }
            }

            forAll(R.rhs(), s)
            {
                label ss = R.rhs()[s].index;
                scalar sl = R.rhs()[s].stoichCoeff; // vAi = v''-v' => here v''
                List<bool> deltaBi(this->nSpecie_, false);
                bool alreadyDisabled(false);
                FIFOStack<label> usedIndex;
                forAll(R.lhs(), j)
                {
                    label sj = R.lhs()[j].index;
                    usedIndex.push(sj);
                    deltaBi[sj] = true;
                    if (disabledSpecies[sj])
                    {
                        alreadyDisabled=true;
                    }
                }
                forAll(R.rhs(), j)
                {
                    label sj = R.rhs()[j].index;
                    usedIndex.push(sj);
                    deltaBi[sj] = true;
                    if (disabledSpecies[sj])
                    {
                        alreadyDisabled=true;
                    }
                }

                deltaBi[ss] = false;

                if (alreadyDisabled)
                {
                    // if one of the species in this reaction is disabled, all
                    // species connected to species ss are modified
                    for (label v=0; v<NbrABInit[ss]; v++)
                    {
                        rABNum(ss, v) += sl*omegai;
                    }
                }
                else
                {
                    while(!usedIndex.empty())
                    {
                        label curIndex = usedIndex.pop();
                        if (deltaBi[curIndex])
                        {
                            deltaBi[curIndex] = false;
                            rABNum(ss, rABPos(ss, curIndex)) += sl*omegai;
                        }
                    }
                }
            }
        }

        forAll(QStart, qi)
        {
            label u = QStart[qi];
            Q.push(u);
        }

        while (!Q.empty())
        {
            label u = Q.pop();
            scalar Den = max(PA[u],CA[u]);
            if (Den!=0.0)
            {
                for (label v=0; v<NbrABInit[u]; v++)
                {
                    label otherSpec = rABOtherSpec(u, v);
                    if (!disabledSpecies[otherSpec])
                    {
                        scalar rAB = mag(rABNum(u, v))/Den;
                        if (rAB > 1)
                        {
                            rAB = 1;
                        }

                        scalar Rtemp = Rvalue[u]*rAB;
                        // a link analysed previously is stronger
                        if (Rvalue[otherSpec] < Rtemp)
                        {
                            Rvalue[otherSpec] = Rtemp;
                            if (Rtemp >= this->tolerance())
                            {
                                Q.push(otherSpec);
                                if (!this->activeSpecies_[otherSpec])
                                {
                                    this->activeSpecies_[otherSpec] = true;
                                    speciesNumber++;
                                    NDisabledSpecies--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // End of group-based reduction

    // Put a flag on the reactions containing at least one removed species
    forAll(this->chemistry_.reactions(), i)
    {
        const Reaction<ThermoType>& R = this->chemistry_.reactions()[i];
        this->chemistry_.reactionsDisabled()[i] = false;
        forAll(R.lhs(), s)
        {
            label ss = R.lhs()[s].index;
            if (!this->activeSpecies_[ss])
            {
                this->chemistry_.reactionsDisabled()[i] = true;
                break;
            }
        }
        if (!this->chemistry_.reactionsDisabled()[i])
        {
            forAll(R.rhs(), s)
            {
                label ss = R.rhs()[s].index;
                if (!this->activeSpecies_[ss])
                {
                    this->chemistry_.reactionsDisabled()[i] = true;
                    break;
                }
            }
        }
    }

    this->NsSimp_ = speciesNumber;
    scalarField& simplifiedC(this->chemistry_.simplifiedC());
    simplifiedC.setSize(this->NsSimp_+2);
    DynamicList<label>& s2c(this->chemistry_.simplifiedToCompleteIndex());
    s2c.setSize(this->NsSimp_);
    Field<label>& c2s(this->chemistry_.completeToSimplifiedIndex());

    label j = 0;
    for (label i=0; i<this->nSpecie_; i++)
    {
        if (this->activeSpecies_[i])
        {
            s2c[j] = i;
            simplifiedC[j] = c[i];
            c2s[i] = j++;
            if (!this->chemistry_.active(i))
            {
                this->chemistry_.setActive(i);
            }
        }
        else
        {
            c2s[i] = -1;
        }
    }
    simplifiedC[this->NsSimp_] = T;
    simplifiedC[this->NsSimp_+1] = p;
    this->chemistry_.setNsDAC(this->NsSimp_);
    // change temporary Ns in chemistryModel
    // to make the function nEqns working
    this->chemistry_.setNSpecie(this->NsSimp_);
}


#endif
