package cool.compiler;

import cool.parser.CoolParser;

public class CaseStatement extends Expression {
	private CoolParser.CaseStatementContext ctx;
	private Id id;
	private Type type;
	private Expression init;

	public CaseStatement(CoolParser.CaseStatementContext ctx,
				  		 Id id,
		   				 Type type,
						 Expression init) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.type = type;
		this.init = init;
	}

	public CoolParser.CaseStatementContext getCtx() {
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
