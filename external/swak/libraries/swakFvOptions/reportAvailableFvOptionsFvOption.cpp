/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "reportAvailableFvOptionsFvOption.hpp"
#include "fvMatrices.hpp"
#include "DimensionedField.hpp"
#include "IFstream.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(reportAvailableFvOptionsFvOption, 0);

    addToRunTimeSelectionTable
    (
        option,
        reportAvailableFvOptionsFvOption,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::fv::reportAvailableFvOptionsFvOption::alwaysApply() const
{
    return true;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fv::reportAvailableFvOptionsFvOption::reportAvailableFvOptionsFvOption
(
    const word& sourceName,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(sourceName, modelType, dict, mesh)
{
    fieldNames_=List<word>(1,"dummy");
    applied_=Field<bool>(1,false);
}

CML::fv::reportAvailableFvOptionsFvOption::~reportAvailableFvOptionsFvOption()
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fv::reportAvailableFvOptionsFvOption::report(
    const string &message
) {
    Info << name() << " called: " << message.c_str() << endl;
}

void CML::fv::reportAvailableFvOptionsFvOption::correct(volVectorField& U)
{
    report("correct(volVectorField& "+U.name()+")");
}


void CML::fv::reportAvailableFvOptionsFvOption::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    report(
        "addSup(fvMatrix<vector>& "+eqn.psi().name()+")"
    );
}


void CML::fv::reportAvailableFvOptionsFvOption::setValue
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    report(
        "setValue(fvMatrix<vector>& "+eqn.psi().name()+")"
    );
}

void CML::fv::reportAvailableFvOptionsFvOption::correct(volScalarField& U)
{
    report("correct(volScalarField& "+U.name()+")");
}


void CML::fv::reportAvailableFvOptionsFvOption::addSup
(
    fvMatrix<scalar>& eqn,
    const label fieldI
)
{
    report(
        "addSup(fvMatrix<scalar>& "+eqn.psi().name()+")"
    );
}


void CML::fv::reportAvailableFvOptionsFvOption::setValue
(
    fvMatrix<scalar>& eqn,
    const label fieldI
)
{
    report(
        "setValue(fvMatrix<scalar>& "+eqn.psi().name()+")"
    );
}

void CML::fv::reportAvailableFvOptionsFvOption::correct(volTensorField& U)
{
    report("correct(volTensorField& "+U.name()+")");
}


void CML::fv::reportAvailableFvOptionsFvOption::addSup
(
    fvMatrix<tensor>& eqn,
    const label fieldI
)
{
    report(
        "addSup(fvMatrix<tensor>& "+eqn.psi().name()+")"
    );
}


void CML::fv::reportAvailableFvOptionsFvOption::setValue
(
    fvMatrix<tensor>& eqn,
    const label fieldI
)
{
    report(
        "setValue(fvMatrix<tensor>& "+eqn.psi().name()+")"
    );
}

void CML::fv::reportAvailableFvOptionsFvOption::correct(volSymmTensorField& U)
{
    report("correct(volSymmTensorField& "+U.name()+")");
}


void CML::fv::reportAvailableFvOptionsFvOption::addSup
(
    fvMatrix<symmTensor>& eqn,
    const label fieldI
)
{
    report(
        "addSup(fvMatrix<symmTensor>& "+eqn.psi().name()+")"
    );
}


void CML::fv::reportAvailableFvOptionsFvOption::setValue
(
    fvMatrix<symmTensor>& eqn,
    const label fieldI
)
{
    report(
        "setValue(fvMatrix<symmTensor>& "+eqn.psi().name()+")"
    );
}

void CML::fv::reportAvailableFvOptionsFvOption::correct(volSphericalTensorField& U)
{
    report("correct(volSphericalTensorField& "+U.name()+")");
}


void CML::fv::reportAvailableFvOptionsFvOption::addSup
(
    fvMatrix<sphericalTensor>& eqn,
    const label fieldI
)
{
    report(
        "addSup(fvMatrix<sphericalTensor>& "+eqn.psi().name()+")"
    );
}


void CML::fv::reportAvailableFvOptionsFvOption::setValue
(
    fvMatrix<sphericalTensor>& eqn,
    const label fieldI
)
{
    report(
        "setValue(fvMatrix<sphericalTensor>& "+eqn.psi().name()+")"
    );
}

void CML::fv::reportAvailableFvOptionsFvOption::makeRelative(
    surfaceScalarField& phi
) const
{
    const_cast<reportAvailableFvOptionsFvOption&>(*this).
            report(
                "makeRelative(surfaceScalarField& "+phi.name()+")"
            );
}

void CML::fv::reportAvailableFvOptionsFvOption::makeRelative(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    const_cast<reportAvailableFvOptionsFvOption&>(*this).
        report(
            "makeRelative(const surfaceScalarField& "+rho.name()+",surfaceScalarField& "+phi.name()+")"
        );
}

void CML::fv::reportAvailableFvOptionsFvOption::makeRelative(
    FieldField<fvsPatchField, scalar>& phi
) const
{
    const_cast<reportAvailableFvOptionsFvOption&>(*this).
            report(
                "makeRelative(FieldField<fvsPatchField, scalar>&)"
            );
}


void CML::fv::reportAvailableFvOptionsFvOption::makeAbsolute(
    surfaceScalarField& phi
) const
{
    const_cast<reportAvailableFvOptionsFvOption&>(*this).
            report("makeAbsolute(surfaceScalarField& "+phi.name()+")");
}

void CML::fv::reportAvailableFvOptionsFvOption::makeAbsolute(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    const_cast<reportAvailableFvOptionsFvOption&>(*this).
        report(
            "makeAbsolute(const surfaceScalarField& "+rho.name()+",surfaceScalarField& "+phi.name()+")"
        );
}


// ************************************************************************* //
