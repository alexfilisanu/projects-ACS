package cool.compiler;

import org.antlr.v4.runtime.Token;

public class IsVoid extends Expression {
	Expression e;

	IsVoid(Token token,
		Expression e) {
		super(token);
		this.e = e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
