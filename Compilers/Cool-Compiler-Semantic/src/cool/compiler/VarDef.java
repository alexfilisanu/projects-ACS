package cool.compiler;

import cool.parser.CoolParser;

public class VarDef extends Expression {
	private CoolParser.VarDefContext ctx;
	private Id id;
	private Type type;
	private Expression init;

	public VarDef(CoolParser.VarDefContext ctx,
				  Type type,
				  Id id) {
		super(ctx.start);
		this.ctx = ctx;
		this.type = type;
		this.id = id;
	}

	public VarDef(CoolParser.VarDefContext ctx,
				  Type type,
				  Id id,
				  Expression init) {
		this(ctx, type, id);
		this.init = init;
	}

	public CoolParser.VarDefContext getCtx() {
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
