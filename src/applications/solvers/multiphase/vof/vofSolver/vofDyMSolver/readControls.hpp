#include "readTimeControls.hpp"

correctPhi = pimple.dict().lookupOrDefault<Switch>("correctPhi", true);

checkMeshCourantNo =
    pimple.dict().lookupOrDefault<Switch>("checkMeshCourantNo", false);

moveMeshOuterCorrectors =
    pimple.dict().lookupOrDefault<Switch>("moveMeshOuterCorrectors", false);
