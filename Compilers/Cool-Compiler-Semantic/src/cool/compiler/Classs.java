package cool.compiler;

import cool.parser.CoolParser;

import java.util.List;

public class Classs extends Expression {
	private CoolParser.ClassContext ctx;
	private Id id;
	private List<Expression> definitions;

	public Classs(CoolParser.ClassContext ctx,
				  Id id,
				  List<Expression> definitions) {
		super(ctx.start);
		this.ctx = ctx;
		this.id = id;
		this.definitions = definitions;
	}

	public CoolParser.ClassContext getCtx() {
		return ctx;
	}

	public Id getId() {
		return id;
	}

	public List<Expression> getDefinitions() {
		return definitions;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
