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
    CML::chemistryReductionMethods::DAC

Description
    The Dynamic Adaptive Chemistry (DAC) method [1] simplify the chemistry
    using the matrix rAB defined by (DRGEP algorithm [2])

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
    the reactions including it are removed. It is computed as in [3] so that
    the algorithm is O(Nr).

    DAC uses a initial set of species that represents the major parts of the
    combustion mechanism, i.e. H2/O2, fuel decomposition and CO2 production.
    Usually, it includes the fuel, HO2 and CO. Then it computes the dependence
    of these set to the other species. This is done by introducing R-value
    defined by

    R_V0 (V) = max_SP(product(rij)),

    where SP is the set of all possible paths leading from V0 to V and
    product(rij) is the chain product of the weights of the edges along the
    given path. The R-value for the initial set species is 1.

    When the R-value of a species is larger than a user-defined tolerance
    then the species is included in the simplified mechanism. Otherwise,
    the species is removed along with all the reactions including it.

    During this process, instead of looking over all species like described
    in [1], the algorithm implemented here creates dynamic list to retain
    the initialized edges only (see [3]).

    References:
    \verbatim
        [1] Liang, L., Stevens, J. G., & Farrell, J. T. (2009).
        A dynamic adaptive chemistry scheme for reactive flow computations.
        Proceedings of the Combustion Institute, 32(1), 527-534.

        [2] Pepiot-Desjardins, P., & Pitsch, H. (2008).
        An efficient error-propagation-based reduction method for large
        chemical kinetic mechanisms.
        Combustion and Flame, 154(1), 67-81.

        [3] Lu, T., & Law, C. K. (2006).
        Linear time reduction of large kinetic mechanisms with directed
        relation graph: n-Heptane and iso-octane.
        Combustion and Flame, 144(1), 24-36.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef DAC_HPP
