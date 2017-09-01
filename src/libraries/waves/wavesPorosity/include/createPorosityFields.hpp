// Get an autoPtr to a porosity model
autoPtr<wavesPorosityModel> pm = CML::wavesPorosityModel::New(mesh);

// Inside a scope to get the field deleted again.
{
#   include "calcPorosity.hpp"

	porosity.write();
}




