package cool.compiler;

import org.antlr.v4.runtime.Token;

class Id extends Expression {
	Id(Token token) {
		super(token);
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
