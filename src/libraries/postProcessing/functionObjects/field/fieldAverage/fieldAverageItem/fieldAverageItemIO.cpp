/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.
 
    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fieldAverageItem.hpp"
#include "IOstreams.hpp"
#include "dictionaryEntry.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fieldAverageItem::fieldAverageItem(Istream& is)
:
    active_(false),
    fieldName_("unknown"),
    mean_(0),
    meanFieldName_("unknown"),
    prime2Mean_(0),
    prime2MeanFieldName_("unknown"),
    base_(ITER),
    window_(-1.0)
{
    is.check("CML::fieldAverageItem::fieldAverageItem(CML::Istream&)");

    const dictionaryEntry entry(dictionary::null, is);

    fieldName_ = entry.keyword();
    entry.lookup("mean") >> mean_;
    entry.lookup("prime2Mean") >> prime2Mean_;
    base_ = baseTypeNames_[entry.lookup("base")];
    window_ = entry.lookupOrDefault<scalar>("window", -1.0);
    windowName_ = entry.lookupOrDefault<word>("windowName", "");

    meanFieldName_ = fieldName_ + EXT_MEAN;
    prime2MeanFieldName_ = fieldName_ + EXT_PRIME2MEAN;
    if ((window_ > 0) && (windowName_ != ""))
    {
        meanFieldName_ = meanFieldName_ + "_" + windowName_;
        prime2MeanFieldName_ = prime2MeanFieldName_ + "_" + windowName_;
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, fieldAverageItem& faItem)
{
    is.check
    (
        "CML::Istream& CML::operator>>"
        "(CML::Istream&, CML::fieldAverageItem&)"
    );

    const dictionaryEntry entry(dictionary::null, is);

    faItem.active_ = false;
    faItem.fieldName_ = entry.keyword();
    entry.lookup("mean") >> faItem.mean_;
    entry.lookup("prime2Mean") >> faItem.prime2Mean_;
    faItem.base_ = faItem.baseTypeNames_[entry.lookup("base")];
    faItem.window_ = entry.lookupOrDefault<scalar>("window", -1.0);
    faItem.windowName_ = entry.lookupOrDefault<word>("windowName", "");

    faItem.meanFieldName_ = faItem.fieldName_ + fieldAverageItem::EXT_MEAN;
    faItem.prime2MeanFieldName_ =
        faItem.fieldName_ + fieldAverageItem::EXT_PRIME2MEAN;

    if ((faItem.window_ > 0) && (faItem.windowName_ != ""))
    {
        faItem.meanFieldName_ =
            faItem.meanFieldName_ + "_" + faItem.windowName_;

        faItem.prime2MeanFieldName_ =
            faItem.prime2MeanFieldName_ + "_" + faItem.windowName_;
    }
    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const fieldAverageItem& faItem)
{
    os.check
    (
        "CML::Ostream& CML::operator<<"
        "(CML::Ostream&, const CML::fieldAverageItem&)"
    );

    os  << faItem.fieldName_ << nl << token::BEGIN_BLOCK << nl;
    os.writeKeyword("mean") << faItem.mean_ << token::END_STATEMENT << nl;
    os.writeKeyword("prime2Mean") << faItem.mean_
        << token::END_STATEMENT << nl;
    os.writeKeyword("base") << faItem.baseTypeNames_[faItem.base_]
        << token::END_STATEMENT << nl;

    if (faItem.window_ > 0)
    {
        os.writeKeyword("window") << faItem.window_
            << token::END_STATEMENT << nl;

        if (faItem.windowName_ != "")
        {
            os.writeKeyword("windowName") << faItem.windowName_
                << token::END_STATEMENT << nl;
        }
    }

    os  << token::END_BLOCK << nl;

    os.check
    (
        "CML::Ostream& CML::operator<<"
        "(CML::Ostream&, const CML::fieldAverageItem&)"
    );

    return os;
}


// ************************************************************************* //
