package cool.compiler;

import org.antlr.v4.runtime.Token;

public class If extends Expression {
	Expression cond;
	Expression thenBranch;
	Expression elseBranch;

	If(Expression cond,
	   Expression thenBranch,
	   Expression elseBranch,
	   Token start) {
		super(start);
		this.cond = cond;
		this.thenBranch = thenBranch;
		this.elseBranch = elseBranch;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
