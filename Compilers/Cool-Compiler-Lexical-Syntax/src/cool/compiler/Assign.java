package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Assign extends Expression {
	Token name;
	Expression value;

	Assign(Token token,
		   Token name,
		   Expression value) {
		super(token);
		this.name = name;
		this.value = value;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
