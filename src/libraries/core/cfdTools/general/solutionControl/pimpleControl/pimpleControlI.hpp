/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation

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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::pimpleControl::nCorrPIMPLE() const
{
    return nCorrPIMPLE_;
}


inline CML::label CML::pimpleControl::nCorrPISO() const
{
    return nCorrPISO_;
}


inline CML::label CML::pimpleControl::corrPISO() const
{
    return corrPISO_;
}


inline bool CML::pimpleControl::correct()
{
    corrPISO_++;

    if (debug)
    {
        Info<< algorithmName_ << " correct: corrPISO = " << corrPISO_ << endl;
    }

    if (corrPISO_ <= nCorrPISO_)
    {
        return true;
    }
    else
    {
        corrPISO_ = 0;
        return false;
    }
}


inline bool CML::pimpleControl::storeInitialResiduals() const
{
    // Start from second PIMPLE iteration
    return (corr_ == 2) && (corrPISO_ == 0) && (corrNonOrtho_ == 0);
}


inline bool CML::pimpleControl::firstIter() const
{
    return corr_ == 1;
}


inline bool CML::pimpleControl::finalIter() const
{
    return converged_ || (corr_ == nCorrPIMPLE_);
}


inline bool CML::pimpleControl::finalInnerIter() const
{
    return
       finalIter()
    && corrPISO_ == nCorrPISO_
    && corrNonOrtho_ == nNonOrthCorr_ + 1;
}


inline bool CML::pimpleControl::turbCorr() const
{
    return !turbOnFinalIterOnly_ || finalIter();
}


// ************************************************************************* //
