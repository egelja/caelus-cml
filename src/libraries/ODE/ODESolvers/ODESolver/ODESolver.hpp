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
    CML::ODESolver

Description
    Abstract base-class for ODE system solvers

SourceFiles
    ODESolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef ODESolver_HPP
#define ODESolver_HPP

#include "ODESystem.hpp"
#include "typeInfo.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class ODESolver Declaration
\*---------------------------------------------------------------------------*/

class ODESolver
{

protected:

    //- Reference to ODESystem
    const ODESystem& odes_;

    //- Maximum size of the ODESystem
    const label maxN_;

    //- Size of the ODESystem (adjustable)
    mutable label n_;

    //- Absolute convergence tolerance per step
    scalarField absTol_;

    //- Relative convergence tolerance per step
    scalarField relTol_;

    //- The maximum number of sub-steps allowed for the integration step
    label maxSteps_;


    // Protected Member Functions

    //- Return the nomalized scalar error
    scalar normalizeError
    (
        const scalarField& y0,
        const scalarField& y,
        const scalarField& err
    ) const;

    //- Disallow default bitwise copy construct
    ODESolver(const ODESolver&);

    //- Disallow default bitwise assignment
    void operator=(const ODESolver&);


public:

    friend class ODESystem;

    //- Runtime type information
    TypeName("ODESolver");

    class stepState
    {
        public:

        const bool forward;
        scalar dxTry;
        scalar dxDid;
        bool first;
        bool last;
        bool reject;
        bool prevReject;

        stepState(const scalar dx)
        :
            forward(dx > 0 ? true : false),
            dxTry(dx),
            dxDid(0),
            first(true),
            last(false),
            reject(false),
            prevReject(false)
        {}
    };


    // Declare run-time constructor selection table

    declareRunTimeSelectionTable
    (
        autoPtr,
        ODESolver,
        dictionary,
        (const ODESystem& ode, const dictionary& dict),
        (ode, dict)
    );


    // Constructors

    //- Construct for given ODESystem
    ODESolver(const ODESystem& ode, const dictionary& dict);

    //- Construct for given ODESystem specifying tolerances
    ODESolver
    (
        const ODESystem& ode,
        const scalarField& absTol,
        const scalarField& relTol
    );


    // Selectors

    //- Select null constructed
    static autoPtr<ODESolver> New
    (
        const ODESystem& ode,
        const dictionary& dict
    );


    //- Destructor
    virtual ~ODESolver()
    {}


    // Member Functions

    //- Return the number of equations to solve
    inline label nEqns() const
    {
        return n_;
    }

    //- Return access to the absolute tolerance field
    inline scalarField& absTol()
    {
        return absTol_;
    }

    //- Return access to the relative tolerance field
    inline scalarField& relTol()
    {
        return relTol_;
    }

    //- Resize the ODE solver
    virtual bool resize() = 0;

    template<class Type>
    static inline void resizeField(UList<Type>& f, const label n);

    template<class Type>
    inline void resizeField(UList<Type>& f) const;

    inline void resizeMatrix(scalarSquareMatrix& m) const
    {
        m.shallowResize(n_);
    }

    //- Solve the ODE system as far as possible up to dxTry
    //  adjusting the step as necessary to provide a solution within
    //  the specified tolerance.
    //  Update the state and return an estimate for the next step in dxTry
    virtual void solve
    (
        scalar& x,
        scalarField& y,
        scalar& dxTry
    ) const;

    //- Solve the ODE system as far as possible up to dxTry
    //  adjusting the step as necessary to provide a solution within
    //  the specified tolerance.
    //  Update the state and return an estimate for the next step in dxTry
    virtual void solve
    (
        scalar& x,
        scalarField& y,
        stepState& step
    ) const;

    //- Solve the ODE system from xStart to xEnd, update the state
    //  and return an estimate for the next step in dxTry
    virtual void solve
    (
        const scalar xStart,
        const scalar xEnd,
        scalarField& y,
        scalar& dxEst
    ) const;
};


} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline void CML::ODESolver::resizeField(UList<Type>& f, const label n)
{
    f = UList<Type>(f.begin(), n);
}


template<class Type>
inline void CML::ODESolver::resizeField(UList<Type>& f) const
{
    resizeField(f, n_);
}


#endif
