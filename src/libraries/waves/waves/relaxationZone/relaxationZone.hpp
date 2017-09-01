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

  Class
    CML::relaxationZone

  Description
    Base class for relaxationSchemes.
    (This is the class formerly known as "waveRelaxationOnEcstacy".)

  SourceFiles
      relaxationZone.cpp

  Author
  Niels Gjoel Jacobsen and Bo Terp Paulsen, The Technical University of Denmark

\*---------------------------------------------------------------------------*/

#ifndef relaxationZone_HPP
#define relaxationZone_HPP

#include "relaxationScheme.hpp"
#include "IOdictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                           Class relaxationZone Declaration
\*---------------------------------------------------------------------------*/

class relaxationZone
{
private:

    const fvMesh& mesh_;
    volVectorField& U_;
    volScalarField& alpha_;

    wordList relaxNames_;

    List<autoPtr<relaxationSchemes::relaxationScheme> > relaxSchemePtr_;

    void resetTargetFields();

    void correctBoundaries();

protected:

    mutable volScalarField* targetAlpha_;

    mutable volVectorField* targetVelocity_;

public:
     //- Runtime type information
    TypeName("relaxationZone");

    // Constructors

        //- Construct from components
        relaxationZone
        (
            const fvMesh&,
            volVectorField&,
            volScalarField&
        );

    // Destructor

        virtual ~relaxationZone()
        {}

    // Member Functions

        virtual void correct();

        virtual void addSup(fvMatrix<vector>& eqn);

        virtual void constrain(fvMatrix<vector>& eqn);

        tmp<volScalarField> numericalBeach();

    // Access Functions

        const volScalarField& targetAlphaField();

        const volVectorField& targetVelocityField();
};


} // End namespace CML

#endif
