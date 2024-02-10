package cool.compiler;

import org.antlr.v4.runtime.Token;

public class Relational extends Expression {
	Expression left;
	Expression right;

	Relational(Expression left,
			   Token op,
			   Expression right) {
		super(op);
		this.left = left;
		this.right = right;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
