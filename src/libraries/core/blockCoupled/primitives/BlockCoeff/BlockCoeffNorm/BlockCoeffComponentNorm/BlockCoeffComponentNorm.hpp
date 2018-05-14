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
    BlockCoeffComponentNorm

Description
    Class for BlockCoeff norm based on a specified component.
    
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

#ifndef BlockCoeffComponentNorm_HPP
#define BlockCoeffComponentNorm_HPP

#include "BlockCoeffNorm.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //


/*---------------------------------------------------------------------------*\
                        Class BlockCoeffComponentNorm Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCoeffComponentNorm
:
    public BlockCoeffNorm<Type>
{

    //- Control data dictionary
    dictionary dict_;

    //- Component from which the norm is determined
    const direction cmpt_;


protected:

    //- Return dictionary
    const dictionary& dict() const
    {
        return dict_;
    }


public:

    //- Runtime type information
    TypeName("componentNorm");


    //- Construct from dictionary
    BlockCoeffComponentNorm
    (
        const dictionary& dict
    );


    virtual ~BlockCoeffComponentNorm()
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
BlockCoeffComponentNorm<Type>::BlockCoeffComponentNorm
(
    const dictionary& dict
)
:
    BlockCoeffNorm<Type>(dict),
    dict_(dict),
    cmpt_(readInt(this->dict().lookup("normComponent")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
scalar BlockCoeffComponentNorm<Type>::normalize
(
    const BlockCoeff<Type>& a
)
{
    return mag(a.component(cmpt_));
}


template<class Type>
void BlockCoeffComponentNorm<Type>::coeffMag
(
    const CoeffField<Type>& a,
    Field<scalar>& b
)
{
    b = mag(a.component(cmpt_));
}


} // End namespace CML
#endif
