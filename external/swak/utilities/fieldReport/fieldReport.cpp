/*---------------------------------------------------------------------------*\
Copyright (C) 1991-2005 OpenCFD Ltd.
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

Application
    fieldReport

Description

Contributors/Copyright:
    2011-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"

#include "FieldValueExpressionDriver.hpp"
#include "PatchValueExpressionDriver.hpp"
#include "CellZoneValueExpressionDriver.hpp"
#include "FaceZoneValueExpressionDriver.hpp"
#include "CellSetValueExpressionDriver.hpp"
#include "FaceSetValueExpressionDriver.hpp"

#include "timeSelector.hpp"

#include "IFstream.hpp"

#include "printSwakVersion.hpp"

#include "AccumulationCalculation.hpp"

#include "IOobjectList.hpp"
#include "IOmanip.hpp"

// Yeah. I know. Global variables. Eeeeevil. But convenient
label nrOfQuantiles=0;
scalarList fractionBiggerThan(0);
scalarList fractionSmallerThan(0);

bool doField=true;
bool doBoundary=false;
bool doZones=false;
bool doSets=false;

string csvHeader="";
string csvLine="";
string csvText="";

fileName distributionsDirectory="";

scalarList splitValuesString (
    const string &values
) {
    DynamicList<scalar> result;

    std::string::size_type start=0;
    std::string::size_type end=0;

    while(start<values.length()) {
        end=values.find(',',start);
        if(end==std::string::npos) {
            end=values.length();
        }
        result.append(
            readScalar(
                IStringStream(
                    values.substr(
                        start,end-start
                    )
                )()
            )
        );
        start=end+1;
    }

    result.shrink();

    return result;
}

void startLine(
    const Time &time,
    const string &entName
) {
    //    Info << csvHeader << endl << csvLine << endl;
    if(csvLine!="") {
        csvText+=csvLine+"\n";
    }
    csvHeader="time,entity";
    csvLine=time.timeName()+","+entName;
}

void endTime() {
    if(csvLine!="") {
        csvText+=csvLine+"\n";
    }
    csvLine="";
    csvHeader+="\n";
}

template <typename Type>
Ostream& writeValue(
    Ostream &o,
    Type value
) {
    for(direction i=0;i<pTraits<Type>::nComponents;i++) {
        o << setw(IOstream::defaultPrecision() + 7)
            << component(value,i);
    }
    return o;
}

template <typename Type>
Ostream& writeData(
    Ostream &o,
    Type value,
    const string annotation,
    const word header,
    bool newLine=false
) {
    const label annotationLen=25;
    if(annotation!="") {
        o << annotation.c_str();
        for(label i=0;i<(annotationLen-label(annotation.size()));i++) {
            o << " ";
        }
    }
    writeValue(o,value);
    for(direction i=0;i<pTraits<Type>::nComponents;i++) {
        scalar v=component(value,i);
        csvHeader+=","+header;
        if(pTraits<Type>::nComponents>1) {
            csvHeader+=string(" ")+pTraits<Type>::componentNames[i];
        }
        OStringStream s;
        s << v;
        csvLine+=","+s.str();
    }
    if(newLine) {
        o << endl;
    }
    return o;
}

template <typename Type>
void reportValues(
    const word &fieldName,
    const fvMesh &mesh,
    CommonValueExpressionDriver &driver,
    const string &entName
) {
    startLine(
        mesh.time(),
        entName
    );

    Field<Type> result(
        driver.evaluate<Type>(
            exprString(
                fieldName,
                dictionary::null
            )
        )
    );

    AccumulationCalculation<Type> calculator(
        result,
        false,
        driver
    );

    writeData(Info,calculator.size(),"Size | Weight Sum","size");
    Info << " | ";
    writeData(Info,calculator.weightSum(),"","weight_sum",true);

    writeData(Info,calculator.minimum(),"Range (min-max)","minimum");
    Info << " | ";
    writeData(Info,calculator.maximum(),"","maximum",true);

    writeData(
        Info,calculator.average(),
        "Average | weighted","average");
    Info << " | ";
    writeData(
        Info,calculator.weightedAverage(),
        "","average_weighted",true);

    writeData(
        Info,calculator.sum(),
        "Sum | weighted","sum");
    Info << " | ";
    writeData(
        Info,calculator.weightedSum(),
        "","sum_weighted",true);

    writeData(
        Info,calculator.distribution().median(),
        "Median | weighted","median");
    Info << " | ";
    writeData(
        Info,calculator.weightedDistribution().median(),
        "","median_weighted",true);

    if(nrOfQuantiles) {
        scalar dx=1./nrOfQuantiles;
        for(label i=1;i<nrOfQuantiles;i++) {
            scalar thisQuantile=dx*i;
            NumericAccumulationNamedEnum::accuSpecification quant(
                NumericAccumulationNamedEnum::numQuantile,
                thisQuantile
            );
            NumericAccumulationNamedEnum::accuSpecification wquant(
                NumericAccumulationNamedEnum::numWeightedQuantile,
                thisQuantile
            );
            OStringStream annotation;
            annotation << thisQuantile << " quantile | weighted";

            writeData(
                Info,calculator(quant),
                annotation.str(),NumericAccumulationNamedEnum::toString(quant));
            Info << " | ";
            writeData(
                Info,calculator(wquant),
                "",NumericAccumulationNamedEnum::toString(wquant),true);
        }
    }

    forAll(fractionSmallerThan,i) {
        scalar value=fractionSmallerThan[i];
        NumericAccumulationNamedEnum::accuSpecification smaller(
            NumericAccumulationNamedEnum::numSmaller,
            value
        );
        NumericAccumulationNamedEnum::accuSpecification wsmaller(
            NumericAccumulationNamedEnum::numWeightedSmaller,
            value
        );
        OStringStream annotation;
        annotation << "x <= " << value << " | weighted";

        writeData(
            Info,calculator(smaller),
            annotation.str(),NumericAccumulationNamedEnum::toString(smaller));
        Info << " | ";
        writeData(
            Info,calculator(wsmaller),
            "",NumericAccumulationNamedEnum::toString(wsmaller),true);
    }
    forAll(fractionBiggerThan,i) {
        scalar value=fractionBiggerThan[i];
        NumericAccumulationNamedEnum::accuSpecification bigger(
            NumericAccumulationNamedEnum::numBigger,
            value
        );
        NumericAccumulationNamedEnum::accuSpecification wbigger(
            NumericAccumulationNamedEnum::numWeightedBigger,
            value
        );
        OStringStream annotation;
        annotation << "x > " << value << " | weighted";

        writeData(
            Info,calculator(bigger),
            annotation.str(),NumericAccumulationNamedEnum::toString(bigger));
        Info << " | ";
        writeData(
            Info,calculator(wbigger),
            "",NumericAccumulationNamedEnum::toString(wbigger),true);
    }

    if(distributionsDirectory!="") {
        string name(entName);
        name.replaceAll(" ","_");

        fileName toDir=distributionsDirectory
            /
            mesh.time().timeName();

        mkDir(toDir);

        Info << "\nWriting distributions to " << toDir << endl;
        calculator.distribution().write(
            toDir
            /
            fieldName+"_"+name
        );
        calculator.weightedDistribution().write(
            toDir
            /
            "weighted_"+fieldName+"_"+name
        );
    }
}

template <typename Type,typename DriverType>
void reportZones(
    const word &fieldName,
    const fvMesh &mesh,
    const ZoneMesh<typename DriverType::EntityType,polyMesh> &zoneMesh
) {
    forAll(zoneMesh,i) {
        Info << "\n" << DriverType::EntityType::typeName << " "
            << zoneMesh[i].name() << endl;

        DriverType zoneDriver(zoneMesh[i]);
        zoneDriver.setAutoInterpolate(true,false);
        reportValues<typename Type::value_type>(
            fieldName,
            mesh,
            zoneDriver,
            DriverType::EntityType::typeName+" "+zoneMesh[i].name()
        );
    }
}

template<class SetType>
wordList findSets(const fvMesh &mesh)
{
    IOobjectList objects
        (
            mesh,
            mesh.pointsInstance(),
            polyMesh::meshSubDir/"sets"
        );

    return objects.lookupClass(SetType::typeName).names();
}

 template <typename Type,typename DriverType>
void reportSets(
    const word &fieldName,
    const fvMesh &mesh,
    const wordList &setNames
) {
    forAll(setNames,i) {
        Info << "\n" << DriverType::EntityType::typeName << " "
            << setNames[i] << endl;

        typename DriverType::EntityType theSet(
            mesh,
            setNames[i]
        );
        DriverType setDriver(theSet);
        setDriver.setAutoInterpolate(true,false);
        reportValues<typename Type::value_type>(
            fieldName,
            mesh,
            setDriver,
            DriverType::EntityType::typeName+" "+setNames[i]
        );
    }
}

template <typename Type>
bool reportAField(
    const word &fieldName,
    const fvMesh &mesh
) {
    IOobject header
        (
            fieldName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );
    if(
        header.headerOk()
        &&
        header.headerClassName()==Type::typeName
    ) {
        Info << " Reading Field " << fieldName << " of type "
            << Type::typeName << endl;
    } else {
        return false;
    }

    Type theField(
        IOobject
        (
            fieldName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        mesh
    );

    if(doField) {
        Info << "\nInternal field:" << endl;
        FieldValueExpressionDriver fieldDriver(
            mesh,
            false,
            true,     // search in Memory
            false
        );
        reportValues<typename Type::value_type>(
            fieldName,
            mesh,
            fieldDriver,
            "internalField"
        );
    }

    if(doBoundary) {
        HashSet<word> isConstraint(fvPatch::constraintTypes());
        const fvBoundaryMesh & bound=mesh.boundary();
        forAll(bound,patchI) {
            if(!isConstraint.found(bound[patchI].type())) {
                Info << "\nPatch field: " << bound[patchI].name() << endl;
                PatchValueExpressionDriver patchDriver(
                    bound[patchI]
                );
                reportValues<typename Type::value_type>(
                    fieldName,
                    mesh,
                    patchDriver,
                    "patch "+bound[patchI].name()
                );
            }
        }
    }

    if(doZones) {
        if(
            Type::PatchFieldType::typeName
            ==
            "fvPatchField"
        ) {
            reportZones<Type,CellZoneValueExpressionDriver>(
                fieldName,
                mesh,
                mesh.cellZones()
            );
        } else if(mesh.cellZones().size()>0) {
            Info << "\nSkipping cellZones because field type "
                << Type::typeName << " is not supported for them"
                << endl;
        }

        reportZones<Type,FaceZoneValueExpressionDriver>(
            fieldName,
            mesh,
            mesh.faceZones()
        );
    }

    if(doSets) {
        wordList cellSets=findSets<cellSet>(mesh);

        if(
            Type::PatchFieldType::typeName
            ==
            "fvPatchField"
        ) {
            reportSets<Type,CellSetValueExpressionDriver>(
                fieldName,
                mesh,
                cellSets
            );
        } else if(cellSets.size()>0) {
            Info << "\nSkipping cellSets because field type "
                << Type::typeName << " is not supported for them"
                << endl;
        }

        wordList faceSets=findSets<faceSet>(mesh);
        reportSets<Type,FaceSetValueExpressionDriver>(
            fieldName,
            mesh,
            faceSets
        );
    }

    endTime();

    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

    CML::timeSelector::addOptions(false);
    CML::argList::validArgs.append("fieldName");
#   include "addRegionOption.hpp"
    argList::validOptions.insert(
        "nrOfQuantiles",
        "<nr of quantiles - defaults to 9>"
    );
    argList::validOptions.insert(
        "fractionBiggerThan",
        "<comma separated list of values>"
    );
    argList::validOptions.insert(
        "fractionSmallerThan",
        "<comma separated list of values>"
    );
    argList::validOptions.insert("noDoField","");
    argList::validOptions.insert("doBoundary","");
    argList::validOptions.insert("doZones","");
    argList::validOptions.insert("doSets","");

    argList::validOptions.insert("csvName","<name of the CSV-file to be written>");
    argList::validOptions.insert("distributionsDirectory","<name of the directory where distributions are written to>");

#   include "setRootCase.hpp"

    printSwakVersion();

    word fieldName(args.args()[1]);
    nrOfQuantiles=0;
    if(args.options().found("nrOfQuantiles")) {
        nrOfQuantiles=readLabel(
            IStringStream(args.options()["nrOfQuantiles"])()
        );
    }
    if(args.options().found("fractionBiggerThan")) {
        fractionBiggerThan=splitValuesString(
            args.options()["fractionBiggerThan"]
        );
    }
    if(args.options().found("fractionSmallerThan")) {
        fractionSmallerThan=splitValuesString(
            args.options()["fractionSmallerThan"]
        );
    }

    doField=!args.options().found("noDoField");
    doBoundary=args.options().found("doBoundary");
    doZones=args.options().found("doZones");
    doSets=args.options().found("doSets");

    if (!args.options().found("time") && !args.options().found("latestTime")) {
        FatalErrorInFunction
            << args.executable()
            << ": time/latestTime option is required" << endl
            << exit(FatalError);
    }


#   include "createTime.hpp"
    CML::instantList timeDirs = CML::timeSelector::select0(runTime, args);

#   include "createNamedMesh.hpp"

    if(args.options().found("distributionsDirectory")) {
        distributionsDirectory=args.options()["distributionsDirectory"];
    }
    autoPtr<OFstream> csv;
    if(args.options().found("csvName")) {
        fileName csvName=args.options()["csvName"]+"_"
            +fieldName+"_"+mesh.name()+".csv";
        csv.set(
            new OFstream(csvName)
        );
    }

    string oldCsvHeader="";

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info << "\nTime = " << runTime.timeName() << endl;
        Info << endl;

        mesh.readUpdate();

        bool found=false;
        found = found || reportAField<volScalarField>(fieldName,mesh);
        found = found || reportAField<volVectorField>(fieldName,mesh);

        found = found || reportAField<surfaceScalarField>(fieldName,mesh);

        //        found = found || reportAField<pointScalarField>(fieldName,mesh);

        if(!found) {
            WarningInFunction
                << "No field " << fieldName << " found at "
                << mesh.time().timeName()
                << endl;
        } else {
            if(csv.valid()) {
                if(oldCsvHeader=="") {
                    csv().write(csvHeader.c_str());
                } else if(csvHeader!=oldCsvHeader) {
                    WarningInFunction
                        << "Current and old CSV-header differ. "
                        << "Probably the field changed the data-type"
                        << endl
                        << "Current: " << csvHeader << endl
                        << "Old: " << oldCsvHeader << endl;
                }
                csv().write(csvText.c_str());
                csvText="";
                oldCsvHeader=csvHeader;
                csv().flush();
            }
        }

        Info << endl;
   }

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
