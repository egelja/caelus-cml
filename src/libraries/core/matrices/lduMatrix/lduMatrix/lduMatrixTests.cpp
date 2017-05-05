/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

Description
    Convergence and singularity tests for solvers.

\*---------------------------------------------------------------------------*/

#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::lduMatrix::solverPerformance::solverPerformance(Istream& is)
{
    is  >> *this;
}


bool CML::lduMatrix::solverPerformance::checkConvergence
(
    const scalar Tolerance,
    const scalar RelTolerance,
    const label  iter,
    const label  minIter
)
{
    if (debug >= 2)
    {
        Info<< solverName_
            << ":  Iteration " << noIterations_
            << " residual = " << finalResidual_
            << endl;
    }

    if
    (
     (   finalResidual_ < Tolerance
     || (
            RelTolerance > SMALL
         && finalResidual_ <= RelTolerance*initialResidual_
        )
     )
     &&
     iter >= minIter
    )
    {
        converged_ = true;
    }
    else
    {
        converged_ = false;
    }

    return converged_;
}

bool CML::lduMatrix::solverPerformance::checkConvergence
(
    const scalar Tolerance,
    const label  iter,
    const label  minIter
)
{
    if (debug >= 2)
    {
        Info<< solverName_
            << ":  Iteration " << noIterations_
            << " residual = " << finalResidual_
            << endl;
    }

    if (finalResidual_ < Tolerance && iter >= minIter)
    {
        converged_ = true;
    }
    else
    {
        converged_ = false;
    }

    return converged_;
}

bool CML::lduMatrix::solverPerformance::checkSingularity
(
    const scalar residual
)
{
    if (residual > VSMALL)
    {
        singular_ = false;
    }
    else
    {
        singular_ = true;
    }

    return singular_;
}


void CML::lduMatrix::solverPerformance::print() const
{
    if (debug)
    {
        Info<< solverName_ << ":  Solving for " << fieldName_;

        if (singular())
        {
            Info<< ":  solution singularity" << endl;
        }
        else
        {
            Info<< ", Initial residual = " << initialResidual_
                << ", Final residual = " << finalResidual_
                << ", No Iterations " << noIterations_
                << endl;
        }
    }
}


bool CML::lduMatrix::solverPerformance::operator!=
(
    const lduMatrix::solverPerformance& sp
) const
{
    return
    (
        solverName()      != sp.solverName()
     || fieldName()       != sp.fieldName()
     || initialResidual() != sp.initialResidual()
     || finalResidual()   != sp.finalResidual()
     || nIterations()     != sp.nIterations()
     || converged()       != sp.converged()
     || singular()        != sp.singular()
    );
}


CML::lduMatrix::solverPerformance CML::max
(
    const lduMatrix::solverPerformance& sp1,
    const lduMatrix::solverPerformance& sp2
)
{
    return lduMatrix::solverPerformance
    (
        sp1.solverName(),
        sp1.fieldName_,
        max(sp1.initialResidual(), sp2.initialResidual()),
        max(sp1.finalResidual(), sp2.finalResidual()),
        max(sp1.nIterations(), sp2.nIterations()),
        sp1.converged() && sp2.converged(),
        sp1.singular() || sp2.singular()
    );
}


CML::Istream& CML::operator>>
(
    Istream& is,
    CML::lduMatrix::solverPerformance& sp
)
{
    is.readBeginList("lduMatrix::solverPerformance");
    is  >> sp.solverName_
        >> sp.fieldName_
        >> sp.initialResidual_
        >> sp.finalResidual_
        >> sp.noIterations_
        >> sp.converged_
        >> sp.singular_;
    is.readEndList("lduMatrix::solverPerformance");

    return is;
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CML::lduMatrix::solverPerformance& sp
)
{
    os  << token::BEGIN_LIST
        << sp.solverName_ << token::SPACE
        << sp.fieldName_ << token::SPACE
        << sp.initialResidual_ << token::SPACE
        << sp.finalResidual_ << token::SPACE
        << sp.noIterations_ << token::SPACE
        << sp.converged_ << token::SPACE
        << sp.singular_ << token::SPACE
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
