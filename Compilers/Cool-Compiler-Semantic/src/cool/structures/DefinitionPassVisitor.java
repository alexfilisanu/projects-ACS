package cool.structures;

import cool.compiler.*;

public class DefinitionPassVisitor implements ASTVisitor<Void> {
	Scope currentScope = null;

	@Override
	public Void visit(Program program) {
		currentScope = new DefaultScope(null);
		currentScope.add(TypeSymbol.OBJECT);
		currentScope.add(TypeSymbol.IO);
		currentScope.add(TypeSymbol.INT);
		currentScope.add(TypeSymbol.STRING);
		currentScope.add(TypeSymbol.BOOL);
		currentScope.add(TypeSymbol.SELF_TYPE);

		for (var stmt: program.getClasses()) {
			stmt.accept(this);
		}

		return null;
	}

	@Override
	public Void visit(Relational relational) {
		relational.getLeft().accept(this);
		relational.getRight().accept(this);
		return null;
	}

	@Override
	public Void visit(Int intt) {
		return null;
	}

	@Override
	public Void visit(IsVoid isVoid) {
		return null;
	}

	@Override
	public Void visit(Let let) {
		Scope oldScope = currentScope;
		currentScope = new DefaultScope(currentScope);
		for(var letVar : let.getLetVars()) {
			letVar.accept(this);
		}
		let.getBody().accept(this);
		currentScope = oldScope;

		return null;
	}

	@Override
	public Void visit(LetVar letVar) {
		var ctx = letVar.getCtx();
		var name = ctx.name;
		var id = letVar.getId();

		var letVarSymbol = new IdSymbol(name.getText());
		currentScope = new DefaultScope(currentScope);

		if ("self".equals(name.getText())) {
			SymbolTable.error(ctx, name,
					"Let variable has illegal name self");
		}

		id.setSymbol(letVarSymbol);
		id.setScope(currentScope);

		if (letVar.getInit() != null) {
			letVar.getInit().accept(this);
		}
		currentScope.add(letVarSymbol);

		return null;
	}

	@Override
	public Void visit(PlusMinus plusMinus) {
		plusMinus.getLeft().accept(this);
		plusMinus.getRight().accept(this);
		return null;
	}

	@Override
	public Void visit(Paren paren) {
		paren.getE().accept(this);
		return null;
	}

	@Override
	public Void visit(Not not) {
		not.getE().accept(this);
		return null;
	}

	@Override
	public Void visit(MultDiv multDiv) {
		multDiv.getLeft().accept(this);
		multDiv.getRight().accept(this);
		return null;
	}

	@Override
	public Void visit(New neww) {
		return null;
	}

	@Override
	public Void visit(Assign assign) {
		if (assign.getId().getToken().getText().equals("self")) {
			SymbolTable.error(assign.getCtx(), assign.getId().getToken(),
					"Cannot assign to self");
		}

		assign.getId().accept(this);
		assign.getValue().accept(this);
		return null;
	}

	@Override
	public Void visit(BitwiseNot bitwiseNot) {
		bitwiseNot.getE().accept(this);
		return null;
	}

	@Override
	public Void visit(Block block) {
		for (var expr : block.getExprs()) {
			expr.accept(this);
		}
		return null;
	}

	@Override
	public Void visit(Bool bool) {
		return null;
	}

	@Override
	public Void visit(CaseStatement caseStatement) {
		var ctx = caseStatement.getCtx();
		var name = ctx.name;
		var id = caseStatement.getId();
		var type = caseStatement.getType();

		if ("self".equals(name.getText())) {
			SymbolTable.error(ctx, name,
					"Case variable has illegal name self");
		}

		if ("SELF_TYPE".equals(type.getToken().getText())) {
			SymbolTable.error(ctx, type.getToken(),
					"Case variable " + name.getText() + " has illegal type SELF_TYPE");
		}

		if (currentScope.lookup(type.getToken().getText()) == null) {
			SymbolTable.error(ctx, type.getToken(),
					"Case variable " + name.getText() + " has undefined type " + type.getToken().getText());
		}

		var caseSymbol = new IdSymbol(id.getToken().getText());
		caseSymbol.setType((TypeSymbol) currentScope.lookup(type.getToken().getText()));
		id.setSymbol(caseSymbol);
		id.setScope(currentScope);

		return null;
	}

	@Override
	public Void visit(Call call) {
		for (var arg : call.getArgs()) {
			arg.accept(this);
		}

		return null;
	}

	@Override
	public Void visit(Case casee) {
		for (var caseStatement : casee.getCaseStatements()) {
			caseStatement.accept(this);
		}
		casee.getE().accept(this);

		return null;
	}

