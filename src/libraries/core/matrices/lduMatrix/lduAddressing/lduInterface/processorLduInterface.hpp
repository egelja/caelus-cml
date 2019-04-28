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

Class
    CML::processorLduInterface

Description
    An abstract base class for processor coupled interfaces.

SourceFiles
    processorLduInterface.cpp
    processorLduInterfaceTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef processorLduInterface_H
#define processorLduInterface_H

#include "lduInterface.hpp"
#include "primitiveFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class processorLduInterface Declaration
\*---------------------------------------------------------------------------*/

class processorLduInterface
{
    // Private data

        //- Send buffer.
        //  Only sized and used when compressed or non-blocking comms used.
        mutable List<char> sendBuf_;

        //- Receive buffer.
        //  Only sized and used when compressed or non-blocking comms used.
        mutable List<char> receiveBuf_;

        //- Resize the buffer if required
        void resizeBuf(List<char>& buf, const label size) const;


public:

    //- Runtime type information
    TypeName("processorLduInterface");


    // Constructors

        //- Construct null
        processorLduInterface();


    //- Destructor
    virtual ~processorLduInterface();


    // Member Functions

        // Access

            //- Return processor number
            virtual int myProcNo() const = 0;

            //- Return neighbour processor number
            virtual int neighbProcNo() const = 0;

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const = 0;

            //- Return message tag used for sending
            virtual int tag() const = 0;

        // Transfer functions

            //- Raw send function
            template<class Type>
            void send
            (
                const Pstream::commsTypes commsType,
                const UList<Type>&
            ) const;

            //- Raw field receive function
            template<class Type>
            void receive
            (
                const Pstream::commsTypes commsType,
                UList<Type>&
            ) const;

            //- Raw field receive function returning field
            template<class Type>
            tmp<Field<Type> > receive
            (
                const Pstream::commsTypes commsType,
                const label size
            ) const;


            //- Raw field send function with data compression
            template<class Type>
            void compressedSend
            (
                const Pstream::commsTypes commsType,
                const UList<Type>&
            ) const;

            //- Raw field receive function with data compression
            template<class Type>
            void compressedReceive
            (
                const Pstream::commsTypes commsType,
                UList<Type>&
            ) const;

