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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "NumericAccumulationNamedEnum.hpp"
#include "HashSet.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<>
const char* NamedEnum
<
    NumericAccumulationNamedEnum::value,
    20
>::names[] =
{
    "min",
    "max",
    "average",
    "weightedAverage",
    "sum",         // 5
    "sumMag",
    "weightedSum",
    "integrate",
    "median",
    "weightedMedian",   // 10
    "quantile",
    "weightedQuantile",
    "range",
    "weightedRange",
    "smaller",          // 15
    "weightedSmaller",
    "bigger",
    "weightedBigger",
    "weightSum",
    "size",             // 20
};

const NamedEnum<NumericAccumulationNamedEnum::value, 20>
    NumericAccumulationNamedEnum::names;


List<NumericAccumulationNamedEnum::accuSpecification>
NumericAccumulationNamedEnum::readAccumulations(
    const dictionary &dict,
    const word &name
) {
    const wordList aNames(dict.lookup(name));

    return readAccumulations(
        aNames,
        dict.name()
    );
}

List<NumericAccumulationNamedEnum::accuSpecification>
NumericAccumulationNamedEnum::readAccumulations(
    const wordList &aNames,
    const fileName &name
) {
    HashSet<NumericAccumulationNamedEnum::value> needsArgument;
    needsArgument.insert(numQuantile);
    needsArgument.insert(numRange);
    needsArgument.insert(numSmaller);
    needsArgument.insert(numBigger);
    needsArgument.insert(numWeightedQuantile);
    needsArgument.insert(numWeightedRange);
    needsArgument.insert(numWeightedSmaller);
    needsArgument.insert(numWeightedBigger);

    List<accuSpecification> accus(aNames.size());

    forAll(aNames,i) {
        scalar value=-HUGE_VAL;
        bool hasValue=false;

        string aName;
        // valid starts for a floating point number
        size_t numPos=aNames[i].find_first_of("-.0123456789");
        if(numPos==std::string::npos) {
            aName=aNames[i];
        } else {
            aName=aNames[i](numPos);
            value=readScalar(
                IStringStream(aNames[i](numPos,aNames[i].size()-numPos))()
            );
            hasValue=true;
        }
        if(!names.found(aName)) {
            // setting it up to fail
            Info << endl << "Problem in " << name
                << " with the entry " << name << ":" << endl;
        }

        accus[i].first()=names[aName];

        if(hasValue) {
            if(!needsArgument.found(accus[i].first())) {
                FatalErrorInFunction
                    << "Problem in " << name << " with " << aNames[i]
                    << endl
                    << "Accumulator " << aName
                    << " does not need an argument" << endl
                    << exit(FatalError);
            }
        } else {
            if(needsArgument.found(accus[i].first())) {
                FatalErrorInFunction
                    << "Problem in " << name << " with " << aNames[i]
                    << endl
                    << "Accumulator " << aName
                    << " needs an argument" << endl
                    << exit(FatalError);
            }
        }

        accus[i].second()=value;
    }
    return accus;
}

word NumericAccumulationNamedEnum::toString(const accuSpecification &accu)
{
    OStringStream o;
    o << NumericAccumulationNamedEnum::names[accu.first()];
    if(accu.second()>-HUGE_VAL) {
        o << accu.second();
    }
    return word(o.str());
}

Ostream &operator<<(
    Ostream &o,
    const NumericAccumulationNamedEnum::accuSpecification &accu
)
{
    o << NumericAccumulationNamedEnum::toString(accu);
    return o;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
