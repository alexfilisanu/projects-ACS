package cool.compiler;

import cool.parser.CoolParser;

public class Formal extends Expression {
	private CoolParser.FormalContext ctx;
	private Id id;
	private Type type;

	public Formal(CoolParser.FormalContext ctx,
				  Id id,
				  Type type) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.type = type;
	}

	public CoolParser.FormalContext getCtx() {
		return ctx;
	}

	public Id getId() {
		return id;
	}

	public Type getType() {
		return type;
	}

	@Override
	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
