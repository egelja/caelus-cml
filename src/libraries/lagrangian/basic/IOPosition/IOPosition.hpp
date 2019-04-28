/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::IOPosition

Description
    Helper IO class to read and write particle positions

SourceFiles
    IOPosition.cpp

\*---------------------------------------------------------------------------*/

#ifndef IOPosition_H
#define IOPosition_H

#include "cloud_.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class IOPosition Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class IOPosition
:
    public regIOobject
{

    // Private data

        //- Reference to the cloud
        const CloudType& cloud_;


public:

    // Static data

        //- Runtime type name information. Use cloud type.
        virtual const word& type() const
        {
            return Cloud<typename CloudType::particleType>::typeName;
        }


    // Constructors

        //- Construct from cloud
        IOPosition(const CloudType&);


    // Member functions

        //- Inherit readData from regIOobject
        using regIOobject::readData;

        virtual void readData(CloudType& c, bool checkClass);

        virtual bool write() const;

        virtual bool writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::IOPosition<CloudType>::IOPosition(const CloudType& c)
:
    regIOobject
    (
        IOobject
        (
            "positions",
            c.time().timeName(),
            c,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    cloud_(c)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::IOPosition<CloudType>::write() const
{
    if (cloud_.size())
    {
        return regIOobject::write();
    }
    else
    {
        return true;
    }
}


template<class CloudType>
bool CML::IOPosition<CloudType>::writeData(Ostream& os) const
{
    os  << cloud_.size() << nl << token::BEGIN_LIST << nl;

    forAllConstIter(typename CloudType, cloud_, iter)
    {
        iter().writePosition(os);
        os  << nl;
    }

    os  << token::END_LIST << endl;

    return os.good();
}


template<class CloudType>
void CML::IOPosition<CloudType>::readData(CloudType& c, bool checkClass)
{
    const polyMesh& mesh = c.pMesh();

    Istream& is = readStream(checkClass ? typeName : "");

    token firstToken(is);

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        is.readBeginList(FUNCTION_NAME);

        for (label i=0; i<s; i++)
        {
            // Read position only
            c.append(new typename CloudType::particleType(mesh, is, false));
        }

        // Read end of contents
        is.readEndList(FUNCTION_NAME);
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorInFunction
            (
                is
            )   << "incorrect first token, '(', found "
                << firstToken.info() << exit(FatalIOError);
        }

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

            // Read position only
            c.append(new typename CloudType::particleType(mesh, is, false));
            is  >> lastToken;
        }
    }
    else
    {
        FatalIOErrorInFunction
        (
            is
        )   << "incorrect first token, expected <int> or '(', found "
            << firstToken.info() << exit(FatalIOError);
    }

    // Check state of IOstream
    is.check(FUNCTION_NAME);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
