package cool.compiler;

import cool.parser.CoolParser;

public class Not extends Expression {
	private CoolParser.NotContext ctx;
	private Expression e;

	public Not(CoolParser.NotContext ctx,
			   Expression e) {
		super(ctx.NOT().getSymbol());
		this.ctx = ctx;
		this.e = e;
	}

	public CoolParser.NotContext getCtx() {
		return ctx;
	}

	public Expression getE() {
		return e;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
