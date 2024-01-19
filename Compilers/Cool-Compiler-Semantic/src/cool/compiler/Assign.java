package cool.compiler;

import cool.parser.CoolParser;

public class Assign extends Expression {
	private CoolParser.AssignContext ctx;
	private Id id;
	private Expression value;

	public Assign(CoolParser.AssignContext ctx,
		   Id id,
		   Expression value) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.value = value;
	}

	public CoolParser.AssignContext getCtx() {
		return ctx;
	}

	public Id getId() {
		return id;
	}

	public Expression getValue() {
		return value;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
