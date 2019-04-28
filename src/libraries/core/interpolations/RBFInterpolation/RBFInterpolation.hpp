/*---------------------------------------------------------------------------*\
 Copyright 2009 TU Delft
 Copyright 2009 FSB Zagreb
 Copyright 2013-2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    RBFInterpolation

Description
    Radial basis function interpolation class

Description
    Interpolation class which uses Radial Basis Functions to interpolate the
    fluid displacements for given boundary displacements.

	The coefficient vectors, alpha and beta are determined by solving
    the system:

	| db | = | Mbb Pb | | alpha |
	| 0  |   | Pb  0  | |  beta |

	where db are the given boundary displacements,
	Mbb the boundary RBF correlation matrix (NbxNb), containing RBF evaluations
    at the boundary nodes, and Pb some linear polynomial matrix (Nbx4).

	Those coefficients are calculated every timestep, with the current
    boundary displacements db, with the inverse of Mbb. Using those
    coefficients, the RBF is evaluated at all fluid points every
    timestep.

	The efficiency of this method is increased by:
		1) using control points which is a subset of the moving
           boundary points. Those control points are selected by
		   a coarsening function.
		2) The outer boundary points are neglected since a cutoff function
           is used toward the outer boundaries.

Author
    Frank Bos, TU Delft.
    Dubravko Matijasevic, FSB Zagreb.
    Darrin Stephens, Applied CCM.

SourceFiles
    RBFInterpolation.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBFInterpolation_HPP
#define RBFInterpolation_HPP

#include "dictionary.hpp"
#include "RBFFunction.hpp"
#include "point.hpp"
#include "Switch.hpp"
#include "simpleMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class RBFInterpolation Declaration
\*---------------------------------------------------------------------------*/

class RBFInterpolation
{
    // Private data

        //- Dictionary
        const dictionary& dict_;

        //- Reference to control points
        const vectorField& controlPoints_;

        //- Rerefence to all points
        const vectorField& allPoints_;

        //- RBF function
        autoPtr<RBFFunction> RBF_;

        //- Interpolation matrix
        mutable scalarSquareMatrix* BPtr_;

        //- Focal point for cut-off radii
        point focalPoint_;

        //- Inner cut-off radius
        scalar innerRadius_;

        //- Outer cut-off radius
        scalar outerRadius_;

        //- Add polynomials to RBF matrix
        Switch polynomials_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const RBFInterpolation&);

        //- Return interpolation matrix
        const scalarSquareMatrix& B() const;

        //- Calculate interpolation matrix
        void calcB() const;

        //- Clear out
        void clearOut();


public:

    // Constructors

        //- Construct from components
        RBFInterpolation
        (
            const dictionary& dict,
            const vectorField& controlPoints,
            const vectorField& allPoints
        );

        //- Construct as copy
        RBFInterpolation(const RBFInterpolation&);


    // Destructor

        ~RBFInterpolation();


    // Member Functions

        //- Interpolate
        template<class Type>
        tmp<Field<Type> > interpolate(const Field<Type>& ctrlField) const;

        //- Move points
        void movePoints();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::RBFInterpolation::interpolate
(
    const Field<Type>& ctrlField
) const
{
    // Collect the values from ALL control points to all CPUs
    // Then, each CPU will do interpolation only on local allPoints_

    if (ctrlField.size() != controlPoints_.size())
    {
        FatalErrorInFunction
            << "Incorrect size of source field.  Size = " << ctrlField.size()
            << " nControlPoints = " << controlPoints_.size()
            << abort(FatalError);
    }

    tmp<Field<Type> > tresult
    (
        new Field<Type>(allPoints_.size(), pTraits<Type>::zero)
    );

    Field<Type>& result = tresult();

    // FB 21-12-2008
    // 1) Calculate alpha and beta coefficients using the Inverse
    // 2) Calculate displacements of internal nodes using RBF values,
    //    alpha's and beta's
    // 3) Return displacements using tresult()

    const label nControlPoints = controlPoints_.size();
    const scalarSquareMatrix& mat = this->B();

    // Determine interpolation coefficients
    Field<Type> alpha(nControlPoints, pTraits<Type>::zero);
    Field<Type> beta(4, pTraits<Type>::zero);

    for (label row = 0; row < nControlPoints; row++)
    {
        for (label col = 0; col < nControlPoints; col++)
        {
            alpha[row] += mat[row][col]*ctrlField[col];
        }
    }

    if (polynomials_)
    {
        for
        (
            label row = nControlPoints;
            row < nControlPoints + 4;
            row++
        )
        {
            for (label col = 0; col < nControlPoints; col++)
            {
                beta[row - nControlPoints] += mat[row][col]*ctrlField[col];
            }
        }
    }

    // Evaluation
    scalar t;

    // Algorithmic improvement, Matteo Lombardi.  21/Mar/2011

    forAll (allPoints_, flPoint)
    {
        // Cut-off function to justify neglecting outer boundary points
        t = (CML::mag(allPoints_[flPoint] - focalPoint_) - innerRadius_)/
            (outerRadius_ - innerRadius_);

        if (t >= 1)
        {
            // Increment is zero: w = 0
            result[flPoint] = 0*result[flPoint];
        }
        else
        {
            // Full calculation of weights
            scalarField weights(RBF_->weights(controlPoints_, allPoints_[flPoint]));

            forAll (controlPoints_, i)
            {
                result[flPoint] += weights[i]*alpha[i];
            }

            if (polynomials_)
            {
                result[flPoint] +=
                    beta[0]
                  + beta[1]*allPoints_[flPoint].x()
                  + beta[2]*allPoints_[flPoint].y()
                  + beta[3]*allPoints_[flPoint].z();
            }

            scalar w;

            if (t <= 0)
            {
                w = 1.0;
            }
            else
            {
                w = 1 - sqr(t)*(3-2*t);
            }

            result[flPoint] = w*result[flPoint];
        }
    }

    return tresult;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
