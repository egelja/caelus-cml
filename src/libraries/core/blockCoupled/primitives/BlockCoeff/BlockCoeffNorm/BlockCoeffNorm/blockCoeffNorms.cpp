/*---------------------------------------------------------------------------*\
Copyright (C) 2013 K. Jareteg
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

Description
    BlockCoeff norms static data
    
    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    The original file contained "For copyright notice see file Copyright",
    but no file named Copyright could be found in FOAM-Extend-4.0 repository.

    The copyright has therefore been attributed to the authour named in the
    BlockCoeffNorm.H file.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Authors
    Klas Jareteg, 2013-01-28
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "hyperVectorFieldTypes.hpp"
#include "hyperVectorTensorFields.hpp"
#include "ExpandHyperTensor.hpp"
#include "ExpandHyperTensorField.hpp"
#include "BlockCoeffNorm.hpp"
#include "BlockCoeffComponentNorm.hpp"
#include "BlockCoeffMaxNorm.hpp"
#include "BlockCoeffTwoNorm.hpp"


namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Make hyperVector types
forAllHyperVectorTypes(makeBlockCoeffNormHyperVector)


} // End namespace CML
