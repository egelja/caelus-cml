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
    CML::chemPointISAT

Description
    Leaf of the binary tree.
    The chemPoint stores the composition 'phi', the mapping of this
    composition Rphi, the mapping gradient matrix A and the matrix describing
    the Ellipsoid Of Accuracy (EOA).

  1)When the chemPoint is created the region of accuracy is approximated by
    an ellipsoid E centered in 'phi' (obtained with the constant):
        E = {x| ||L^T.(x-phi)|| <= 1},
    with x a point in the composition space and L^T the transpose of an upper
    triangular matrix describing the EOA (see below: "Computation of L" ).

  2)To RETRIEVE the mapping from the chemPoint phi, the query point phiq has to
    be in the EOA of phi. It follows that, dphi=phiq-phi and to test if phiq
    is in the ellipsoid there are two methods. First, compare r=||dphi|| with
    rmin and rmax. If r < rmin, phiq is in the EOA. If r > rmax, phiq is out of
    the EOA. This operations is O(completeSpaceSize) and is performed first.
    If rmin < r < rmax, then the second method is used:
        ||L^T.dphi|| <= 1 to be in the EOA.

    If phiq is in the EOA, Rphiq is obtained by linear interpolation:
        Rphiq= Rphi + A.dphi.

  3)If phiq is not in the EOA, then the mapping is computed. But as the EOA
    is a conservative approximation of the region of accuracy surrounding the
    point phi, we could expand it by comparing the computed results with the
    one obtained by linear interpolation. The error epsGrow is calculated:
        epsGrow = ||B.(dR - dRl)||,
    with dR = Rphiq - Rphi, dRl = A.dphi and B the diagonal scale factor
    matrix.
    If epsGrow <= tolerance, the EOA is too conservative and a GROW is perforned
    otherwise, the newly computed mapping is associated to the initial
    composition and added to the tree.

  4)To GROW the EOA, we expand it to include the previous EOA and the query
    point phiq. The rank-one matrix method is used. The EOA is transformed
    to a hypersphere centered at the origin. Then it is expanded to include
    the transformed point phiq' on its boundary. Then the inverse transformation
    give the modified matrix L' (see below: "Grow the EOA").


  Computation of L :
    In [1], the EOA of the constant approximation is given by
        E = {x| ||B.A/tolerance.(x-phi)|| <= 1},
    with B a scale factor diagonal matrix, A the mapping gradient matrix and
    tolerance the absolute tolerance. If we take the QR decomposition of
    (B.A)/tolerance= Q.R, with Q an orthogonal matrix and R an upper triangular
    matrix such that the EOA is described by
    (phiq-phi0)^T.R^T.R.(phiq-phi0) <= 1
    L^T = R, both Cholesky decomposition of A^T.B^T.B.A/tolerance^2
    This representation of the ellipsoid is used in [2] and in order to avoid
    large value of semi-axe length in certain direction, a Singular Value
    Decomposition (SVD) is performed on the L matrix:
        L = UDV^T,
    with the orthogonal matrix U giving the directions of the principal axes
    and 1/di the inverse of the element of the diagonal matrix D giving the
    length of the principal semi-axes. To avoid very large value of those
    length,
    di' = max(di, 1/(alphaEOA*sqrt(tolerance))), with alphaEOA = 0.1 (see [2])
    di' = max(di, 1/2), see [1]. The latter will be used in this implementation.
    And L' = UD'V^T, with D' the diagonal matrix with the modified di'.

  Grow the EOA :
    More details about the minimum-volume ellipsoid covering an ellispoid E and
    a point p are found in [3]. Here is the main steps to obtain the modified
    matrix L' describind the new ellipsoid.
        1) calculate the point p' in the transformed space :
            p' = L^T.(p-phi)
        2) compute the rank-one decomposition:
            G = I + gamma.p'.p'^T,
           with gamma = (1/|p'|-1)*1/|p'|^2
        3) compute L':
            L' = L.G.

    References:
    \verbatim
        [1] Pope, S. B. (1997).
        Computationally efficient implementation of combustion chemistry using
        in situ adaptive tabulation.
        Combustion Theory and Modelling, 1, 41-63.

        [2] Lu, L., & Pope, S. B. (2009).
        An improved algorithm for in situ adaptive tabulation.
        Journal of Computational Physics, 228(2), 361-386.

        [3] Pope, S. B. (2008).
        Algorithms for ellipsoids.
        Cornell University Report No. FDA, 08-01.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef chemPointISAT_HPP
