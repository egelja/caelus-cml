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

#include "procLduInterface.hpp"
#include "lduInterfaceField.hpp"
#include "cyclicLduInterface.hpp"
#include "processorLduInterface.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::procLduInterface::procLduInterface
(
    const lduInterfaceField& interface,
    const scalarField& coeffs
)
:
    faceCells_(interface.interface().faceCells()),
    coeffs_(coeffs),
    myProcNo_(-1),
    neighbProcNo_(-1),
    tag_(-1)
{
    if (isA<processorLduInterface>(interface.interface()))
    {
        const processorLduInterface& pldui =
            refCast<const processorLduInterface>(interface.interface());

        myProcNo_ = pldui.myProcNo();
        neighbProcNo_ = pldui.neighbProcNo();
        tag_ = pldui.tag();
    }
    else if (isA<cyclicLduInterface>(interface.interface()))
    {
    }
    else
    {
        FatalErrorInFunction
            << "Unknown lduInterface type "
            << interface.interface().type()
            << exit(FatalError);
    }
}


CML::procLduInterface::procLduInterface(Istream& is)
:
    faceCells_(is),
    coeffs_(is),
    myProcNo_(readLabel(is)),
    neighbProcNo_(readLabel(is)),
    tag_(readLabel(is))
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const procLduInterface& cldui)
{
    os  << cldui.faceCells_
        << cldui.coeffs_
        << cldui.myProcNo_
        << cldui.neighbProcNo_
        << cldui.tag_;

    return os;
}


// ************************************************************************* //
