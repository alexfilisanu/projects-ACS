package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Type extends ASTNode {
	public Type(Token token) {
		super(token);
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
