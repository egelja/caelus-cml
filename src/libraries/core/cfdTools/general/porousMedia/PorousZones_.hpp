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
    CML::PorousZones

Description
    A centralized ZoneType collection.

    Container class for a set of ZoneType with the ZoneType member
    functions implemented to loop over the functions for each ZoneType.

SourceFiles
    PorousZones_.cpp

\*---------------------------------------------------------------------------*/

#ifndef PorousZones__H
#define PorousZones__H

#include "IOPtrList.hpp"

#include "volFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"
#include "dimensionedScalarFwd.hpp"
#include "geometricOneField.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
class fvMesh;


/*---------------------------------------------------------------------------*\
                           Class PorousZones Declaration
\*---------------------------------------------------------------------------*/

template<class ZoneType>
class PorousZones
:
    public IOPtrList<ZoneType>
{
    // Private data

        //- Reference to the finite volume mesh this zone is part of
        const fvMesh& mesh_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        PorousZones(const PorousZones<ZoneType>&);

        //- Disallow default bitwise assignment
        void operator=(const PorousZones<ZoneType>&);


        //- modify time derivative elements
        template<class Type>
        void modifyDdt(fvMatrix<Type>&) const;

public:

    // Constructors

        //- Construct from fvMesh
        //  with automatically constructed coordinate systems list
        PorousZones(const fvMesh&);


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

        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation
        void addResistance(fvVectorMatrix& UEqn) const;

        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation using rho and mu provided
        void addResistance
        (
            fvVectorMatrix& UEqn,
            const volScalarField& rho,
            const volScalarField& mu
        ) const;

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

#include "Time.hpp"
#include "volFields.hpp"
#include "fvm.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ZoneType>
template<class Type>
void CML::PorousZones<ZoneType>::modifyDdt(fvMatrix<Type>& m) const
{
    forAll(*this, i)
    {
        this->operator[](i).modifyDdt(m);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ZoneType>
CML::PorousZones<ZoneType>::PorousZones
(
    const fvMesh& mesh
)
:
    IOPtrList<ZoneType>
    (
        IOobject
        (
            "porousZones",
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        typename ZoneType::iNew(mesh)
    ),
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ZoneType>
template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::PorousZones<ZoneType>::ddt
(
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(vf);
    modifyDdt(tres());
    return tres;
}


template<class ZoneType>
template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::PorousZones<ZoneType>::ddt
(
    const geometricOneField&,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(vf);
    modifyDdt(tres());
    return tres;
}


template<class ZoneType>
template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::PorousZones<ZoneType>::ddt
(
    const dimensionedScalar& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(rho,vf);
    modifyDdt(tres());
    return tres;
}


template<class ZoneType>
template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::PorousZones<ZoneType>::ddt
(
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tres = fvm::ddt(rho,vf);
    modifyDdt(tres());
    return tres;
}

template<class ZoneType>
void CML::PorousZones<ZoneType>::addResistance(fvVectorMatrix& UEqn) const
{
    forAll(*this, i)
    {
        this->operator[](i).addResistance(UEqn);
    }
}


template<class ZoneType>
void CML::PorousZones<ZoneType>::addResistance
(
    fvVectorMatrix& UEqn,
    const volScalarField& rho,
    const volScalarField& mu
) const
{
    forAll(*this, i)
    {
        this->operator[](i).addResistance(UEqn, rho, mu);
    }
}


template<class ZoneType>
void CML::PorousZones<ZoneType>::addResistance
(
    const fvVectorMatrix& UEqn,
    volTensorField& AU
) const
{
    // addResistance for each zone, delaying the correction of the
    // processor BCs of AU
    forAll(*this, i)
    {
        this->operator[](i).addResistance(UEqn, AU, false);
    }

    // Correct the boundary conditions of the tensorial diagonal to ensure
    // processor bounaries are correctly handled when AU^-1 is interpolated
    // for the pressure equation.
    AU.correctBoundaryConditions();
}


template<class ZoneType>
bool CML::PorousZones<ZoneType>::readData(Istream& is)
{
    this->clear();

    IOPtrList<ZoneType> newLst
    (
        IOobject
        (
            "porousZones",
            mesh_.time().constant(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false     // Don't re-register new zones with objectRegistry
        ),
        typename ZoneType::iNew(mesh_)
    );

    this->transfer(newLst);

    return is.good();
}


template<class ZoneType>
bool CML::PorousZones<ZoneType>::writeData(Ostream& os, bool subDict) const
{
    // Write size of list
    os << nl << this->size();

    // Write beginning of contents
    os << nl << token::BEGIN_LIST;

    // Write list contents
    forAll(*this, i)
    {
        os << nl;
        this->operator[](i).writeDict(os, subDict);
    }

    // Write end of contents
    os << token::END_LIST << nl;

    // Check state of IOstream
    return os.good();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
