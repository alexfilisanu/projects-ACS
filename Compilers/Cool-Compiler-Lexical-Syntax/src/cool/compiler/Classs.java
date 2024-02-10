package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Classs extends Expression {
	Token inherit;
	List<Expression> definitions;

	Classs(Token type,
		 Token inherit,
		 List<Expression> definitions) {
		super(type);
		this.inherit = inherit;
		this.definitions = definitions;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
