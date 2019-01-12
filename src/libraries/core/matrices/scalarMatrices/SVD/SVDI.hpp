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

// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //

template<class T>
inline const T CML::SVD::sign(const T& a, const T& b)
{
    // return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
    return b >= 0 ? a : -a;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::scalarRectangularMatrix& CML::SVD::U() const
{
    return U_;
}


inline const CML::scalarRectangularMatrix& CML::SVD::V() const
{
    return V_;
}


inline const CML::scalarDiagonalMatrix& CML::SVD::S() const
{
    return S_;
}


inline bool CML::SVD::converged() const
{
    return converged_;
}


inline CML::label CML::SVD::nZeros() const
{
    return nZeros_;
}


inline CML::scalar CML::SVD::minNonZeroS() const
{
    scalar minS = S_[0];
    for (label i=1; i<S_.size(); i++)
    {
        scalar s = S_[i];
        if (s > VSMALL && s < minS) minS = s;
    }
    return minS;
}


// ************************************************************************* //
