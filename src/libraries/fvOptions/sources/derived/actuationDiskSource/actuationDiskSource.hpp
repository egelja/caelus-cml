/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fv::actuationDiskSource

Description
    Actuation disk source

    Constant values for momentum source for actuation disk
    \f[
        T = 2 \rho A U_{o}^2 a (1-a)
    \f]
    and
    \f[
        U_1 = (1 - a)U_{o}
    \f]

    where:
    \vartable
        A   | disk area
        U_o | upstream velocity
        a   | 1 - Cp/Ct
        U_1 | velocity at the disk
    \endvartable

    \heading Source usage

    Example usage:
    \verbatim
    actuationDiskSourceCoeffs
    {
        fieldNames      (U);        // names of fields to apply source
        diskDir         (-1 0 0);   // disk direction
        Cp              0.1;        // power coefficient
        Ct              0.5;        // thrust coefficient
        diskArea        5.0;        // disk area
        upstreamPoint   (0 0 0);    // upstream point
    }
    \endverbatim


SourceFiles
    actuationDiskSource.cpp
    actuationDiskSourceTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef actuationDiskSource_H
#define actuationDiskSource_H

#include "fvOption.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                     Class actuationDiskSource Declaration
\*---------------------------------------------------------------------------*/

class actuationDiskSource
:
    public option
{

protected:

    // Protected data

        //- Disk area normal
        vector diskDir_;

        //- Power coefficient
        scalar Cp_;

        //- Thrust coefficient
        scalar Ct_;

        //- Disk area
        scalar diskArea_;

        //- Upstream point sample
        point upstreamPoint_;

        //- Upstream cell ID
        label upstreamCellId_;


private:

    // Private Member Functions

        //- Check data
        void checkData() const;

        //- Add resistance to the UEqn
        template<class RhoFieldType>
        void addActuationDiskAxialInertialResistance
        (
            vectorField& Usource,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Disallow default bitwise copy construct
        actuationDiskSource(const actuationDiskSource&);

        //- Disallow default bitwise assignment
        void operator=(const actuationDiskSource&);


public:

    //- Runtime type information
    TypeName("actuationDiskSource");


    // Constructors

        //- Construct from components
        actuationDiskSource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~actuationDiskSource()
    {}


    // Member Functions

        // Access

            //- Return Cp
            scalar Cp() const
            {
                return Cp_;
            }

            //- Return Ct
            scalar Ct() const
            {
                return Ct_;
            }

            //- Normal disk direction
            const vector& diskDir() const
            {
                return diskDir_;
            }

            //- Disk area
            scalar diskArea() const
            {
                return diskArea_;
            }


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

} // End namespace fv
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

// * * * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * //

template<class RhoFieldType>
void CML::fv::actuationDiskSource::addActuationDiskAxialInertialResistance
(
    vectorField& Usource,
    const labelList& cells,
    const scalarField& Vcells,
    const RhoFieldType& rho,
    const vectorField& U
) const
{
    scalar a = 1.0 - Cp_/Ct_;
    vector uniDiskDir = diskDir_/mag(diskDir_);
    tensor E(tensor::zero);
    E.xx() = uniDiskDir.x();
    E.yy() = uniDiskDir.y();
    E.zz() = uniDiskDir.z();

    vector upU = vector(VGREAT, VGREAT, VGREAT);
    scalar upRho = VGREAT;
    if (upstreamCellId_ != -1)
    {
        upU =  U[upstreamCellId_];
        upRho = rho[upstreamCellId_];
    }
    reduce(upU, minOp<vector>());
    reduce(upRho, minOp<scalar>());

    scalar T = 2.0*upRho*diskArea_*mag(upU)*a*(1 - a);

    forAll(cells, i)
    {
        Usource[cells[i]] += ((Vcells[cells[i]]/V())*T*E) & upU;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
