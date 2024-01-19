package cool.compiler;

import cool.parser.CoolParser;

public class PlusMinus extends Expression {
	private CoolParser.PlusMinusContext ctx;
	private Expression left;
	private Expression right;

	public PlusMinus(CoolParser.PlusMinusContext ctx,
					 Expression left,
			  		 Expression right) {
		super(ctx.op);
		this.ctx = ctx;
		this.left = left;
		this.right = right;
	}

	public CoolParser.PlusMinusContext getCtx() {
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
