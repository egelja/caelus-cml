/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen, Deltares.
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
    CML::jjc2014Zones

Description
    A simplified version of the original OF porosity module, now added the
    functionality of runTime selection of the type of resistance coefficients.

SourceFiles
    jjc2014Zones.cpp

\*---------------------------------------------------------------------------*/

#ifndef jjc2014Zones_HPP
#define jjc2014Zones_HPP

#include "jjc2014Zone.hpp"
#include "IOPtrList.hpp"

#include "fvCFD.hpp"
#include "volFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "geometricOneField.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class jjc2014Zones Declaration
\*---------------------------------------------------------------------------*/

class jjc2014Zones
:
    public IOPtrList<jjc2014Zone>
{
    // Private data

        //- Reference to the finite volume mesh this zone is part of
        const fvMesh& mesh_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        jjc2014Zones(const jjc2014Zones&);

        //- Disallow default bitwise assignment
        void operator=(const jjc2014Zones&);


        //- modify time derivative elements
        template<class Type>
        void modifyDdt(fvMatrix<Type>&) const;

public:

    // Constructors

        //- Construct from fvMesh
        //  with automatically constructed coordinate systems list
        jjc2014Zones(const fvMesh&);


    // Member Functions

        //- mirror fvm::ddt with porosity
        template<class Type>
        tmp<fvMatrix<Type> > ddt
        (
            GeometricField<Type, fvPatchField, volMesh>&
        );

        //- mirror fvm::ddt with porosity
        template<class Type>
        tmp<fvMatrix<Type> > ddt
        (
            const geometricOneField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        //- mirror fvm::ddt with porosity
        template<class Type>
        tmp<fvMatrix<Type> > ddt
        (
            const dimensionedScalar&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        //- mirror fvm::ddt with porosity
        template<class Type>
        tmp<fvMatrix<Type> > ddt
        (
            const volScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        //- Return a volScalarField with the porosities
        tmp<volScalarField> porosity() const;

        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation
        void addResistance(fvVectorMatrix& UEqn) const;

        //- Add the viscous and inertial resistance force contribution
        //  to the tensorial diagonal
        void addResistance
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU
        ) const;

        //- read modified data
        virtual bool readData(Istream&);

        //- write data
        bool writeData(Ostream&, bool subDict = true) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "fvMatrix.hpp"
#include "fvm.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::jjc2014Zones::modifyDdt(fvMatrix<Type>& m) const
{
    forAll(*this, i)
    {
        operator[](i).modifyDdt(m);
    }
}


// * * * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::jjc2014Zones::ddt
(
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(vf);
    modifyDdt(tres());
    
    return tres;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::jjc2014Zones::ddt
(
    const geometricOneField&,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(vf);
    modifyDdt(tres());

    return tres;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::jjc2014Zones::ddt
(
    const dimensionedScalar& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(rho,vf);
    modifyDdt(tres());

    return tres;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::jjc2014Zones::ddt
(
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(rho,vf);
    modifyDdt(tres());

    return tres;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
