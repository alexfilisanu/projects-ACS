package cool.compiler;

import org.antlr.v4.runtime.Token;

abstract class Expression extends ASTNode {
	Expression(Token token) {
		super(token);
	}
}
