/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "PtrList.hpp"
#include "SLList.hpp"
#include "Istream.hpp"
#include "Ostream.hpp"
#include "INew.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T>
template<class INew>
void CML::PtrList<T>::read(Istream& is, const INew& inewt)
{
    is.fatalCheck("PtrList<T>::read(Istream&, const INew&)");

    token firstToken(is);

    is.fatalCheck
    (
        "PtrList<T>::read(Istream&, const INew&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        // Read size of list
        label s = firstToken.labelToken();

        setSize(s);

        // Read beginning of contents
        char delimiter = is.readBeginList("PtrList");

        if (s)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                forAll(*this, i)
                {
                    set(i, inewt(is));

                    is.fatalCheck
                    (
                        "PtrList<T>::read(Istream&, const INew&) : "
                        "reading entry"
                    );
                }
            }
            else
            {
                T* tPtr = inewt(is).ptr();
                set(0, tPtr);

                is.fatalCheck
                (
                    "PtrList<T>::read(Istream&, const INew&) : "
                    "reading the single entry"
                );

                for (label i=1; i<s; i++)
                {
                    set(i, tPtr->clone());
                }
            }
        }

        // Read end of contents
        is.readEndList("PtrList");
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorIn
            (
                "PtrList<T>::read(Istream&, const INew&)",
                is
            )   << "incorrect first token, '(', found " << firstToken.info()
                << exit(FatalIOError);
        }

        SLList<T*> sllPtrs;

        token lastToken(is);
        while
        (
           !(
                lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST
            )
        )
        {
            is.putBack(lastToken);

            if (is.eof())
            {
                FatalIOErrorIn
                (
                    "PtrList<T>::read(Istream&, const INew&)",
                    is
                )   << "Premature EOF after reading " << lastToken.info()
                    << exit(FatalIOError);
            }

            sllPtrs.append(inewt(is).ptr());
            is >> lastToken;
        }

        setSize(sllPtrs.size());

        label i = 0;
        for
        (
            typename SLList<T*>::iterator iter = sllPtrs.begin();
            iter != sllPtrs.end();
            ++iter
        )
        {
            set(i++, iter());
        }
    }
    else
    {
        FatalIOErrorIn
        (
            "PtrList<T>::read(Istream&, const INew&)",
            is
        )   << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
template<class INew>
CML::PtrList<T>::PtrList(Istream& is, const INew& inewt)
{
    read(is, inewt);
}


template<class T>
CML::PtrList<T>::PtrList(Istream& is)
{
    read(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class T>
CML::Istream& CML::operator>>(Istream& is, PtrList<T>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class T>
CML::Ostream& CML::operator<<(Ostream& os, const PtrList<T>& L)
{
    // Write size and start delimiter
    os << nl << L.size() << nl << token::BEGIN_LIST;

    // Write contents
    forAll(L, i)
    {
        os << nl << L[i];
    }

    // Write end delimiter
    os << nl << token::END_LIST << nl;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const PtrList&)");

    return os;
}


// ************************************************************************* //
