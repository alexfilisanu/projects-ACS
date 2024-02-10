lexer grammar CoolLexer;

tokens { ERROR } 

@header{
    package cool.lexer;	
}

@members{
    int stringSize = 0;

    private void raiseError(String msg) {
        setText(msg);
        setType(ERROR);
    }
}

IF : 'if';
THEN : 'then';
ELSE : 'else';
FI: 'fi';

WHILE : 'while';
LOOP : 'loop';
POOL : 'pool';

LET : 'let';
IN : 'in';

CASE : 'case';
OF : 'of';
ESAC : 'esac';

NEW : 'new';
CLASS : 'class';
INHERITS : 'inherits';
NOT : 'not';
IS_VOID : 'isvoid';
SELF : 'self';

fragment LOWERCASE : [a-z];
fragment UPPERCASE : [A-Z];
fragment LETTER : [a-zA-Z];
fragment DIGIT : [0-9];
fragment NULLCHAR: '\u0000';
fragment SPECIALCHARS : '\\t' | '\\n' | '\\b' | '\\f';
fragment NEW_LINE : '\r'? '\n';

TYPE : UPPERCASE(LETTER | '_' | DIGIT)*;
ID : LOWERCASE(LETTER | '_' | DIGIT)*;

INT : DIGIT+;
BOOL : 'true' | 'false';

STRING : QUOTATION { stringSize = 0; }
    (
        NULLCHAR { raiseError("String contains null character"); }
        | '\\\r\n'
        | SPECIALCHARS { stringSize++; }
        | . { stringSize++; }
    )*?
    (
        QUOTATION { if (stringSize > 1024) { raiseError("String constant too long"); } }
        | NEW_LINE { raiseError("Unterminated string constant"); }
        | EOF { raiseError("EOF in string constant"); }
    );

SEMI : ';';
COMMA : ',';
DOT : '.';
COLON : ':';
QUOTATION : '"';
ASSIGN : '<-';
LPAREN : '(';
RPAREN : ')';
LBRACE : '{';
RBRACE : '}';
AT_SIGN : '@';
TILDE : '~';
PLUS : '+';
MINUS : '-';
MULT : '*';
DIV : '/';
EQUAL : '=';
LT : '<';
LE : '<=';
RESULT : '=>';

LINE_COMMENT : '--' .*? (NEW_LINE | EOF) -> skip;

BLOCK_COMMENT : '(*' (BLOCK_COMMENT | .)*? '*)' -> skip;

// comment can contain either another BLOCK_COMMENT or any character
// that is not * or ) and if EOF is before *) => "EOF in comment"
EOF_IN_COMMENT : '(*' (BLOCK_COMMENT | ~([*)]).)*? (EOF { raiseError("EOF in comment"); });

UNMATCHED_END_COMMENT : '*)' { raiseError("Unmatched *)"); };

HASHTAG : '#' { raiseError("Invalid character: #"); };

WS : [ \n\f\r\t]+ -> skip;