#define chemPointISAT_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
class binaryNode;

template<class CompType, class ThermoType>
class TDACChemistryModel;


/*---------------------------------------------------------------------------*\
                       Class chemPointISAT Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class chemPointISAT
{

    //- Pointer to the chemistryModel object
    TDACChemistryModel<CompType, ThermoType>& chemistry_;

    //- Vector storing the composition, temperature and pressure
    //  and deltaT if a variable time step is set on
    scalarField phi_;

    //- Vector storing the mapping of the composition phi
    scalarField Rphi_;

    //- LT the transpose of the L matrix describing the Ellipsoid Of
    //  Accuracy use List of Lists to be able to change size if DAC is used
    scalarSquareMatrix LT_;

    //- A the mapping gradient matrix
    scalarSquareMatrix A_;

    //- Vector storing the scale factor
    scalarField scaleFactor_;

    //- Reference to the node in the binary tree holding this chemPoint
    binaryNode<CompType, ThermoType>* node_;

    //- The size of the composition space (size of the vector phi)
    label completeSpaceSize_;

    //- Number of times the element has been grown
    label nGrowth_;

    //- Tolerance for the Ellipsoid of accuracy
    static scalar tolerance_;

    //- Number of active species stored in the chemPoint
    label nActiveSpecies_;

    //- Vectors that store the index conversion between the simplified
    //  and the complete chemical mechanism
    List<label> simplifiedToCompleteIndex_;

    label timeTag_;
    label lastTimeUsed_;

    bool toRemove_;

    label maxNumNewDim_;

    Switch printProportion_;

    //- Variable to store the number of retrieves the chemPoint
    //  will generate at each time step
    label numRetrieve_;

    //- Variable to store the number of time steps the chempoint is allowed
    //   to still live according to the maxChPLifeTime_ parameter
    label nLifeTime_;

    List<label> completeToSimplifiedIndex_;

    //- Number of equations in addition to the species eqs.
    label nAdditionalEqns_;

    label idT_;
    label idp_;
    label iddeltaT_;

    //- QR decomposition of a matrix
    //  Input : nCols cols number
    //  R the matrix to decompose
    //  QT an empty matrix that stores the transpose of the Q matrix
    //  R is returned in the given R matrix
    //  which is used to store the ellipsoid of accuracy
    void qrDecompose
    (
        const label nCols,
        scalarSquareMatrix& R
    );

    //- QR update of the matrix A
    void qrUpdate
    (
        scalarSquareMatrix& R,
        const label n,
        const scalarField& u,
        const scalarField& v
    );

    void rotate
    (
        scalarSquareMatrix& R,
        const label i,
        const scalar a,
        const scalar b,
        label n
    );


public:


    //- Construct from components
    chemPointISAT
    (
        TDACChemistryModel<CompType, ThermoType>& chemistry,
        const scalarField& phi,
        const scalarField& Rphi,
        const scalarSquareMatrix& A,
        const scalarField& scaleFactor,
        const scalar& tolerance,
        const label& completeSpaceSize,
        const dictionary& coeffsDict,
        binaryNode<CompType, ThermoType>* node = nullptr
    );

    //- Construct from another chemPoint and reference to a binary node
    chemPointISAT
    (
        const chemPointISAT<CompType, ThermoType>& p,
        binaryNode<CompType, ThermoType>* node
    );

    //- Construct from another chemPoint
    chemPointISAT
    (
        chemPointISAT<CompType, ThermoType>& p
    );


    // Member functions

    //- Access to the TDACChemistryModel
    inline TDACChemistryModel<CompType, ThermoType>& chemistry()
    {
        return chemistry_;
    }

    inline label nGrowth()
    {
        return nGrowth_;
    }

    inline label& completeSpaceSize()
    {
        return completeSpaceSize_;
    }

    inline const scalarField& phi() const
    {
        return phi_;
    }

    inline const scalarField& Rphi() const
    {
        return Rphi_;
    }

    inline const scalarField& scaleFactor()
    {
        return scaleFactor_;
    }

    inline const scalar& tolerance()
    {
        return tolerance_;
    }

    static void changeTolerance(scalar newTol)
    {
        tolerance_ = newTol;
    }

    inline binaryNode<CompType, ThermoType>*& node()
    {
        return node_;
    }

    inline const scalarSquareMatrix& A() const
    {
        return A_;
    }

    inline scalarSquareMatrix& A()
    {
        return A_;
    }

    inline const scalarSquareMatrix& LT() const
    {
        return LT_;
    }

    inline scalarSquareMatrix& LT()
    {
        return LT_;
    }

    inline label nActiveSpecies()
    {
        return nActiveSpecies_;
    }

    inline List<label>& completeToSimplifiedIndex()
    {
        return completeToSimplifiedIndex_;
    }

    inline List<label>& simplifiedToCompleteIndex()
    {
        return simplifiedToCompleteIndex_;
    }

    //- Increases the number of retrieves the chempoint has generated
    void increaseNumRetrieve();

    //- Resets the number of retrieves at each time step
    void resetNumRetrieve();

    //- Increases the "counter" of the chP life
    void increaseNLifeTime();

    label simplifiedToCompleteIndex(const label i);

    inline const label& timeTag()
    {
        return timeTag_;
    }

    inline label& lastTimeUsed()
    {
        return lastTimeUsed_;
    }

    inline bool& toRemove()
    {
        return toRemove_;
    }

    inline label& maxNumNewDim()
    {
        return maxNumNewDim_;
    }

    inline const label& numRetrieve()
    {
        return numRetrieve_;
    }

    inline const label& nLifeTime()
    {
        return nLifeTime_;
    }

    inline bool variableTimeStep() const
    {
        return chemistry_.variableTimeStep();
    }

    // ISAT functions
    //- To RETRIEVE the mapping from the stored chemPoint phi, the query
    // point phiq has to be in the EOA of phi.
    // To test if phiq is in the ellipsoid:
    // ||L^T.dphi|| <= 1
    bool inEOA(const scalarField& phiq);

    //- More details about the minimum-volume ellipsoid covering an
    //  ellispoid E and a point p are found in [1].
    //  Here is the main steps to obtain the
    //  modified matrix L' describind the new ellipsoid.
    //  1) calculate the point p' in the transformed space :
    //  p' = L^T.(p-phi)
    //  2) compute the rank-one decomposition:
    //  G = I + gamma.p'.p'^T,
    //  with gamma = (1/|p'|-1)*1/|p'|^2
    //  3) compute L':
    //  L'L'^T = (L.G)(L.G)^T,
    //  L'^T is then obtained by QR decomposition of (L.G)^T = G^T.L^T
    //  [1] Stephen B. Pope, "Algorithms for ellipsoids", FDA 08-01,
    //  Cornell University, 2008
    bool grow(const scalarField& phiq);

    //- If phiq is not in the EOA, then the mapping is computed.
    //  But as the EOA is a conservative approximation of the region of
    //  accuracy surrounding the point phi, we could expand it by
    //  comparing thecomputed results with the one obtained by linear
    //  interpolation.  The error eps is calculated:
    //  eps = ||B.(dR - dRl)||,
    //  with dR = Rphiq - Rphi, dRl = A.dphi and B the diagonal scale
    //  factor matrix.
    //  If eps <= tolerance, the EOA is too conservative and a GROW is
    //  performed,
    //  otherwise, the newly computed mapping is associated to the
    //  initial composition and added to the tree.
    bool checkSolution
    (
        const scalarField& phiq,
        const scalarField& Rphiq
    );
};


} // End namespace CML

#include "SVD.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Defined as static to be able to dynamically change it during simulations
// (all chemPoints refer to the same object)
template<class CompType, class ThermoType>
CML::scalar CML::chemPointISAT<CompType, ThermoType>::tolerance_;

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::qrDecompose
(
    const label nCols,
    scalarSquareMatrix& R
)
{
    scalarField c(nCols);
    scalarField d(nCols);
    scalar scale, sigma, sum;

    for (label k=0; k<nCols-1; k++)
    {
        scale = 0;
        for (label i=k; i<nCols; i++)
        {
            scale=max(scale, mag(R(i, k)));
        }
        if (scale == 0)
        {
            c[k] = d[k] = 0;
        }
        else
        {
            for (label i=k; i<nCols; i++)
            {
                R(i, k) /= scale;
            }
            sum = 0;
            for (label i=k; i<nCols; i++)
            {
                sum += sqr(R(i, k));
            }
            sigma = sign(R(k, k))*sqrt(sum);
            R(k, k) += sigma;
            c[k] = sigma*R(k, k);
            d[k] = -scale*sigma;
            for (label j=k+1; j<nCols; j++)
            {
                sum = 0;
                for ( label i=k; i<nCols; i++)
                {
                    sum += R(i, k)*R(i, j);
                }
                scalar tau = sum/c[k];
                for ( label i=k; i<nCols; i++)
                {
                    R(i, j) -= tau*R(i, k);
                }
            }
        }
    }

    d[nCols-1] = R(nCols-1, nCols-1);

    // form R
    for (label i=0; i<nCols; i++)
    {
        R(i, i) = d[i];
        for ( label j=0; j<i; j++)
        {
            R(i, j)=0;
        }
    }
}


template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::qrUpdate
(
    scalarSquareMatrix& R,
    const label n,
    const CML::scalarField &u,
    const CML::scalarField &v
)
{
    label k;

    scalarField w(u);
    for (k=n-1; k>=0; k--)
    {
        if (w[k] != 0)
        {
            break;
        }
    }

    if (k < 0)
    {
        k = 0;
    }

    for (label i=k-1; i>=0; i--)
    {
        rotate(R, i, w[i],-w[i+1], n);
        if (w[i] == 0)
        {
            w[i] = mag(w[i+1]);
        }
        else if (mag(w[i]) > mag(w[i+1]))
        {
            w[i] = mag(w[i])*sqrt(1.0 + sqr(w[i+1]/w[i]));
        }
        else
        {
            w[i] = mag(w[i+1])*sqrt(1.0 + sqr(w[i]/w[i+1]));
        }
    }

    for (label i=0; i<n; i++)
    {
        R(0, i) += w[0]*v[i];
    }

    for (label i=0; i<k; i++)
    {
        rotate(R, i, R(i, i), -R(i+1, i), n);
    }
}


template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::rotate
(
    scalarSquareMatrix& R,
    const label i,
    const scalar a,
    const scalar b,
    label n
)
{
    scalar c, fact, s, w, y;
    if (a == 0)
    {
        c = 0;
        s = (b >= 0 ? 1 : -1);
    }
    else if (mag(a) > mag(b))
    {
        fact = b/a;
        c = sign(a)/sqrt(1.0 + sqr(fact));
        s = fact*c;
    }
    else
    {
        fact = a/b;
        s = sign(b)/sqrt(1.0 + sqr(fact));
        c = fact*s;
    }
    for (label j=i;j<n;j++)
    {
        y = R(i, j);
        w = R(i+1, j);
        R(i, j) = c*y-s*w;
        R(i+1, j) = s*y+c*w;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemPointISAT<CompType, ThermoType>::chemPointISAT
(
    TDACChemistryModel<CompType, ThermoType>& chemistry,
    const scalarField& phi,
    const scalarField& Rphi,
    const scalarSquareMatrix& A,
    const scalarField& scaleFactor,
    const scalar& tolerance,
    const label& completeSpaceSize,
    const dictionary& coeffsDict,
    binaryNode<CompType, ThermoType>* node
)
:
    chemistry_(chemistry),
    phi_(phi),
    Rphi_(Rphi),
    A_(A),
    scaleFactor_(scaleFactor),
    node_(node),
    completeSpaceSize_(completeSpaceSize),
    nGrowth_(0),
    nActiveSpecies_(chemistry.mechRed()->NsSimp()),
    simplifiedToCompleteIndex_(nActiveSpecies_),
    timeTag_(chemistry_.timeSteps()),
    lastTimeUsed_(chemistry_.timeSteps()),
    toRemove_(false),
    maxNumNewDim_(coeffsDict.lookupOrDefault("maxNumNewDim",0)),
    printProportion_(coeffsDict.lookupOrDefault("printProportion",false)),
    numRetrieve_(0),
    nLifeTime_(0),
    completeToSimplifiedIndex_
    (
        completeSpaceSize - (2 + (variableTimeStep() == 1 ? 1 : 0))
    )
{
    tolerance_ = tolerance;

    if (variableTimeStep())
    {
        nAdditionalEqns_ = 3;
        iddeltaT_ = completeSpaceSize - 1;
        scaleFactor_[iddeltaT_] *= phi_[iddeltaT_] / tolerance_;
    }
    else
    {
        nAdditionalEqns_ = 2;
        iddeltaT_ = completeSpaceSize; // will not be used
    }
    idT_ = completeSpaceSize - nAdditionalEqns_;
    idp_ = completeSpaceSize - nAdditionalEqns_ + 1;

    bool isMechRedActive = chemistry_.mechRed()->active();
    if (isMechRedActive)
    {
        for (label i=0; i<completeSpaceSize-nAdditionalEqns_; i++)
        {
            completeToSimplifiedIndex_[i] =
                chemistry.completeToSimplifiedIndex()[i];
        }
        for (label i=0; i<nActiveSpecies_; i++)
        {
            simplifiedToCompleteIndex_[i] =
                chemistry.simplifiedToCompleteIndex()[i];
        }
    }

    label reduOrCompDim = completeSpaceSize;
    if (isMechRedActive)
    {
        reduOrCompDim = nActiveSpecies_+nAdditionalEqns_;
    }

    // SVD decomposition A = U*D*V^T
    SVD svdA(A);

    scalarDiagonalMatrix D(reduOrCompDim);
    const scalarDiagonalMatrix& S = svdA.S();

    // Replace the value of vector D by max(D, 1/2), first ISAT paper
    for (label i=0; i<reduOrCompDim; i++)
    {
        D[i] = max(S[i], 0.5);
    }

    // Rebuild A with max length, tol and scale factor before QR decomposition
    scalarRectangularMatrix Atilde(reduOrCompDim);

    // Result stored in Atilde
    multiply(Atilde, svdA.U(), D, svdA.V().T());

    for (label i=0; i<reduOrCompDim; i++)
    {
        for (label j=0; j<reduOrCompDim; j++)
        {
            label compi = i;

            if (isMechRedActive)
            {
                compi = simplifiedToCompleteIndex(i);
            }

            // SF*A/tolerance
            // (where SF is diagonal with inverse of scale factors)
            // SF*A is the same as dividing each line by the scale factor
            // corresponding to the species of this line
            Atilde(i, j) /= (tolerance*scaleFactor[compi]);
        }
    }

    // The object LT_ (the transpose of the Q) describe the EOA, since we have
    // A^T B^T B A that should be factorized into L Q^T Q L^T and is set in the
    // qrDecompose function
    LT_ = scalarSquareMatrix(Atilde);

    qrDecompose(reduOrCompDim, LT_);
}


template<class CompType, class ThermoType>
CML::chemPointISAT<CompType, ThermoType>::chemPointISAT
(
    CML::chemPointISAT<CompType, ThermoType>& p
)
:
    chemistry_(p.chemistry()),
    phi_(p.phi()),
    Rphi_(p.Rphi()),
    LT_(p.LT()),
    A_(p.A()),
    scaleFactor_(p.scaleFactor()),
    node_(p.node()),
    completeSpaceSize_(p.completeSpaceSize()),
    nGrowth_(p.nGrowth()),
    nActiveSpecies_(p.nActiveSpecies()),
    simplifiedToCompleteIndex_(p.simplifiedToCompleteIndex()),
    timeTag_(p.timeTag()),
    lastTimeUsed_(p.lastTimeUsed()),
    toRemove_(p.toRemove()),
    maxNumNewDim_(p.maxNumNewDim()),
    numRetrieve_(0),
    nLifeTime_(0),
    completeToSimplifiedIndex_(p.completeToSimplifiedIndex())
{
    tolerance_ = p.tolerance();

    if (variableTimeStep())
    {
        nAdditionalEqns_ = 3;
        idT_ = completeSpaceSize() - 3;
        idp_ = completeSpaceSize() - 2;
        iddeltaT_ = completeSpaceSize() - 1;
    }
    else
    {
        nAdditionalEqns_ = 2;
        idT_ = completeSpaceSize() - 2;
        idp_ = completeSpaceSize() - 1;
        iddeltaT_ = completeSpaceSize(); // will not be used
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
bool CML::chemPointISAT<CompType, ThermoType>::inEOA(const scalarField& phiq)
{
    scalarField dphi(phiq-phi());
    bool isMechRedActive = chemistry_.mechRed()->active();
    label dim(0);
    if (isMechRedActive)
    {
        dim = nActiveSpecies_;
    }
    else
    {
        dim = completeSpaceSize() - nAdditionalEqns_;
    }

    scalar epsTemp = 0;
    List<scalar> propEps(completeSpaceSize(), scalar(0));

    for (label i=0; i<completeSpaceSize()-nAdditionalEqns_; i++)
    {
        scalar temp = 0;

        // When mechanism reduction is inactive OR on active species multiply L
        // by dphi to get the distance in the active species direction else (for
        // inactive species), just multiply the diagonal element and dphi
        if
        (
            !(isMechRedActive)
          ||(isMechRedActive && completeToSimplifiedIndex_[i] != -1)
        )
        {
            label si=(isMechRedActive) ? completeToSimplifiedIndex_[i] : i;

            for (label j=si; j<dim; j++)// LT is upper triangular
            {
                label sj=(isMechRedActive) ? simplifiedToCompleteIndex_[j] : j;
                temp += LT_(si, j)*dphi[sj];
            }

            temp += LT_(si, dim)*dphi[idT_];
            temp += LT_(si, dim+1)*dphi[idp_];
            if (variableTimeStep())
            {
                temp += LT_(si, dim+2)*dphi[iddeltaT_];
            }
        }
        else
        {
            temp = dphi[i]/(tolerance_*scaleFactor_[i]);
        }

        epsTemp += sqr(temp);

        if (printProportion_)
        {
            propEps[i] = temp;
        }
    }

    // Temperature
    if (variableTimeStep())
    {
        epsTemp +=
            sqr
            (
                LT_(dim, dim)*dphi[idT_]
               +LT_(dim, dim+1)*dphi[idp_]
               +LT_(dim, dim+2)*dphi[iddeltaT_]
            );
    }
    else
    {
        epsTemp +=
            sqr
            (
                LT_(dim, dim)*dphi[idT_]
               +LT_(dim, dim+1)*dphi[idp_]
            );
    }

    // Pressure
    if (variableTimeStep())
    {
        epsTemp +=
            sqr
            (
                LT_(dim+1, dim+1)*dphi[idp_]
               +LT_(dim+1, dim+2)*dphi[iddeltaT_]
            );
    }
    else
    {
        epsTemp += sqr(LT_(dim+1, dim+1)*dphi[idp_]);
    }

    if (variableTimeStep())
    {
        epsTemp += sqr(LT_[dim+2][dim+2]*dphi[iddeltaT_]);
    }

    if (printProportion_)
    {
        propEps[idT_] = sqr
        (
            LT_(dim, dim)*dphi[idT_]
          + LT_(dim, dim+1)*dphi[idp_]
        );

        propEps[idp_] =
            sqr(LT_(dim+1, dim+1)*dphi[idp_]);

        if (variableTimeStep())
        {
            propEps[iddeltaT_] =
                sqr(LT_[dim+2][dim+2]*dphi[iddeltaT_]);
        }

    }

    if (sqrt(epsTemp) > 1 + tolerance_)
    {
        if (printProportion_)
        {
            scalar max = -1;
            label maxIndex = -1;
            for (label i=0; i<completeSpaceSize(); i++)
            {
                if(max < propEps[i])
                {
                    max = propEps[i];
                    maxIndex = i;
                }
            }
            word propName;
            if (maxIndex >= completeSpaceSize() - nAdditionalEqns_)
            {
                if (maxIndex == idT_)
                {
                    propName = "T";
                }
                else if (maxIndex == idp_)
                {
                    propName = "p";
                }
                else if (maxIndex == iddeltaT_)
                {
                    propName = "deltaT";
                }
            }
            else
            {
                propName = chemistry_.Y()[maxIndex].member();
            }
            Info<< "Direction maximum impact to error in ellipsoid: "
                << propName << endl;
            Info<< "Proportion to the total error on the retrieve: "
                << max/(epsTemp+SMALL) << endl;
        }
        return false;
    }
    else
    {
        return true;
    }
}


template<class CompType, class ThermoType>
bool CML::chemPointISAT<CompType, ThermoType>::checkSolution
(
    const scalarField& phiq,
    const scalarField& Rphiq
)
{
    scalar eps2 = 0;
    scalarField dR(Rphiq - Rphi());
    scalarField dphi(phiq - phi());
    const scalarField& scaleFactorV(scaleFactor());
    const scalarSquareMatrix& Avar(A());
    bool isMechRedActive = chemistry_.mechRed()->active();
    scalar dRl = 0;
    label dim = completeSpaceSize()-2;
    if (isMechRedActive)
    {
        dim = nActiveSpecies_;
    }

    // Since we build only the solution for the species, T and p are not
    // included
    for (label i=0; i<completeSpaceSize()-nAdditionalEqns_; i++)
    {
        dRl = 0;
        if (isMechRedActive)
        {
            label si = completeToSimplifiedIndex_[i];

            // If this species is active
            if (si != -1)
            {
                for (label j=0; j<dim; j++)
                {
                    label sj=simplifiedToCompleteIndex_[j];
                    dRl += Avar(si, j)*dphi[sj];
                }
                dRl += Avar(si, nActiveSpecies_)*dphi[idT_];
                dRl += Avar(si, nActiveSpecies_+1)*dphi[idp_];
                if (variableTimeStep())
                {
                    dRl += Avar(si, nActiveSpecies_+2)*dphi[iddeltaT_];
                }
            }
            else
            {
                dRl = dphi[i];
            }
        }
        else
        {
            for (label j=0; j<completeSpaceSize(); j++)
            {
                dRl += Avar(i, j)*dphi[j];
            }
        }
        eps2 += sqr((dR[i]-dRl)/scaleFactorV[i]);
    }

    eps2 = sqrt(eps2);
    if (eps2 > tolerance())
    {
        return false;
    }
    else
    {
        // if the solution is in the ellipsoid of accuracy
        return true;
    }
}


template<class CompType, class ThermoType>
bool CML::chemPointISAT<CompType, ThermoType>::grow(const scalarField& phiq)
{
    scalarField dphi(phiq - phi());
    label dim = completeSpaceSize();
    label initNActiveSpecies(nActiveSpecies_);
    bool isMechRedActive = chemistry_.mechRed()->active();

    if (isMechRedActive)
    {
        label activeAdded(0);
        DynamicList<label> dimToAdd(0);

        // check if the difference of active species is lower than the maximum
        // number of new dimensions allowed
        for (label i=0; i<completeSpaceSize()-nAdditionalEqns_; i++)
        {
            // first test if the current chemPoint has an inactive species
            // corresponding to an active one in the query point
            if
            (
                completeToSimplifiedIndex_[i] == -1
             && chemistry_.completeToSimplifiedIndex()[i]!=-1
            )
            {
                activeAdded++;
                dimToAdd.append(i);
            }
            // then test if an active species in the current chemPoint
            // corresponds to an inactive on the query side
            if
            (
                completeToSimplifiedIndex_[i] != -1
             && chemistry_.completeToSimplifiedIndex()[i] == -1
            )
            {
                activeAdded++;
                // we don't need to add a new dimension but we count it to have
                // control on the difference through maxNumNewDim
            }
            // finally test if both points have inactive species but
            // with a dphi!=0
            if
            (
                completeToSimplifiedIndex_[i] == -1
             && chemistry_.completeToSimplifiedIndex()[i] == -1
             && dphi[i] != 0
            )
            {
                activeAdded++;
                dimToAdd.append(i);
            }
        }

        // if the number of added dimension is too large, growth fail
        if (activeAdded > maxNumNewDim_)
        {
            return false;
        }

        // the number of added dimension to the current chemPoint
        nActiveSpecies_ += dimToAdd.size();
        simplifiedToCompleteIndex_.setSize(nActiveSpecies_);
        forAll(dimToAdd, i)
        {
            label si = nActiveSpecies_ - dimToAdd.size() + i;
            // add the new active species
            simplifiedToCompleteIndex_[si] = dimToAdd[i];
            completeToSimplifiedIndex_[dimToAdd[i]] = si;
        }

        // update LT and A :
        //-add new column and line for the new active species
        //-transfer last two lines of the previous matrix (p and T) to the end
        //  (change the diagonal position)
        //-set all element of the new lines and columns to zero except diagonal
        //  (=1/(tolerance*scaleFactor))
        if (nActiveSpecies_ > initNActiveSpecies)
        {
            scalarSquareMatrix LTvar = LT_; // take a copy of LT_
            scalarSquareMatrix Avar = A_; // take a copy of A_
            LT_ = scalarSquareMatrix(nActiveSpecies_+nAdditionalEqns_, Zero);
            A_ = scalarSquareMatrix(nActiveSpecies_+nAdditionalEqns_, Zero);

            // write the initial active species
            for (label i=0; i<initNActiveSpecies; i++)
            {
                for (label j=0; j<initNActiveSpecies; j++)
                {
                    LT_(i, j) = LTvar(i, j);
                    A_(i, j) = Avar(i, j);
                }
            }

            // write the columns for temperature and pressure
            for (label i=0; i<initNActiveSpecies; i++)
            {
                for (label j=1; j>=0; j--)
                {
                    LT_(i, nActiveSpecies_+j)=LTvar(i, initNActiveSpecies+j);
                    A_(i, nActiveSpecies_+j)=Avar(i, initNActiveSpecies+j);
                    LT_(nActiveSpecies_+j, i)=LTvar(initNActiveSpecies+j, i);
                    A_(nActiveSpecies_+j, i)=Avar(initNActiveSpecies+j, i);
                }
            }
            // end with the diagonal elements for temperature and pressure
            LT_(nActiveSpecies_, nActiveSpecies_)=
                LTvar(initNActiveSpecies, initNActiveSpecies);
            A_(nActiveSpecies_, nActiveSpecies_)=
                Avar(initNActiveSpecies, initNActiveSpecies);
            LT_(nActiveSpecies_+1, nActiveSpecies_+1)=
                LTvar(initNActiveSpecies+1, initNActiveSpecies+1);
            A_(nActiveSpecies_+1, nActiveSpecies_+1)=
                Avar(initNActiveSpecies+1, initNActiveSpecies+1);

            if (variableTimeStep())
            {
                LT_(nActiveSpecies_+2, nActiveSpecies_+2)=
                    LTvar(initNActiveSpecies+2, initNActiveSpecies+2);
                A_(nActiveSpecies_+2, nActiveSpecies_+2)=
                    Avar(initNActiveSpecies+2, initNActiveSpecies+2);
            }

            for (label i=initNActiveSpecies; i<nActiveSpecies_;i++)
            {
                LT_(i, i)=
                    1.0
                   /(tolerance_*scaleFactor_[simplifiedToCompleteIndex_[i]]);
                A_(i, i) = 1;
            }
        }

        dim = nActiveSpecies_ + nAdditionalEqns_;
    }

    // beginning of grow algorithm
    scalarField phiTilde(dim, 0);
    scalar normPhiTilde = 0;
    // p' = L^T.(p-phi)

    for (label i=0; i<dim; i++)
    {
        for (label j=i; j<dim-nAdditionalEqns_; j++)// LT is upper triangular
        {
            label sj = j;
            if (isMechRedActive)
            {
                sj = simplifiedToCompleteIndex_[j];
            }
            phiTilde[i] += LT_(i, j)*dphi[sj];
        }

        phiTilde[i] += LT_(i, dim-nAdditionalEqns_)*dphi[idT_];
        phiTilde[i] += LT_(i, dim-nAdditionalEqns_+1)*dphi[idp_];

        if (variableTimeStep())
        {
            phiTilde[i] += LT_(i, dim-nAdditionalEqns_ + 2)*dphi[iddeltaT_];
        }
        normPhiTilde += sqr(phiTilde[i]);
    }

    scalar invSqrNormPhiTilde = 1.0/normPhiTilde;
    normPhiTilde = sqrt(normPhiTilde);

    // gamma = (1/|p'| - 1)/|p'|^2
    scalar gamma = (1/normPhiTilde - 1)*invSqrNormPhiTilde;
    scalarField u(gamma*phiTilde);
    scalarField v(dim, 0);

    for (label i=0; i<dim; i++)
    {
        for (label j=0; j<=i;j++)
        {
            v[i] += phiTilde[j]*LT_(j, i);
        }
    }

    qrUpdate(LT_,dim, u, v);
    nGrowth_++;

    return true;
}


template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::increaseNumRetrieve()
{
    this->numRetrieve_++;
}


template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::resetNumRetrieve()
{
    this->numRetrieve_ = 0;
}


template<class CompType, class ThermoType>
void CML::chemPointISAT<CompType, ThermoType>::increaseNLifeTime()
{
    this->nLifeTime_++;
}


template<class CompType, class ThermoType>
CML::label CML::chemPointISAT<CompType, ThermoType>::
simplifiedToCompleteIndex
(
    const label i
)
{
    if (i < nActiveSpecies_)
    {
        return simplifiedToCompleteIndex_[i];
    }
    else if (i == nActiveSpecies_)
    {
        return completeSpaceSize_-nAdditionalEqns_;
    }
    else if (i == nActiveSpecies_ + 1)
    {
        return completeSpaceSize_-nAdditionalEqns_ + 1;
    }
    else if (variableTimeStep() && (i == nActiveSpecies_ + 2))
    {
        return completeSpaceSize_-nAdditionalEqns_ + 2;
    }
    else
    {
        return -1;
    }
}


#endif
