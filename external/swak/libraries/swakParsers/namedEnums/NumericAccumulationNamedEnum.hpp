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
    CML::NumericAccumulationNamedEnum

Description

SourceFiles
    NumericAccumulationNamedEnum.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef NumericAccumulationNamedEnum_H
#define NumericAccumulationNamedEnum_H

#include "NamedEnum.hpp"
#include "dictionary.hpp"

#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class NumericAccumulationNamedEnum Declaration
\*---------------------------------------------------------------------------*/

class NumericAccumulationNamedEnum
{
    // Private data

public:

    enum value {
        numMin,
        numMax,
        numAverage,
        numWeightedAverage,
        numSum,
        numSumMag,
        numWeightedSum,
        numIntegrate,
        numMedian,
        numWeightedMedian,
        numQuantile,
        numWeightedQuantile,
        numRange,
        numWeightedRange,
        numSmaller,
        numWeightedSmaller,
        numBigger,
        numWeightedBigger,
        numWeightSum,
        numSize
    };

    typedef Tuple2<value,scalar> accuSpecification;

    //- String representation of order enums
    static const NamedEnum<value, 20> names;

    static List<accuSpecification> readAccumulations(
        const dictionary &dict,
        const word &name
    );

    static List<accuSpecification> readAccumulations(
        const wordList &names,
        const fileName &name
    );

    static word toString(const accuSpecification &);
};

Ostream &operator<<(
    Ostream &,
    const NumericAccumulationNamedEnum::accuSpecification &
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
