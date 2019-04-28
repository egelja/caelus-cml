/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "mapDistribute.hpp"
#include "globalIndexAndTransform.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
defineTypeNameAndDebug(mapDistribute, 0);
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<>
void CML::mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<label>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    UList<label>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<label>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<label>&
) const
{}


template<>
void CML::mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<scalar>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    UList<scalar>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<scalar>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<scalar>&
) const
{}


template<>
void CML::mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<bool>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    UList<bool>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<bool>&
) const
{}
template<>
void CML::mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<bool>&
) const
{}

void CML::mapDistribute::printLayout(Ostream& os) const
{
    mapDistributeBase::printLayout(os);

    forAll(transformElements_, trafoI)
    {
        if (transformElements_[trafoI].size() > 0)
        {
            os  << "transform " << trafoI << ':' << endl
                << "    start : " << transformStart_[trafoI] << endl
                << "    size  : " << transformElements_[trafoI].size() << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
CML::mapDistribute::mapDistribute()
:
    mapDistributeBase()
{}


//- Construct from components
CML::mapDistribute::mapDistribute
(
    const label constructSize,
    const Xfer<labelListList>& subMap,
    const Xfer<labelListList>& constructMap,
    const bool subHasFlip,
    const bool constructHasFlip
)
:
    mapDistributeBase
    (
        constructSize,
        subMap,
        constructMap,
        subHasFlip,
        constructHasFlip
    )
{}


//- Construct from components
CML::mapDistribute::mapDistribute
(
    const label constructSize,
    const Xfer<labelListList>& subMap,
    const Xfer<labelListList>& constructMap,
    const Xfer<labelListList>& transformElements,
    const Xfer<labelList>& transformStart,
    const bool subHasFlip,
    const bool constructHasFlip
)
:
    mapDistributeBase
    (
        constructSize,
        subMap,
        constructMap,
        subHasFlip,
        constructHasFlip
    ),
    transformElements_(transformElements),
    transformStart_(transformStart)
{}


CML::mapDistribute::mapDistribute
(
    const labelList& sendProcs,
    const labelList& recvProcs
)
:
    mapDistributeBase(sendProcs, recvProcs)
{}


CML::mapDistribute::mapDistribute
(
    const globalIndex& globalNumbering,
    labelList& elements,
    List<Map<label> >& compactMap,
    const int tag
)
:
    mapDistributeBase
    (
        globalNumbering,
        elements,
        compactMap,
        tag
    )
{}


CML::mapDistribute::mapDistribute
(
    const globalIndex& globalNumbering,
    labelListList& cellCells,
    List<Map<label> >& compactMap,
    const int tag
)
:
    mapDistributeBase
    (
        globalNumbering,
        cellCells,
        compactMap,
        tag
    )
{}



CML::mapDistribute::mapDistribute
(
    const globalIndex& globalNumbering,
    labelList& elements,
    const globalIndexAndTransform& globalTransforms,
    const labelPairList& transformedElements,
    labelList& transformedIndices,
    List<Map<label> >& compactMap,
    const int tag
)
:
    mapDistributeBase()
{
    // Construct per processor compact addressing of the global elements
    // needed. The ones from the local processor are not included since
    // these are always all needed.
    calcCompactAddressing
    (
        globalNumbering,
        elements,
        compactMap
    );

    // Add all (non-local) transformed elements needed.
    forAll(transformedElements, i)
    {
        labelPair elem = transformedElements[i];
        label proci = globalIndexAndTransform::processor(elem);
        if (proci != Pstream::myProcNo())
        {
            label index = globalIndexAndTransform::index(elem);
            label nCompact = compactMap[proci].size();
            compactMap[proci].insert(index, nCompact);
        }
    }


    // Exchange what I need with processor that supplies it. Renumber elements
    // into compact numbering
    labelList compactStart;
    exchangeAddressing
    (
        tag,
        globalNumbering,
        elements,
        compactMap,
        compactStart
    );


    // Renumber the transformed elements
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Count per transformIndex
    label nTrafo = globalTransforms.transformPermutations().size();
    labelList nPerTransform(nTrafo, 0);
    forAll(transformedElements, i)
    {
        labelPair elem = transformedElements[i];
        label trafoI = globalIndexAndTransform::transformIndex(elem);
        nPerTransform[trafoI]++;
    }
    // Offset per transformIndex
    transformStart_.setSize(nTrafo);
    transformElements_.setSize(nTrafo);
    forAll(transformStart_, trafoI)
    {
        transformStart_[trafoI] = constructSize_;
        constructSize_ += nPerTransform[trafoI];
        transformElements_[trafoI].setSize(nPerTransform[trafoI]);
    }

    // Sort transformed elements into their new slot.
    nPerTransform = 0;

    transformedIndices.setSize(transformedElements.size());
    forAll(transformedElements, i)
    {
        labelPair elem = transformedElements[i];
        label proci = globalIndexAndTransform::processor(elem);
        label index = globalIndexAndTransform::index(elem);
        label trafoI = globalIndexAndTransform::transformIndex(elem);

        // Get compact index for untransformed element
        label rawElemI =
        (
            proci == Pstream::myProcNo()
          ? index
          : compactMap[proci][index]
        );

        label& n = nPerTransform[trafoI];
        // index of element to transform
        transformElements_[trafoI][n] = rawElemI;
        // destination of transformed element
        transformedIndices[i] = transformStart_[trafoI]+n;
        n++;
    }

    if (debug)
    {
        printLayout(Pout);
    }
}


CML::mapDistribute::mapDistribute
(
    const globalIndex& globalNumbering,
    labelListList& cellCells,
    const globalIndexAndTransform& globalTransforms,
    const List<labelPairList>& transformedElements,
    labelListList& transformedIndices,
    List<Map<label> >& compactMap,
    const int tag
)
:
    mapDistributeBase()
{
    // Construct per processor compact addressing of the global elements
    // needed. The ones from the local processor are not included since
    // these are always all needed.
    calcCompactAddressing
    (
        globalNumbering,
        cellCells,
        compactMap
    );

    // Add all (non-local) transformed elements needed.
    forAll(transformedElements, celli)
    {
        const labelPairList& elems = transformedElements[celli];

        forAll(elems, i)
        {
            label proci = globalIndexAndTransform::processor(elems[i]);
            if (proci != Pstream::myProcNo())
            {
                label index = globalIndexAndTransform::index(elems[i]);
                label nCompact = compactMap[proci].size();
                compactMap[proci].insert(index, nCompact);
            }
        }
    }


    // Exchange what I need with processor that supplies it. Renumber elements
    // into compact numbering
    labelList compactStart;
    exchangeAddressing
    (
        tag,
        globalNumbering,
        cellCells,
        compactMap,
        compactStart
    );


    // Renumber the transformed elements
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Count per transformIndex
    label nTrafo = globalTransforms.transformPermutations().size();
    labelList nPerTransform(nTrafo, 0);
    forAll(transformedElements, celli)
    {
        const labelPairList& elems = transformedElements[celli];

        forAll(elems, i)
        {
            label trafoI = globalIndexAndTransform::transformIndex(elems[i]);
            nPerTransform[trafoI]++;
        }
    }
    // Offset per transformIndex
    transformStart_.setSize(nTrafo);
    transformElements_.setSize(nTrafo);
    forAll(transformStart_, trafoI)
    {
        transformStart_[trafoI] = constructSize_;
        constructSize_ += nPerTransform[trafoI];
        transformElements_[trafoI].setSize(nPerTransform[trafoI]);
    }

    // Sort transformed elements into their new slot.
    nPerTransform = 0;

    transformedIndices.setSize(transformedElements.size());
    forAll(transformedElements, celli)
    {
        const labelPairList& elems = transformedElements[celli];
        transformedIndices[celli].setSize(elems.size());

        forAll(elems, i)
        {
            label proci = globalIndexAndTransform::processor(elems[i]);
            label index = globalIndexAndTransform::index(elems[i]);
            label trafoI = globalIndexAndTransform::transformIndex(elems[i]);

            // Get compact index for untransformed element
            label rawElemI =
            (
                proci == Pstream::myProcNo()
              ? index
              : compactMap[proci][index]
            );

            label& n = nPerTransform[trafoI];
            // index of element to transform
            transformElements_[trafoI][n] = rawElemI;
            // destination of transformed element
            transformedIndices[celli][i] = transformStart_[trafoI]+n;
            n++;
        }
    }

    if (debug)
    {
        printLayout(Pout);
    }
}


CML::mapDistribute::mapDistribute(const mapDistribute& map)
:
    mapDistributeBase(map),
    transformElements_(map.transformElements_),
    transformStart_(map.transformStart_)
{}


CML::mapDistribute::mapDistribute(const Xfer<mapDistribute>& map)
:
    mapDistributeBase
    (
        map().constructSize_,
        map().subMap_.xfer(),
        map().constructMap_.xfer(),
        map().subHasFlip(),
        map().constructHasFlip()
    ),
    transformElements_(map().transformElements_.xfer()),
    transformStart_(map().transformStart_.xfer())
{}


CML::mapDistribute::mapDistribute(Istream& is)
{
    is  >> *this;
}


CML::autoPtr<CML::mapDistribute> CML::mapDistribute::clone() const
{
    return autoPtr<mapDistribute>(new mapDistribute(*this));
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::label CML::mapDistribute::whichTransform(const label index)
const
{
    return findLower(transformStart_, index+1);
}


void CML::mapDistribute::transfer(mapDistribute& rhs)
{
    mapDistributeBase::transfer(rhs);
    transformElements_.transfer(rhs.transformElements_);
    transformStart_.transfer(rhs.transformStart_);
}


CML::Xfer<CML::mapDistribute> CML::mapDistribute::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::mapDistribute::operator=(const mapDistribute& rhs)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }
    mapDistributeBase::operator=(rhs);
    transformElements_ = rhs.transformElements_;
    transformStart_ = rhs.transformStart_;
}


// * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, mapDistribute& map)
{
    is.fatalCheck("operator>>(Istream&, mapDistribute&)");

    is  >> static_cast<mapDistributeBase&>(map)
        >> map.transformElements_ >> map.transformStart_;

    return is;
}


// * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const mapDistribute& map)
{
    os  << static_cast<const mapDistributeBase&>(map) << token::NL
        << map.transformElements_ << token::NL
        << map.transformStart_;

    return os;
}


// ************************************************************************* //
