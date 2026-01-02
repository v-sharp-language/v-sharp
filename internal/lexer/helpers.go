package lexer

import (
	"fmt"
	"unicode"
)

func StripNumericSeparators(s string) string {
	out := []rune{}
	for _, r := range s {
		if r != '_' {
			out = append(out, r)
		}
	}
	return string(out)
}

func (_type TokenType) Precedence() int {
	if p, ok := precedence[_type]; ok {
		return p
	}
	return 0
}

func LookupIdentifier(name string) TokenType {
	if k, ok := KeywordMap[name]; ok {
		return k
	}
	return Identifier
}

func isIdentifierStart(r rune) bool {
	return unicode.IsLetter(r) || r == '_' || r == '$'
}

func isIdentifierPart(r rune) bool {
	return unicode.IsLetter(r) || unicode.IsDigit(r) || r == '_' || r == '$' || r == '\''
}

func (t Token) String() string {
	return fmt.Sprintf("%d('%s') at %d:%d", t.Type, t.Lexeme, t.Line, t.Column)
}
