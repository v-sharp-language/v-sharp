package lexer

type TokenType int

type Token struct {
	Type   TokenType
	Lexeme string
	Line   int
	Column int

	File   string
	Source []rune
}

const (
	Illegal TokenType = iota
	Comment

	Identifier
	Integer
	Float
	Unsigned
	Int8
	Int16
	Int32
	Int64
	Float32
	Float64
	UInt8
	UInt16
	UInt32
	UInt64
	Boolean
	String
	Byte
	Void

	Plus
	Minus
	Asterisk
	Slash
	Percent

	LeftParen
	RightParen
	LeftBrace
	RightBrace
	LeftBracket
	RightBracket
	Comma
	Semicolon
	Colon
	Dot
	Vbar

	Assign
	Equal
	NotEqual
	LessThan
	GreaterThan
	LessEqual
	GreaterEqual

	Not
	And
	Or

	KwPublic
	KwPrivate
	KwVirtual
	KwOverride
	KwStatic
	KwConst
	KwVar
	KwIf
	KwElse
	KwMatch
	KwFor
	KwReturn
	KwStructure
	KwEnumeration
	KwDefine
	KwTypedef
	KwClass
	KwInt8
	KwInt16
	KwInt32
	KwInt64
	KwFloat32
	KwFloat64
	KwUInt8
	KwUInt16
	KwUInt32
	KwUInt64
	KwBoolean
	KwString
	KwByte
	KwVoid
	EndOfFile
)

var KeywordMap = map[string]TokenType{
	"public":      KwPublic,
	"private":     KwPrivate,
	"virtual":     KwVirtual,
	"override":    KwOverride,
	"static":      KwStatic,
	"const":       KwConst,
	"var":         KwVar,
	"if":          KwIf,
	"else":        KwElse,
	"match":       KwMatch,
	"for":         KwFor,
	"return":      KwReturn,
	"structure":   KwStructure,
	"enumeration": KwEnumeration,
	"define":      KwDefine,
	"typedef":     KwTypedef,
	"class":       KwClass,
	"true":        Boolean,
	"false":       Boolean,
	"int8":        KwInt8,
	"int16":       KwInt16,
	"int32":       KwInt32,
	"int64":       KwInt64,
	"float32":     KwFloat32,
	"float64":     KwFloat64,
	"uint8":       KwUInt8,
	"uint16":      KwUInt16,
	"uint32":      KwUInt32,
	"uint64":      KwUInt64,
	"boolean":     KwBoolean,
	"string":      KwString,
	"byte":        KwByte,
	"void":        KwVoid,
}

var precedence = map[TokenType]int{
	Or:       1,
	Vbar:     1,
	Colon:    1,
	And:      2,
	Equal:    3,
	NotEqual: 3,

	LessThan:     4,
	LessEqual:    4,
	GreaterThan:  4,
	GreaterEqual: 4,

	Plus:     5,
	Minus:    5,
	Asterisk: 6,
	Slash:    6,
	Percent:  6,
}
