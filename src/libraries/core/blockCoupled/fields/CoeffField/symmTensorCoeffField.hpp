/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
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
    CoeffField<T>

Description
    Template specialisation for symmTensor coefficients

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

SourceFiles
    symmTensorCoeffField.cpp

\*---------------------------------------------------------------------------*/

#ifndef symmTensorCoeffField_HPP
#define symmTensorCoeffField_HPP

#include "CoeffField.hpp"
#include "DecoupledCoeffField.hpp"
#include "symmTensor.hpp"


namespace CML
{

// Forward declaration of friend functions and operators
Ostream& operator<<
(
    Ostream&,
    const CoeffField<symmTensor>&
);

Ostream& operator<<
(
    Ostream&,
    const tmp<CoeffField<symmTensor> >&
);


template<>
class CoeffField<symmTensor>
:
    public DecoupledCoeffField<symmTensor>
{
public:

    //- Construct given size
    explicit CoeffField(const label);

    //- Construct as copy
    CoeffField(const CoeffField<symmTensor>&);

    //- Construct as copy of base
    explicit CoeffField(const DecoupledCoeffField<symmTensor>&);

    //- Construct as copy of base
    explicit CoeffField(const tmp<DecoupledCoeffField<symmTensor> >&);

    //- Construct from Istream
    explicit CoeffField(Istream&);

    // Member operators
    void operator=(const CoeffField<symmTensor>&);
    void operator=(const tmp<CoeffField<symmTensor> >&);
    void operator=(const scalarTypeField&);
    void operator=(const tmp<scalarTypeField>&);
    void operator=(const linearTypeField&);
    void operator=(const tmp<linearTypeField>&);

    // IOstream operators
    friend Ostream& operator<<
    (
        Ostream&,
        const CoeffField<symmTensor>&
    );

    friend Ostream& operator<<
    (
        Ostream&,
        const tmp<CoeffField<symmTensor> >&
    );
};

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

tmp<CoeffField<symmTensor> > inv(const CoeffField<symmTensor>& f);


template<>
void multiply
(
    Field<symmTensor>& f,
    const CoeffField<symmTensor>& f1,
    const symmTensor& f2
);


template<>
void multiply
(
    Field<symmTensor>& f,
    const CoeffField<symmTensor>& f1,
    const Field<symmTensor>& f2
);


template<>
void multiply
(
    Field<symmTensor>& f,
    const Field<symmTensor>& f1,
    const CoeffField<symmTensor>& f2
);


} // End namespace CML
#endif
