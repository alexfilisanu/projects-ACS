package cool.compiler;

import cool.parser.CoolParser;

import java.util.List;

public class Let extends Expression {
	private CoolParser.LetContext ctx;
	private List<LetVar> letVars;
	private Expression body;

	public Let(CoolParser.LetContext ctx,
			   List<LetVar> letVars,
			   Expression body) {
		super(ctx.start);
		this.ctx = ctx;
		this.letVars = letVars;
		this.body = body;
	}

	public CoolParser.LetContext getCtx() {
		return ctx;
	}

	public List<LetVar> getLetVars() {
		return letVars;
	}

	public Expression getBody() {
		return body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
