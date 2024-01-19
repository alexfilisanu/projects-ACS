package cool.compiler;

import cool.parser.CoolParser;

public class BitwiseNot extends Expression {
	private CoolParser.BitwiseNotContext ctx;
	private Expression e;

	public BitwiseNot(CoolParser.BitwiseNotContext ctx,
					  Expression e) {
		super(ctx.TILDE().getSymbol());
		this.ctx = ctx;
		this.e = e;
	}

	public CoolParser.BitwiseNotContext getCtx() {
		return ctx;
	}

	public Expression getE() {
		return e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
