package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Paren extends Expression {
	private Expression e;

	public Paren(Expression e,
		  		 Token start) {
		super(start);
		this.e = e;
	}

	public Expression getE() {
		return e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
