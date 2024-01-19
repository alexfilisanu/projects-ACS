package cool.compiler;

import cool.parser.CoolParser;

public class New extends Expression {
	private CoolParser.NewContext ctx;
	private Type type;

	public New(CoolParser.NewContext ctx,
			   Type type) {
		super(ctx.NEW().getSymbol());
		this.ctx = ctx;
		this.type = type;
	}

	public CoolParser.NewContext getCtx() {
		return ctx;
	}

	public Type getType() {
		return type;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
