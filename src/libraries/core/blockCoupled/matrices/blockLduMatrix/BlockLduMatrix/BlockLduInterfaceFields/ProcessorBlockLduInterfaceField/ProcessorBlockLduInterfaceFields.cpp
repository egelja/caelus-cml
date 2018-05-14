/*---------------------------------------------------------------------------*\
Copyright (C) 2013 K. Jareteg
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
    ProcessorBlockLduInterfaceField

Description
    Processor interface field for block coefficients

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

Author
    Klas Jareteg, 2013-02-08
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "ProcessorBlockLduInterfaceField.hpp"
#include "hyperVectorFieldTypes.hpp"


namespace CML
{

#define makeTemplateTypeNameAndDebug(type, Type, args...)                     \
defineTemplateTypeNameAndDebug(ProcessorBlockLduInterfaceField<type>, 0);     \

    forAllHyperVectorTypes(makeTemplateTypeNameAndDebug);

#undef makeTemplateTypeNameAndDebug

} // End namespace CML
