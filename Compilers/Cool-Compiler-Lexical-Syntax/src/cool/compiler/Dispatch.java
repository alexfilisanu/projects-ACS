package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Dispatch extends Expression {
	Expression e;
	Token type;
	List<Expression> args;

	Dispatch(Token name,
			 Expression e,
			 Token type,
			 List<Expression> args) {
		super(name);
		this.e = e;
		this.type = type;
		this.args = args;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
