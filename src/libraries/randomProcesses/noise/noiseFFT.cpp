/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "noiseFFT.hpp"
#include "IFstream.hpp"
#include "DynamicList.hpp"
#include "fft.hpp"
#include "mathematicalConstants.hpp"
#include "SubField.hpp"

// * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * //

CML::scalar CML::noiseFFT::p0 = 2e-5;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from pressure field
CML::noiseFFT::noiseFFT
(
    const scalar deltat,
    const scalarField& pressure
)
:
    scalarField(pressure),
    deltat_(deltat)
{}


// Construct from pressure field file name
CML::noiseFFT::noiseFFT(const fileName& pFileName, const label skip)
:
    scalarField(),
    deltat_(0.0)
{
    // Construct control dictionary
    IFstream pFile(pFileName);

    // Check pFile stream is OK
    if (!pFile.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << pFileName
            << exit(FatalError);
    }

    if (skip)
    {
        scalar dummyt, dummyp;

        for (label i=0; i<skip; i++)
        {
            pFile >> dummyt;

            if (!pFile.good() || pFile.eof())
            {
                FatalErrorInFunction
                    << "Number of points in file " << pFileName
                    << " is less than the number to be skipped = " << skip
                    << exit(FatalError);
            }

            pFile >> dummyp;
        }
    }

    scalar t = 0, T = 0;
    DynamicList<scalar> pData(100000);
    label i = 0;

    while (!(pFile >> t).eof())
    {
        T = t;
        pFile >> pData(i++);
    }

    deltat_ = T/pData.size();

    scalarField::operator=(pData.shrink());
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::graph CML::noiseFFT::pt() const
{
    scalarField t(size());
    forAll(t, i)
    {
        t[i] = i*deltat_;
    }

    return graph
    (
        "p(t)",
        "t [s]",
        "p(t) [Pa]",
        t,
        *this
    );
}


CML::tmp<CML::scalarField> CML::noiseFFT::window
(
    const label N,
    const label ni
) const
{
    label windowOffset = N;

    if ((N + ni*windowOffset) > size())
    {
        FatalErrorInFunction
            << "Requested window is outside set of data" << endl
            << "number of data = " << size() << endl
            << "size of window = " << N << endl
            << "window  = " << ni
            << exit(FatalError);
    }

    tmp<scalarField> tpw(new scalarField(N));
    scalarField& pw = tpw();

    label offset = ni*windowOffset;

    forAll(pw, i)
    {
        pw[i] = operator[](i + offset);
    }

    return tpw;
}


CML::tmp<CML::scalarField> CML::noiseFFT::Hanning(const label N) const
{
    scalarField t(N);
    forAll(t, i)
    {
        t[i] = i*deltat_;
    }

    scalar T = N*deltat_;

    return 2*(0.5 - 0.5*cos(constant::mathematical::twoPi*t/T));
}


CML::tmp<CML::scalarField> CML::noiseFFT::Pf
(
    const tmp<scalarField>& tpn
) const
{
    tmp<scalarField> tPn2
    (
        mag
        (
            fft::reverseTransform
            (
                ReComplexField(tpn),
                labelList(1, tpn().size())
            )
        )
    );

    tpn.clear();

    tmp<scalarField> tPn
    (
        new scalarField
        (
            scalarField::subField(tPn2(), tPn2().size()/2)
        )
    );
    scalarField& Pn = tPn();

    Pn *= 2.0/sqrt(scalar(tPn2().size()));
    Pn[0] /= 2.0;

    return tPn;
}


CML::graph CML::noiseFFT::meanPf
(
    const label N,
    const label nw
) const
{
    if (N > size())
    {
        FatalErrorInFunction
            << "Requested window is outside set of data" << endl
            << "number of data = " << size() << endl
            << "size of window = " << N << endl
            << "window  = " << nw
            << exit(FatalError);
    }

    scalarField MeanPf(N/2, 0.0);

    scalarField Hwf(Hanning(N));

    for (label wi=0; wi<nw; ++wi)
    {
        MeanPf += Pf(Hwf*window(N, wi));
    }

    MeanPf /= nw;

    scalarField f(MeanPf.size());
    scalar deltaf = 1.0/(N*deltat_);

    forAll(f, i)
    {
        f[i] = i*deltaf;
    }

    return graph
    (
        "P(f)",
        "f [Hz]",
        "P(f) [Pa]",
        f,
        MeanPf
    );
}


CML::graph CML::noiseFFT::RMSmeanPf
(
    const label N,
    const label nw
) const
{
    if (N > size())
    {
        FatalErrorInFunction
            << "Requested window is outside set of data" << endl
            << "number of data = " << size() << endl
            << "size of window = " << N << endl
            << "window  = " << nw
            << exit(FatalError);
    }

    scalarField RMSMeanPf(N/2, 0.0);

    scalarField Hwf(Hanning(N));

    for (label wi=0; wi<nw; ++wi)
    {
        RMSMeanPf += sqr(Pf(Hwf*window(N, wi)));
    }

    RMSMeanPf = sqrt(RMSMeanPf/nw);

    scalarField f(RMSMeanPf.size());
    scalar deltaf = 1.0/(N*deltat_);

    forAll(f, i)
    {
        f[i] = i*deltaf;
    }

    return graph
    (
        "P(f)",
        "f [Hz]",
        "P(f) [Pa]",
        f,
        RMSMeanPf
    );
}


CML::graph CML::noiseFFT::Lf(const graph& gPf) const
{
    return graph
    (
        "L(f)",
        "f [Hz]",
        "L(f) [dB]",
        gPf.x(),
        20*log10(gPf.y()/p0)
    );
}


CML::graph CML::noiseFFT::Ldelta
(
    const graph& gLf,
    const scalar f1,
    const scalar fU
) const
{
    const scalarField& f = gLf.x();
    const scalarField& Lf = gLf.y();

    scalarField ldelta(Lf.size(), 0.0);
    scalarField fm(ldelta.size());

    scalar fratio = cbrt(2.0);
    scalar deltaf = 1.0/(2*Lf.size()*deltat_);

    scalar fl = f1/sqrt(fratio);
    scalar fu = fratio*fl;

    label istart = label(fl/deltaf);
    label j = 0;

    for (label i = istart; i<Lf.size(); i++)
    {
        scalar fmi = sqrt(fu*fl);

        if (fmi > fU + 1) break;

        if (f[i] >= fu)
        {
            fm[j] = fmi;
            ldelta[j] = 10*log10(ldelta[j]);

            j++;

            fl = fu;
            fu *= fratio;
        }

        ldelta[j] += pow(10, Lf[i]/10.0);
    }

    fm.setSize(j);
    ldelta.setSize(j);

    return graph
    (
        "Ldelta",
        "fm [Hz]",
        "Ldelta [dB]",
        fm,
        ldelta
    );
}


CML::graph CML::noiseFFT::Pdelta
(
    const graph& gPf,
    const scalar f1,
    const scalar fU
) const
{
    const scalarField& f = gPf.x();
    const scalarField& Pf = gPf.y();

    scalarField pdelta(Pf.size(), 0.0);
    scalarField fm(pdelta.size());

    scalar fratio = cbrt(2.0);
    scalar deltaf = 1.0/(2*Pf.size()*deltat_);

    scalar fl = f1/sqrt(fratio);
    scalar fu = fratio*fl;

    label istart = label(fl/deltaf + 1.0 - SMALL);
    label j = 0;

    for (label i = istart; i<Pf.size(); i++)
    {
        scalar fmi = sqrt(fu*fl);

        if (fmi > fU + 1) break;

        if (f[i] >= fu)
        {
            fm[j] = fmi;
            pdelta[j] = sqrt((2.0/3.0)*pdelta[j]);

            j++;

            fl = fu;
            fu *= fratio;
        }

        pdelta[j] += sqr(Pf[i]);
    }

    fm.setSize(j);
    pdelta.setSize(j);

    return graph
    (
        "Pdelta",
        "fm [Hz]",
        "Pdelta [dB]",
        fm,
        pdelta
    );
}


CML::scalar CML::noiseFFT::Lsum(const graph& gLf) const
{
    const scalarField& Lf = gLf.y();

    scalar lsum = 0.0;

    forAll(Lf, i)
    {
        lsum += pow(10, Lf[i]/10.0);
    }

    lsum = 10*log10(lsum);

    return lsum;
}


CML::scalar CML::noiseFFT::dbToPa(const scalar db) const
{
    return p0*pow(10.0, db/20.0);
}


CML::tmp<CML::scalarField> CML::noiseFFT::dbToPa
(
    const tmp<scalarField>& db
) const
{
    return p0*pow(10.0, db/20.0);
}


// ************************************************************************* //
