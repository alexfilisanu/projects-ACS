package cool.compiler;

import org.antlr.v4.runtime.Token;

public class While extends Expression {
	Expression cond;
	Expression body;

	While(Expression cond,
		  Expression body,
		  Token start) {
		super(start);
		this.cond = cond;
		this.body = body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
