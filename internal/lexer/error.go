package lexer

import (
	"fmt"
	"strings"
)

func (lexer *Lexer) error(msg string) {
	line := lexer.getLineText(lexer.lineNumber)
	caret := makeCaret(lexer.columnNumber)

	fmt.Printf(
		"%s: %s\n  %s %s:%d:%d\n   %s\n%2d | %s\n   | %s\n",
		"error",
		msg,
		"-->",
		lexer.fileName, lexer.lineNumber, lexer.columnNumber,
		"|",
		lexer.lineNumber, line, caret,
	)
}

func (lexer *Lexer) getLineText(lineNum int) string {
	if lineNum < 1 {
		return ""
	}
	start := 0
	currentLine := 1
	for i, r := range lexer.source {
		if currentLine == lineNum {
			start = i
			break
		}
		if r == '\n' {
			currentLine++
		}
	}
	end := len(lexer.source)
	for i := start; i < len(lexer.source); i++ {
		if lexer.source[i] == '\n' {
			end = i
			break
		}
	}
	return string(lexer.source[start:end])
}

func makeCaret(col int) string {
	if col < 1 {
		col = 1
	}
	return fmt.Sprintf("%s^", strings.Repeat(" ", col-2))
}
