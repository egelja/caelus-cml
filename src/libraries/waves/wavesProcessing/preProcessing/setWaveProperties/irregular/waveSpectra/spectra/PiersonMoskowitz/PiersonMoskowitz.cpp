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

#include "PiersonMoskowitz.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(PiersonMoskowitz, 0);
addToRunTimeSelectionTable(waveSpectra, PiersonMoskowitz, waveSpectra);

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

scalarField PiersonMoskowitz::spectralValue
(
    const scalar& Hs,
    const scalar& Tp,
    const scalarField& freq
) const
{
    scalar fp = 1.0/Tp;

    // Compute spectrum
    scalarField S(5.0/16.0*CML::pow(Hs,2.0)*CML::pow(fp,4.0)
        *CML::pow(freq,-5.0)*CML::exp(- 5.0/4.0*CML::pow(fp/freq, 4.0)));

    return S;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


PiersonMoskowitz::PiersonMoskowitz
(
    const Time& rT,
    dictionary& dict,
    scalarField& amp,
    scalarField& freq,
    scalarField& phi,
    vectorField& k
)
:
    waveSpectra(rT, dict, amp, freq, phi, k)
{
    Info << "\nConstructing: " << this->type() << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


wordList PiersonMoskowitz::list()
{
    wordList res(4);

    res[0] = "Hs";
    res[1] = "Tp";
    res[2] = "depth";
    res[3] = "direction";

    return res;
}


void PiersonMoskowitz::set(Ostream& os)
{
    // Get the input parameters
    scalar Hs(readScalar(dict_.lookup("Hs")));

    scalar Tp(readScalar(dict_.lookup("Tp")));

    scalar depth(readScalar(dict_.lookup("depth")));

    vector direction(vector(dict_.lookup("direction")));

    label N = readLabel(dict_.lookup("N"));

    // Calculate the frequency axis
    autoPtr<CML::frequencyAxis> fA = CML::frequencyAxis::New(rT_, dict_);
    scalarField nodeFrequency(N + 1, 0);

    // An intermediate step needed for certain discretisation types
    // Placed in scopes such that the temporary variables to not 'survive'
    {
        equidistantFrequencyAxis equiFA(rT_, dict_);

        scalarField tempFreqAxis = equiFA.freqAxis(10000);
        scalarField tempSpectrum
            = this->spectralValue(Hs, Tp, tempFreqAxis);

        nodeFrequency = fA->freqAxis(tempFreqAxis, tempSpectrum, N);
    }

    // Prepare variables
    freq_.setSize(N);
    amp_.setSize(N);
    phi_.setSize(N);
    k_.setSize(N);

    // Compute spectrum
    scalarField S = this->spectralValue(Hs, Tp, nodeFrequency);

    // Prepare stokesFirst to compute wave numbers
    CML::stokesFirstProperties stp( rT_, dict_ );

    // Compute return variables
    for (int i = 1; i < N + 1; i++)
    {
        // The frequency is the mid-point between two nodes
        freq_[i - 1] = 0.5*(nodeFrequency[i - 1] + nodeFrequency[i]);

        // Amplitude is the square root of the trapezoidal integral
        amp_[i - 1] =
            CML::sqrt
            (
                (S[i-1] + S[i])
               *(nodeFrequency[i] - nodeFrequency[i - 1])
            );

        // Wave number based on linear wave theory
        k_[i - 1] = direction*stp.linearWaveNumber(depth, freq_[i-1]);

        // The phase is computed based on the phase-function
        phi_[i - 1] = phases_->phase(freq_[i - 1], k_[i - 1]);
    }

    writeSpectrum(os, nodeFrequency, S);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
