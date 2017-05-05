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

Class
    solvePDECommon

Description
    Common functionality for functionObjects that solve PDEs

SourceFiles
    solvePDECommon.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#ifndef solvePDECommon_H
#define solvePDECommon_H

#include "primitiveFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "NamedEnum.hpp"
#include "volFields.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "exprString.hpp"

#include "fvIOoptionList.hpp"

namespace CML
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class polyMesh;

class solvePDECommon
{
    //- Disallow default bitwise copy construct
    solvePDECommon(const solvePDECommon&);

    //- Disallow default bitwise assignment
    void operator=(const solvePDECommon&);

    //- standin if the solver did not define an option list
    fv::optionList dummyOptionList_;

    bool warnedAboutMissingOptionList_;

public:
    enum solveAt {
        saStartup,
        saTimestep,
        saWrite
    };
protected:

    //- are we working?
    bool active_;

    //- mesh where the set goes
    const objectRegistry& obr_;

    //- name of the function object
    const word name_;

    //- name of the field that is being solved for
    word fieldName_;

    //- is this a steady calculation
    bool steady_;

    //- should unsteady runs allow relaxation too
    bool relaxUnsteady_;

    //- the last iteration will also be relaxed
    bool relaxLastIteration_;

    //- should we relax?
    bool doRelax(bool last);

    //- read expression and dimension
    void readExpressionAndDimension(
        const dictionary &dict,
        const word &name,
        exprString &expr,
        dimensionSet &dim
    );

    fv::optionList &fvOptions() const;

private:
    //- do the actual solving
    virtual void solve() = 0;

protected:
    //- Wrapper around solve()
    void solveWrapper();

    //- write specific data
    virtual void writeData() = 0;

    static const NamedEnum<solveAt,3> solveAtNames_;
    solveAt solveAt_;

    //- should the fields before and after be written?
    bool writeBeforeAfter_;

    virtual void writeOldField() = 0;

    virtual void writeNewField() = 0;

public:

    //- Runtime type information
    TypeName("solvePDECommon");


    // Constructors

    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    solvePDECommon
    (
        const word& name,
        const objectRegistry&,
        const dictionary&,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~solvePDECommon();


    // Member Functions

    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();

    //- Read the forces data
    virtual void read(const dictionary&);

    //- Execute
    virtual void execute();

    //- Execute at the final time-loop
    virtual void end();

    //- Write the forces
    virtual void write();

    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
        {}

    //- Update for changes of mesh
    virtual void movePoints(const pointField&)
        {}

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif
