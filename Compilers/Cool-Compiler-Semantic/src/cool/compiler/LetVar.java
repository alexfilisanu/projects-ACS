package cool.compiler;

import cool.parser.CoolParser;

public class LetVar extends Expression {
	private CoolParser.LetVarContext ctx;
	private Id id;
	private Type type;
	private Expression init;

	public LetVar(CoolParser.LetVarContext ctx,
		   Id id,
		   Type type) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.type = type;
	}

	public LetVar(CoolParser.LetVarContext ctx,
		   Id id,
		   Type type,
		   Expression init) {
		this(ctx, id, type);
		this.init = init;
	}

	public CoolParser.LetVarContext getCtx() {
		return ctx;
	}

	public Id getId() {
		return id;
	}

	public Type getType() {
		return type;
	}

	public Expression getInit() {
		return init;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
