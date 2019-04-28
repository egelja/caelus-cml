/*---------------------------------------------------------------------------*\
Copyright (C) 2013 K. Jareteg
Copyright (C) 2018 Applied CCM Pty Ltd
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
    BlockCoeffMaxNorm

Description
    Class for BlockCoeff maximum element magnitude norm.
    
    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    The original file contained "For copyright notice see file Copyright",
    but no file named Copyright could be found in FOAM-Extend-4.0 repository.

    The copyright has therefore been attributed to the authour named in the
    original file, as shown below.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Klas Jareteg, 2013-03-07

\*---------------------------------------------------------------------------*/

#ifndef BlockCoeffMaxNorm_HPP
#define BlockCoeffMaxNorm_HPP

#include "BlockCoeffNorm.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //


/*---------------------------------------------------------------------------*\
                        Class BlockCoeffMaxNorm Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCoeffMaxNorm
:
    public BlockCoeffNorm<Type>
{

    //- Control data dictionary
    dictionary dict_;


protected:

    //- Return dictionary
    const dictionary& dict() const
    {
        return dict_;
    }


public:

    //- Runtime type information
    TypeName("maxNorm");


    //- Construct from dictionary
    BlockCoeffMaxNorm
    (
        const dictionary& dict
    );


    virtual ~BlockCoeffMaxNorm()
    {}


    // Member functions

    //- Solve
    virtual scalar normalize
    (
        const BlockCoeff<Type>& a
    );

    virtual void coeffMag
    (
        const CoeffField<Type>& a,
        Field<scalar>& b
    );
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::BlockCoeffMaxNorm<Type>::BlockCoeffMaxNorm
(
    const dictionary& dict
)
:
    BlockCoeffNorm<Type>(dict),
    dict_(dict)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
scalar BlockCoeffMaxNorm<Type>::normalize
(
    const BlockCoeff<Type>& a
)
{
    if (a.activeType() == BlockCoeff<Type>::SCALAR)
    {
        return mag(a.asScalar());
    }
    else if (a.activeType() == BlockCoeff<Type>::LINEAR)
    {
        return cmptMax(cmptMag(a.asLinear()));
    }
    else if (a.activeType() == BlockCoeff<Type>::SQUARE)
    {
        return cmptMax(cmptMag(a.asSquare()));
    }
    else
    {
        FatalErrorInFunction
            << "Unknown type" << abort(FatalError);

        return 0;
    }
}


template<class Type>
void BlockCoeffMaxNorm<Type>::coeffMag
(
    const CoeffField<Type>& a,
    Field<scalar>& b
)
{
    if (a.activeType() == BlockCoeff<Type>::SCALAR)
    {
        b = mag(a.asScalar());
    }
    else if (a.activeType() == BlockCoeff<Type>::LINEAR)
    {
        b = cmptMax(cmptMag(a.asLinear()));
    }
    else if (a.activeType() == BlockCoeff<Type>::SQUARE)
    {
        b = cmptMax(cmptMag(a.asSquare()));
    }
    else
    {
        FatalErrorInFunction
            << "Unknown type" << abort(FatalError);
    }
}


} // End namespace CML
#endif
