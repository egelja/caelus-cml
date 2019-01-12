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
    None

Description
    Helper function that usually issues a NotImplemented unless the user
    decided to switch it off in the dictionary

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SWITCHABLE_NOT_IMPLEMENTED_H
#define SWITCHABLE_NOT_IMPLEMENTED_H

#include "error.hpp"

namespace CML {
    void switchableNotImplemented(
        const word &methodName,
        const dictionary &dict
    );
}

#endif
