package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Block extends Expression {
	List<Expression> exprs;

	Block(List<Expression> exprs,
		  Token start) {
		super(start);
		this.exprs = exprs;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
