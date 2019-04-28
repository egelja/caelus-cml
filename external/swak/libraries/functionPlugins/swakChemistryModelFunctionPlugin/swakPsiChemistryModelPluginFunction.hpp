/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

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
    CML::swakPsiChemistryModelPluginFunction

Description

SourceFiles
    swakPsiChemistryModelPluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakPsiChemistryModelPluginFunction_H
#define swakPsiChemistryModelPluginFunction_H

#include "FieldValuePluginFunction.hpp"

#include "runTimeSelectionTables.hpp"

#include "swakThermoTypes.hpp"

#include "BasicChemistryModel_.hpp"
#include "psiReactionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakPsiChemistryModelPluginFunction Declaration
\*---------------------------------------------------------------------------*/
class swakPsiChemistryModelPluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const swakPsiChemistryModelPluginFunction &);

    swakPsiChemistryModelPluginFunction(const swakPsiChemistryModelPluginFunction &);

protected:
    typedef BasicChemistryModel<psiReactionThermo> ChemistryModelType;

    static const  swakPsiChemistryModelPluginFunction::ChemistryModelType &chemistryInternal(const fvMesh &reg);

    const  swakPsiChemistryModelPluginFunction::ChemistryModelType &chemistry();

    void updateChemistry(const scalar dt);

    tmp<volScalarField> wrapDimField(
        const DimensionedField<scalar,volMesh> &dimField
    );

public:
    swakPsiChemistryModelPluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &returnValueType,
        const string &spec=""
    );

    virtual ~swakPsiChemistryModelPluginFunction() {}

    TypeName("swakPsiChemistryModelPluginFunction");

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
