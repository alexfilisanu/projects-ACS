package cool.compiler;

import cool.parser.CoolParser;

public class If extends Expression {
	private CoolParser.IfContext ctx;
	private Expression cond;
	private Expression thenBranch;
	private Expression elseBranch;

	public If(CoolParser.IfContext ctx,
			  Expression cond,
			  Expression thenBranch,
			  Expression elseBranch) {
		super(ctx.start);
		this.ctx = ctx;
		this.cond = cond;
		this.thenBranch = thenBranch;
		this.elseBranch = elseBranch;
	}

	public CoolParser.IfContext getCtx() {
		return ctx;
	}

	public Expression getCond() {
		return cond;
	}

	public Expression getThenBranch() {
		return thenBranch;
	}

	public Expression getElseBranch() {
		return elseBranch;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
