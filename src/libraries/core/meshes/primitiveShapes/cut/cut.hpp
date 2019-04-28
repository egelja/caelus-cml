/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Description:
    Functions which cut triangles and tetrahedra. Generic operations are
    applied to each half of a cut.


\*---------------------------------------------------------------------------*/

#ifndef cut_HPP
#define cut_HPP

#include "FixedList.hpp"
#include "nil.hpp"
#include "plane.hpp"
#include "tetPointRef.hpp"
#include "triPointRef.hpp"
#include "zero.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace cut
{

/*---------------------------------------------------------------------------*\
                          Class uniformOp Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformOp
{
private:

    //- Data
    Type data_;


public:

    // Constructors

        //- Construct null
        uniformOp()
        {}

        //- Construct from data
        uniformOp(Type data)
        :
            data_(data)
        {}


    // Member functions

        //- Access the data
        Type data() const
        {
            return data_;
        }
};


/*---------------------------------------------------------------------------*\
                             Class noOp Declaration
\*---------------------------------------------------------------------------*/

class noOp
:
    public uniformOp<nil>
{
public:

    // Typedefs

        //- Result type
        typedef zero result;


    // Constructors

        //- Construct null
        noOp()
        {}

        //- Construct from base
        noOp(const uniformOp<nil>& op)
        :
            uniformOp<nil>(op)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return result();
        }

        //- Operate on a triangle or tetrahedron
        template<unsigned Size>
        result operator()(const FixedList<point, Size>& p) const
        {
            return result();
        }
};


/*---------------------------------------------------------------------------*\
                            Class areaOp Declaration
\*---------------------------------------------------------------------------*/

class areaOp
:
    public uniformOp<nil>
{
public:

    // Typedefs

        //- Result type
        typedef vector result;


    // Constructors

        //- Construct null
        areaOp()
        {}

        //- Construct from base
        areaOp(const uniformOp<nil>& op)
        :
            uniformOp<nil>(op)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return vector::zero;
        }

        //- Operate on a triangle
        result operator()(const FixedList<point, 3>& p) const
        {
            return result(triPointRef(p[0], p[1], p[2]).area());
        }
};


/*---------------------------------------------------------------------------*\
                           Class volumeOp Declaration
\*---------------------------------------------------------------------------*/

class volumeOp
:
    public uniformOp<nil>
{
public:

    // Typedefs

        //- Result type
        typedef scalar result;


    // Constructors

        //- Construct null
        volumeOp()
        {}

        //- Construct from base
        volumeOp(const uniformOp<nil>& op)
        :
            uniformOp<nil>(op)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return 0;
        }

        //- Operate on a tetrahedron
        result operator()(const FixedList<point, 4>& p) const
        {
            return result(tetPointRef(p[0], p[1], p[2], p[3]).mag());
        }
};


/*---------------------------------------------------------------------------*\
                       Class areaIntegrateOp Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class areaIntegrateOp
:
    public FixedList<Type, 3>
{
public:

    // Typedefs

        //- Result type
        typedef typename outerProduct<Type, vector>::type result;


    // Constructors

        //- Construct from base
        areaIntegrateOp(const FixedList<Type, 3>& x)
        :
            FixedList<Type, 3>(x)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return pTraits<result>::zero;
        }

        //- Operate on a triangle
        result operator()(const FixedList<point, 3>& p) const
        {
            const FixedList<Type, 3>& x = *this;
            return result(areaOp()(p)*(x[0] + x[1] + x[2])/3);
        }
};


/*---------------------------------------------------------------------------*\
                      Class volumeIntegrateOp Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class volumeIntegrateOp
:
    public FixedList<Type, 4>
{
public:

    // Typedefs

        //- Result type
        typedef Type result;


    // Constructors

        //- Construct from base
        volumeIntegrateOp(const FixedList<Type, 4>& x)
        :
            FixedList<Type, 4>(x)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return pTraits<result>::zero;
        }

        //- Operate on a tetrahedron
        result operator()(const FixedList<point, 4>& p) const
        {
            const FixedList<Type, 4>& x = *this;
            return result(volumeOp()(p)*(x[0] + x[1] + x[2] + x[3])/4);
        }
};


/*---------------------------------------------------------------------------*\
                            Class listOp Declaration
\*---------------------------------------------------------------------------*/

