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

func (l *Lexer) getLineText(lineNum int) string {
	if lineNum < 1 {
		return ""
	}
	currentLine := 1
	start := 0
	for i, r := range l.source {
		if currentLine == lineNum {
			start = i
			break
		}
		if r == '\n' {
			currentLine++
		}
	}
	end := start
	for end < len(l.source) && l.source[end] != '\n' {
		end++
	}
	return string(l.source[start:end])
}

func makeCaret(col int) string {
	if col < 1 {
		col = 1
	}
	return fmt.Sprintf("%s^", strings.Repeat(" ", col-2))
}
