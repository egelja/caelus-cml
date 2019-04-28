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
    2010-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "surfaceMesh.hpp"
#include "fvsPatchField.hpp"
#include "pointPatchField.hpp"
#include "primitivePatchInterpolation.hpp"

#include <cassert>

namespace CML {
    template<class Type,class volField,class surfField>
    tmp<Field<Type> >
    SampledSurfaceValueExpressionDriver::sampleOrInterpolateInternal(
        const word &name,
        bool oldTime
    )
    {
        if(
            this->hasVariable(name)
            &&
            this->variable(name).valueType()==pTraits<Type>::typeName
        ) {
            return this->variable(name).getResult<Type>(true);
        }
        autoPtr<Field<Type> > result;

        if(
            this->searchInMemory()
            &&
            this->mesh().foundObject<volField>(name)
        ) {
            if(interpolate_) {
                autoPtr<interpolation<Type> > inter(
                    interpolation<Type>::New(
                        interpolationType_,
                        (
                            !oldTime ?
                            this->mesh().lookupObject<volField>(name) :
                            this->mesh().lookupObject<volField>(name).oldTime()
                        )
                    )
                );

                result.set(
                    new Field<Type>(
                        this->theSurface_.interpolate(
                            inter()
                        )
                    )
                );
            } else {
                result.set(
                    new Field<Type>(
                        this->theSurface_.sample(
                            (
                                !oldTime ?
                                this->mesh().lookupObject<volField>(name) :
                                this->mesh().lookupObject<volField>(name).oldTime()
                            )
                        )
                    )
                );
            }
        } else {
            autoPtr<volField> f=this->getOrReadField<volField>(
                name,
                true, // fail if not found
                oldTime
            );

            if(interpolate_) {
                autoPtr<interpolation<Type> > inter(
                    interpolation<Type>::New(
                        interpolationType_,
                        (
                            !oldTime ?
                            f() :
                            f->oldTime()
                        )
                    )
                );

                result.set(
                    new Field<Type>(
                        this->theSurface_.interpolate(
                            inter()
                        )
                    )
                );
            } else {
                result.set(
                    new Field<Type>(
                        this->theSurface_.sample(
                            (
                                !oldTime ?
                                f() :
                                f->oldTime()
                            )
                        )
                    )
                );
            }
        }

        if(!result.valid()) {
            FatalErrorInFunction
                << "No valid result"
                << endl
                << exit(FatalError);
        }

        if(debug) {
            Pout << "SampledSurfaceValueExpressionDriver::sampleOrInterpolateInternal()"
                << "Type: " << pTraits<Type>::typeName
                << " name: " << name
                << " size: " << this->size() << " result: "
                << result().size()
                << endl;
        }

        return tmp<Field<Type> >(result.ptr());
    }

}


// ************************************************************************* //
