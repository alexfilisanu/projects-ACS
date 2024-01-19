package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Int extends Expression {
	Int(Token token) {
		super(token);
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
