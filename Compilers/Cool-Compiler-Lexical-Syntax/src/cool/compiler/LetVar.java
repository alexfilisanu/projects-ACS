package cool.compiler;

import org.antlr.v4.runtime.Token;

public class LetVar extends Expression {
	Token name;
	Token type;
	Expression init;

	LetVar(Token name,
		   Token type,
		   Token start) {
		super(start);
		this.name = name;
		this.type = type;
	}

	LetVar(Token name,
		   Token type,
		   Expression init,
		   Token start) {
		this(name, type, start);
		this.init = init;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
