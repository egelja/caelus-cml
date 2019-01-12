/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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

Contributors/Copyright:
    2010, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "clearExpressionField.hpp"

#include "FieldValueExpressionDriver.hpp"

#include "expressionFieldFunctionObject.hpp"

namespace CML {
    defineTypeNameAndDebug(clearExpressionField,0);
}

CML::clearExpressionField::clearExpressionField
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    obr_(obr)
{
    read(dict);
}

CML::clearExpressionField::~clearExpressionField()
{}

void CML::clearExpressionField::timeSet()
{
    // Do nothing
}

void CML::clearExpressionField::read(const dictionary& dict)
{
    name_=word(dict.lookup("fieldName"));
}

void CML::clearExpressionField::execute()
{
    const functionObjectList &fol=obr_.time().functionObjects();
    bool found=false;

    forAll(fol,i) {
        if(isA<expressionFieldFunctionObject>(fol[i])) {
            expressionField &ef=const_cast<expressionField &>(
                //                dynamicCast<const expressionFieldFunctionObject&>(fol[i]).outputFilter() // doesn't work with gcc 4.2
                dynamic_cast<const expressionFieldFunctionObject&>(fol[i]).outputFilter()
            );

            if(ef.name()==name_) {
                found=true;                
                ef.clearData();
            }
        }
    }

    if(!found) {
        WarningInFunction
            << "No function object named " << name_ << " found" 
            << endl;
    }
}


void CML::clearExpressionField::end()
{
}

void CML::clearExpressionField::write()
{
}

// ************************************************************************* //