template<unsigned Size>
class listOp
:
    public uniformOp<nil>
{
public:

    // Classes

        //- Result class
        class result
        :
            public DynamicList<FixedList<point, Size>>
        {
        public:

            // Constructors

                //- Construct from a single element
                result(const FixedList<point, Size>& x)
                :
                    DynamicList<FixedList<point, Size>>(1, x)
                {}


            // Member operators

                //- Add together two lists
                result operator+(const result& x) const
                {
                    result r(*this);
                    r.append(x);
                    return r;
                }
        };


    // Constructors

        //- Construct null
        listOp()
        {}

        //- Construct from base
        listOp(const uniformOp<nil>& op)
        :
            uniformOp<nil>(op)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return result();
        }

        //- Operate on a triangle or tetrahedron
        result operator()(const FixedList<point, Size>& p) const
        {
            return result(p);
        }
};


typedef listOp<3> listTriOp;


typedef listOp<4> listTetOp;


/*---------------------------------------------------------------------------*\
                          Class appendOp Declaration
\*---------------------------------------------------------------------------*/

template<class Container>
class appendOp
:
    public uniformOp<Container&>
{
public:

    // Typedefs

        //- Result type
        typedef zero result;


    // Constructors

        //- Construct from a container reference
        appendOp(Container& x)
        :
            uniformOp<Container&>(x)
        {}

        //- Construct from base
        appendOp(const uniformOp<Container&>& op)
        :
            uniformOp<Container&>(op)
        {}


    // Member operators

        //- Operate on nothing
        result operator()() const
        {
            return result();
        }

        //- Operate on a triangle or tetrahedron
        template<unsigned Size>
        result operator()(const FixedList<point, Size>& p) const
        {
            this->data().append(p);
            return result();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Trait to determine the result of the addition of two operations
template<class AheadOp, class BehindOp>
class opAddResult;

template<class Op>
class opAddResult<Op, Op>
{
public:

    typedef typename Op::result type;
};

template<>
class opAddResult<noOp, noOp>
{
public:

    typedef typename noOp::result type;
};

template<class Op>
class opAddResult<noOp, Op>
{
public:

    typedef typename Op::result type;
};

template<class Op>
class opAddResult<Op, noOp>
{
public:

    typedef typename Op::result type;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace cut

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Cut a triangle along the zero plane defined by the given levels. Templated
//  on aboveOp and belowOp; the operations applied to the regions on either side
//  of the cut.
template<class AboveOp, class BelowOp>
typename cut::opAddResult<AboveOp, BelowOp>::type triCut
(
    const FixedList<point, 3>& tri,
    const FixedList<scalar, 3>& level,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
);

//- As above, but with a plane specifying the location of the cut
template<class AboveOp, class BelowOp>
typename cut::opAddResult<AboveOp, BelowOp>::type triCut
(
    const FixedList<point, 3>& tri,
    const plane& s,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
);

//- As triCut, but for a tetrahedron.
template<class AboveOp, class BelowOp>
typename cut::opAddResult<AboveOp, BelowOp>::type tetCut
(
    const FixedList<point, 4>& tet,
    const FixedList<scalar, 4>& level,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
);

//- As above, but with a plane specifying the location of the cut
template<class AboveOp, class BelowOp>
typename cut::opAddResult<AboveOp, BelowOp>::type tetCut
(
    const FixedList<point, 4>& tet,
    const plane& s,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
);


} // End namespace CML


namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Modify a uniform operation for reordering a tri (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& triReorder
(
    const cut::uniformOp<Type>& x,
    const FixedList<label, 3>&
)
{
    return x;
}


//- Modify a uniform operation for cutting a tri from a tri (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& triCutTri
(
    const cut::uniformOp<Type>& x,
    const Pair<scalar>&
)
{
    return x;
}


//- Modify a uniform operation for cutting a quad from a tri (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& triCutQuad
(
    const cut::uniformOp<Type>& x,
    const Pair<scalar>&
)
{
    return x;
}


//- Modify a uniform operation for reordering a tet (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& tetReorder
(
    const cut::uniformOp<Type>& x,
    const FixedList<label, 4>&
)
{
    return x;
}


//- Modify a uniform operation for cutting a tet from a tet (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& tetCutTet
(
    const cut::uniformOp<Type>& x,
    const FixedList<scalar, 3>&
)
{
    return x;
}


//- Modify a uniform operation for cutting prism0 from a tet (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& tetCutPrism0
(
    const cut::uniformOp<Type>& x,
    const FixedList<scalar, 3>&
)
{
    return x;
}


//- Modify a uniform operation for cutting prism01 from a tet (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& tetCutPrism01
(
    const cut::uniformOp<Type>& x,
    const FixedList<scalar, 4>&
)
{
    return x;
}


//- Modify a uniform operation for cutting prism23 from a tet (does nothing)
template<class Type>
inline const cut::uniformOp<Type>& tetCutPrism23
(
    const cut::uniformOp<Type>& x,
    const FixedList<scalar, 4>&
)
{
    return x;
}


//- Modify a fixed list for reordering a tri (does nothing)
template<class Type, unsigned Size>
inline FixedList<Type, 3> triReorder
(
    const FixedList<Type, Size>& x,
    const FixedList<label, 3>& indices
)
{
    FixedList<Type, 3> result;
    for (unsigned i = 0; i < 3; ++ i)
    {
        result[i] = x[indices[i]];
    }
    return result;
}


//- Modify a list for cutting a tri from a tri
template<class Type>
inline FixedList<Type, 3> triCutTri
(
    const FixedList<Type, 3>& x,
    const Pair<scalar>& f
)
{
    FixedList<Type, 3> result;
    result[0] = x[0];
    for (label i = 0; i < 2; ++ i)
    {
        result[i+1] = x[0] + f[i]*(x[i+1] - x[0]);
    }
    return result;
}


//- Modify a list for cutting a quad from a tri
template<class Type>
inline FixedList<Type, 4> triCutQuad
(
    const FixedList<Type, 3>& x,
    const Pair<scalar>& f
)
{
    FixedList<Type, 4> result;
    for (label i = 0; i < 2; ++ i)
    {
        result[i] = x[i+1];
        result[3-i] = x[0] + f[i]*(x[i+1] - x[0]);
    }
    return result;
}


//- Modify a fixed list for reordering a tet (does nothing)
template<class Type, unsigned Size>
inline FixedList<Type, 4> tetReorder
(
    const FixedList<Type, Size>& x,
    const FixedList<label, 4>& indices
)
{
    FixedList<Type, 4> result;
    for (unsigned i = 0; i < 4; ++ i)
    {
        result[i] = x[indices[i]];
    }
    return result;
}


//- Modify a list for cutting a tet from a tet
template<class Type>
inline FixedList<Type, 4> tetCutTet
(
    const FixedList<Type, 4>& x,
    const FixedList<scalar, 3>& f
)
{
    FixedList<Type, 4> result;
    result[0] = x[0];
    for (label i = 0; i < 3; ++ i)
    {
        result[i+1] = x[0] + f[i]*(x[i+1] - x[0]);
    }
    return result;
}


//- Modify a list for cutting prism0 from a tet
template<class Type>
inline FixedList<Type, 6> tetCutPrism0
(
    const FixedList<Type, 4>& x,
    const FixedList<scalar, 3>& f
)
{
    FixedList<Type, 6> result;
    for (label i = 0; i < 3; ++ i)
    {
        result[i] = x[0] + f[i]*(x[i+1] - x[0]);
        result[i+3] = x[i+1];
    }
    return result;
}


//- Modify a list for cutting prism01 from a tet
template<class Type>
inline FixedList<Type, 6> tetCutPrism01
(
    const FixedList<Type, 4>& x,
    const FixedList<scalar, 4>& f
)
{
    FixedList<Type, 6> result;
    for (label i = 0; i < 2; ++ i)
    {
        result[3*i] = x[i];
        for (label j = 0; j < 2; ++ j)
        {
            result[3*i+j+1] = x[i] + f[2*i+j]*(x[j+2] - x[i]);
        }
    }

    return result;
}


//- Modify a list for cutting prism23 from a tet
template<class Type>
inline FixedList<Type, 6> tetCutPrism23
(
    const FixedList<Type, 4>& x,
    const FixedList<scalar, 4>& f
)
{
    FixedList<Type, 6> result = tetCutPrism01(x, f);
    result[0] = x[2];
    result[3] = x[3];
    Swap(result[2], result[4]);
    return result;
}


//- Cut a tri from a tri and apply an operation to the result. The cut is made
//  along the two edges connected to vertex 0, and the cut locations are given
//  as factors along these edges. The result is the side connected to vertex 0.
template<class Op>
inline typename Op::result triCutTri
(
    const Op& op,
    const FixedList<point, 3>& p,
    const Pair<scalar>& f
)
{
    return Op(triCutTri(op, f))(triCutTri(p, f));
}


//- Apply an operation to a quad. Splits the quad into two tris.
template<class Op, class OpData>
inline typename Op::result quadOp
(
    const OpData& opData,
    const FixedList<point, 4>& p
)
{
    static const FixedList<FixedList<label, 3>, 2> i =
        {{0, 1, 2}, {0, 2, 3}};
    return
        Op(triReorder(opData, i[0]))(triReorder(p, i[0]))
      + Op(triReorder(opData, i[1]))(triReorder(p, i[1]));
}


//- Cut a quad from a tri and apply an operation to the result. The cuts are
//  the same as for triCutTri. The result is the side connected to vertices 1
//  and 2.
template<class Op>
inline typename Op::result triCutQuad
(
    const Op& op,
    const FixedList<point, 3>& p,
    const FixedList<scalar, 2>& f
)
{
    return quadOp<Op>(triCutQuad(op, f), triCutQuad(p, f));
}


//- Cut a tet from a tet and apply an operation to the result. The cut is made
//  along the three edges connected to vertex 0, and the cut locations are given
//  as factors along these edges. The result is the side connected to vertex 0.
template<class Op>
inline typename Op::result tetCutTet
(
    const Op& op,
    const FixedList<point, 4>& p,
    const FixedList<scalar, 3>& f
)
{
    return Op(tetCutTet(op, f))(tetCutTet(p, f));
}


//- Apply an operation to a prism. Splits the prism into three tets.
template<class Op, class OpData>
inline typename Op::result prismOp
(
    const OpData& opData,
    const FixedList<point, 6>& p
)
{
    static const FixedList<FixedList<label, 4>, 3> i =
        {{0, 1, 2, 4}, {0, 2, 5, 4}, {0, 4, 5, 3}};
    return
        Op(tetReorder(opData, i[0]))(tetReorder(p, i[0]))
      + Op(tetReorder(opData, i[1]))(tetReorder(p, i[1]))
      + Op(tetReorder(opData, i[2]))(tetReorder(p, i[2]));
}


//- Cut a prism from a tet and apply an operation to the result. The cuts are
//  the same as for tetCutTet. The result is the side connected to vertices 1,
//  2 and 3.
template<class Op>
inline typename Op::result tetCutPrism0
(
    const Op& op,
    const FixedList<point, 4>& p,
    const FixedList<scalar, 3>& f
)
{
    return prismOp<Op>(tetCutPrism0(op, f), tetCutPrism0(p, f));
}


//- Cut a prism from a tet and apply an operation to the result. The cut is made
//  along four edges, not edges 01 or 23, and the cut locations are given as
//  factors along these edges. The result is the side connected to edge 01.
template<class Op>
inline typename Op::result tetCutPrism01
(
    const Op& op,
    const FixedList<point, 4>& p,
    const FixedList<scalar, 4>& f
)
{
    return prismOp<Op>(tetCutPrism01(op, f), tetCutPrism01(p, f));
}


//- Cut a prism from a tet and apply an operation to the result. The cuts are
//  the same as for tetCutPrism01. The result is the side connected to edge 23.
template<class Op>
inline typename Op::result tetCutPrism23
(
    const Op& op,
    const FixedList<point, 4>& p,
    const FixedList<scalar, 4>& f
)
{
    return prismOp<Op>(tetCutPrism23(op, f), tetCutPrism23(p, f));
}


} // End namespace CML


template<class AboveOp, class BelowOp>
typename CML::cut::opAddResult<AboveOp, BelowOp>::type CML::triCut
(
    const FixedList<point, 3>& tri,
    const FixedList<scalar, 3>& level,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
)
{
    // If everything is positive or negative, then process the triangle as a
    // whole, and do a quick return
    if (level[0] >= 0 && level[1] >= 0 && level[2] >= 0)
    {
        return aboveOp(tri) + belowOp();
    }
    if (level[0] <= 0 && level[1] <= 0 && level[2] <= 0)
    {
        return aboveOp() + belowOp(tri);
    }

    // There will be just one edge without a sign change. Find it, and put it
    // opposite the first vertex. This may change the sign of the tri.
    FixedList<label, 3> indices({0, 1, 2});
    label i;
    for (i = 0; i < 3; ++ i)
    {
        if (level[(i + 1)%3]*level[(i + 2)%3] >= 0)
        {
            Swap(indices[0], indices[i]);
            break;
        }
    }
    if (i == 3)
    {
        FatalErrorInFunction
            << "The number of tri vertices above the level set should always "
            << "be 1" << exit(FatalError);
    }

    // Correct the sign
    if (indices[0] != 0)
    {
        Swap(indices[1], indices[2]);
    }

    // Permute the data
    const FixedList<point, 3> p = triReorder(tri, indices);
    const FixedList<scalar, 3> l = triReorder(level, indices);
    AboveOp a = triReorder(aboveOp, indices);
    BelowOp b = triReorder(belowOp, indices);

    // Slice off one corner to form a tri and a quad
    Pair<scalar> f;
    for (label i = 0; i < 2; ++ i)
    {
        f[i] = l[0]/(l[0] - l[i+1]);
    }
    if (l[0] > 0)
    {
        return triCutTri(a, p, f) + triCutQuad(b, p, f);
    }
    else
    {
        return triCutQuad(a, p, f) + triCutTri(b, p, f);
    }
}


template<class AboveOp, class BelowOp>
typename CML::cut::opAddResult<AboveOp, BelowOp>::type CML::triCut
(
    const FixedList<point, 3>& tri,
    const plane& p,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
)
{
    // Set the level set to the signed distance from the plane
    FixedList<scalar, 3> level;
    for (label i = 0; i < 3; ++ i)
    {
        level[i] = (tri[i] - p.refPoint()) & p.normal();
    }

    // Run the level set function
    return triCut(tri, level, aboveOp, belowOp);
}


template<class AboveOp, class BelowOp>
typename CML::cut::opAddResult<AboveOp, BelowOp>::type CML::tetCut
(
    const FixedList<point, 4>& tet,
    const FixedList<scalar, 4>& level,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
)
{
    // Get the min and max over all four vertices and quick return if there is
    // no change of sign
    scalar levelMin = VGREAT, levelMax = - VGREAT;
    for (label i = 0; i < 4; ++ i)
    {
        levelMin = min(levelMin, level[i]);
        levelMax = max(levelMax, level[i]);
    }
    if (levelMin >= 0)
    {
        return aboveOp(tet) + belowOp();
    }
    if (levelMax <= 0)
    {
        return aboveOp() + belowOp(tet);
    }

    // Partition the level so that positive values are at the start. This is
    // like a single iteration of quick-sort, except that the pivot is a hard-
    // coded zero, rather than an element of the array. This can change the sign
    // of the tet.
    FixedList<label, 4> indices({0, 1, 2, 3});
    bool signChange = false;
    label i = 0, j = 3;
    while (true)
    {
        while (i < j && level[indices[i]] > 0)
        {
            i ++;
        }
        while (j > i && level[indices[j]] <= 0)
        {
            j --;
        }
        if (i == j)
        {
            break;
        }
        Swap(indices[i], indices[j]);
        signChange = !signChange;
    }

    // The number of vertices above the slice
    label n = i;

    // If there are more positives than negatives then reverse the order so that
    // the negatives are at the start
    if (n > 2)
    {
        n = 4 - n;
        for (label i = 0; i < 2; ++ i)
        {
            Swap(indices[i], indices[3-i]);
        }
    }

    // Correct the sign
    if (signChange)
    {
        Swap(indices[2], indices[3]);
    }

    // Permute the data
    const FixedList<point, 4> p = tetReorder(tet, indices);
    const FixedList<scalar, 4> l = tetReorder(level, indices);
    AboveOp a = tetReorder(aboveOp, indices);
    BelowOp b = tetReorder(belowOp, indices);

    // Calculate the integrals above and below the level set
    if (n == 1)
    {
        // Slice off one corner to form a tet and a prism
        FixedList<scalar, 3> f;
        for (label i = 0; i < 3; ++ i)
        {
            f[i] = l[0]/(l[0] - l[i+1]);
        }
        if (l[0] > 0)
        {
            return tetCutTet(a, p, f) + tetCutPrism0(b, p, f);
        }
        else
        {
            return tetCutPrism0(a, p, f) + tetCutTet(b, p, f);
        }
    }
    else if (n == 2)
    {
        // Slice off two corners to form two prisms
        FixedList<scalar, 4> f;
        for (label i = 0; i < 2; ++ i)
        {
            for (label j = 0; j < 2; ++ j)
            {
                f[2*i+j] = l[i]/(l[i] - l[j+2]);
            }
        }
        if (l[0] > 0)
        {
            return tetCutPrism01(a, p, f) + tetCutPrism23(b, p, f);
        }
        else
        {
            return tetCutPrism23(a, p, f) + tetCutPrism01(b, p, f);
        }
    }

    FatalErrorInFunction
        << "The number of tet vertices above the level set should always be "
        << "either 1 or 2" << exit(FatalError);

    return aboveOp() + belowOp();
}


template<class AboveOp, class BelowOp>
typename CML::cut::opAddResult<AboveOp, BelowOp>::type CML::tetCut
(
    const FixedList<point, 4>& tet,
    const plane& p,
    const AboveOp& aboveOp,
    const BelowOp& belowOp
)
{
    // Set the level set to the signed distance from the plane
    FixedList<scalar, 4> level;
    for (label i = 0; i < 4; ++ i)
    {
        level[i] = (tet[i] - p.refPoint()) & p.normal();
    }

    // Run the level set function
    return tetCut(tet, level, aboveOp, belowOp);
}


#endif
