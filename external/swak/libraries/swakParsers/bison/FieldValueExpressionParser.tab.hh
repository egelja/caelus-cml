/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H



#include <string>
#include <iostream>
#include "FieldValueExpressionParser_stack.hh"
#include "FieldValueExpressionParser_location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


namespace parserField {

/* Line 35 of lalr1.cc  */
#line 68 "FieldValueExpressionParser.tab.hh"

  /// A Bison parser.
  class FieldValueExpressionParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 104 "../FieldValueExpressionParser.yy"

    CML::scalar val;
    CML::label integer;
    CML::vector *vec;
    CML::tensor *ten;
    CML::symmTensor *yten;
    CML::sphericalTensor *hten;
    CML::word *name;
    CML::volVectorField *vfield;
    CML::volScalarField *sfield;
    CML::volTensorField *tfield;
    CML::volSymmTensorField *yfield;
    CML::volSphericalTensorField *hfield;
    CML::surfaceScalarField *fsfield;
    CML::surfaceVectorField *fvfield;
    CML::surfaceTensorField *ftfield;
    CML::surfaceSymmTensorField *fyfield;
    CML::surfaceSphericalTensorField *fhfield;
    CML::pointScalarField *psfield;
    CML::pointVectorField *pvfield;
    CML::pointTensorField *ptfield;
    CML::pointSymmTensorField *pyfield;
    CML::pointSphericalTensorField *phfield;



/* Line 35 of lalr1.cc  */
#line 108 "FieldValueExpressionParser.tab.hh"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     TOKEN_LINE = 258,
     TOKEN_LOOKUP = 259,
     TOKEN_SID = 260,
     TOKEN_VID = 261,
     TOKEN_TID = 262,
     TOKEN_YID = 263,
     TOKEN_HID = 264,
     TOKEN_FSID = 265,
     TOKEN_FVID = 266,
     TOKEN_FTID = 267,
     TOKEN_FYID = 268,
     TOKEN_FHID = 269,
     TOKEN_PSID = 270,
     TOKEN_PVID = 271,
     TOKEN_PTID = 272,
     TOKEN_PYID = 273,
     TOKEN_PHID = 274,
     TOKEN_FUNCTION_SID = 275,
     TOKEN_FUNCTION_FSID = 276,
     TOKEN_FUNCTION_PSID = 277,
     TOKEN_FUNCTION_VID = 278,
     TOKEN_FUNCTION_FVID = 279,
     TOKEN_FUNCTION_PVID = 280,
     TOKEN_FUNCTION_TID = 281,
     TOKEN_FUNCTION_FTID = 282,
     TOKEN_FUNCTION_PTID = 283,
     TOKEN_FUNCTION_YID = 284,
     TOKEN_FUNCTION_FYID = 285,
     TOKEN_FUNCTION_PYID = 286,
     TOKEN_FUNCTION_HID = 287,
     TOKEN_FUNCTION_FHID = 288,
     TOKEN_FUNCTION_PHID = 289,
     TOKEN_FUNCTION_LID = 290,
     TOKEN_FUNCTION_FLID = 291,
     TOKEN_FUNCTION_PLID = 292,
     TOKEN_OTHER_MESH_ID = 293,
     TOKEN_OTHER_MESH_SID = 294,
     TOKEN_OTHER_MESH_VID = 295,
     TOKEN_OTHER_MESH_TID = 296,
     TOKEN_OTHER_MESH_HID = 297,
     TOKEN_OTHER_MESH_YID = 298,
     TOKEN_SETID = 299,
     TOKEN_ZONEID = 300,
     TOKEN_FSETID = 301,
     TOKEN_FZONEID = 302,
     TOKEN_PSETID = 303,
     TOKEN_PZONEID = 304,
     TOKEN_PATCHID = 305,
     TOKEN_NUM = 306,
     TOKEN_INT = 307,
     TOKEN_VEC = 308,
     TOKEN_TEN = 309,
     TOKEN_YTEN = 310,
     TOKEN_HTEN = 311,
     START_DEFAULT = 331,
     START_VOL_SCALAR_COMMA = 332,
     START_VOL_SCALAR_CLOSE = 333,
     START_VOL_VECTOR_COMMA = 334,
     START_VOL_VECTOR_CLOSE = 335,
     START_VOL_TENSOR_COMMA = 336,
     START_VOL_TENSOR_CLOSE = 337,
     START_VOL_YTENSOR_COMMA = 338,
     START_VOL_YTENSOR_CLOSE = 339,
     START_VOL_HTENSOR_COMMA = 340,
     START_VOL_HTENSOR_CLOSE = 341,
     START_VOL_LOGICAL_COMMA = 342,
     START_VOL_LOGICAL_CLOSE = 343,
     START_SURFACE_SCALAR_COMMA = 344,
     START_SURFACE_SCALAR_CLOSE = 345,
     START_SURFACE_VECTOR_COMMA = 346,
     START_SURFACE_VECTOR_CLOSE = 347,
     START_SURFACE_TENSOR_COMMA = 348,
     START_SURFACE_TENSOR_CLOSE = 349,
     START_SURFACE_YTENSOR_COMMA = 350,
     START_SURFACE_YTENSOR_CLOSE = 351,
     START_SURFACE_HTENSOR_COMMA = 352,
     START_SURFACE_HTENSOR_CLOSE = 353,
     START_SURFACE_LOGICAL_COMMA = 354,
     START_SURFACE_LOGICAL_CLOSE = 355,
     START_POINT_SCALAR_COMMA = 356,
     START_POINT_SCALAR_CLOSE = 357,
     START_POINT_VECTOR_COMMA = 358,
     START_POINT_VECTOR_CLOSE = 359,
     START_POINT_TENSOR_COMMA = 360,
     START_POINT_TENSOR_CLOSE = 361,
     START_POINT_YTENSOR_COMMA = 362,
     START_POINT_YTENSOR_CLOSE = 363,
     START_POINT_HTENSOR_COMMA = 364,
     START_POINT_HTENSOR_CLOSE = 365,
     START_POINT_LOGICAL_COMMA = 366,
     START_POINT_LOGICAL_CLOSE = 367,
     START_CLOSE_ONLY = 368,
     START_COMMA_ONLY = 369,
     TOKEN_LAST_FUNCTION_CHAR = 370,
     TOKEN_IN_FUNCTION_CHAR = 371,
     TOKEN_VECTOR = 372,
     TOKEN_TENSOR = 373,
     TOKEN_SYMM_TENSOR = 374,
     TOKEN_SPHERICAL_TENSOR = 375,
     TOKEN_TRUE = 376,
     TOKEN_FALSE = 377,
     TOKEN_x = 378,
     TOKEN_y = 379,
     TOKEN_z = 380,
     TOKEN_xx = 381,
     TOKEN_xy = 382,
     TOKEN_xz = 383,
     TOKEN_yx = 384,
     TOKEN_yy = 385,
     TOKEN_yz = 386,
     TOKEN_zx = 387,
     TOKEN_zy = 388,
     TOKEN_zz = 389,
     TOKEN_ii = 390,
     TOKEN_unitTensor = 391,
     TOKEN_pi = 392,
     TOKEN_rand = 393,
     TOKEN_randFixed = 394,
     TOKEN_id = 395,
     TOKEN_cpu = 396,
     TOKEN_weight = 397,
     TOKEN_randNormal = 398,
     TOKEN_randNormalFixed = 399,
     TOKEN_position = 400,
     TOKEN_fposition = 401,
     TOKEN_fprojection = 402,
     TOKEN_pposition = 403,
     TOKEN_face = 404,
     TOKEN_area = 405,
     TOKEN_volume = 406,
     TOKEN_dist = 407,
     TOKEN_distToPatch = 408,
     TOKEN_nearDist = 409,
     TOKEN_rdist = 410,
     TOKEN_set = 411,
     TOKEN_zone = 412,
     TOKEN_fset = 413,
     TOKEN_fzone = 414,
     TOKEN_pset = 415,
     TOKEN_pzone = 416,
     TOKEN_onPatch = 417,
     TOKEN_internalFace = 418,
     TOKEN_div = 419,
     TOKEN_grad = 420,
     TOKEN_curl = 421,
     TOKEN_snGrad = 422,
     TOKEN_magSqrGradGrad = 423,
     TOKEN_laplacian = 424,
     TOKEN_ddt = 425,
     TOKEN_oldTime = 426,
     TOKEN_d2dt2 = 427,
     TOKEN_meshPhi = 428,
     TOKEN_flux = 429,
     TOKEN_integrate = 430,
     TOKEN_surfSum = 431,
     TOKEN_interpolate = 432,
     TOKEN_interpolateToPoint = 433,
     TOKEN_interpolateToCell = 434,
     TOKEN_faceAverage = 435,
     TOKEN_reconstruct = 436,
     TOKEN_surf = 437,
     TOKEN_point = 438,
     TOKEN_deltaT = 439,
     TOKEN_time = 440,
     TOKEN_pow = 441,
     TOKEN_log = 442,
     TOKEN_exp = 443,
     TOKEN_mag = 444,
     TOKEN_magSqr = 445,
     TOKEN_sin = 446,
     TOKEN_cos = 447,
     TOKEN_tan = 448,
     TOKEN_min = 449,
     TOKEN_max = 450,
     TOKEN_minPosition = 451,
     TOKEN_maxPosition = 452,
     TOKEN_sum = 453,
     TOKEN_average = 454,
     TOKEN_sqr = 455,
     TOKEN_sqrt = 456,
     TOKEN_transpose = 457,
     TOKEN_diag = 458,
     TOKEN_tr = 459,
     TOKEN_dev = 460,
     TOKEN_symm = 461,
     TOKEN_skew = 462,
     TOKEN_det = 463,
     TOKEN_cof = 464,
     TOKEN_inv = 465,
     TOKEN_sph = 466,
     TOKEN_twoSymm = 467,
     TOKEN_dev2 = 468,
     TOKEN_eigenValues = 469,
     TOKEN_eigenVectors = 470,
     TOKEN_log10 = 471,
     TOKEN_asin = 472,
     TOKEN_acos = 473,
     TOKEN_atan = 474,
     TOKEN_sinh = 475,
     TOKEN_cosh = 476,
     TOKEN_tanh = 477,
     TOKEN_asinh = 478,
     TOKEN_acosh = 479,
     TOKEN_atanh = 480,
     TOKEN_erf = 481,
     TOKEN_erfc = 482,
     TOKEN_lgamma = 483,
     TOKEN_besselJ0 = 484,
     TOKEN_besselJ1 = 485,
     TOKEN_besselY0 = 486,
     TOKEN_besselY1 = 487,
     TOKEN_sign = 488,
     TOKEN_pos = 489,
     TOKEN_neg = 490,
     TOKEN_OR = 491,
     TOKEN_AND = 492,
     TOKEN_NEQ = 493,
     TOKEN_EQ = 494,
     TOKEN_GEQ = 495,
     TOKEN_LEQ = 496,
     TOKEN_HODGE = 497,
     TOKEN_NOT = 498,
     TOKEN_NEG = 499
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    FieldValueExpressionParser (void * scanner_yyarg, FieldValueExpressionDriver& driver_yyarg, int start_token_yyarg, int numberOfFunctionChars_yyarg);
    virtual ~FieldValueExpressionParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned short int token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned short int yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const unsigned short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned short int yystos_[];

    /// For a rule, its LHS.
    static const unsigned short int yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    void * scanner;
    FieldValueExpressionDriver& driver;
    int start_token;
    int numberOfFunctionChars;
  };

} // parserField

/* Line 35 of lalr1.cc  */
#line 525 "FieldValueExpressionParser.tab.hh"



#endif /* ! defined PARSER_HEADER_H */