	@Override
	public Void visit(Classs classs) {
		var ctx = classs.getCtx();
		var child = ctx.type;
		var parent = ctx.inherit;

		if ("SELF_TYPE".equals(child.getText())) {
			SymbolTable.error(ctx, child,
					"Class has illegal name SELF_TYPE");
			return null;
		}

		var childClassSymbol = new ClassSymbol(child.getText(), currentScope);
		currentScope = childClassSymbol;

		if (!SymbolTable.globals.add(childClassSymbol)) {
			SymbolTable.error(ctx, child,
					"Class " + child.getText() + " is redefined");
			return null;
		}

		classs.getId().setScope(currentScope);
		childClassSymbol.setType(new TypeSymbol(ctx.type.getText()));
		classs.getId().setSymbol(childClassSymbol);

		if (parent != null) {
			SymbolTable.childToParentMap.put(child.getText(), parent.getText());
		} else {
			SymbolTable.childToParentMap.put(child.getText(), null);
		}
		currentScope.getParent().add(new TypeSymbol(childClassSymbol.name));

		var definitions = classs.getDefinitions();
		for (var def : definitions) {
			def.accept(this);
		}

		currentScope = currentScope.getParent();
		return null;
	}

	@Override
	public Void visit(Formal formal) {
		var ctx = formal.getCtx();
		var name = ctx.name;
		var id = formal.getId();
		var typeToken = formal.getType().getToken();
		var className = currentScope.getParent().toString();

		var symbol = new IdSymbol(name.getText());

		if ("self".equals(name.getText())) {
			SymbolTable.error(ctx, name,
					"Method self of class " + className + " has formal parameter with illegal name self");
		}

		if ("SELF_TYPE".equals(typeToken.getText())) {
			SymbolTable.error(ctx, typeToken,
					"Method " + currentScope.toString() + " of class " + className + " has formal parameter "
							+ name.getText() + " with illegal type SELF_TYPE");
		}

		if (!currentScope.add(symbol)) {
			SymbolTable.error(ctx, name,
					"Method " + currentScope.toString() + " of class " + className
							+ " redefines formal parameter " + name.getText());
		}

		symbol.setType((TypeSymbol) currentScope.lookup(typeToken.getText()));
		id.setSymbol(symbol);
		id.setScope(currentScope);

		symbol.setType((TypeSymbol) currentScope.lookup(typeToken.getText()));

		return null;
	}

	@Override
	public Void visit(If iff) {
		iff.getCond().accept(this);
		iff.getThenBranch().accept(this);
		iff.getElseBranch().accept(this);
		return null;
	}

	@Override
	public Void visit(Id id) {
		var idToken = id.getToken();

		var symbol = (IdSymbol)currentScope.lookup(idToken.getText());
		if (currentScope.lookup(idToken.getText()) != null) {
			symbol.setType(((IdSymbol) currentScope.lookup(idToken.getText())).getType());
		}

		id.setScope(currentScope);
		id.setSymbol(symbol);

		return null;
	}

	@Override
	public Void visit(Dispatch dispatch) {
		dispatch.getE().accept(this);
		for (var arg : dispatch.getArgs()) {
			arg.accept(this);
		}

		return null;
	}

	@Override
	public Void visit(FuncDef funcDef) {
		var ctx = funcDef.getCtx();
		var name = ctx.name;
		var id = funcDef.getId();
		var type = funcDef.getType();

		var functionSymbol = new FunctionSymbol(name.getText(), currentScope);
		currentScope = functionSymbol;

		if (!currentScope.getParent().add(functionSymbol)) {
			SymbolTable.error(ctx, name,
					"Class " + currentScope.getParent().toString() + " redefines method " + name.getText());
			return null;
		}

		functionSymbol.setType((TypeSymbol) currentScope.lookup(type.getToken().getText()));
		id.setSymbol(functionSymbol);
		id.setScope(currentScope);

		id.getSymbol().setType((TypeSymbol) currentScope.lookup(type.getToken().getText()));

		for (var param: funcDef.getParams()) {
			param.accept(this);
		}

		funcDef.getBody().accept(this);

		currentScope = currentScope.getParent();
		return null;
	}

	@Override
	public Void visit(Stringg stringg) {
		return null;
	}

	@Override
	public Void visit(VarDef varDef) {
		var ctx = varDef.getCtx();
		var name = ctx.name;
		var id = varDef.getId();
		var type = varDef.getType();

		var varDefSymbol = new IdSymbol(name.getText());

		if ("self".equals(name.getText())) {
			SymbolTable.error(ctx, name,
					"Class " + currentScope.toString() + " has attribute with illegal name " + name.getText());
			return null;
		}

		if (!currentScope.add(varDefSymbol)) {
			SymbolTable.error(ctx, name,
					"Class " + currentScope.toString() + " redefines attribute " + name.getText());
			return null;
		}

		id.setSymbol(varDefSymbol);
		id.setScope(currentScope);

		varDefSymbol.setType((TypeSymbol) currentScope.lookup(type.getToken().getText()));

		if (varDef.getInit() != null) {
			varDef.getInit().accept(this);
		}

		return null;
	}

	@Override
	public Void visit(Type type) {
		return null;
	}

	@Override
	public Void visit(While whilee) {
		whilee.getCond().accept(this);
		whilee.getBody().accept(this);
		return null;
	}
}
