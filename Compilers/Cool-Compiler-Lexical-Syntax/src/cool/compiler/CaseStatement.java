package cool.compiler;

import org.antlr.v4.runtime.Token;

public class CaseStatement extends Expression {
	Token name;
	Token type;
	Expression init;

	CaseStatement(Token name,
		   Token type,
		   Expression init) {
		super(name);
		this.name = name;
		this.type = type;
		this.init = init;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
