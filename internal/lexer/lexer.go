package lexer

import (
	"fmt"
	"strconv"
	"unicode"
)

type Lexer struct {
	source       []rune
	position     int
	lineNumber   int
	columnNumber int
	fileName     string
}

func Tokenize(source, filename string) ([]Token, error) {
	lexer := New(source, filename)
	out := []Token{}
	for {
		token := lexer.Next()
		out = append(out, token)
		if token.Type == EndOfFile {
			break
		}
		if token.Type == Illegal {
			lexer.error("Illegal character")
		}
	}
	return out, nil
}

func New(source, filename string) *Lexer {
	return &Lexer{
		source:       []rune(source),
		position:     0,
		lineNumber:   1,
		columnNumber: 1,
		fileName:     filename,
	}
}

func (lexer *Lexer) Next() Token {
	lexer.consumeWhitespace()
	startLine, startColumn := lexer.lineNumber, lexer.columnNumber
	char := lexer.peekRuneAt(0)

	if char == 0 {
		return lexer.makeToken(EndOfFile, "", startLine, startColumn)
	}

	if isIdentifierStart(char) {
		lex := lexer.scanIdentifier()
		type_ := LookupIdentifier(lex)
		return lexer.makeToken(type_, lex, startLine, startColumn)
	}

	if unicode.IsDigit(char) {
		lex, type_ := lexer.scanNumberLiteral()
		return lexer.makeToken(type_, lex, startLine, startColumn)
	}

	if char == '"' {
		lex := lexer.scanStringLiteral()
		return lexer.makeToken(String, lex, startLine, startColumn)
	}

	if char == '\'' {
		lex := lexer.scanByteLiteral()
		return lexer.makeToken(Byte, lex, startLine, startColumn)
	}

	if char == '/' && lexer.peekRuneAt(1) == '/' {
		lex := lexer.scanLineComment()
		return lexer.makeToken(Comment, lex, startLine, startColumn)
	}

	switch char {
	case '=':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '=' {
			lexer.advanceRune()
			return lexer.makeToken(Equal, "==", startLine, startColumn)
		}
		return lexer.makeToken(Assign, "=", startLine, startColumn)
	case '!':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '=' {
			lexer.advanceRune()
			return lexer.makeToken(NotEqual, "!=", startLine, startColumn)
		}
		return lexer.makeToken(Not, "!", startLine, startColumn)
	case '<':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '=' {
			lexer.advanceRune()
			return lexer.makeToken(LessEqual, "<=", startLine, startColumn)
		}
		return lexer.makeToken(LessThan, "<", startLine, startColumn)
	case '>':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '=' {
			lexer.advanceRune()
			return lexer.makeToken(GreaterEqual, ">=", startLine, startColumn)
		}
		return lexer.makeToken(GreaterThan, ">", startLine, startColumn)
	case '+':
		lexer.advanceRune()
		return lexer.makeToken(Plus, "+", startLine, startColumn)
	case '-':
		lexer.advanceRune()
		return lexer.makeToken(Minus, "-", startLine, startColumn)
	case '*':
		lexer.advanceRune()
		return lexer.makeToken(Asterisk, "*", startLine, startColumn)
	case '/':
		lexer.advanceRune()
		return lexer.makeToken(Slash, "/", startLine, startColumn)
	case '%':
		lexer.advanceRune()
		return lexer.makeToken(Percent, "%", startLine, startColumn)
	case ':':
		lexer.advanceRune()
		return lexer.makeToken(Colon, ":", startLine, startColumn)
	case ',':
		lexer.advanceRune()
		return lexer.makeToken(Comma, ",", startLine, startColumn)
	case '{':
		lexer.advanceRune()
		return lexer.makeToken(LeftBrace, "{", startLine, startColumn)
	case '}':
		lexer.advanceRune()
		return lexer.makeToken(RightBrace, "}", startLine, startColumn)
	case '(':
		lexer.advanceRune()
		return lexer.makeToken(LeftParen, "(", startLine, startColumn)
	case ')':
		lexer.advanceRune()
		return lexer.makeToken(RightParen, ")", startLine, startColumn)
	case '[':
		lexer.advanceRune()
		return lexer.makeToken(LeftBracket, "[", startLine, startColumn)
	case ']':
		lexer.advanceRune()
		return lexer.makeToken(RightBracket, "]", startLine, startColumn)
	case '.':
		lexer.advanceRune()
		return lexer.makeToken(Dot, ".", startLine, startColumn)
	case ';':
		lexer.advanceRune()
		return lexer.makeToken(Semicolon, ";", startLine, startColumn)
	case '&':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '&' {
			lexer.advanceRune()
			return lexer.makeToken(And, "&&", startLine, startColumn)
		}
		return lexer.makeToken(Illegal, "&", startLine, startColumn)
	case '|':
		lexer.advanceRune()
		if lexer.peekRuneAt(0) == '|' {
			lexer.advanceRune()
			return lexer.makeToken(Or, "||", startLine, startColumn)
		}
		return lexer.makeToken(Vbar, "|", startLine, startColumn)
	default:
		r := lexer.advanceRune()
		return lexer.makeToken(Illegal, string(r), startLine, startColumn)
	}
}

