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
    CML::setWaveProperties

Description
    An abstract base class for a pre-processing tool for setting the wave
    properties in the dictionary waveProperties.

    A description of the general wave framework is given in

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    setWaveProperties.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef setWaveProperties_HPP
#define setWaveProperties_HPP

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "OFstream.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "uniformDimensionedFields.hpp"

#include "gsl_vector.h"
#include "gsl_matrix.h"
#include "gsl_roots.h"
#include "gsl_multiroots.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class setWaveProperties Declaration
\*---------------------------------------------------------------------------*/

class setWaveProperties
{

protected:

    // Protected data
        bool write_;
        dictionary& dict_;
        vector g_;
        scalar G_;

        scalar PI_;

        label Nspaces_;

    // Protected member functions
        void writeBeginning(Ostream&);

        void writeGiven(Ostream&);

        void writeGiven(Ostream& , word);

        void writeDerived(Ostream&, word, scalar);

        void writeDerived(Ostream&, word, vector);

        void writeDerived(Ostream&, word, scalarField);

        void writeDerived(Ostream&, word, vectorField);

        void writeRelaxationZone(Ostream&);

        void writeEnding(Ostream&);

        void addITstream(Ostream&, const word&, const ITstream&);

private:

    // Private Member Functions
        void lineFormatting(Ostream&, const word&);

        //- Disallow default bitwise copy construct
        setWaveProperties(const setWaveProperties&);

        //- Disallow default bitwise assignment
        void operator=(const setWaveProperties&);

public:

    //- Runtime type information
    TypeName("setWaveProperties");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            setWaveProperties,
            setWaveProperties,
            (
                const Time& rT, dictionary& dict, bool write
            ),
            (rT, dict, write)
        );

    // Constructors

        //- Construct from components
        setWaveProperties
        (
            const Time&,
            dictionary&,
            bool
        );

    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<setWaveProperties> New
        (
            const Time&,
            dictionary&,
            bool
        );

    // Destructor

        virtual ~setWaveProperties();

    // Member Functions

        virtual void set(Ostream&) = 0;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
