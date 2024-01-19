package cool.compiler;

import cool.parser.CoolParser;

import java.util.List;

public class Case extends Expression {
	private CoolParser.CaseContext ctx;
	private Expression e;
	private List<CaseStatement> caseStatements;

	public Case(CoolParser.CaseContext ctx,
				Expression e,
		 		List<CaseStatement> caseStatements) {
		super(ctx.start);
		this.ctx = ctx;
		this.e = e;
		this.caseStatements = caseStatements;
	}

	public CoolParser.CaseContext getCtx() {
		return ctx;
	}

	public Expression getE() {
		return e;
	}

	public List<CaseStatement> getCaseStatements() {
		return caseStatements;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