func (lexer *Lexer) advanceRune() rune {
	if lexer.position >= len(lexer.source) {
		return 0
	}
	r := lexer.source[lexer.position]
	lexer.position++
	if r == '\n' {
		lexer.lineNumber++
		lexer.columnNumber = 1
	} else {
		lexer.columnNumber++
	}
	return r
}

func (lexer *Lexer) peekRuneAt(offset int) rune {
	index := lexer.position + offset
	if index >= len(lexer.source) {
		return 0
	}
	return lexer.source[index]
}

func (lexer *Lexer) makeToken(_type TokenType, lexeme string, line, column int) Token {
	return Token{
		Type:   _type,
		Lexeme: lexeme,
		Line:   line,
		Column: column,
		File:   lexer.fileName,
		Source: lexer.source,
	}
}

func (lexer *Lexer) scanIdentifier() string {
	start := lexer.position
	for {
		ch := lexer.peekRuneAt(0)
		if ch == 0 || !(isIdentifierPart(ch)) {
			break
		}
		lexer.advanceRune()
	}
	return string(lexer.source[start:lexer.position])
}

func (lexer *Lexer) scanNumberLiteral() (string, TokenType) {
	start := lexer.position
	isFloat := false
	for {
		ch := lexer.peekRuneAt(0)
		if ch == '.' {
			if lexer.peekRuneAt(1) == '.' {
				break
			}
			isFloat = true
			lexer.advanceRune()
			continue
		}
		if !unicode.IsDigit(ch) && ch != '_' {
			break
		}
		lexer.advanceRune()
	}
	lex := string(lexer.source[start:lexer.position])
	clean := StripNumericSeparators(lex)
	if isFloat {
		if _, err := strconv.ParseFloat(clean, 64); err == nil {
			return lex, Float
		}
	}
	if _, err := strconv.ParseInt(clean, 10, 64); err == nil {
		return lex, Integer
	}
	if _, err := strconv.ParseFloat(clean, 64); err == nil {
		return lex, Float
	}
	return lex, Illegal
}

func (lexer *Lexer) scanStringLiteral() string {
	quote := lexer.advanceRune()
	start := lexer.position - 1
	if quote != '"' {
		return string(lexer.source[start:lexer.position])
	}
	ch := lexer.advanceRune()
	if ch == 0 {
		lexer.error("unterminated string literal")
		return string(lexer.source[start:lexer.position])
	}
	if ch == quote {
		lexer.error("empty string literal")
		return string(lexer.source[start:lexer.position])
	}
	runeCount := 0
	for {
		if ch == '\\' {
			runeCount++
			esc := lexer.advanceRune()
			if esc == 0 {
				lexer.error("unterminated escape sequence in string literal")
				return string(lexer.source[start:lexer.position])
			}
			switch esc {
			case 'n', 't', 'r', '\\', '\'', '"', '0':
			default:
				lexer.error(fmt.Sprintf("invalid escape character: \\%c", esc))
				return string(lexer.source[start:lexer.position])
			}
		} else {
			runeCount++
		}
		ch = lexer.advanceRune()
		if ch == 0 {
			lexer.error("unterminated string literal")
			return string(lexer.source[start:lexer.position])
		}
		if ch == quote {
			break
		}
	}
	return string(lexer.source[start:lexer.position])
}

func (lexer *Lexer) scanByteLiteral() string {
	quote := lexer.advanceRune()
	start := lexer.position - 1
	if quote != '\'' {
		return string(lexer.source[start:lexer.position])
	}
	ch := lexer.advanceRune()
	if ch == 0 {
		lexer.error("unterminated character literal")
		return string(lexer.source[start:lexer.position])
	}
	if ch == quote {
		lexer.error("empty character literal")
		return string(lexer.source[start:lexer.position])
	}
	var runeCount int = 0
	if ch == '\\' {
		runeCount++
		esc := lexer.advanceRune()
		if esc == 0 {
			lexer.error("unterminated escape sequence in character literal")
			return string(lexer.source[start:lexer.position])
		}
		switch esc {
		case 'n', 't', 'r', '\\', '\'', '"', '0':
		default:
			lexer.error(fmt.Sprintf("invalid escape character: \\%c", esc))
			return string(lexer.source[start:lexer.position])
		}
	} else {
		runeCount++
	}
	end := lexer.advanceRune()
	if end == 0 {
		lexer.error("unterminated character literal")
		return string(lexer.source[start:lexer.position])
	}
	if end != '\'' {
		lexer.error("extra characters in character literal (expected closing ')")
		return string(lexer.source[start:lexer.position])
	}
	if runeCount != 1 {
		lexer.error("character literal must contain exactly 1 character or valid escape")
	}
	return string(lexer.source[start:lexer.position])
}

func (lexer *Lexer) scanLineComment() string {
	start := lexer.position
	lexer.advanceRune()
	lexer.advanceRune()
	for {
		ch := lexer.peekRuneAt(0)
		if ch == 0 || ch == '\n' {
			break
		}
		lexer.advanceRune()
	}
	return string(lexer.source[start:lexer.position])
}

func (lexer *Lexer) consumeWhitespace() {
	for {
		ch := lexer.peekRuneAt(0)
		if ch == 0 {
			return
		}
		if ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' {
			lexer.advanceRune()
			continue
		}
		break
	}
}