            //- Raw field receive function with data compression returning field
            template<class Type>
            tmp<Field<Type> > compressedReceive
            (
                const Pstream::commsTypes commsType,
                const label size
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IPstream.hpp"
#include "OPstream.hpp"

// * * * * * * * * * * * * * * * Member Functions * * *  * * * * * * * * * * //

template<class Type>
void CML::processorLduInterface::send
(
    const Pstream::commsTypes commsType,
    const UList<Type>& f
) const
{
    label nBytes = f.byteSize();

    if (commsType == Pstream::blocking || commsType == Pstream::scheduled)
    {
        OPstream::write
        (
            commsType,
            neighbProcNo(),
            reinterpret_cast<const char*>(f.begin()),
            nBytes,
            tag()
        );
    }
    else if (commsType == Pstream::nonBlocking)
    {
        resizeBuf(receiveBuf_, nBytes);

        IPstream::read
        (
            commsType,
            neighbProcNo(),
            receiveBuf_.begin(),
            nBytes,
            tag()
        );

        resizeBuf(sendBuf_, nBytes);
        memcpy(sendBuf_.begin(), f.begin(), nBytes);

        OPstream::write
        (
            commsType,
            neighbProcNo(),
            sendBuf_.begin(),
            nBytes,
            tag()
        );
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type " << commsType
            << exit(FatalError);
    }
}


template<class Type>
void CML::processorLduInterface::receive
(
    const Pstream::commsTypes commsType,
    UList<Type>& f
) const
{
    if (commsType == Pstream::blocking || commsType == Pstream::scheduled)
    {
        IPstream::read
        (
            commsType,
            neighbProcNo(),
            reinterpret_cast<char*>(f.begin()),
            f.byteSize(),
            tag()
        );
    }
    else if (commsType == Pstream::nonBlocking)
    {
        memcpy(f.begin(), receiveBuf_.begin(), f.byteSize());
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type " << commsType
            << exit(FatalError);
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::processorLduInterface::receive
(
    const Pstream::commsTypes commsType,
    const label size
) const
{
    tmp<Field<Type> > tf(new Field<Type>(size));
    receive(commsType, tf());
    return tf;
}


template<class Type>
void CML::processorLduInterface::compressedSend
(
    const Pstream::commsTypes commsType,
    const UList<Type>& f
) const
{
    if (sizeof(scalar) != sizeof(float) && Pstream::floatTransfer && f.size())
    {
        static const label nCmpts = sizeof(Type)/sizeof(scalar);
        label nm1 = (f.size() - 1)*nCmpts;
        label nlast = sizeof(Type)/sizeof(float);
        label nFloats = nm1 + nlast;
        label nBytes = nFloats*sizeof(float);

        const scalar *sArray = reinterpret_cast<const scalar*>(f.begin());
        const scalar *slast = &sArray[nm1];
        resizeBuf(sendBuf_, nBytes);
        float *fArray = reinterpret_cast<float*>(sendBuf_.begin());

        for (register label i=0; i<nm1; i++)
        {
            fArray[i] = sArray[i] - slast[i%nCmpts];
        }

        reinterpret_cast<Type&>(fArray[nm1]) = f.last();

        if (commsType == Pstream::blocking || commsType == Pstream::scheduled)
        {
            OPstream::write
            (
                commsType,
                neighbProcNo(),
                sendBuf_.begin(),
                nBytes,
                tag()
            );
        }
        else if (commsType == Pstream::nonBlocking)
        {
            resizeBuf(receiveBuf_, nBytes);

            IPstream::read
            (
                commsType,
                neighbProcNo(),
                receiveBuf_.begin(),
                nBytes,
                tag()
            );

            OPstream::write
            (
                commsType,
                neighbProcNo(),
                sendBuf_.begin(),
                nBytes,
                tag()
            );
        }
        else
        {
            FatalErrorInFunction
                << "Unsupported communications type " << commsType
                << exit(FatalError);
        }
    }
    else
    {
        this->send(commsType, f);
    }
}

template<class Type>
void CML::processorLduInterface::compressedReceive
(
    const Pstream::commsTypes commsType,
    UList<Type>& f
) const
{
    if (sizeof(scalar) != sizeof(float) && Pstream::floatTransfer && f.size())
    {
        static const label nCmpts = sizeof(Type)/sizeof(scalar);
        label nm1 = (f.size() - 1)*nCmpts;
        label nlast = sizeof(Type)/sizeof(float);
        label nFloats = nm1 + nlast;
        label nBytes = nFloats*sizeof(float);

        if (commsType == Pstream::blocking || commsType == Pstream::scheduled)
        {
            resizeBuf(receiveBuf_, nBytes);

            IPstream::read
            (
                commsType,
                neighbProcNo(),
                receiveBuf_.begin(),
                nBytes,
                tag()
            );
        }
        else if (commsType != Pstream::nonBlocking)
        {
            FatalErrorInFunction
                << "Unsupported communications type " << commsType
                << exit(FatalError);
        }

        const float *fArray =
            reinterpret_cast<const float*>(receiveBuf_.begin());
        f.last() = reinterpret_cast<const Type&>(fArray[nm1]);
        scalar *sArray = reinterpret_cast<scalar*>(f.begin());
        const scalar *slast = &sArray[nm1];

        for (register label i=0; i<nm1; i++)
        {
            sArray[i] = fArray[i] + slast[i%nCmpts];
        }
    }
    else
    {
        this->receive<Type>(commsType, f);
    }
}

template<class Type>
CML::tmp<CML::Field<Type> > CML::processorLduInterface::compressedReceive
(
    const Pstream::commsTypes commsType,
    const label size
) const
{
    tmp<Field<Type> > tf(new Field<Type>(size));
    compressedReceive(commsType, tf());
    return tf;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
