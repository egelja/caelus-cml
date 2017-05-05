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
    CML::fvcSurfCoeffLaplacianFunctionPlugin

Description

SourceFiles
    fvcSurfCoeffLaplacianFunctionPlugin.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef fvcSurfCoeffLaplacianFunctionPlugin_H
#define fvcSurfCoeffLaplacianFunctionPlugin_H

#include "FieldValuePluginFunction.hpp"

#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class fvcSurfCoeffLaplacianFunctionPlugin Declaration
\*---------------------------------------------------------------------------*/

template<class T,class GT>
class fvcSurfCoeffLaplacianFunctionPlugin
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const fvcSurfCoeffLaplacianFunctionPlugin &);

    fvcSurfCoeffLaplacianFunctionPlugin(const fvcSurfCoeffLaplacianFunctionPlugin &);

    typedef GeometricField<T,fvPatchField,volMesh> originalType;
    typedef GeometricField<T,fvPatchField,volMesh> resultType;
    typedef GeometricField<GT,fvsPatchField,surfaceMesh> coeffType;

    autoPtr<originalType> original_;
    autoPtr<coeffType> coeff_;

    string specString_;

public:
    fvcSurfCoeffLaplacianFunctionPlugin(
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    );

    virtual ~fvcSurfCoeffLaplacianFunctionPlugin() {}

    TypeName("fvcSurfCoeffLaplacianFunctionPlugin");

    void doEvaluation();

    void setArgument(
        label index,
        const string &content,
        const CommonValueExpressionDriver &driver
    );

    void setArgument(label index,const string &);

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
