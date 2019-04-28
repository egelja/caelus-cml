/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
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

#include "powerSpectraLS.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(powerSpectraLS, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    powerSpectraLS,
    postProcessingWaves
);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


void powerSpectraLS::evaluateScalar()
{
    Info << "        - Power spectra computed for scalar quantities" << endl;

    spectralMethodsLeastSquaresBased smls(rT_, actionProperties_);

    List<scalarField> input = readScalarFields(indices_);

    scalarField time = readIOScalarField(callName_ + "_time");

    List<scalarField> spectra =
        smls.powerSpectra(time, input, N_, 1.0 /period_);

    scalarField frequencies = smls.frequencies(N_);

    writeScalar(frequencies, spectra);
}


void powerSpectraLS::writeScalar
(
    const scalarField& frequencies,
    const List<scalarField>& spectra
)
{
    Info << "        - Writing computed spectra to: " << directDir_.c_str()
         << this->type() << endl;

    mkDir(directDir_ + this->type());

    autoPtr<OFstream> spectrumPtr_;

    forAll (indices_, indexi)
    {
        std::stringstream ss;
        ss << callName_ << "_" << indices_[indexi];

        spectrumPtr_.reset
        (
            new OFstream
            (
                directDir_ + "/" + this->type() + "/"
              + ss.str() + "_spectrum.dat"
            )
        );

        const scalarField& spectrum( spectra[indexi] );

        forAll (frequencies, freqi)
        {
            spectrumPtr_() << frequencies[freqi] << tab << spectrum[freqi]
                           << endl;
        }
    }
}


void powerSpectraLS::evaluateVector()
{
    Info << "        - Power spectra computed for vector quantities" << endl;

    spectralMethodsLeastSquaresBased smls(rT_, actionProperties_);

    List<vectorField> input = readVectorFields(indices_);

    scalarField time = readIOScalarField(callName_ + "_time");

    List<vectorField> spectra =
        smls.powerSpectra(time, input, N_, 1.0/period_);

    scalarField frequencies = smls.frequencies(N_);

    writeVector(frequencies, spectra);
}


void powerSpectraLS::writeVector
(
    const scalarField& frequencies,
    const List<vectorField>& spectra
)
{
    Info << "        - Writing computed spectra to: " << directDir_.c_str()
         << this->type() << endl;

    mkDir(directDir_ + this->type());

    autoPtr<OFstream> spectrumPtr_;

    forAll (indices_, indexi)
    {
        std::stringstream ss;
        ss << callName_ << "_" << indices_[indexi];

        spectrumPtr_.reset
        (
            new OFstream
            (
                directDir_ + "/" + this->type() + "/"
              + ss.str() + "_spectrum.dat"
            )
        );

        const vectorField& spectrum(spectra[indexi]);

        forAll (frequencies, freqi)
        {
            spectrumPtr_() << frequencies[freqi]  << tab
                           << spectrum[freqi].x() << tab
                           << spectrum[freqi].y() << tab
                           << spectrum[freqi].z() << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


powerSpectraLS::powerSpectraLS
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action),

#   include "../../../dataDict.hpp"
    ,

    N_(readLabel(actionProperties_.lookup("nFreq"))),

    period_(readScalar(actionProperties_.lookup("period")))
{
    readIndices(dataDict_, indices_);
}


powerSpectraLS::~powerSpectraLS()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void powerSpectraLS::evaluate()
{
    if (dataType() == "scalar")
    {
        evaluateScalar();
    }
    else if (dataType() == "vector")
    {
        evaluateVector();
    }
    else
    {
        NotImplemented;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
