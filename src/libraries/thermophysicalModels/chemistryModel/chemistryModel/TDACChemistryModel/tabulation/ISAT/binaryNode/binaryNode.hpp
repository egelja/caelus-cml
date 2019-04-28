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
    CML::binaryNode

Description
    Node of the binary tree


\*---------------------------------------------------------------------------*/

#ifndef binaryNode_HPP
#define binaryNode_HPP

#include "chemPointISAT.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class binaryNode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class binaryNode
{

public:

    //- Element on the left
    chemPointISAT<CompType, ThermoType>* leafLeft_;

    //- Element on the right
    chemPointISAT<CompType, ThermoType>* leafRight_;

    //- Node which follows on the left
    binaryNode<CompType, ThermoType>* nodeLeft_;

    //- Node which follows on the right
    binaryNode<CompType, ThermoType>* nodeRight_;

    //- Parent node
    binaryNode<CompType, ThermoType>* parent_;

    //- Number of equations in addition to the species eqs.
    label nAdditionalEqns_;

    scalarField v_;

    scalar a_;

    //- Compute vector v:
    //  Let E be the ellipsoid which covers the region of accuracy of
    //  the left leaf (previously defined). E is described by
    //  E={phi| ||L^T.(phi-phi0)|| <= 1}, (see chemPoint for more details).
    //  let E' be the transformation of E in a space where E' is a hypersphere
    //  centered at the origin, in this space y=L^T.(phi-phi0) and then
    //  E'={y| ||y||<=1}
    //  let u be the unit vector joining the center of E' and the newly added
    //  composition point in the transformed space
    //  (y2=L^T.(phiq-phi0)),u = y2/||y2|
    //  Then the hyperplane separating the two points is defined as the
    //  perpendicular bisector of the segment linking 0 to y2
    //      H' = {y| u^T.(y-yh) = 0},
    //  where yh = y2/2.
    //  In the original composition space, the hyperplane H is defined by
    //      H = {y| v^T(phi-phih) = 0},
    //  where phih = phi0 + L^-T.yh = (phi0 + phiq) / 2 and v is
    //            L.L^T (phiq-phi0)
    //      v = -------------------- .
    //          ||L.L^T (phiq-phi0)||
    //
    //  Note :  v is not normalised in this implementation since it is used
    //              on both side of an equality to know if one should go on the
    //              left or the right in the binary tree
    //  Parameter:
    //      elementLeft : chemPoint of the left element
    //      elementRight: chemPoint of the right element
    //       v : empty scalar field to store v
    //  Returnq: void (v is stored in the empty scalarField)
    void calcV
    (
        chemPointISAT<CompType, ThermoType>*& elementLeft,
        chemPointISAT<CompType, ThermoType>*& elementRight,
        scalarField& v
    );

    //- Compute a the product v^T.phih, with phih = (phi0 + phiq)/2.
    //  When travelling in the binary tree,
    //  to know in which part of the composition space the query point 'phi'
    //  belongs to, v^T.phi is performed. If the result is "> a" then it belongs
    //  to the right part (where phiq is), otherwise it belongs to the left
    // part (where phi0 is).
    scalar calcA
    (
        chemPointISAT<CompType, ThermoType>* elementLeft,
        chemPointISAT<CompType, ThermoType>* elementRight
    );


    //- Construct null
    binaryNode();

    //- Construct from components
    binaryNode
    (
        chemPointISAT<CompType, ThermoType>* elementLeft,
        chemPointISAT<CompType, ThermoType>* elementRight,
        binaryNode<CompType, ThermoType>* parent
    );


    // Member functions

    //- Access
    inline chemPointISAT<CompType, ThermoType>*& leafLeft()
    {
        return leafLeft_;
    }

    inline chemPointISAT<CompType, ThermoType>*& leafRight()
    {
        return leafRight_;
    }

    inline binaryNode<CompType, ThermoType>*& nodeLeft()
    {
        return nodeLeft_;
    }

    inline binaryNode<CompType, ThermoType>*& nodeRight()
    {
        return nodeRight_;
    }

    inline binaryNode<CompType, ThermoType>*& parent()
    {
        return parent_;
    }

    //- Topology

    inline const scalarField& v() const
    {
        return v_;
    }

    inline scalarField& v()
    {
        return v_;
    }

    inline const scalar& a() const
    {
        return a_;
    }

    inline scalar& a()
    {
        return a_;
    }
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::binaryNode<CompType, ThermoType>::binaryNode()
:
    leafLeft_(nullptr),
    leafRight_(nullptr),
    nodeLeft_(nullptr),
    nodeRight_(nullptr),
    parent_(nullptr),
    nAdditionalEqns_(0)
{}


template<class CompType, class ThermoType>
CML::binaryNode<CompType, ThermoType>::binaryNode
(
    chemPointISAT<CompType, ThermoType>* elementLeft,
    chemPointISAT<CompType, ThermoType>* elementRight,
    binaryNode<CompType, ThermoType>* parent
)
:
    leafLeft_(elementLeft),
    leafRight_(elementRight),
    nodeLeft_(nullptr),
    nodeRight_(nullptr),
    parent_(parent),
    v_(elementLeft->completeSpaceSize(), 0)
{
    if (elementLeft->variableTimeStep())
    {
        nAdditionalEqns_ = 3;
    }
    else
    {
        nAdditionalEqns_ = 2;
    }

    calcV(elementLeft, elementRight, v_);
    a_ = calcA(elementLeft, elementRight);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
void CML::binaryNode<CompType, ThermoType>::calcV
(
    chemPointISAT<CompType, ThermoType>*& elementLeft,
    chemPointISAT<CompType, ThermoType>*& elementRight,
    scalarField& v
)
{
    // LT is the transpose of the L matrix
    scalarSquareMatrix& LT = elementLeft->LT();
    bool mechReductionActive = elementLeft->chemistry().mechRed()->active();

    // Difference of composition in the full species domain
    scalarField phiDif(elementRight->phi() - elementLeft->phi());
    const scalarField& scaleFactor(elementLeft->scaleFactor());
    scalar epsTol = elementLeft->tolerance();

    // v = LT.T()*LT*phiDif
    for (label i=0; i<elementLeft->completeSpaceSize(); i++)
    {
        label si = i;
        bool outOfIndexI = true;
        if (mechReductionActive)
        {
            if (i<elementLeft->completeSpaceSize() - nAdditionalEqns_)
            {
                si = elementLeft->completeToSimplifiedIndex()[i];
                outOfIndexI = (si == -1);
            }
            else // temperature and pressure
            {
                outOfIndexI = false;
                const label dif =
                    i - (elementLeft->completeSpaceSize() - nAdditionalEqns_);
                si = elementLeft->nActiveSpecies() + dif;
            }
        }
        if (!mechReductionActive || (mechReductionActive && !(outOfIndexI)))
        {
            v[i] = 0;
            for (label j=0; j<elementLeft->completeSpaceSize(); j++)
            {
                label sj = j;
                bool outOfIndexJ = true;
                if (mechReductionActive)
                {
                    if (j < elementLeft->completeSpaceSize() - nAdditionalEqns_)
                    {
                        sj = elementLeft->completeToSimplifiedIndex()[j];
                        outOfIndexJ = (sj==-1);
                    }
                    else
                    {
                        outOfIndexJ = false;
                        const label dif =
                            j
                          - (
                                elementLeft->completeSpaceSize()
                              - nAdditionalEqns_
                            );
                        sj = elementLeft->nActiveSpecies() + dif;
                    }
                }
                if
                (
                    !mechReductionActive
                  ||(mechReductionActive && !(outOfIndexJ))
                )
                {
                    // Since L is a lower triangular matrix k=0->min(i, j)
                    for (label k=0; k<=min(si, sj); k++)
                    {
                        v[i] += LT(k, si)*LT(k, sj)*phiDif[j];
                    }
                }
            }
        }
        else
        {
            // When it is an inactive species the diagonal element of LT is
            // 1/(scaleFactor*epsTol)
            v[i] = phiDif[i]/sqr(scaleFactor[i]*epsTol);
        }
    }
}


template<class CompType, class ThermoType>
CML::scalar CML::binaryNode<CompType, ThermoType>::calcA
(
    chemPointISAT<CompType, ThermoType>* elementLeft,
    chemPointISAT<CompType, ThermoType>* elementRight
)
{
    scalarField phih((elementLeft->phi() + elementRight->phi())/2);
    scalar a = 0;
    forAll(phih, i)
    {
        a += v_[i]*phih[i];
    }

    return a;
}


#endif
