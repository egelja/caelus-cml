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
#include "CloudValueExpressionParser_stack.hh"
#include "CloudValueExpressionParser_location.hh"

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


namespace parserCloud {

/* Line 35 of lalr1.cc  */
#line 68 "CloudValueExpressionParser.tab.hh"

  /// A Bison parser.
  class CloudValueExpressionParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 83 "../CloudValueExpressionParser.yy"

    CML::scalar val;
    CML::label integer;
    CML::vector *vec;
    CML::word *name;
    CML::vectorField *vfield;
    CML::scalarField *sfield;
    CML::Field<bool> *lfield;
    CML::tensorField *tfield;
    CML::symmTensorField *yfield;
    CML::sphericalTensorField *hfield;



/* Line 35 of lalr1.cc  */
#line 96 "CloudValueExpressionParser.tab.hh"
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
     TOKEN_LID = 262,
     TOKEN_TID = 263,
     TOKEN_YID = 264,
     TOKEN_HID = 265,
     TOKEN_GAS_SID = 266,
     TOKEN_GAS_VID = 267,
     TOKEN_GAS_TID = 268,
     TOKEN_GAS_YID = 269,
     TOKEN_GAS_HID = 270,
     TOKEN_FUNCTION_SID = 271,
     TOKEN_FUNCTION_VID = 272,
     TOKEN_FUNCTION_TID = 273,
     TOKEN_FUNCTION_YID = 274,
     TOKEN_FUNCTION_HID = 275,
     TOKEN_FUNCTION_LID = 276,
     TOKEN_SETID = 277,
     TOKEN_ZONEID = 278,
     TOKEN_NUM = 279,
     TOKEN_INT = 280,
     TOKEN_VEC = 281,
     START_DEFAULT = 288,
     START_CLOUD_SCALAR_COMMA = 289,
     START_CLOUD_SCALAR_CLOSE = 290,
     START_CLOUD_VECTOR_COMMA = 291,
     START_CLOUD_VECTOR_CLOSE = 292,
     START_CLOUD_TENSOR_COMMA = 293,
     START_CLOUD_TENSOR_CLOSE = 294,
     START_CLOUD_YTENSOR_COMMA = 295,
     START_CLOUD_YTENSOR_CLOSE = 296,
     START_CLOUD_HTENSOR_COMMA = 297,
     START_CLOUD_HTENSOR_CLOSE = 298,
     START_CLOUD_LOGICAL_COMMA = 299,
     START_CLOUD_LOGICAL_CLOSE = 300,
     START_CLOSE_ONLY = 301,
     START_COMMA_ONLY = 302,
     TOKEN_LAST_FUNCTION_CHAR = 303,
     TOKEN_IN_FUNCTION_CHAR = 304,
     TOKEN_VECTOR = 305,
     TOKEN_TENSOR = 306,
     TOKEN_SYMM_TENSOR = 307,
     TOKEN_SPHERICAL_TENSOR = 308,
     TOKEN_TRUE = 309,
     TOKEN_FALSE = 310,
     TOKEN_x = 311,
     TOKEN_y = 312,
     TOKEN_z = 313,
     TOKEN_xx = 314,
     TOKEN_xy = 315,
     TOKEN_xz = 316,
     TOKEN_yx = 317,
     TOKEN_yy = 318,
     TOKEN_yz = 319,
     TOKEN_zx = 320,
     TOKEN_zy = 321,
     TOKEN_zz = 322,
     TOKEN_ii = 323,
     TOKEN_fluidPhase = 324,
     TOKEN_unitTensor = 325,
     TOKEN_pi = 326,
     TOKEN_rand = 327,
     TOKEN_randFixed = 328,
     TOKEN_id = 329,
     TOKEN_randNormal = 330,
     TOKEN_randNormalFixed = 331,
     TOKEN_position = 332,
     TOKEN_deltaT = 333,
     TOKEN_time = 334,
     TOKEN_pow = 335,
     TOKEN_log = 336,
     TOKEN_exp = 337,
     TOKEN_mag = 338,
     TOKEN_magSqr = 339,
     TOKEN_sin = 340,
     TOKEN_cos = 341,
     TOKEN_tan = 342,
     TOKEN_min = 343,
     TOKEN_max = 344,
     TOKEN_minPosition = 345,
     TOKEN_maxPosition = 346,
     TOKEN_average = 347,
     TOKEN_sum = 348,
     TOKEN_sqr = 349,
     TOKEN_sqrt = 350,
     TOKEN_log10 = 351,
     TOKEN_asin = 352,
     TOKEN_acos = 353,
     TOKEN_atan = 354,
     TOKEN_sinh = 355,
     TOKEN_cosh = 356,
     TOKEN_tanh = 357,
     TOKEN_asinh = 358,
     TOKEN_acosh = 359,
     TOKEN_atanh = 360,
     TOKEN_erf = 361,
     TOKEN_erfc = 362,
     TOKEN_lgamma = 363,
     TOKEN_besselJ0 = 364,
     TOKEN_besselJ1 = 365,
     TOKEN_besselY0 = 366,
     TOKEN_besselY1 = 367,
     TOKEN_sign = 368,
     TOKEN_pos = 369,
     TOKEN_neg = 370,
     TOKEN_transpose = 371,
     TOKEN_diag = 372,
     TOKEN_tr = 373,
     TOKEN_dev = 374,
     TOKEN_symm = 375,
     TOKEN_skew = 376,
     TOKEN_det = 377,
     TOKEN_cof = 378,
     TOKEN_inv = 379,
     TOKEN_sph = 380,
     TOKEN_twoSymm = 381,
     TOKEN_dev2 = 382,
     TOKEN_eigenValues = 383,
     TOKEN_eigenVectors = 384,
     TOKEN_cpu = 385,
     TOKEN_weight = 386,
     TOKEN_set = 387,
     TOKEN_zone = 388,
     TOKEN_OR = 389,
     TOKEN_AND = 390,
     TOKEN_NEQ = 391,
     TOKEN_EQ = 392,
     TOKEN_GEQ = 393,
     TOKEN_LEQ = 394,
     TOKEN_HODGE = 395,
     TOKEN_NOT = 396,
     TOKEN_NEG = 397
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    CloudValueExpressionParser (void * scanner_yyarg, CloudValueExpressionDriver& driver_yyarg, int start_token_yyarg, int numberOfFunctionChars_yyarg);
    virtual ~CloudValueExpressionParser ();

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
    typedef unsigned char token_number_type;
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
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
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
    CloudValueExpressionDriver& driver;
    int start_token;
    int numberOfFunctionChars;
  };

} // parserCloud

/* Line 35 of lalr1.cc  */
#line 424 "CloudValueExpressionParser.tab.hh"



#endif /* ! defined PARSER_HEADER_H */
