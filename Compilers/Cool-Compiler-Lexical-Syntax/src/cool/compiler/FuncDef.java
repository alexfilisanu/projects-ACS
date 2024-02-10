package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class FuncDef extends Expression {
	Token name;
	Token type;
	List<Formal> params;
	Expression body;

	FuncDef(Token name,
		    Token type,
		    List<Formal> params,
			Expression body,
			Token start) {
		super(start);
		this.name = name;
		this.type = type;
		this.params = params;
		this.body = body;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
