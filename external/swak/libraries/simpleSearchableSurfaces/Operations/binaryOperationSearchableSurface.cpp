/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright held by original author
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

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "binaryOperationSearchableSurface.hpp"
#include "SortableList.hpp"

#include <cassert>


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(binaryOperationSearchableSurface, 0);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::binaryOperationSearchableSurface::binaryOperationSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    aName_(dict.lookupOrDefault<word>("aName","A")),
    bName_(dict.lookupOrDefault<word>("bName","B")),
    a_(
        searchableSurface::New
        (
            word(dict.subDict("a").lookup("type")),
            IOobject(
                name()+"_"+word(dict.lookup("type"))+"_"+aName_,
                io.instance(),
                io.db(),
                io.readOpt(),
                io.writeOpt()
            ),
            dict.subDict("a")
        )
    ),
    b_(
        searchableSurface::New
        (
            word(dict.subDict("b").lookup("type")),
            IOobject(
                name()+"_"+word(dict.lookup("type"))+"_"+bName_,
                io.instance(),
                io.db(),
                io.readOpt(),
                io.writeOpt()
            ),
            dict.subDict("b")
        )
    ),
    nrARegions_(
        a().regions().size()
    ),
    nrBRegions_(
        b().regions().size()
    )
{
    if(aName_==bName_) {
        FatalErrorInFunction
            << "'aName' and 'bName' have the same value " << aName_
            << " for " << name()
            << endl
            << exit(FatalError);
    }
    if(regions().size()!=size()) {
        FatalErrorInFunction
            << "Number of regions " << regions().size() << " not equal to size "
            << size() << nl << "Regions: " << regions()
            << endl
            << exit(FatalError);

    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::binaryOperationSearchableSurface::~binaryOperationSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


CML::label CML::binaryOperationSearchableSurface::toIndexA(
    const label index
) const
{
    if(index>=nrARegions_) {
        Info << "A: " << index << "/" << nrARegions_
            << " " << regions() << endl;
        return -1;
        //        return 0;
    } else {
        return index;
    }
}

CML::label CML::binaryOperationSearchableSurface::toIndexB(
    const label index
) const
{
    if(index<nrARegions_) {
        Info << "B: " << index << "/" << nrARegions_ << endl;
        return -1;
        //        return 0;
    } else {
        return index-nrARegions_;
    }
}

const CML::wordList& CML::binaryOperationSearchableSurface::regions() const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::regions " << name() << endl;
    }

    if(regions_.size() == 0 ) {
        regions_.setSize(nrARegions_+nrBRegions_);
        for(label i=0;i<nrARegions_;i++) {
            regions_[i]=aName_+"_"+a().regions()[i];
        }
        for(label i=0;i<nrBRegions_;i++) {
            regions_[i+nrARegions_]=bName_+"_"+b().regions()[i];
        }
    }
    return regions_;
}

CML::tmp<CML::pointField> CML::binaryOperationSearchableSurface::points() const
{
    return coordinates();
}

CML::tmp<CML::pointField>
CML::binaryOperationSearchableSurface::coordinates() const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::coordinates " << name() << endl;
    }

    pointField aCoords(a().coordinates());
    pointField bCoords(b().coordinates());

    pointField result(aCoords.size()+bCoords.size());

    forAll(aCoords,i) {
        result[i]=aCoords[i];
    }
    forAll(bCoords,i) {
        result[i+aCoords.size()]=bCoords[i];
    }
    if(debug) {
        Info << "A: " << aCoords.size() << " B: " << bCoords.size()
            << " -> " << result.size() << "(" << this->size() << ")" << endl;
    }
    // if(result.size()!=this->size()) {
    //     WarningInFunction
    //         << "Number of coordinates " << result.size() << " does not match "
    //             << " the promised size: " << this->size() << endl
    //             << "Coordinates: " << result << endl
    //             << "Regions: " << regions() << endl;
    //         //                << exit(FatalError);

    // }

    return tmp<pointField>(new pointField(result));
}

bool CML::binaryOperationSearchableSurface::overlaps(const boundBox& bb) const
{
    NotImplemented;

    return false;
}

