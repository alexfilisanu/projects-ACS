package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Self extends Expression {
	Self(Token token) {
		super(token);
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
