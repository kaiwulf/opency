grammar Sample;

options{
	language = Java; 
}

@header {
	pagage a.b.c
}

@lexer::header {
	package a.b.c
}

start
	:	'start' IDENT ':'
		(const, var)*
	 	'stop' IDENT ';'
	;

const
	:	'const' IDENT ':' type '=>' expression '.'
	;

var
	:	'var' IDENT (',' IDENT)* ':' type '.'

main
	:	'mod' 'main' IDENT ':'
		'end' 'main' IDENT ';'
	;

type
	:	'int'
	;

expression
	:	INTEGER
	;

IDENT : ('a'..'z' | 'A'..'Z')('a'..'z' | 'A'..'Z' | '0'..'9')*;
WS : (' ' | '\t' | '\n' | '\r' | '\f')+ ($channel = HIDDEN;);