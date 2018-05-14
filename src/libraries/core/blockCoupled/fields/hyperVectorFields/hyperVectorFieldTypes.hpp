/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2013 H. Jasak
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

Description
    Include the header files for all the primitive types that Fields are
    instantiated for.

    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.

    The original file contained "Copyright held by original author",
    but no author was named in the original file. Authorship confirmed by
    Ivor Clifford (2017).

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Authors
    Ivor Clifford
    Hrvoje Jasak
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef hyperVectorFieldTypes_HPP
#define hyperVectorFieldTypes_HPP

#include "hyperVector2.hpp"
#include "hyperVector3.hpp"
#include "hyperVector4.hpp"
#include "hyperVector6.hpp"
#include "hyperVector8.hpp"
#include "hyperTensor2.hpp"
#include "hyperTensor3.hpp"
#include "hyperTensor4.hpp"
#include "hyperTensor6.hpp"
#include "hyperTensor8.hpp"


#define forAllHyperVectorTypes(m, args...)                                    \
    m(hyperVector2, hyperVector2, args)                                       \
    m(hyperVector3, hyperVector3, args)                                       \
    m(hyperVector4, hyperVector4, args)                                       \
    m(hyperVector6, hyperVector6, args)                                       \
    m(hyperVector8, hyperVector8, args)


#define forAllHyperTensorTypes(m, args...)                                    \
    m(hyperTensor2, hyperTensor2, args)                                       \
    m(hyperTensor3, hyperTensor3, args)                                       \
    m(hyperTensor4, hyperTensor4, args)                                       \
    m(hyperTensor6, hyperTensor6, args)                                       \
    m(hyperTensor8, hyperTensor8, args)


#define forAllHyperVectorTensorTypes(m, args...)                              \
    m(hyperTensor2, hyperVector2, scalar, args)                               \
    m(hyperTensor3, hyperVector3, scalar, args)                               \
    m(hyperTensor4, hyperVector4, scalar, args)                               \
    m(hyperTensor6, hyperVector6, scalar, args)                               \
    m(hyperTensor8, hyperVector8, scalar, args)


#endif
