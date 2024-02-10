package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Formal extends Expression {
	Token name;
	Token type;

	Formal(Token name,
		   Token type,
		   Token start) {
		super(start);
		this.name = name;
		this.type = type;
	}

	@Override
	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
