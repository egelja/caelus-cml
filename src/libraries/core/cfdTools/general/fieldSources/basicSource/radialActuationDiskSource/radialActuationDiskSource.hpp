/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    CML::radialActuationDiskSource

Description
    Actuation disk zone definition.
    Constant values for momentum source for actuation disk

        T = 2*rho*A*sqr(Uo)*a*(1-a)
        U1 = (1 -a)Uo

    where:
        A: disk area
        Uo: upstream velocity
        a: 1 - Cp/Ct
        U1: velocity at the disk


    The thrust is distributed by a radial function:

        thrust(r) = T*(C0 + C1*r^2 + C2*r^4)

    Sources described by:

        actuationDiskSourceCoeffs
        {
            fieldName       U;          // name of field to apply source
            diskDir         (-1 0 0);   // disk direction
            Cp              0.1;        // power coefficient
            Ct              0.5;        // thrust coefficient
            diskArea        5.0;        // disk area
            coeffs          (0.1 0.5 0.01); // radial distribution coefficients
        }


SourceFiles
    radialActuationDiskSource.cpp
    radialActuationDiskSourceTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef radialActuationDiskSource_H
#define radialActuationDiskSource_H

#include "actuationDiskSource.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class radialActuationDiskSource Declaration
\*---------------------------------------------------------------------------*/

class radialActuationDiskSource
:
    public actuationDiskSource
{
    // Private data

        //- Coeffcients for the radial distribution
        FixedList<scalar, 3> radialCoeffs_;


    // Private Member Functions

        //- Add resistance to the UEqn
        template<class RhoFieldType>
        void addRadialActuationDiskAxialInertialResistance
        (
            vectorField& Usource,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Disallow default bitwise copy construct
        radialActuationDiskSource(const radialActuationDiskSource&);

        //- Disallow default bitwise assignment
        void operator=(const radialActuationDiskSource&);


public:

    //- Runtime type information
    TypeName("radialActuationDiskSource");


    // Constructors

        //- Construct from components
        radialActuationDiskSource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~radialActuationDiskSource()
    {}


        // Public Functions

            //- Source term to fvMatrix<vector>
            virtual void addSup(fvMatrix<vector>& eqn, const label fieldI);


        // I-O

            //- Write data
            virtual void writeData(Ostream&) const;

            //- Read dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "fvMatrix.hpp"
#include "fvm.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * //

template<class RhoFieldType>
void CML::radialActuationDiskSource::
addRadialActuationDiskAxialInertialResistance
(
    vectorField& Usource,
    const labelList& cells,
    const scalarField& Vcells,
    const RhoFieldType& rho,
    const vectorField& U
) const
{
    scalar a = 1.0 - Cp_/Ct_;
    scalarField Tr(cells.size());
    const vector uniDiskDir = diskDir_/mag(diskDir_);


    tensor E(tensor::zero);
    E.xx() = uniDiskDir.x();
    E.yy() = uniDiskDir.y();
    E.zz() = uniDiskDir.z();

    const Field<vector> zoneCellCentres(mesh().cellCentres(), cells);
    const Field<scalar> zoneCellVolumes(mesh().cellVolumes(), cells);

    const vector avgCentre = gSum(zoneCellVolumes*zoneCellCentres)/V();
    const scalar maxR = gMax(mag(zoneCellCentres - avgCentre));

    scalar intCoeffs =
        radialCoeffs_[0]
      + radialCoeffs_[1]*sqr(maxR)/2.0
      + radialCoeffs_[2]*pow4(maxR)/3.0;

    vector upU = vector(VGREAT, VGREAT, VGREAT);
    scalar upRho = VGREAT;
    if (upstreamCellId_ != -1)
    {
        upU =  U[upstreamCellId_];
        upRho = rho[upstreamCellId_];
    }
    reduce(upU, minOp<vector>());
    reduce(upRho, minOp<scalar>());

    scalar T = 2.0*upRho*diskArea_*mag(upU)*a*(1.0 - a);
    forAll(cells, i)
    {
        scalar r2 = magSqr(mesh().cellCentres()[cells[i]] - avgCentre);

        Tr[i] =
            T
           *(radialCoeffs_[0] + radialCoeffs_[1]*r2 + radialCoeffs_[2]*sqr(r2))
           /intCoeffs;
    }

    forAll(cells, i)
    {
        Usource[cells[i]] += ((Vcells[cells[i]]/V_)*Tr[i]*E) & upU;
    }

    if (debug)
    {
        Info<< "Source name: " << name() << nl
            << "Average centre: " << avgCentre << nl
            << "Maximum radius: " << maxR << endl;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
