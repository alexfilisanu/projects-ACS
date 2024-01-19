parser grammar CoolParser;

options {
    tokenVocab = CoolLexer;
}

@header{
    package cool.parser;
}

program : (classes+=class SEMI)+ EOF;

class : CLASS type=TYPE (INHERITS inherit=TYPE)?
            LBRACE (definitions+=definition SEMI)* RBRACE;

definition
    :   name=ID COLON type=TYPE (ASSIGN init=expr)?                 # varDef
    |   name=ID LPAREN (formals+=formal (COMMA formals+=formal)*)? RPAREN
            COLON type=TYPE LBRACE (body=expr)? RBRACE              # funcDef
    ;

letVar : name=ID COLON type=TYPE (ASSIGN init=expr)?;

caseStatement : name=ID COLON type=TYPE RESULT init=expr SEMI;

formal : name=ID COLON type=TYPE;

expr
    :   e=expr (AT_SIGN type=TYPE)? DOT
            name=ID LPAREN (args+=expr (COMMA args+=expr)*)? RPAREN # dispatch
    |   name=ID LPAREN (args+=expr (COMMA args+=expr)*)? RPAREN     # call
    |   LBRACE (exprs+=expr SEMI)* RBRACE                           # block
    |   TILDE e=expr                                                # bitwiseNot
    |   NEW name=TYPE                                               # new
    |   left=expr op=(MULT | DIV) right=expr                        # multDiv
    |   left=expr op=(PLUS | MINUS) right=expr                      # plusMinus
    |   left=expr op=(LT | LE | EQUAL) right=expr                   # relational
    |   IF cond=expr THEN thenBranch=expr ELSE elseBranch=expr FI   # if
    |   WHILE cond=expr LOOP body=expr POOL                         # while
    |   LET letVars+=letVar (COMMA letVars+=letVar)* IN body=expr   # let
    |   CASE e=expr OF (caseStatements+=caseStatement)* ESAC        # case
    |   name=ID ASSIGN value=expr                                   # assign
    |   IS_VOID e=expr                                              # isvoid
    |   NOT e=expr                                                  # not
    |   LPAREN e=expr RPAREN                                        # paren
    |   ID                                                          # id
    |   INT                                                         # int
    |   BOOL                                                        # bool
    |   STRING                                                      # string
    ;