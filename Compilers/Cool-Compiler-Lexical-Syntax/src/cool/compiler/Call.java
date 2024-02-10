package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Call extends Expression {
	List<Expression> args;

	Call(Token name,
		 List<Expression> args) {
		super(name);
		this.args = args;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
