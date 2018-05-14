/*---------------------------------------------------------------------------*\
Copyright (C) 2013 K. Jareteg
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::ProcessorBlockLduInterfaceField

Description
    Abstract base class for processor coupled interfaces.

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

Author
    Klas Jareteg, 2013-02-08

SourceFiles
    ProcessorBlockLduInterfaceField.cpp

\*---------------------------------------------------------------------------*/

#ifndef ProcessorBlockLduInterfaceField_HPP
#define ProcessorBlockLduInterfaceField_HPP

#include "primitiveFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "Field.hpp"
#include "diagTensorField.hpp"
#include "transformField.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
               Class ProcessorBlockLduInterfaceField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class ProcessorBlockLduInterfaceField
{
public:

    //- Runtime type information
    TypeName("ProcessorBlockLduInterfaceField");

    //- Construct given coupled patch
    ProcessorBlockLduInterfaceField()
    {}

    virtual ~ProcessorBlockLduInterfaceField();

    // Member Functions
    //- Return processor number
    virtual int myProcNo() const = 0;

    //- Return neigbour processor number
    virtual int neighbProcNo() const = 0;

    //- Is the transform required
    virtual bool doTransform() const = 0;

    //- Return face transformation tensor
    virtual const tensorField& forwardT() const = 0;

    //- Transform given patch field
    void transformCoupleField
    (
        Field<Type>& f
    ) const;
};

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
ProcessorBlockLduInterfaceField<Type>::~ProcessorBlockLduInterfaceField()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void ProcessorBlockLduInterfaceField<Type>::transformCoupleField
(
    Field<Type>& f
) const
{
    // KRJ: 2013-02-08: Transform not tested
    if (doTransform())
    {
        if (forwardT().size() == 1)
        {
            transform(f, forwardT()[0], f);
        }
        else
        {
            transform(f, forwardT(), f);
        }
    }
}


} // End namespace CML
#endif
