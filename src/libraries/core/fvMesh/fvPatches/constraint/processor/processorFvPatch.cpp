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

\*---------------------------------------------------------------------------*/

#include "processorFvPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(processorFvPatch, 0);
addToRunTimeSelectionTable(fvPatch, processorFvPatch, polyPatch);


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void processorFvPatch::makeWeights(scalarField& w) const
{
    if (Pstream::parRun())
    {
        // The face normals point in the opposite direction on the other side
        scalarField neighbFaceCentresCn
        (
            (
                procPolyPatch_.neighbFaceAreas()
               /(mag(procPolyPatch_.neighbFaceAreas()) + VSMALL)
            )
          & (
              procPolyPatch_.neighbFaceCentres()
            - procPolyPatch_.neighbFaceCellCentres())
        );

        w = neighbFaceCentresCn
           /((nf()&coupledFvPatch::delta()) + neighbFaceCentresCn);
    }
    else
    {
        w = 1.0;
    }
}


tmp<vectorField> processorFvPatch::delta() const
{
    if (Pstream::parRun())
    {
        // To the transformation if necessary
        if (parallel())
        {
            return
                coupledFvPatch::delta()
              - (
                    procPolyPatch_.neighbFaceCentres()
                  - procPolyPatch_.neighbFaceCellCentres()
                );
        }
        else
        {
            return
                coupledFvPatch::delta()
              - transform
                (
                    forwardT(),
                    (
                        procPolyPatch_.neighbFaceCentres()
                      - procPolyPatch_.neighbFaceCellCentres()
                    )
                );
        }
    }
    else
    {
        return coupledFvPatch::delta();
    }
}


tmp<labelField> processorFvPatch::interfaceInternalField
(
    const labelUList& internalData
) const
{
    return patchInternalField(internalData);
}


void processorFvPatch::initInternalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const labelUList& iF
) const
{
    send(commsType, patchInternalField(iF)());
}


tmp<labelField> processorFvPatch::internalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const labelUList&
) const
{
    return receive<label>(commsType, this->size());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
