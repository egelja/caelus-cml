/*---------------------------------------------------------------------------*\
Copyright  Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

\*---------------------------------------------------------------------------*/

#include "cnoidalFirstProperties.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "gsl_sf_ellint.h"
#include "gsl_vector.h"
#include "gsl_matrix.h"
#include "gsl_roots.h"
#include "gsl_multiroots.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(cnoidalFirstProperties, 0);
addToRunTimeSelectionTable
(
    setWaveProperties,
    cnoidalFirstProperties,
    setWaveProperties
);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //


double lowerMBound_f (double m, void *params)
{
    // Solves to find the value of m at which CML::sqrt(1.0 + H/d*A) == 0.0
    struct cnoidalFirstParams *p = (struct cnoidalFirstParams *) params;

    double d = p->depth_;
    double H = p->height_;

    double K = gsl_sf_ellint_Kcomp(CML::sqrt(m), GSL_PREC_DOUBLE);
    double E = gsl_sf_ellint_Ecomp(CML::sqrt(m), GSL_PREC_DOUBLE);

    double A = 2.0/m - 1.0 - 3.0/m*E/K;

    // The value of 1.0e-8 is added to ensure strictly larger than 0!
    return 1.0 - 1.0e-8 + H/d*A;
}


double cnoidalFirst_f (double m, void *params)
{
    struct cnoidalFirstParams *p = (struct cnoidalFirstParams *) params;

    double T = p->period_;
    double d = p->depth_;
    double G = p->g_;
    double H = p->height_;

    double K = gsl_sf_ellint_Kcomp(CML::sqrt(m), GSL_PREC_DOUBLE);
    double E = gsl_sf_ellint_Ecomp(CML::sqrt(m), GSL_PREC_DOUBLE);

    double A = 2.0/m - 1.0 - 3.0/m*E/K;

    return T*CML::sqrt(G/d)*CML::sqrt(1.0 + H/d*A)
        - CML::sqrt(16.0*d/(3.0*H)*m * CML::pow(K, 2.0) );
}


double cnoidalFirstProperties::solve()
{

    int status, maxIter = 1000;
    scalar eps = 1.0e-10, m, mLower = 1.0e-15, mUpper = 1.0 - 1.0e-15;

    const gsl_root_fsolver_type *T;

    gsl_root_fsolver *s;

    gsl_function FlowerBound, F ;

    struct cnoidalFirstParams params = {d_ , H_ , T_ , G_ };

    FlowerBound.function = & lowerMBound_f;
    FlowerBound.params   = & params;

    T = gsl_root_fsolver_bisection;
    s = gsl_root_fsolver_alloc(T);

    gsl_root_fsolver_set(s, &FlowerBound, mLower, mUpper);

    for (int i = 0; i < maxIter; i++)
    {
        gsl_root_fsolver_iterate(s);
        m = gsl_root_fsolver_root(s);

        status = gsl_root_test_residual(lowerMBound_f(m, &params), eps);

        if (status == 0)
        {
            break;
        }
    }

    mLower = m;

    while (true)
    {
        if
        (
            ( cnoidalFirst_f(mLower, &params) < 0.0 &&
              cnoidalFirst_f(mUpper, &params) < 0.0 )
            ||
            ( cnoidalFirst_f(mLower, &params) > 0.0 &&
              cnoidalFirst_f(mUpper, &params)    > 0.0 )
        )
        {
            mLower = 0.999*mLower + 0.001*mUpper;
        }
        else
        {
            break;
        }

        if (CML::mag(mLower - mUpper) < 10e-8)
        {
            return -1;
        }
    }

    F.function = &cnoidalFirst_f;
    F.params   = &params;

    gsl_root_fsolver_set(s, &F, mLower, mUpper);

    for (int i = 0; i < maxIter; i++)
    {
        gsl_root_fsolver_iterate(s);
        m = gsl_root_fsolver_root(s);

        status = gsl_root_test_residual(cnoidalFirst_f(m, &params), eps);

        if (status == 0)
        {
            break;
        }
    }

    Info << m << endl;


    return m;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


cnoidalFirstProperties::cnoidalFirstProperties
(
    const Time& rT,
    dictionary& dict,
    bool write
)
:
    setWaveProperties(rT, dict, write),
    T_(readScalar( dict.lookup("period"))),
    d_(readScalar( dict.lookup("depth"))),
    H_(readScalar( dict.lookup("height")))
{
    Info << "\nConstructing: " << this->type() << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void cnoidalFirstProperties::set(Ostream& os)
{
    scalar m = solve();

    // Write the beginning of the sub-dictionary
    writeBeginning(os);

    // Write the already given parameters
    writeGiven(os, "waveType");

    if (dict_.found("Tsoft"))
    {
        writeGiven(os, "Tsoft");
    }

    writeGiven(os, "depth");
    writeGiven(os, "period");
    writeGiven(os, "height");

    if (m < 0.0)
    {
        Info << "\nPARAMETERS NOT SET\nNo cnoidal wave solution"
             << " exists for given input\n" << endl;
    }
    else
    {
        double K = gsl_sf_ellint_Kcomp(CML::sqrt(m), GSL_PREC_DOUBLE);
        double E = gsl_sf_ellint_Ecomp(CML::sqrt(m), GSL_PREC_DOUBLE);

        double A = 2.0/m - 1.0 - 3.0/m*E/K;

        double L =
            CML::sqrt(16.0*m * CML::pow(K, 2.0)*CML::pow(d_, 3.0)/(3.0*H_));
        double c = CML::sqrt(G_*d_*(1 + A*H_/d_));
        double omega = 2*PI_/T_;

        if (write_)
        {
            writeDerived(os, "omega", omega);
            writeDerived(os, "length", L);
            writeDerived(os, "celerity", c);

            // Locally change the write precision for m to avoid it being
            // written as 1 instead of 0.9999999999 which makes elliptic
            // integrals to infinity.
            unsigned int pre = os.precision(14);
            writeDerived(os, "m", m);
            os.precision( pre );
        }
    }

    writeGiven(os, "direction");

    // Write the relaxation zone
    writeRelaxationZone(os);

    // Write the closing bracket
    writeEnding(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
