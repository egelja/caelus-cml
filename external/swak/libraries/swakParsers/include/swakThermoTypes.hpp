/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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


Description
    Define types that were renamed in CAELUS

SourceFiles

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SwakThermoMacroHeader_H
#define SwakThermoMacroHeader_H

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "psiThermo.hpp"
#include "rhoThermo.hpp"

namespace CML {
    typedef fluidThermo swakFluidThermoType;
    typedef psiThermo swakPsiThermoType;
    typedef rhoThermo swakRhoThermoType;
}

#include "DebugOStream.hpp"

#endif

// ************************************************************************* //
