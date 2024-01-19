package cool.compiler;

import cool.parser.CoolParser;
import org.antlr.v4.runtime.Token;

import java.util.List;

public class Dispatch extends Expression {
	private CoolParser.DispatchContext ctx;
	private Expression e;
	private Type type;
	private List<Expression> args;

	public Dispatch(CoolParser.DispatchContext ctx,
			 		Expression e,
			 		Type type,
			 		List<Expression> args) {
		super(ctx.name);
		this.ctx = ctx;
		this.e = e;
		this.type = type;
		this.args = args;
	}

	public CoolParser.DispatchContext getCtx() {
		return ctx;
	}

	public Expression getE() {
		return e;
	}

	public Type getType() {
		return type;
	}

	public List<Expression> getArgs() {
		return args;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
