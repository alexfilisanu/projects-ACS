package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Let extends Expression {
	List<LetVar> letVars;
	Expression body;

	Let(List<LetVar> letVars,
	    Expression body,
	    Token start) {
		super(start);
		this.letVars = letVars;
		this.body = body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
