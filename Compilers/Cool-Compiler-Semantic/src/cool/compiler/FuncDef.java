package cool.compiler;

import cool.parser.CoolParser;

import java.util.List;

public class FuncDef extends Expression {
	private CoolParser.FuncDefContext ctx;
	private Id id;
	private Type type;
	private List<Formal> params;
	private Expression body;

	public FuncDef(CoolParser.FuncDefContext ctx,
				   Id id,
				   Type type,
				   List<Formal> params,
				   Expression body) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.type = type;
		this.params = params;
		this.body = body;
	}

	public CoolParser.FuncDefContext getCtx() {
		return ctx;
	}

	public Id getId() {
		return id;
	}

	public Type getType() {
		return type;
	}

	public List<Formal> getParams() {
		return params;
	}

	public Expression getBody() {
		return body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
