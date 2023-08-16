package token

type TokenType string

type Token struct {
	Type    TokenType
	Literal string
}

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
	POINTER   = "*"
	DEREF     = "&"
	PIPE      = "|"
	FUNC      = "FUNCTION"
	LET       = "LET"
)
