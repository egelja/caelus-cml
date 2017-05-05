//
// checkTimeOptions.H
// ~~~~~~~~~~~~~~~~~~

CML::label startTime = 0;
CML::label endTime = Times.size();

// unless -constant is present, skip startTime if it is "constant"
#include "checkConstantOption.hpp"

// check -time and -latestTime options
#include "checkTimeOption.hpp"