void CML::binaryOperationSearchableSurface::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::findNearest "
            << name() << " : "
            << samples.size() << " samples" << endl;
    }

    List<pointIndexHit> hitA;
    List<pointIndexHit> hitB;
    if(debug) {
        Info << "Doing A" << endl;
    }
    a().findNearest(samples,nearestDistSqr,hitA);
    if(debug) {
        Info << "Doing B" << endl;
    }
    b().findNearest(samples,nearestDistSqr,hitB);

    List<bool> inAA,inAB;
    List<bool> inBA,inBB;
    List<bool> same;

    insideA(hitB,inAB);
    insideB(hitA,inBA);
    insideB(hitB,inBB);
    insideA(hitA,inAA);
    samePoint(hitA,hitB,same);

    info.setSize(samples.size());

    forAll(info,i) {
        hitWhom hA=HITSA;
        hitWhom hB=HITSB;
        if(same[i]) {
            hA=BOTH;
            hB=BOTH;
        }
        bool validA=(hitA[i].hit() && this->decidePoint(hA,inAA[i],inBA[i]));
        bool validB=(hitB[i].hit() && this->decidePoint(hB,inAB[i],inBB[i]));
        if(!validA && !validB) {
            // WarningInFunction
            //     << "Neither hit " << hitA[i] << " nor " << hitB[i]
            //         << " near " << samples[i] << " valid" << endl
            //         << same[i] << " " << inAA[i] << " " << inBA[i]
            //         << " " << inAB[i] << " " << inBB[i] << endl
            //         << mag(hitA[i].rawPoint()-samples[i]) << " "
            //         << mag(hitB[i].rawPoint()-samples[i])
            //         << endl;
        } else if( validA && validB ) {
            if(same[i]) {
                info[i]=hitA[i];
            } else if(
                mag(samples[i]-hitA[i].rawPoint())
                <
                mag(samples[i]-hitB[i].rawPoint())
            ) {
                info[i]=hitA[i];
            } else {
                info[i]=hitB[i];
            }
        } else if( validA ) {
            info[i]=hitA[i];
        } else {
            info[i]=hitB[i];
        }
    }
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::findNearest - leaving" << endl;
    }
}

void CML::binaryOperationSearchableSurface::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::findLine " << name() << endl;
    }

    List<List<pointIndexHit> > infoAll;
    findLineAll(start,end,infoAll);
    info.setSize(start.size());

    forAll(info,i) {
        if(infoAll[i].size()>0) {
            info[i]=infoAll[i][0];
        }
    }
}


void CML::binaryOperationSearchableSurface::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::findLineAny " << name() << endl;
    }

    findLine(start,end,info);
}


void CML::binaryOperationSearchableSurface::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::findLineAll " << name() << endl;
    }
    List<List<pointIndexHit> > infoA;
    List<List<pointIndexHit> > infoB;

    a().findLineAll(start,end,infoA);
    b().findLineAll(start,end,infoB);

    if(debug) {
        Info << "Found a: " << infoA.size() << " b: " << infoB.size()
            << endl;
    }

    info.setSize(start.size());

    forAll(info,i) {
        if(
            infoA[i].size()==0
            &&
            infoB[i].size()==0
        ) {
            info[i].setSize(0);
            continue;
        }

        filter(start[i],infoA[i],infoB[i],info[i]);
        if(debug>3) {
            Info << infoA[i] << infoB[i] << info[i] << endl;
        }
       // it's only ugly bubble-sort but OK for the small number of points
        for(label j=0;j<(info[i].size()-1);j++) {
            for(label k=j+1;k<info[i].size();k++) {
                if
                    (
                        mag(start[i]-info[i][k].rawPoint())
                        <
                        mag(start[i]-info[i][j].rawPoint())
                    ) {
                    pointIndexHit tmp=info[i][j];
                    info[i][j]=info[i][k];
                    info[i][k]=tmp;
                }
            }
        }

        scalar identicalThreshhold=1e-10;

        bool clipIdentical=false;
        for(label j=1;j<info[i].size();j++) {
            if(
                mag(
                    info[i][j-1].rawPoint()
                    -
                    info[i][j].rawPoint()
                )<identicalThreshhold
            ) {
                clipIdentical=true;
                break;
            }
        }
        if(clipIdentical) {
            //            Info << "Identical points in " << info[i] << endl;
            DynamicList<pointIndexHit> cleaned;
            cleaned.append(info[i][0]);
            for(label j=1;j<info[i].size();j++) {
                if(
                    mag(
                        cleaned[cleaned.size()-1].rawPoint()
                        -
                        info[i][j].rawPoint()
                    )>identicalThreshhold
                ){
                    cleaned.append(info[i][j]);
                }
            }
            info[i]=cleaned;
            //            Info << "Cleaned: " << info[i] << endl;
        }
    }

    if(debug) {
        label cntA=0,cntB=0,cnt=0;
        forAll(infoA,j) {
            cntA+=infoA[j].size();
            cntB+=infoB[j].size();
            cnt +=info[j].size();
        }
        Info << "Found A: " << cntA << " B: " << cntB
            << " Total: " << cnt << endl;
    }
 }

