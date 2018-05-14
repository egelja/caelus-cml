/*---------------------------------------------------------------------------*\
Copyright (C) 2012 Applied CCM Pty Ltd
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

Author
    Darrin Stephens, Applied CCM Pty Ltd

\*---------------------------------------------------------------------------*/

#ifndef restrict_H
#define restrict_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Restrict macro
#ifdef windows
# define RESTRICT __restrict
#else
# define RESTRICT __restrict__
#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