#define DAC_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace chemistryReductionMethods
{

/*---------------------------------------------------------------------------*\
                           Class DAC Declaration
\*---------------------------------------------------------------------------*/
template<class CompType, class ThermoType>
class DAC
:
    public chemistryReductionMethod<CompType, ThermoType>
{

    //- List of label for the search initiating set
    labelList searchInitSet_;

    scalar zprime_;
    label nbCLarge_;
    List<label> sC_,sH_,sO_,sN_;
    label CO2Id_,COId_,HO2Id_,H2OId_,NOId_;
    Switch automaticSIS_;
    scalar phiTol_;
    scalar NOxThreshold_;
    wordList fuelSpecies_;
    scalarField fuelSpeciesProp_;
    List<label> fuelSpeciesID_;
    word CO2Name_, COName_, HO2Name_, H2OName_, NOName_;
    Switch forceFuelInclusion_;

public:

    //- Runtime type information
    TypeName("DAC");


    //- Construct from components
    DAC
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    //- Destructor
    virtual ~DAC()
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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryReductionMethods::DAC<CompType, ThermoType>::DAC
(
    const IOdictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    chemistryReductionMethod<CompType, ThermoType>(dict, chemistry),
    searchInitSet_(this->coeffsDict_.subDict("initialSet").size()),
    zprime_(0),
    nbCLarge_(3),
    sC_(this->nSpecie_,0),
    sH_(this->nSpecie_,0),
    sO_(this->nSpecie_,0),
    sN_(this->nSpecie_,0),
    CO2Id_(-1),
    COId_(-1),
    HO2Id_(-1),
    H2OId_(-1),
    NOId_(-1),
    automaticSIS_(true),
    phiTol_(this->tolerance()),
    NOxThreshold_(1800),
    CO2Name_
    (
        dict.subDict("reduction").lookupOrDefault<word>
        (
            "CO2Name","CO2"
        )
    ),
    COName_
    (
        dict.subDict("reduction").lookupOrDefault<word>
        (
            "COName","CO"
        )
    ),
    HO2Name_
    (
        dict.subDict("reduction").lookupOrDefault<word>
        (
            "HO2Name","HO2"
        )
    ),
    H2OName_
    (
        dict.subDict("reduction").lookupOrDefault<word>
        (
            "H2OName","H2O"
        )
    ),
    NOName_
    (
        dict.subDict("reduction").lookupOrDefault<word>
        (
            "NOName","NO"
        )
    ),
    forceFuelInclusion_(false)
{
    label j=0;
    dictionary initSet = this->coeffsDict_.subDict("initialSet");
    for (label i=0; i<chemistry.nSpecie(); i++)
    {
        if (initSet.found(chemistry.Y()[i].member()))
        {
            searchInitSet_[j++] = i;
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

    if (this->coeffsDict_.found("automaticSIS"))
    {
        automaticSIS_.readIfPresent("automaticSIS", this->coeffsDict_);
    }

    if (this->coeffsDict_.found("forceFuelInclusion"))
    {
        forceFuelInclusion_.readIfPresent
        (
            "forceFuelInclusion",this->coeffsDict_
        );
    }

    if (this->coeffsDict_.found("phiTol"))
    {
        phiTol_ = readScalar(this->coeffsDict_.lookup("phiTol"));
    }

    if (this->coeffsDict_.found("NOxThreshold"))
    {
        NOxThreshold_ = readScalar(this->coeffsDict_.lookup("NOxThreshold"));
    }
    const List<List<specieElement> >& specieComposition =
        chemistry.specieComp();

    for (label i=0; i<this->nSpecie_; i++)
    {
        const List<specieElement>& curSpecieComposition =
            specieComposition[i];
        // For all elements in the current species
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
                Info<< "element not considered"<<endl;
            }
        }
        if (this->chemistry_.Y()[i].member() == CO2Name_)
        {
            CO2Id_ = i;
        }
        else if (this->chemistry_.Y()[i].member() == COName_)
        {
            COId_ = i;
        }
        else if (this->chemistry_.Y()[i].member() == HO2Name_)
        {
            HO2Id_ = i;
        }
        else if (this->chemistry_.Y()[i].member() == H2OName_)
        {
            H2OId_ = i;
        }
        else if (this->chemistry_.Y()[i].member() == NOName_)
        {
            NOId_ = i;
        }
    }

    if ((CO2Id_==-1 || COId_==-1 || HO2Id_==-1 || H2OId_==-1) && automaticSIS_)
    {
        FatalErrorInFunction
            << "The name of the species used in automatic SIS are not found in "
            << " the mechanism. You should either set the name for CO2, CO, H2O"
            << " and HO2 properly or set automaticSIS to off "
            << exit(FatalError);
    }

    // To compute zprime, the fuel species should be specified.
    // According to the given mass fraction, an equivalent O/C ratio is computed
    if (automaticSIS_)
    {
        dictionary fuelDict;
        if (this->coeffsDict_.found("fuelSpecies"))
        {
            fuelDict = this->coeffsDict_.subDict("fuelSpecies");
            fuelSpecies_ = fuelDict.toc();
            if (fuelSpecies_.size() == 0)
            {
                FatalErrorInFunction
                    << "With automatic SIS, the fuel species should be "
                    << "specified in the fuelSpecies subDict"
                    << exit(FatalError);
            }
        }
        else
        {
            FatalErrorInFunction
                << "With automatic SIS, the fuel species should be "
                << "specified in the fuelSpecies subDict"
                << exit(FatalError);
        }

        if (this->coeffsDict_.found("nbCLarge"))
        {
            nbCLarge_ = readLabel(fuelDict.lookup("nbCLarge"));
        }

        fuelSpeciesID_.setSize(fuelSpecies_.size());
        fuelSpeciesProp_.setSize(fuelSpecies_.size());
        scalar Mmtot(0.0);

        forAll(fuelSpecies_, i)
        {
            fuelSpeciesProp_[i] = readScalar(fuelDict.lookup(fuelSpecies_[i]));
            for (label j=0; j<this->nSpecie_; j++)
            {
                if (this->chemistry_.Y()[j].member() == fuelSpecies_[i])
                {
                    fuelSpeciesID_[i] = j;
                    break;
                }
            }
            scalar curMm =
                this->chemistry_.specieThermo()[fuelSpeciesID_[i]].W();
            Mmtot += fuelSpeciesProp_[i]/curMm;
        }

        Mmtot = 1.0/Mmtot;
        scalar nbC(0.0);
        scalar nbO(0.0);
        forAll(fuelSpecies_, i)
        {
            label curID = fuelSpeciesID_[i];
            scalar curMm = this->chemistry_.specieThermo()[curID].W();

            nbC += fuelSpeciesProp_[i]*Mmtot/curMm*sC_[curID];
            nbO += fuelSpeciesProp_[i]*Mmtot/curMm*sO_[curID];
        }
        zprime_ = nbO/nbC;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class CompType, class ThermoType>
void CML::chemistryReductionMethods::DAC<CompType, ThermoType>::reduceMechanism
(
    const scalarField &c,
    const scalar T,
    const scalar p
)
{
    scalarField& completeC(this->chemistry_.completeC());
    scalarField c1(this->chemistry_.nEqns(), 0.0);
    for(label i=0; i<this->nSpecie_; i++)
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
    forAll(this->chemistry_.reactions(), i)
    {
        const Reaction<ThermoType>& R = this->chemistry_.reactions()[i];
        // for each reaction compute omegai
        scalar omegai = this->chemistry_.omega
        (
            R, c1, T, p, pf, cf, lRef, pr, cr, rRef
        );

        // Then for each pair of species composing this reaction,
        // compute the rAB matrix (separate the numerator and
        // denominator)

        // While computing the rAB for all the species involved in the reaction
        // we should consider that one can write a reaction A+B=2C as A+B=C+C
        // In this case, the following algorithm only take once the effect
        // of the species. It stores the species encountered in the reaction but
        // use another list to see if this species has already been used

        DynamicList<scalar> wA(R.lhs().size()+R.rhs().size());
        DynamicList<label> wAID(R.lhs().size()+R.rhs().size());

        forAll(R.lhs(), s) // Compute rAB for all species in the left hand side
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
                    // Disable to avoid counting it more than once
                    deltaBi[curIndex] = false;
                    // Test if this rAB is not initialized
                    if (rABPos(ss, curIndex)==-1)
                    {
                        // It starts at rABPos(ss, sj)=0
                        rABPos(ss, curIndex) = NbrABInit[ss];
                        NbrABInit[ss]++;
                        // to avoid overflow
                        rABNum(ss, rABPos(ss, curIndex)) = sl*omegai;
                        // store the other specie involved
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

        forAll(R.rhs(), s) // Compute rAB for all species in the right hand side
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
                    // Disable to avoid counting it more than once
                    deltaBi[curIndex] = false;

                    // Test if this rAB is not initialized
                    if (rABPos(ss, curIndex) == -1)
                    {
                        // it starts at rABPos(ss, sj)=0
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

    scalar phiLarge(0.0);
    scalar phiProgress(0.0);
    if (automaticSIS_)
    {
        // Compute the progress equivalence ratio
        // and the equivalence ratio for fuel decomposition
        label nElements = 4; // 4 main elements (C, H, O, N)

        // Total number of C, H and O (in this order)
        scalarList Na(nElements,0.0);
        scalarList Nal(nElements,0.0); // for large hydrocarbons

        for (label i=0; i<this->nSpecie_; i++)
        {
            // Complete combustion products are not considered
            if
            (
                this->chemistry_.Y()[i].member() == "CO2"
             || this->chemistry_.Y()[i].member() == "H2O"
            )
            {
                continue;
            }
            Na[0] += sC_[i]*c[i];
            Na[1] += sH_[i]*c[i];
            Na[2] += sO_[i]*c[i];
            if (sC_[i]>nbCLarge_ || this->chemistry_.Y()[i].member() == "O2")
            {
                Nal[0] += sC_[i]*c[i];
                Nal[1] += sH_[i]*c[i];
                Nal[2] += sO_[i]*c[i];
            }
        }

        //                              2C(-CO2) + H(-H2O)/2 - z'C(-CO2)
        // Progress equivalence ratio = ----------------------------------
        //                                  O(-CO2-H2O) - z' C(-CO2)
        // where minus means that this species is not considered for the number
        // of atoms and z' is the ratio of the number of O and C in the fuel(s)
        phiProgress = (2*Na[0]+Na[1]/2-zprime_*Na[0])/(Na[2]-zprime_*Na[0]);

        //                                           2Cl + Hl/2
        // Equivalence ratio for fuel decomposition = ----------
        //                                            Ol(+O2)
        phiLarge = (2*Nal[0]+Nal[1]/2)/Nal[2];
    }

    // Using the rAB matrix (numerator and denominator separated)
    // compute the R value according to the search initiating set
    scalarField Rvalue(this->nSpecie_,0.0);
    label speciesNumber = 0;

    // Set all species to inactive and activate them according
    // to rAB and initial set
    for (label i=0; i<this->nSpecie_; i++)
    {
        this->activeSpecies_[i] = false;
    }

    // Initialize the FIFOStack for search set
    FIFOStack<label> Q;

    const labelList& SIS(searchInitSet_);

    // If automaticSIS is on, the search initiating set is selected according to
    // phiProgress and phiLarge
    if (automaticSIS_)
    {
        if (phiLarge >= phiTol_ && phiProgress >= phiTol_)
        {
            // When phiLarge and phiProgress >= phiTol then
            // CO, HO2 and fuel are in the SIS
            Q.push(COId_);
            speciesNumber++;
            this->activeSpecies_[COId_] = true;
            Rvalue[COId_] = 1.0;
            Q.push(HO2Id_);
            speciesNumber++;
            this->activeSpecies_[HO2Id_] = true;
            Rvalue[HO2Id_] = 1.0;
            forAll(fuelSpeciesID_,i)
            {
                Q.push(fuelSpeciesID_[i]);
                speciesNumber++;
                this->activeSpecies_[fuelSpeciesID_[i]] = true;
                Rvalue[fuelSpeciesID_[i]] = 1.0;
            }

        }
        else if (phiLarge < phiTol_ && phiProgress >= phiTol_)
        {
            // When phiLarge < phiTol and phiProgress >= phiTol then
            // CO, HO2 are in the SIS
            Q.push(COId_);
            speciesNumber++;
            this->activeSpecies_[COId_] = true;
            Rvalue[COId_] = 1.0;
            Q.push(HO2Id_);
            speciesNumber++;
            this->activeSpecies_[HO2Id_] = true;
            Rvalue[HO2Id_] = 1.0;

            if (forceFuelInclusion_)
            {
                forAll(fuelSpeciesID_,i)
                {
                    Q.push(fuelSpeciesID_[i]);
                    speciesNumber++;
                    this->activeSpecies_[fuelSpeciesID_[i]] = true;
                    Rvalue[fuelSpeciesID_[i]] = 1.0;
                }
            }
        }
        else
        {
            // When phiLarge and phiProgress< phiTol then
            // CO2, H2O are in the SIS
            Q.push(CO2Id_);
            speciesNumber++;
            this->activeSpecies_[CO2Id_] = true;
            Rvalue[CO2Id_] = 1.0;

            Q.push(H2OId_);
            speciesNumber++;
            this->activeSpecies_[H2OId_] = true;
            Rvalue[H2OId_] = 1.0;
            if (forceFuelInclusion_)
            {
                forAll(fuelSpeciesID_,i)
                {
                    Q.push(fuelSpeciesID_[i]);
                    speciesNumber++;
                    this->activeSpecies_[fuelSpeciesID_[i]] = true;
                    Rvalue[fuelSpeciesID_[i]] = 1.0;
                }
            }
        }

        if (T>NOxThreshold_ && NOId_!=-1)
        {
            Q.push(NOId_);
            speciesNumber++;
            this->activeSpecies_[NOId_] = true;
            Rvalue[NOId_] = 1.0;
        }
    }
    else // No automaticSIS => all species of the SIS are added
    {
        for (label i=0; i<SIS.size(); i++)
        {
            label q = SIS[i];
            this->activeSpecies_[q] = true;
            speciesNumber++;
            Q.push(q);
            Rvalue[q] = 1.0;
        }
    }

    // Execute the main loop for R-value
    while (!Q.empty())
    {
        label u = Q.pop();
        scalar Den = max(PA[u],CA[u]);
        if (Den != 0)
        {
            for (label v=0; v<NbrABInit[u]; v++)
            {
                label otherSpec = rABOtherSpec(u, v);
                scalar rAB = mag(rABNum(u, v))/Den;
                if (rAB > 1)
                {
                    rAB = 1;
                }
                // The direct link is weaker than the user-defined tolerance
                if (rAB >= this->tolerance())
                {
                    scalar Rtemp = Rvalue[u]*rAB;
                    // a link analysed previously is stronger
                    // the (composed) link is stronger than the user-defined
                    // tolerance
                    if ((Rvalue[otherSpec]<Rtemp) && (Rtemp>=this->tolerance()))
                    {
                        Q.push(otherSpec);
                        Rvalue[otherSpec] = Rtemp;
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
                // Flag the reaction to disable it
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
                    // Flag the reaction to disable it
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

    // Change temporary Ns in chemistryModel
    // to make the function nEqns working
    this->chemistry_.setNSpecie(this->NsSimp_);
}


#endif
