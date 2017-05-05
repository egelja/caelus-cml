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

Contributors/Copyright:
    2010-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "surfaceMesh.hpp"
#include "fvsPatchField.hpp"
#include "pointPatchField.hpp"
#include "primitivePatchInterpolation.hpp"
#include "interpolation.hpp"

#include <cassert>

#include "CloudProxy.hpp"

namespace CML {

    template <typename Type>
    tmp<Field<Type > > CloudValueExpressionDriver::getFluidField(
        word const&name
    )
    {
        Field<vector> positions(proxy_->getPositions());
        Field<label> cells(proxy_->getCells());

        tmp<Field<Type> > pResult(
            new Field<Type>(
                cells.size(),
                pTraits<Type>::zero
            )
        );
        Field<Type> &result=pResult();

        typedef GeometricField< Type, fvPatchField, volMesh > volField;

        word interpolationScheme("none");
        if(
            !interpolationSchemes_.found(name)
            &&
            interpolationSchemes_.found("default")
        ) {
            interpolationScheme=word(interpolationSchemes_["default"]);
        } else if(interpolationSchemes_.found(name)) {
            interpolationScheme=word(interpolationSchemes_[name]);
        }

        if(
            this->searchInMemory()
            &&
            this->mesh().foundObject<volField>(name)
        ) {
            const volField &f=this->mesh().lookupObject<volField>(name);

            autoPtr<interpolation<Type> > inter;
            inter=interpolation<Type>::New(
                interpolationScheme,
                f
            );
            forAll(result,i) {
                result[i]=inter().interpolate(
                    positions[i],
                    cells[i]
                );
            }
        } else {
            autoPtr<volField> f=this->getOrReadField<volField>(
                name,
                true // fail if not found
            );

            autoPtr<interpolation<Type> > inter=interpolation<Type>::New(
                interpolationScheme,
                f
            );
            forAll(result,i) {
                result[i]=inter().interpolate(
                    positions[i],
                    cells[i]
                );
            }
        }

        return pResult;
   }
}


// ************************************************************************* //
