package cool.compiler;

import org.antlr.v4.runtime.Token;

public abstract class ASTNode {
	Token token;

	ASTNode(Token token) {
		this.token = token;
	}

	public abstract <T> T accept(ASTVisitor<T> visitor);
}