void CML::binaryOperationSearchableSurface::filter
(
    const point &start,
    const List<pointIndexHit>& hitsA,
    const List<pointIndexHit>& hitsB,
    List<pointIndexHit>& result
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::filter" << endl;
    }

    List<bool> inA;
    List<bool> inB;
    List<hitWhom> whom;
    List<pointIndexHit> hits;
    collectInfo(
        start,
        hitsA,
        hitsB,
        hits,
        inA,
        inB,
        whom
    );

    DynamicList<pointIndexHit> h;
    forAll(hits,i) {
        if(
            this->decidePoint(
                whom[i],
                inA[i],
                inB[i]
            )
        ) {
            h.append(hits[i]);
        }
    }
    result=h;
}


void CML::binaryOperationSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::getRegion " << name() << endl;
    }

    List<hitWhom> who;
    whose(info,who);

    List<pointIndexHit> infoA;
    List<pointIndexHit> infoB;
    splitHits(
        info,
        who,
        infoA,
        infoB
    );

    labelList regionA;
    labelList regionB;
    a().getRegion(infoA,regionA);
    b().getRegion(infoB,regionB);

    region.setSize(who.size());

    label cntA=0,cntB=0;
    forAll(who,i) {
        if(who[i]==BOTH || who[i]==HITSA) {
            region[i]=regionA[cntA];
            cntA++;
            if(who[i]==BOTH) {
                cntB++;
            }
        } else if(who[i]==HITSB) {
            if(regionB[cntB]>=0) {
                region[i]=regionB[cntB]+nrARegions_;
            } else {
                region[i]=regionB[cntB];
            }
            cntB++;
        } else if(who[i]==NONE) {
            //            region[i]=-1;
            region[i]=0;
            WarningInFunction
                << "Hit " << info[i] << " does not fit any component" << endl
                    << "Defaulting to 0" << endl;

            // FatalErrorInFunction
            //     << "The hit " << info[i] << " hits none of both"
            //     << endl
            //     << abort(FatalError);
        }
    }

    if(cntA!=regionA.size() || cntB!=regionB.size()) {
        label cntInA=0,cntInB=0,cntBoth=0,cntNone=0;
        forAll(who,i) {
            switch(who[i]) {
                case HITSA:
                    cntInA++; break;
                case HITSB:
                    cntInB++; break;
                case BOTH:
                    cntBoth++; break;
                case NONE:
                    cntNone++; break;
            }
        }
        Info << who.size() << " points. A: " << cntInA
            << " B: " << cntInB << " Both: " << cntBoth
            << " None: " << cntNone << endl;

        FatalErrorInFunction
            << "Something went horribly wrong. The sizes "
            << cntA << "!=" << regionA.size() << " and/or "
            << cntB << "!=" << regionB.size() << "\n"
            << "We're lucky that the program got here\n"
            << endl
            << abort(FatalError);
    }

    assert(region.size()==info.size());

    if(debug) {
        HashTable<label,label> cnts;
        forAll(region,i) {
            label reg=region[i];
            if(!cnts.found(reg)) {
                cnts.insert(reg,1);
            } else {
                cnts[reg]++;
            }
        }
        Info << "Counts: " << cnts << endl;
    }
}


void CML::binaryOperationSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::getNormal " << name() << endl;
    }

    List<hitWhom> who;
    whose(info,who);

    List<pointIndexHit> infoA;
    List<pointIndexHit> infoB;
    splitHits(
        info,
        who,
        infoA,
        infoB
    );

    vectorField normalA;
    vectorField normalB;
    a().getNormal(infoA,normalA);
    b().getNormal(infoB,normalB);

    normal.setSize(info.size());

    label cntA=0,cntB=0;
    forAll(who,i) {
        if(who[i]==BOTH || who[i]==HITSA || who[i]==NONE) {
            if(who[i]==BOTH || who[i]==HITSA) {
                normal[i]=normalA[cntA];
            } else {
                normal[i]=vector::zero;
            }
            if(revertNormalA(info[i])) {
                normal[i]*=-1;
            }
            if(who[i]!=NONE) {
                cntA++;
            }
            if(who[i]==BOTH) {
                cntB++;
            }
        } else if(who[i]==HITSB) {
            normal[i]=normalB[cntB];
            if(revertNormalB(info[i])) {
                normal[i]*=-1;
            }
            cntB++;
        }
    }

    if(cntA!=normalA.size() || cntB!=normalB.size()) {
        label cntInA=0,cntInB=0,cntBoth=0,cntNone=0;
        forAll(who,i) {
            switch(who[i]) {
                case HITSA:
                    cntInA++; break;
                case HITSB:
                    cntInB++; break;
                case BOTH:
                    cntBoth++; break;
                case NONE:
                    cntNone++; break;
            }
        }
        Info << who.size() << " points. A: " << cntInA
            << " B: " << cntInB << " Both: " << cntBoth
            << " None: " << cntNone << endl;

        FatalErrorInFunction
            << "Something went horribly wrong. The sizes "
            << cntA << "!=" << normalA.size() << " and/or "
            << cntB << "!=" << normalB.size() << "\n"
            << "We're lucky that the program got here\n"
            << endl
            << abort(FatalError);
    }
}

