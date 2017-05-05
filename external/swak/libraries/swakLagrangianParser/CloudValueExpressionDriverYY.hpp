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
    CML::CloudValueExpressionDriver

Description

SourceFiles
    CloudValueExpressionDriverI.hpp
    CloudValueExpressionDriver.cpp

Contributors/Copyright:
    2010, 2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudValueExpressionDriverYY_H
#define CloudValueExpressionDriverYY_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "CloudValueExpressionDriver.hpp"

#define YY_DECL int parserCloudlex(parserCloud::CloudValueExpressionParser::semantic_type* yylval_param,parserCloud::location* yylloc,void *yyscanner,CML::CloudValueExpressionDriver &driver,int& start_token,int& numberOfFunctionChars)

YY_DECL;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
