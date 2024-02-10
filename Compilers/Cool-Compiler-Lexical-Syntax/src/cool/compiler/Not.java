package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Not extends Expression {
	Expression e;

	Not(Token token,
			   Expression e) {
		super(token);
		this.e = e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
