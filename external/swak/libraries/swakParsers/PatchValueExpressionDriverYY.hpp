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
    CML::PatchValueExpressionDriver

Description

SourceFiles
    PatchValueExpressionDriverI.hpp
    PatchValueExpressionDriver.cpp

Contributors/Copyright:
    2009-2010, 2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef PatchValueExpressionDriverYY_H
#define PatchValueExpressionDriverYY_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "PatchValueExpressionDriver.hpp"

#define YY_DECL int parserPatchlex(parserPatch::PatchValueExpressionParser::semantic_type* yylval_param,parserPatch::location* yylloc,void *yyscanner,CML::PatchValueExpressionDriver &driver,int& start_token,int& numberOfFunctionChars)

YY_DECL;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
