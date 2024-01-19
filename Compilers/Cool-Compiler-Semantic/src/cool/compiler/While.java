package cool.compiler;

import cool.parser.CoolParser;

public class While extends Expression {
	private CoolParser.WhileContext ctx;
	private Expression cond;
	private Expression body;

	public While(CoolParser.WhileContext ctx,
				 Expression cond,
		  		 Expression body) {
		super(ctx.start);
		this.ctx = ctx;
		this.cond = cond;
		this.body = body;
	}

	public CoolParser.WhileContext getCtx() {
		return ctx;
	}

	public Expression getCond() {
		return cond;
	}

	public Expression getBody() {
		return body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
