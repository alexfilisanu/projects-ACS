package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Paren extends Expression {
	Expression e;

	Paren(Expression e,
		  Token start) {
		super(start);
		this.e = e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
