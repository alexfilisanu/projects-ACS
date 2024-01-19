package cool.compiler;

import org.antlr.v4.runtime.Token;

import java.util.List;

public class Program extends ASTNode {
	List<Classs> classes;

	public Program(List<Classs> classes,
				   Token start) {
		super(start);
		this.classes = classes;
	}

	public List<Classs> getClasses() {
		return classes;
	}

	public <T> T accept(ASTVisitor<T> visitor) {
		return visitor.visit(this);
	}
}
