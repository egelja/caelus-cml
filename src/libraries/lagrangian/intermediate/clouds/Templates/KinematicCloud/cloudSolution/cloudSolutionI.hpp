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

inline const CML::fvMesh& CML::cloudSolution::mesh() const
{
    return mesh_;
}


inline const CML::dictionary& CML::cloudSolution::dict() const
{
    return dict_;
}


inline const CML::Switch CML::cloudSolution::active() const
{
    return active_;
}


inline const CML::dictionary& CML::cloudSolution::sourceTermDict() const
{
    return dict_.subDict("sourceTerms");
}


inline const CML::dictionary& CML::cloudSolution::interpolationSchemes() const
{
    return dict_.subDict("interpolationSchemes");
}


inline const CML::dictionary& CML::cloudSolution::integrationSchemes() const
{
    return dict_.subDict("integrationSchemes");
}


inline const CML::Switch CML::cloudSolution::transient() const
{
    return transient_;
}


inline const CML::Switch CML::cloudSolution::steadyState() const
{
    return !transient_;
}


inline CML::label CML::cloudSolution::calcFrequency() const
{
    return calcFrequency_;
}


inline CML::scalar CML::cloudSolution::maxCo() const
{
    return maxCo_;
}


inline CML::label CML::cloudSolution::iter() const
{
    return iter_;
}


inline CML::label CML::cloudSolution::nextIter()
{
    return ++iter_;
}


inline CML::scalar CML::cloudSolution::trackTime() const
{
    return trackTime_;
}


inline CML::scalar CML::cloudSolution::deltaTMax() const
{
    return deltaTMax_;
}


inline CML::Switch& CML::cloudSolution::coupled()
{
    return coupled_;
}


inline const CML::Switch CML::cloudSolution::coupled() const
{
    return coupled_;
}


inline const CML::Switch CML::cloudSolution::cellValueSourceCorrection() const
{
    return cellValueSourceCorrection_;
}


inline CML::scalar CML::cloudSolution::maxTrackTime() const
{
    return maxTrackTime_;
}


inline const CML::Switch CML::cloudSolution::resetSourcesOnStartup() const
{
    return resetSourcesOnStartup_;
}


// ************************************************************************* //
