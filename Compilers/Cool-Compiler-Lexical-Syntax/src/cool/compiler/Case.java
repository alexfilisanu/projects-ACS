package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Case extends Expression {
	Expression e;
	List<CaseStatement> caseStatements;

	Case(Expression e,
		 List<CaseStatement> caseStatements,
		 Token start) {
		super(start);
		this.e = e;
		this.caseStatements = caseStatements;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
