package token

type TokenType string

type Token struct {
	Type    TokenType
	Literal string
}

// TODO: Look into using integers or bytes instead of strings for better perfomance

const (
	EOF       = "EOF"
	ILLEGAL   = "ILLEGAL"
	IDENT     = "IDENT"
	INT       = "INT"
	ASSIGN    = "="
	PLUS      = "+"
	MINUS     = "-"
	COMMA     = ","
	SEMICOLON = ";"
	LPAR      = "("
	RPAR      = ")"
	LBRACE    = "{"
	RBRACE    = "}"
	ASTERISK  = "*"
	DEREF     = "&"
	PIPE      = "|"
	FUNC      = "FUNCTION"
	LET       = "LET"
	BANG      = "!"
	SLASH     = "/"
	LT        = "<" // Larger than
	GT        = ">" // Greater than
)

var keywords = map[string]TokenType{
	"fn":  FUNC,
	"let": LET,
}

func LookupIdent(ident string) TokenType {
	if tok, ok := keywords[ident]; ok {
		return tok
	}

	return IDENT
}
