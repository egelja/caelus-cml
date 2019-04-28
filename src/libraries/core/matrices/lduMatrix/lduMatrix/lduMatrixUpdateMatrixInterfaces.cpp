/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "lduMatrix.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::lduMatrix::initMatrixInterfaces
(
    const FieldField<Field, scalar>& coupleCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const scalarField& psiif,
    scalarField& result,
    const direction cmpt
) const
{
    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll(interfaces, interfaceI)
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].initInterfaceMatrixUpdate
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt,
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI=patchSchedule.size()/2;
            interfaceI<interfaces.size();
            interfaceI++
        )
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].initInterfaceMatrixUpdate
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt,
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}


void CML::lduMatrix::updateMatrixInterfaces
(
    const FieldField<Field, scalar>& coupleCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const scalarField& psiif,
    scalarField& result,
    const direction cmpt
) const
{
    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        // Block until all sends/receives have been finished
        if
        (
            Pstream::parRun()
         && Pstream::defaultCommsType == Pstream::nonBlocking
        )
        {
            UPstream::waitRequests();
        }

        forAll(interfaces, interfaceI)
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt,
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over all the "normal" interfaces relating to standard patches
        forAll(patchSchedule, i)
        {
            label interfaceI = patchSchedule[i].patch;

            if (interfaces.set(interfaceI))
            {
                if (patchSchedule[i].init)
                {
                    interfaces[interfaceI].initInterfaceMatrixUpdate
                    (
                        psiif,
                        result,
                        *this,
                        coupleCoeffs[interfaceI],
                        cmpt,
                        Pstream::scheduled
                    );
                }
                else
                {
                    interfaces[interfaceI].updateInterfaceMatrix
                    (
                        psiif,
                        result,
                        *this,
                        coupleCoeffs[interfaceI],
                        cmpt,
                        Pstream::scheduled
                    );
                }
            }
        }

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI=patchSchedule.size()/2;
            interfaceI<interfaces.size();
            interfaceI++
        )
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt,
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}


// ************************************************************************* //
