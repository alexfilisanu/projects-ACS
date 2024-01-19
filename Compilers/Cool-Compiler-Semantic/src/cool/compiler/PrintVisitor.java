package cool.compiler;

import java.util.Objects;

public class PrintVisitor implements ASTVisitor<Void> {
	int indent = 0;

	@Override
	public Void visit(Assign assign) {
		var ctx = assign.getCtx();
		printIndent(ctx.ASSIGN().getSymbol().getText());

		indent++;
		printIndent(ctx.name.getText());
		assign.getValue().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(BitwiseNot bitwiseNot) {
		printIndent(bitwiseNot.getToken().getText());

		indent++;
		bitwiseNot.getE().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Block block) {
		printIndent("block");

		indent++;
		block.getExprs().forEach(x -> x.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(Bool bool) {
		printIndent(bool.getToken().getText());

		return null;
	}

	@Override
	public Void visit(Call call) {
		printIndent("implicit dispatch");

		indent++;
		printIndent(call.getToken().getText());
		call.getArgs().forEach(x -> x.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(Case casee) {
		printIndent("case");

		indent++;
		casee.getE().accept(this);
		casee.getCaseStatements().forEach(caseStatement -> caseStatement.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(CaseStatement caseStatement) {
		var ctx = caseStatement.getCtx();
		printIndent("case branch");

		indent++;
		printIndent(ctx.name.getText());
		printIndent(ctx.type.getText());
		caseStatement.getInit().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Classs classs) {
		var ctx = classs.getCtx();
		printIndent("class");

		indent++;
		printIndent(ctx.type.getText());
		if (ctx.inherit != null) {
			printIndent(ctx.inherit.getText());
		}
		classs.getDefinitions().forEach(definition -> definition.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(Dispatch dispatch) {
		printIndent(".");

		indent++;
		dispatch.getE().accept(this);
		if (Objects.nonNull(dispatch.getType().getToken())) {
			printIndent(dispatch.getType().getToken().getText());
		}
		printIndent(dispatch.getToken().getText());
		// sublist(1,) because e(first element of args) is already accepted before
		dispatch.getArgs().subList(1, dispatch.getArgs().size())
				.forEach(x -> x.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(Formal formal) {
		var ctx = formal.getCtx();
		printIndent("formal");

		indent++;
		printIndent(ctx.name.getText());
		printIndent(ctx.type.getText());
		indent--;

		return null;
	}

	@Override
	public Void visit(FuncDef funcDef) {
		var ctx = funcDef.getCtx();
		printIndent("method");

		indent++;
		printIndent(ctx.name.getText());
		funcDef.getParams().forEach(definition -> definition.accept(this));
		printIndent(ctx.type.getText());
		if (Objects.nonNull(funcDef.getBody())) {
			funcDef.getBody().accept(this);
		}
		indent--;

		return null;
	}

	@Override
	public Void visit(Id id) {
		printIndent(id.getToken().getText());

		return null;
	}

	@Override
	public Void visit(If iff) {
		printIndent("if");

		indent++;
		iff.getCond().accept(this);
		iff.getThenBranch().accept(this);
		iff.getElseBranch().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Int intt) {
		printIndent(intt.getToken().getText());

		return null;
	}

	@Override
	public Void visit(IsVoid isVoid) {
		printIndent(isVoid.getToken().getText());

		indent++;
		isVoid.e.accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Let let) {
		printIndent("let");

		indent++;
		let.getLetVars().forEach(varDef -> varDef.accept(this));
		let.getBody().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(LetVar letVar) {
		var ctx = letVar.getCtx();
		printIndent("local");

		indent++;
		printIndent(ctx.name.getText());
		printIndent(ctx.type.getText());
		if (letVar.getInit() != null) {
			letVar.getInit().accept(this);
		}
		indent--;

		return null;
	}

	@Override
	public Void visit(MultDiv multDiv) {
		printIndent(multDiv.getToken().getText());

		indent++;
		multDiv.getLeft().accept(this);
		multDiv.getRight().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(New neww) {
		printIndent(neww.getToken().getText());

		indent++;
		printIndent(neww.getType().getToken().getText());
		indent--;

		return null;
	}

	@Override
	public Void visit(Not not) {
		printIndent(not.getToken().getText());

		indent++;
		not.getE().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Paren paren) {
		paren.getE().accept(this);

		return null;
	}

	@Override
	public Void visit(PlusMinus plusMinus) {
		printIndent(plusMinus.getToken().getText());

		indent++;
		plusMinus.getLeft().accept(this);
		plusMinus.getRight().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Program program) {
		printIndent("program");

		indent++;
		program.classes.forEach(x -> x.accept(this));
		indent--;

		return null;
	}

	@Override
	public Void visit(Relational relational) {
		printIndent(relational.getToken().getText());

		indent++;
		relational.getLeft().accept(this);
		relational.getRight().accept(this);
		indent--;

		return null;
	}

	@Override
	public Void visit(Stringg stringg) {
		// replace for special characters
		String interpretedString = stringg.getToken().getText()
				.replace("\\r", "")
				.replace("\\t", "\t")
				.replace("\\n", "\n")
				.replace("\\b", "\b")
				.replace("\\f", "\f")
				.replace("\\\\", "#")  // Use a temporary placeholder
				.replace("\\", "")
				.replace("#", "\\");   // Restore double backslashes

		int len = interpretedString.length() - 1;
		// remove quotes from string
		printIndent(interpretedString.substring(1, len));

		return null;
	}

	@Override
	public Void visit(Type type) {
		printIndent(type.getToken().getText());

		return null;
	}

	@Override
	public Void visit(VarDef varDef) {
		var ctx = varDef.getCtx();
		printIndent("attribute");

		indent++;
		printIndent(ctx.name.getText());
		printIndent(ctx.type.getText());
		if (varDef.getInit() != null) {
			varDef.getInit().accept(this);
		}
		indent--;

		return null;
	}

	@Override
	public Void visit(While whilee) {
		printIndent("while");

		indent++;
		whilee.getCond().accept(this);
		whilee.getBody().accept(this);
		indent--;

		return null;
	}

	private void printIndent(String str) {
		for (int i = 0; i < indent; i++)
			System.out.print("  ");
		System.out.println(str);
	}
}
