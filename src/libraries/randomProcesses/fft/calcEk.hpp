#ifndef calcEk_H
#define calcEk_H

#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class graph;
class Kmesh;

graph calcEk
(
    const volVectorField& U,
    const Kmesh& K
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#endif