void  CML::binaryOperationSearchableSurface::whose
(
    const List<pointIndexHit>& hits,
    List<hitWhom> &whom
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::whose " << name() << endl;
    }

    pointField samples(hits.size());
    scalarField distance(hits.size(),1e5);

    forAll(hits,i) {
        samples[i]=hits[i].rawPoint();
    }

    List<pointIndexHit> nearestA;
    List<pointIndexHit> nearestB;

    a().findNearest(samples,distance,nearestA);
    b().findNearest(samples,distance,nearestB);

    whom.setSize(hits.size(),NONE);

    //    Info << nearestA << nearestB << endl;

    const scalar sameTolerance=1e-10;
    const scalar farTolerance=1e-5;

    forAll(whom,i) {
        scalar distA=HUGE_VAL;
        scalar distB=HUGE_VAL;
        if(nearestA[i].hit()) {
            distA=mag(samples[i]-nearestA[i].hitPoint());
        }
        if(nearestB[i].hit()) {
            distB=mag(samples[i]-nearestB[i].hitPoint());
        }
        if(
            distA>farTolerance
            &&
            distB>farTolerance
        ) {
            whom[i]=NONE;
        } else if(
            distA<sameTolerance
            &&
            distB<sameTolerance
        ) {
            whom[i]=BOTH;
        } else {
            whom[i]=( distA <= distB ? HITSA : HITSB);
        }
    }
    if(debug) {
        label cntA=0,cntB=0,cntBoth=0,cntNone=0;
        forAll(whom,i) {
            switch(whom[i]) {
                case HITSA:
                    cntA++; break;
                case HITSB:
                    cntB++; break;
                case BOTH:
                    cntBoth++; break;
                case NONE:
                    cntNone++; break;
            }
        }
        Info << whom.size() << " points. A: " << cntA
            << " B: " << cntB << " Both: " << cntBoth
            << " None: " << cntNone << endl;
    }
}

void  CML::binaryOperationSearchableSurface::splitHits
(
    const List<pointIndexHit>& hits,
    const List<hitWhom> &isA,
    List<pointIndexHit>& hitsA,
    List<pointIndexHit>& hitsB
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::splitHits " << name() << endl;
    }

    label nrA=0;
    label nrB=0;
    forAll(isA,i) {
        if(isA[i]==BOTH || isA[i]==HITSA){
            nrA++;
        }
        if(isA[i]==BOTH || isA[i]==HITSB){
            nrB++;
        }
    }

    hitsA.setSize(nrA);
    hitsB.setSize(nrB);

    label cntA=0;
    label cntB=0;

    forAll(hits,i) {
        if(isA[i]==BOTH || isA[i]==HITSA){
            hitsA[cntA]=hits[i];
            //            hitsA[cntA].setIndex(toIndexA(hitsA[cntA].index()));
            cntA++;
        }
        if(isA[i]==BOTH || isA[i]==HITSB){
            hitsB[cntB]=hits[i];
            //            hitsB[cntB].setIndex(toIndexB(hitsB[cntB].index()));
            cntB++;
        }
    }
    if( cntA!=nrA || cntB!=nrB ) {
        FatalErrorInFunction
            << "Counts differ: " << cntA << "/" << nrA << " " << cntB << "/" << nrB
            << "\n" << endl
            << abort(FatalError);
    }
    if(debug) {
        Info << "From " << hits.size() << " to A: " << hitsA.size()
            << " to B: " << hitsB.size() << endl;
    }
}

