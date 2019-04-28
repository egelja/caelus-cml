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

#include "waveSpectra.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(waveSpectra, 0);
defineRunTimeSelectionTable(waveSpectra, waveSpectra);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


waveSpectra::waveSpectra
(
    const Time& rT,
    dictionary& dict,
    scalarField& amp,
    scalarField& freq,
    scalarField& phi,
    vectorField& k
)
:
    rT_(rT),
    dict_(dict),
    amp_(amp),
    freq_(freq),
    phi_(phi),
    k_(k),

    G_
    (
        CML::mag
        (
            uniformDimensionedVectorField
            (
                rT_.db().lookupObject<uniformDimensionedVectorField>("g")
            ).value()
        )
    ),

    PI_(M_PI),

    phases_(CML::phases::New(rT_, dict_))
{
}


waveSpectra::~waveSpectra()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void waveSpectra::writeSpectrum
(
    Ostream& os,
    const scalarField& freq,
    const scalarField& S
) const
{
    if (dict_.subDict("frequencyAxis").lookupOrDefault<Switch>("writeSpectrum",false))
    {
        S.writeEntry("spectramValues", os);
        os << nl;

        freq.writeEntry("fspectrum", os);
        os << nl;
    }
}


autoPtr<waveSpectra> waveSpectra::New
(
    const Time& rT,
    dictionary& dict,
    scalarField& amp,
    scalarField& freq,
    scalarField& phi,
    vectorField& k
)
{
    word spectrumName;
    dict.lookup("spectrum") >> spectrumName;

    waveSpectraConstructorTable::iterator cstrIter =
            waveSpectraConstructorTablePtr_->find(spectrumName);

    if (cstrIter == waveSpectraConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown wave spectrum '" << spectrumName << "'"
            << endl << endl
            << "Valid wave spectra are:" << endl
            << waveSpectraConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<waveSpectra>(cstrIter()(rT, dict, amp, freq, phi, k));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
