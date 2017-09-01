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
    CML::wavesPorosityModel

Description


SourceFiles
    wavesPorosityModel.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef wavesPorosityModel_HPP
#define wavesPorosityModel_HPP

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "uniformDimensionedFields.hpp"

#include "fvCFD.hpp"
#include "volFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "geometricOneField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class wavesPorosityModel Declaration
\*---------------------------------------------------------------------------*/

class wavesPorosityModel
{

protected:

    // Protected data
        mutable volScalarField porosity_;

    // Protected member functions


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        wavesPorosityModel(const wavesPorosityModel&);

        //- Disallow default bitwise assignment
        void operator=(const wavesPorosityModel&);

public:

    //- Runtime type information
    TypeName("wavesPorosityModel");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            wavesPorosityModel,
            wavesPorosityModel,
            (
                const fvMesh& mesh
            ),
            (mesh)
        );

    // Constructors

        //- Construct from components
        wavesPorosityModel
        (
            const fvMesh&
        );

    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<wavesPorosityModel> New
        (
            const fvMesh&
        );

    // Destructor

        virtual ~wavesPorosityModel();

    // Member Functions
        //- The templated part had to be skipped, because it should also be a
        //  virtual function. I could not get this part working.
        //  NGJ, 29.10.2014

        //- mirror fvm::ddt with porosity
        virtual tmp<fvMatrix<vector> > ddt
        (
            GeometricField<vector, fvPatchField, volMesh>&
        ) = 0;

        //- mirror fvm::ddt with porosity
        virtual tmp<fvMatrix<vector> > ddt
        (
            const geometricOneField&,
            GeometricField<vector, fvPatchField, volMesh>&
        ) = 0;

        //- mirror fvm::ddt with porosity
        virtual tmp<fvMatrix<vector> > ddt
        (
            const dimensionedScalar&,
            GeometricField<vector, fvPatchField, volMesh>&
        ) = 0;

        //- mirror fvm::ddt with porosity
        virtual tmp<fvMatrix<vector> > ddt
        (
            const volScalarField&,
            GeometricField<vector, fvPatchField, volMesh>&
        ) = 0;

        //- Update the porosity field
        virtual void updatePorosity() = 0;

        //- Return a volScalarField with the porosities
        virtual const volScalarField& porosity() const = 0;

        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation
        virtual void addResistance(fvVectorMatrix& UEqn) const = 0;

        //- Add the viscous and inertial resistance force contribution
        //  to the tensorial diagonal
        virtual void addResistance
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU
        ) const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
