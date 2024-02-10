package cool.compiler;

import org.antlr.v4.runtime.Token;

public class New extends Expression {
	Token name;

	New(Token token,
		Token name) {
		super(token);
		this.name = name;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
