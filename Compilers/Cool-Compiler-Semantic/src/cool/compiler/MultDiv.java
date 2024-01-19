package cool.compiler;

import cool.parser.CoolParser;

public class MultDiv extends Expression {
	private CoolParser.MultDivContext ctx;
	private Expression left;
	private Expression right;

	public MultDiv(CoolParser.MultDivContext ctx,
				   Expression left,
				   Expression right) {
		super(ctx.op);
		this.ctx = ctx;
		this.left = left;
		this.right = right;
	}

	public CoolParser.MultDivContext getCtx() {
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
