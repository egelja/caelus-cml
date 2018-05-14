/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2017 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    BlockLduInterfaceField

Description
    Abstract base class for interface fields with block coefficients

    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011 and modified using code from FOAM-Extend-4.0
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Ivor Clifford, ivor.clifford@gmail.com
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "BlockLduInterfaceField.hpp"
#include "hyperVectorFieldTypes.hpp"


namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTemplateTypeNameAndDebug(BlockLduInterfaceField<scalar>, 0);
defineTemplateTypeNameAndDebug(BlockLduInterfaceField<vector>, 0);
defineTemplateTypeNameAndDebug(BlockLduInterfaceField<tensor>, 0);
defineTemplateTypeNameAndDebug(BlockLduInterfaceField<sphericalTensor>, 0);
defineTemplateTypeNameAndDebug(BlockLduInterfaceField<symmTensor>, 0);

#define makeTemplateTypeNameAndDebug(type, Type, args...)                     \
defineTemplateTypeNameAndDebug(BlockLduInterfaceField<type>, 0);

    forAllHyperVectorTypes(makeTemplateTypeNameAndDebug);

#undef makeTemplateTypeNameAndDebug

} // End namespace CML
