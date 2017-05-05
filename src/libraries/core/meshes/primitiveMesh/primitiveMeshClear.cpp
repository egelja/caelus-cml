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

#include "primitiveMesh.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::primitiveMesh::printAllocated() const
{
    Pout<< "primitiveMesh allocated :" << endl;

    // Topology
    if (cellShapesPtr_)
    {
        Pout<< "    Cell shapes" << endl;
    }

    if (edgesPtr_)
    {
        Pout<< "    Edges" << endl;
    }

    if (ccPtr_)
    {
        Pout<< "    Cell-cells" << endl;
    }

    if (ecPtr_)
    {
        Pout<< "    Edge-cells" << endl;
    }

    if (pcPtr_)
    {
        Pout<< "    Point-cells" << endl;
    }

    if (cfPtr_)
    {
        Pout<< "    Cell-faces" << endl;
    }

    if (efPtr_)
    {
        Pout<< "    Edge-faces" << endl;
    }

    if (pfPtr_)
    {
        Pout<< "    Point-faces" << endl;
    }

    if (cePtr_)
    {
        Pout<< "    Cell-edges" << endl;
    }

    if (fePtr_)
    {
        Pout<< "    Face-edges" << endl;
    }

    if (pePtr_)
    {
        Pout<< "    Point-edges" << endl;
    }

    if (ppPtr_)
    {
        Pout<< "    Point-point" << endl;
    }

    if (cpPtr_)
    {
        Pout<< "    Cell-point" << endl;
    }

    // Geometry
    if (cellCentresPtr_)
    {
        Pout<< "    Cell-centres" << endl;
    }

    if (cellCentresGeometricPtr_)
    {
        Pout<< "    Cell-centres geometric" << endl;
    }

    if (faceCentresPtr_)
    {
        Pout<< "    Face-centres" << endl;
    }

    if (cellVolumesPtr_)
    {
        Pout<< "    Cell-volumes" << endl;
    }

    if (faceAreasPtr_)
    {
        Pout<< "    Face-areas" << endl;
    }

}


void CML::primitiveMesh::clearGeom()
{
    if (debug)
    {
        Pout<< "primitiveMesh::clearGeom() : "
            << "clearing geometric data"
            << endl;
    }

    deleteDemandDrivenData(cellCentresPtr_);
    deleteDemandDrivenData(cellCentresGeometricPtr_);
    deleteDemandDrivenData(faceCentresPtr_);
    deleteDemandDrivenData(cellVolumesPtr_);
    deleteDemandDrivenData(faceAreasPtr_);
}


void CML::primitiveMesh::clearAddressing()
{
    if (debug)
    {
        Pout<< "primitiveMesh::clearAddressing() : "
            << "clearing topology"
            << endl;
    }

    deleteDemandDrivenData(cellShapesPtr_);

    clearOutEdges();

    deleteDemandDrivenData(ccPtr_);
    deleteDemandDrivenData(ecPtr_);
    deleteDemandDrivenData(pcPtr_);

    deleteDemandDrivenData(cfPtr_);
    deleteDemandDrivenData(efPtr_);
    deleteDemandDrivenData(pfPtr_);

    deleteDemandDrivenData(cePtr_);
    deleteDemandDrivenData(fePtr_);
    deleteDemandDrivenData(pePtr_);
    deleteDemandDrivenData(ppPtr_);
    deleteDemandDrivenData(cpPtr_);
}


void CML::primitiveMesh::clearOut()
{
    clearGeom();
    clearAddressing();
}


// ************************************************************************* //
