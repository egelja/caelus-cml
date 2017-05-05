/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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


Description
    Checks for the swak-Grammars

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakChecks_H
#define swakChecks_H

#include "OStringStream.hpp"

namespace CML {
    template<class T1,class T2>
    string swakAssertSameSize(const T1 *a,const T2 *b) 
    {
        if(a->size()!=b->size()) {
            OStringStream o;

            o << "Operands have different sizes: " 
                << a->size() << " and " << b->size();

            return string(o.str());
        }

        return string("");
    }
}

#define sameSize(a,b) {CML::string err=CML::swakAssertSameSize(a,b); if(err!="") { error(yylloc,err); YYERROR; }}

#endif