void  CML::binaryOperationSearchableSurface::inside
(
    const searchableSurface &s,
    const List<pointIndexHit>& hits,
    List<bool> &in
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::inside " << name() << endl;
    }
    pointField samples(hits.size());

    forAll(hits,i) {
        samples[i]=hits[i].rawPoint();
    }

    List<volumeType> vol;

    s.getVolumeType(samples,vol);

    in.setSize(vol.size());

    forAll(vol,i) {
        //        in[i]=(vol[i]==INSIDE || vol[i]==UNKNOWN);
        in[i]=(vol[i]==INSIDE);
    }

    if(debug) {
        label cntIn=0,cntInside=0,cntOutside=0,cntUnknown=0,cntMixed=0;
        forAll(in,i) {
            if(in[i]) {
                cntIn++;
            }
            switch(vol[i]) {
                case INSIDE:
                    cntInside++; break;
                case OUTSIDE:
                    cntOutside++; break;
                case MIXED:
                    cntMixed++; break;
                case UNKNOWN:
                    cntUnknown++; break;
            }
        }
        Info << "Inside " << cntIn << " of " << in.size() << endl;
        Info << "VolType: Inside: " << cntInside << " Outside: "
            << cntOutside << " Mixed: " << cntMixed << " Unknown: "
            << cntUnknown << endl;
    }
}

void CML::binaryOperationSearchableSurface::samePoint(
    const List<pointIndexHit>& hitA,
    const List<pointIndexHit>& hitB,
    List<bool> &same
) const
{
    if(hitA.size()!=hitB.size()) {
        FatalErrorInFunction
            << "hitA (" << hitA.size() << ") and "
            << "hitB (" << hitB.size() << ") are not of same size"
            << endl
            << abort(FatalError);
    }
    const scalar sameTolerance=1e-10;

    same.resize(hitA.size(),false);
    forAll(same,i) {
        same[i]=(
            mag(
                hitA[i].rawPoint()-hitB[i].rawPoint()
            )<sameTolerance
        );
    }
}

void CML::binaryOperationSearchableSurface::collectInfo(
    const point &start,
    const List<pointIndexHit>& hitsA,
    const List<pointIndexHit>& hitsB,
    List<pointIndexHit>& allHits,
    List<bool>& inA,
    List<bool>& inB,
    List<hitWhom>& who
) const
{
    if(debug) {
        Info << "CML::binaryOperationSearchableSurface::collectInfo " << name() << endl;
    }

    DynamicList<pointIndexHit> hits;
    DynamicList<hitWhom> whom;

    const label sizeA=hitsA.size();
    const label sizeB=hitsB.size();
    label cntA=0;
    label cntB=0;
    bool goOn=true;

    const scalar sameTolerance=1e-10;

    while(goOn) {
        if(
            cntA==sizeA
            &&
            cntB==sizeB
        ) {
            goOn=false;
        } else if(cntA==sizeA) {
            hits.append(hitsB[cntB]);
            whom.append(HITSB);
            cntB++;
        } else if(cntB==sizeB) {
            hits.append(hitsA[cntA]);
            whom.append(HITSA);
            cntA++;
        } else if(
            mag(
                hitsA[cntA].rawPoint()
                -
                hitsB[cntB].rawPoint()
            ) < sameTolerance
        ) {
            hits.append(hitsA[cntA]);
            if(
                !hitsA[cntA].hit()
                &&
                hitsB[cntB].hit()
            ) {
                hits[hits.size()-1].setHit();
                hits[hits.size()-1].setIndex(hitsB[cntB].index());
            }
            whom.append(BOTH);
            cntA++; cntB++;
        } else if(
            mag(hitsA[cntA].rawPoint()-start)
            <
            mag(hitsB[cntB].rawPoint()-start)
        ) {
            hits.append(hitsA[cntA]);
            whom.append(HITSA);
            cntA++;
        } else {
            hits.append(hitsB[cntB]);
            whom.append(HITSB);
            cntB++;
        }
    }

    allHits=hits;
    who=whom;

    insideA(allHits,inA);
    insideB(allHits,inB);

    if(debug>1) {
        forAll(allHits,i) {
            Info << allHits[i] << " whom:  " << whom[i] << " A: "
                << inA[i] << " inB: " << inB[i] << endl;
        }
    }
}

void CML::binaryOperationSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    a().boundingSpheres(
        centres,
        radiusSqr
    );
    pointField centresB;
    scalarField radiusB;

    b().boundingSpheres(
        centresB,
        radiusB
    );

    centres.append(centresB);
    radiusSqr.append(radiusB);
}

// ************************************************************************* //
