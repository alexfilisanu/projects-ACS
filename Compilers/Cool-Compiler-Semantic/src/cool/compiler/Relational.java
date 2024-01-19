package cool.compiler;

import cool.parser.CoolParser;

public class Relational extends Expression {
	private CoolParser.RelationalContext ctx;
	private Expression left;
	private Expression right;

	Relational(CoolParser.RelationalContext ctx,
			   Expression left,
			   Expression right) {
		super(ctx.op);
		this.ctx = ctx;
		this.left = left;
		this.right = right;
	}

	public CoolParser.RelationalContext getCtx() {
		return ctx;
	}

	public Expression getLeft() {
		return left;
	}

	public Expression getRight() {
		return right;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
