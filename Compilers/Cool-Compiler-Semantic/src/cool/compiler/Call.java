package cool.compiler;

import cool.parser.CoolParser;
import org.antlr.v4.runtime.Token;

import java.util.List;

public class Call extends Expression {
	private CoolParser.CallContext ctx;
	private List<Expression> args;

	public Call(CoolParser.CallContext ctx,
		 		List<Expression> args) {
		super(ctx.name);
		this.ctx = ctx;
		this.args = args;
	}

	public CoolParser.CallContext getCtx() {
		return ctx;
	}

	public List<Expression> getArgs() {
		return args;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
