package cool.compiler;

import org.antlr.v4.runtime.Token;

public abstract class ASTNode {
	private Token token;

	public ASTNode(Token token) {
		this.token = token;
	}

	public Token getToken() {
		return token;
	}

	public abstract <T> T accept(ASTVisitor<T> visitor);
}
