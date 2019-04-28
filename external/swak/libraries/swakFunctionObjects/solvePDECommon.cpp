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
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "solvePDECommon.hpp"

#include "polyMesh.hpp"

namespace CML {
    defineTypeNameAndDebug(solvePDECommon,0);
}

template<>
const char* CML::NamedEnum<CML::solvePDECommon::solveAt,3>::names[]=
{
    "startup",
    "timestep",
    "write"
};

const CML::NamedEnum<CML::solvePDECommon::solveAt,3> CML::solvePDECommon::solveAtNames_;

CML::solvePDECommon::solvePDECommon
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    dummyOptionList_(
       dynamicCast<const fvMesh&>(obr)
    ),
    warnedAboutMissingOptionList_(
       false
    ),
    active_(true),
    obr_(obr),
    name_(name),
    steady_(false),
    relaxUnsteady_(false),
    relaxLastIteration_(false)
{
    if (!isA<polyMesh>(obr))
    {
        active_=false;
        WarningInFunction
            << "Not a polyMesh. Nothing I can do"
            << endl;
    }
}

CML::solvePDECommon::~solvePDECommon()
{}

CML::fv::optionList &CML::solvePDECommon::fvOptions() const
{
    if(obr_.foundObject<fv::optionList>("fvOptions")) {
        return const_cast<fv::optionList&>(
            obr_.lookupObject<fv::optionList>("fvOptions")
        );
    } else {
        if(!warnedAboutMissingOptionList_) {
            const_cast<solvePDECommon&>(*this).warnedAboutMissingOptionList_=true;
            WarningInFunction
                 << "No 'fvOptions' found. Returning dummy (no further warnings"
                 << endl;
        }
        return const_cast<solvePDECommon&>(*this).dummyOptionList_;
    }
}

void CML::solvePDECommon::timeSet()
{
    // Do nothing
}

void CML::solvePDECommon::readExpressionAndDimension(
    const dictionary &dict,
    const word &name,
    exprString &expr,
    dimensionSet &dim
)
{
    ITstream in(dict.lookup(name));

    expr=exprString(
        in,
        dict
    );

    in >> dim;
}

bool CML::solvePDECommon::doRelax(bool last)
{
    return
        (steady_ || relaxUnsteady_)
        &&
        (!last || relaxLastIteration_);
}

void CML::solvePDECommon::read(const dictionary& dict)
{
    if(debug) {
        Info << "CML::solvePDECommon::read()" << endl;
    }
    if(active_) {
        solveAt_=
            solveAtNames_.read(
                dict.lookup("solveAt")
            );
        fieldName_=word(dict.lookup("fieldName"));

        steady_=readBool(dict.lookup("steady"));
        if(steady_) {
            relaxUnsteady_=false;
        } else {
            if(dict.found("relaxUnsteady")) {
                relaxUnsteady_=readBool(dict.lookup("relaxUnsteady"));
            } else {
                WarningInFunction
                    << "If you want the unsteady run to use relaxation set "
                    << "'relaxUnsteady true;' in " << dict.name()
                    << endl;
                relaxUnsteady_=false;
            }
        }
        if(steady_ || relaxUnsteady_) {
            if(dict.found("relaxLastIteration")) {
                relaxLastIteration_=readBool(dict.lookup("relaxLastIteration"));
            } else {
                WarningInFunction
                    << "If in case of relaxation you want to relax the last "
                    << "iteration as well set "
                    << "'relaxLastIteration true;' in " << dict.name()
                    << endl;
                relaxLastIteration_=false;
            }
        }
        writeBeforeAfter_=dict.lookupOrDefault<bool>("writeBeforeAfter",false);
    }
}

void CML::solvePDECommon::execute()
{
    if(debug) {
        Info << "CML::solvePDECommon::execute()" << endl;
    }
    if(solveAt_==saTimestep) {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        solveWrapper();

        // as this is executed after the general write, write the field separately
        if(mesh.time().outputTime()) {
            writeData();
        }
    }
}


void CML::solvePDECommon::end()
{
    if(debug) {
        Info << "CML::solvePDECommon::end()" << endl;
    }
    execute();
}

void CML::solvePDECommon::solveWrapper()
{
    if(debug) {
        Info << "CML::solvePDECommon::solveWrapper()" << endl;
    }
    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if(writeBeforeAfter_) {
        Info << "Write " << fieldName_ << " before" << endl;
        this->writeOldField();
    }

    solve();

    if(writeBeforeAfter_ && !mesh.time().outputTime()) {
        Info << "Write " << fieldName_ << " after" << endl;
        this->writeNewField();
    }
}

void CML::solvePDECommon::write()
{
    if(debug) {
        Info << "CML::solvePDECommon::write()" << endl;
    }
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    if(
        solveAt_==saWrite
        &&
        mesh.time().outputTime()
    ) {
        solveWrapper();

        writeData();
    }
}

// ************************************************************************* //
