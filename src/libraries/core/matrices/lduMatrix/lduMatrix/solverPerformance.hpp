/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
Copyright (C) 2018 Applied CCM
Copyright (C) 2016 OpenCFD Ltd.
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
    CML::solverPerformance

Description
    solverPerformance is the class returned by the LduMatrix solver
    containing performance statistics.

SourceFiles
    solverPerformance.cpp

\*---------------------------------------------------------------------------*/

#ifndef solverPerformance_H
#define solverPerformance_H

#include "solverPerformance.hpp"
#include "IOstreams.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class solverPerformance;

solverPerformance max
(
    const solverPerformance&,
    const solverPerformance&
);

Istream& operator>>
(
    Istream&,
    solverPerformance&
);

Ostream& operator<<
(
    Ostream&,
    const solverPerformance&
);


/*---------------------------------------------------------------------------*\
                       Class solverPerformance Declaration
\*---------------------------------------------------------------------------*/

class solverPerformance
{
    word   solverName_;
    word   fieldName_;
    scalar initialResidual_;
    scalar finalResidual_;
    label  noIterations_;
    bool   converged_;
    bool   singular_;


public:

    // Static data

    // Declare name of the class and its debug switch
    ClassName("solverPerformance");

    //- Construct null
    solverPerformance()
    :
        initialResidual_(0),
        finalResidual_(0),
        noIterations_(0),
        converged_(false),
        singular_(false)
    {}

    //- Construct from components
    solverPerformance
    (
        const word&  solverName,
        const word&  fieldName,
        const scalar iRes = 0,
        const scalar fRes = 0,
        const label  nIter = 0,
        const bool   converged = false,
        const bool   singular = false
    )
    :
        solverName_(solverName),
        fieldName_(fieldName),
        initialResidual_(iRes),
        finalResidual_(fRes),
        noIterations_(nIter),
        converged_(converged),
        singular_(singular)
    {}

    //- Construct from Istream
    solverPerformance(Istream&);


    // Member functions

        //- Return solver name
        const word& solverName() const
        {
            return solverName_;
        }

        //- Return solver name
        word& solverName()
        {
            return solverName_;
        }

        //- Return field name
        const word& fieldName() const
        {
            return fieldName_;
        }


        //- Return initial residual
        scalar initialResidual() const
        {
            return initialResidual_;
        }

        //- Return initial residual
        scalar& initialResidual()
        {
            return initialResidual_;
        }


        //- Return final residual
        scalar finalResidual() const
        {
            return finalResidual_;
        }

        //- Return final residual
        scalar& finalResidual()
        {
            return finalResidual_;
        }


        //- Return number of iterations
        label nIterations() const
        {
            return noIterations_;
        }

        //- Return number of iterations
        label& nIterations()
        {
            return noIterations_;
        }


        //- Has the solver converged?
        bool converged() const
        {
            return converged_;
        }

        //- Is the matrix singular?
        bool singular() const
        {
           return singular_;
        }

        //- Convergence test
        bool checkConvergence
        (
            const scalar tolerance,
            const scalar relTolerance,
            const label  iter,
            const label  minIter
        );

        bool checkConvergence
        (
            const scalar tolerance,
            const label  iter,
            const label  minIter
        );

        //- Singularity test
        bool checkSingularity(const scalar residual);

        //- Print summary of solver performance
        void print() const;

    // Member Operators

        bool operator!=(const solverPerformance&) const;


    // Friend functions

        //- Return the element-wise maximum of two solverPerformances
        friend solverPerformance max
        (
            const solverPerformance&,
            const solverPerformance&
        );


    // Ostream Operator

        friend Istream& operator>>
        (
            Istream&,
            solverPerformance&
        );

        friend Ostream& operator<<
        (
            Ostream&,
            const solverPerformance&
        );
};


} // End namespace CML

#endif
