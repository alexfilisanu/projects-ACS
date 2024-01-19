package cool.structures;

import cool.compiler.*;

import java.util.*;

public class ResolutionPassVisitor implements ASTVisitor<TypeSymbol> {

	@Override
	public TypeSymbol visit(Program program) {
		for (var stmt: program.getClasses()) {
			stmt.accept(this);
		}

		return null;
	}

	@Override
	public TypeSymbol visit(Relational relational) {
		var ctx = relational.getCtx();
		var token = relational.getToken();
		var exprTypeLeft = relational.getLeft().accept(this);
		var exprTypeRight = relational.getRight().accept(this);

		if (exprTypeLeft == null || exprTypeRight == null) {
			return null;
		}

		if (exprTypeLeft != exprTypeRight) {
			if ("=".equals(token.getText())) {
				if (SymbolTable.isPrimitiveType(exprTypeLeft) && SymbolTable.isPrimitiveType(exprTypeRight)) {
					SymbolTable.error(ctx, token,
							"Cannot compare " + exprTypeLeft.getName() + " with " + exprTypeRight.getName());
				}
			} else {
				SymbolTable.error(ctx, relational.getRight().getToken(),
						"Operand of " + token.getText() + " has type " + exprTypeRight.getName()
								+ " instead of " + exprTypeLeft.getName());
			}
		}

		return TypeSymbol.BOOL;
	}

	@Override
	public TypeSymbol visit(Int intt) {
		return TypeSymbol.INT;
	}

	@Override
	public TypeSymbol visit(IsVoid isVoid) {
		return null;
	}

	@Override
	public TypeSymbol visit(Let let) {
		for (var letVar : let.getLetVars()) {
			letVar.accept(this);
		}

		return let.getBody().accept(this);
	}

	@Override
	public TypeSymbol visit(LetVar letVar) {
		var ctx = letVar.getCtx();
		var id = letVar.getId();
		var typeToken = letVar.getType().getToken();

		if (id.getScope() != null) {
			if (SymbolTable.isUndefinedToken(typeToken)
					&& !"SELF_TYPE".equals(typeToken.getText())) {
				SymbolTable.error(ctx, typeToken,
						"Let variable " + ctx.name.getText() + " has undefined type " + typeToken.getText());
				return null;
			}

			id.getSymbol().setType((TypeSymbol) id.getScope().lookup(typeToken.getText()));
		}

		TypeSymbol idType = SymbolTable.getTypeSymbol(letVar.getType());

		if (letVar.getInit() == null) {
			return idType;
		}

		var initType = letVar.getInit().accept(this);

		if (initType == null || idType == null) {
			return null;
		}

        if (SymbolTable.areIncompatibleTypes(idType, initType)) {
			SymbolTable.error(letVar.getCtx(), letVar.getInit().getToken(),
					"Type " + initType.getName() + " of initialization expression of identifier "
							+ letVar.getId().getToken().getText() + " is incompatible with declared type " + idType.getName());
		}

		return idType;
	}

	@Override
	public TypeSymbol visit(PlusMinus plusMinus) {
		var ctx = plusMinus.getCtx();
		var left = plusMinus.getLeft();
		var right = plusMinus.getRight();
		var exprTypeLeft = left.accept(this);
		var exprTypeRight = right.accept(this);

		if (exprTypeLeft == null || exprTypeRight == null) {
			return null;
		}

		if (exprTypeLeft != TypeSymbol.INT) {
			SymbolTable.error(ctx, left.getToken(),
					"Operand of " + plusMinus.getToken().getText() + " has type "
							+ exprTypeLeft.getName() + " instead of Int");
		}

		if (exprTypeRight != TypeSymbol.INT) {
			SymbolTable.error(ctx, right.getToken(),
					"Operand of " + plusMinus.getToken().getText() + " has type "
							+ exprTypeRight.getName() + " instead of Int");
		}

		return TypeSymbol.INT;
	}

	@Override
	public TypeSymbol visit(Paren paren) {
        return paren.getE().accept(this);
	}

	@Override
	public TypeSymbol visit(Not not) {
		var exprType = not.getE().accept(this);

		if (exprType == null) {
			return null;
		}

		if (exprType != TypeSymbol.BOOL) {
			SymbolTable.error(not.getCtx(), not.getE().getToken(),
					"Operand of " + not.getToken().getText() + " has type "
							+ exprType.getName() + " instead of Bool");
		}

		return TypeSymbol.BOOL;
	}

	@Override
	public TypeSymbol visit(MultDiv multDiv) {
		var ctx = multDiv.getCtx();
		var left = multDiv.getLeft();
		var right = multDiv.getRight();
		var exprTypeLeft = left.accept(this);
		var exprTypeRight = right.accept(this);

		if (exprTypeLeft == null || exprTypeRight == null) {
			return null;
		}

		if (exprTypeLeft != TypeSymbol.INT) {
			SymbolTable.error(ctx, left.getToken(),
					"Operand of " + multDiv.getToken().getText() + " has type "
							+ exprTypeLeft.getName() + " instead of Int");
		}

		if (exprTypeRight != TypeSymbol.INT) {
			SymbolTable.error(ctx, right.getToken(),
					"Operand of " + multDiv.getToken().getText() + " has type "
							+ exprTypeRight.getName() + " instead of Int");
		}

		return TypeSymbol.INT;
	}

	@Override
	public TypeSymbol visit(New neww) {
		var ctx = neww.getCtx();
		var typeToken = neww.getType().getToken();

		if ("SELF_TYPE".equals(typeToken.getText())) {
			return TypeSymbol.SELF_TYPE;
		}

		if (SymbolTable.isUndefinedToken(typeToken)) {
			SymbolTable.error(ctx, typeToken,
					"new is used with undefined type " + typeToken.getText());
			return null;
		}

		return ((IdSymbol) SymbolTable.globals.lookup(typeToken.getText())).getType();
	}

	@Override
	public TypeSymbol visit(Assign assign) {
		var ctx = assign.getCtx();
		var idType = assign.getId().accept(this);
		var exprType = assign.getValue().accept(this);

		if (exprType == null) {
			return null;
		}

		if (SymbolTable.areIncompatibleTypes(idType, exprType)) {
			if (assign.getValue() instanceof Dispatch) {
				SymbolTable.error(ctx, ((Dispatch) assign.getValue()).getCtx().start,
						"Type " + exprType.getName() + " of assigned expression is incompatible with declared type "
								+ idType.getName() + " of identifier " + assign.getId().getToken().getText());
			} else {
				SymbolTable.error(ctx, assign.getValue().getToken(),
						"Type " + exprType.getName() + " of assigned expression is incompatible with declared type "
								+ idType.getName() + " of identifier " + assign.getId().getToken().getText());
			}
		}

		return idType;
	}

	@Override
	public TypeSymbol visit(BitwiseNot bitwiseNot) {
		var ctx = bitwiseNot.getCtx();
		var exprType = bitwiseNot.getE().accept(this);

		if (exprType == null) {
			return null;
		}

		if (exprType != TypeSymbol.INT) {
			SymbolTable.error(ctx, bitwiseNot.getE().getToken(),
					"Operand of " + bitwiseNot.getToken().getText() + " has type "
							+ exprType.getName() + " instead of Int");
		}

		return TypeSymbol.INT;
	}

	@Override
	public TypeSymbol visit(Block block) {
		TypeSymbol type = null;
		for (var expr : block.getExprs()) {
			type = expr.accept(this);
			if (expr instanceof Assign) {
				type = null;
			}
		}

		return type;
	}

	@Override
	public TypeSymbol visit(Bool bool) {
		return TypeSymbol.BOOL;
	}

	@Override
	public TypeSymbol visit(CaseStatement caseStatement) {
		var scope = caseStatement.getId().getScope();
		var init = caseStatement.getInit();

		if (scope != null && init.getToken() != null) {
			if (init instanceof Int) {
				return TypeSymbol.INT;
			}

			if (init instanceof Bool) {
				return TypeSymbol.BOOL;
			}

			if (init instanceof Stringg) {
				return TypeSymbol.STRING;
			}

			return ((IdSymbol) scope.lookup(init.getToken().getText())).getType();
		}

		return null;
	}

	@Override
	public TypeSymbol visit(Call call) {
		var ctx = call.getCtx();
		var args = call.getArgs();

		if ("abort".equals(call.getToken().getText())) {
			return TypeSymbol.OBJECT;
		} else {
			List<TypeSymbol> symbolList = args.stream()
					.map(x -> x.accept(this))
					.toList();

			if (!args.isEmpty()) {
				var className = ((ClassSymbol) ((Id) args.get(0)).getScope().getParent()).getType().getName();
				Object[] result = SymbolTable.isCallingMethodWithDifferentTypeOfFormals(symbolList, className, call.getToken().getText());
				if ((boolean) result[0]) {
					var pos = (int) result[3];
					SymbolTable.error(ctx, args.get(pos).getToken(),
							"In call to method " + call.getToken().getText() + " of class " + className
									+ ", actual type " + symbolList.get(pos).getName() + " of formal parameter " + result[1]
									+ " is incompatible with declared type " + result[2]);
				}

				return ((FunctionSymbol) ((Id) args.get(0)).getScope()).getType();
			}

			return null;
		}
	}

	@Override
	public TypeSymbol visit(Case casee) {
		var eType = casee.getE().accept(this);

		Set<TypeSymbol> caseTypesPrimitives = new HashSet<>();
		ArrayList<TypeSymbol> caseTypesList = new ArrayList<>();
		for (var caseStatement : casee.getCaseStatements()) {
			var caseType = caseStatement.accept(this);
			if (SymbolTable.isPrimitiveType(caseType)) {
				caseTypesPrimitives.add(caseType);
				if (caseTypesPrimitives.size() > 1) {
					return TypeSymbol.OBJECT;
				}
			} else {
				caseTypesList.add(caseType);
			}
		}

		if (caseTypesPrimitives.size() == 1) {
			return eType;
		} else {
			return SymbolTable.getMostInheritedClass(caseTypesList);
		}
	}

	@Override
	public TypeSymbol visit(Classs classs) {
		var ctx = classs.getCtx();
		var child = ctx.type;
		var parent = ctx.inherit;

		if (parent != null) {
			if (SymbolTable.isIllegalParent(parent)) {
				SymbolTable.error(ctx, parent,
						"Class " + child.getText() + " has illegal parent " + parent.getText());
				return null;
			}

			if (SymbolTable.isUndefinedToken(parent)) {
				SymbolTable.error(ctx, parent,
						"Class " + child.getText() + " has undefined parent " + parent.getText());
				return null;
			}

			if (SymbolTable.isInheritanceCycle(child)) {
				SymbolTable.error(ctx, child,
						"Inheritance cycle for class " + child.getText());
				return null;
			}
		}

		var definitions = classs.getDefinitions();
		for (var def : definitions) {
			def.accept(this);
		}

		return null;
	}

	@Override
	public TypeSymbol visit(Formal formal) {
		var ctx = formal.getCtx();
		var name = ctx.name;
		var id = formal.getId();
		var typeToken = formal.getType().getToken();
		var scope = id.getScope();

		if (scope != null) {
			if (scope.lookup(typeToken.getText()) == null) {
				SymbolTable.error(ctx, typeToken,
						"Method " + scope + " of class " + scope.getParent().toString()
							+ " has formal parameter " + name.getText() + " with undefined type " + typeToken.getText());
				return null;
			}

			id.getSymbol().setType((TypeSymbol) id.getScope().lookup(typeToken.getText()));
		}

		return id.getSymbol().getType();
	}

	@Override
	public TypeSymbol visit(If iff) {
		var ctx = iff.getCtx();
		var condType = iff.getCond().accept(this);
		var thenType = iff.getThenBranch().accept(this);
		var elseType = iff.getElseBranch().accept(this);

		if (condType != TypeSymbol.BOOL) {
			SymbolTable.error(ctx, iff.getCond().getToken(),
					"If condition has type " + condType.getName() + " instead of Bool");
			return TypeSymbol.OBJECT;
		}

		return thenType;
	}

	@Override
	public TypeSymbol visit(Id id) {
		var ctx = id.getCtx();
		var idToken = id.getToken();

		if (id.getScope() == null || "self".equals(idToken.getText())) {
			return null;
		}

		if ("true".equals(idToken.getText()) || "false".equals(idToken.getText())) {
			return TypeSymbol.BOOL;
		}

		var	symbol = id.getScope().lookup(idToken.getText());
		if (symbol == null) {
			if (ctx != null) {
				SymbolTable.error(ctx, idToken,
						"Undefined identifier " + idToken.getText());
			}

			return null;
		}

		return ((IdSymbol) symbol).getType();
	}

	@Override
	public TypeSymbol visit(Dispatch dispatch) {
		var ctx = dispatch.getCtx();
		var dispatchType = dispatch.getType().getToken();

		if (dispatchType != null) {
			if ("SELF_TYPE".equals(dispatchType.getText())) {
				SymbolTable.error(ctx, dispatchType,
						"Type of static dispatch cannot be SELF_TYPE");
				return null;
			}

			if (SymbolTable.isUndefinedToken(dispatchType)) {
				SymbolTable.error(ctx, dispatchType,
						"Type " + dispatchType.getText() + " of static dispatch is undefined");
				return null;
			}

			TypeSymbol idType = SymbolTable.getTypeSymbol(dispatch.getType());
			var eType = dispatch.getE().accept(this);
			if (!SymbolTable.isInheritedClass(eType, idType)) {
				SymbolTable.error(ctx, dispatchType,
					"Type " + idType + " of static dispatch is not a superclass of type "
							+ eType.getName());
			}

			if (SymbolTable.isUndefinedMethod(idType, dispatch.getToken().getText())) {
				SymbolTable.error(ctx, dispatch.getToken(),
						"Undefined method " + dispatch.getToken().getText() + " in class " + idType.getName());
				return null;
			}
		}

		if ("copy".equals(dispatch.getToken().getText())) {
			return TypeSymbol.SELF_TYPE;
		}

		if ("out_string".equals(dispatch.getToken().getText())
				|| "in_string".equals(dispatch.getToken().getText())) {
			return TypeSymbol.STRING;
		}

		if (dispatch.getToken().getText().equals("out_int")
				|| dispatch.getToken().getText().equals("in_int")) {
			return TypeSymbol.INT;
		}

		if (dispatch.getToken().getText().equals("length")) {
			return TypeSymbol.INT;
		}

		if (dispatch.getToken().getText().equals("concat")) {
			for (var arg : dispatch.getArgs()) {
				var typeArg = ((IdSymbol)(((Id) dispatch.getE()).getScope().lookup(arg.getToken().getText()))).getType();
				if (typeArg != TypeSymbol.STRING) {
					SymbolTable.error(ctx, arg.getToken(),
							"In call to method concat of class String, actual type " + typeArg.getName()
									+ " of formal parameter s is incompatible with declared type String");
				}
			}

			return TypeSymbol.STRING;
		}

		if (dispatch.getToken().getText().equals("substr")) {
			var typeArg = ((IdSymbol)(((Id) dispatch.getE()).getScope().lookup(dispatch.getArgs().get(0).getToken().getText()))).getType();
			if (typeArg != TypeSymbol.STRING) {
				SymbolTable.error(ctx, dispatch.getArgs().get(0).getToken(),
						"In call to method substr of class String, actual type " + typeArg.getName()
							+ " of formal parameter s is incompatible with declared type String");
			}

			for (var arg : dispatch.getArgs().subList(1, dispatch.getArgs().size())) {
				typeArg = ((IdSymbol)(((Id) dispatch.getE()).getScope().lookup(arg.getToken().getText()))).getType();
				if (typeArg != TypeSymbol.INT) {
					SymbolTable.error(ctx, arg.getToken(),
							"In call to method substr of class String, actual type " + typeArg.getName()
								+ " of formal parameter l is incompatible with declared type Int");
				}
			}

			return TypeSymbol.STRING;
		}

		var eType = dispatch.getE().accept(this);
		if (eType == null && dispatch.getE().getToken() != null
				&& "self".equals(dispatch.getE().getToken().getText())) {
			if (((Id) dispatch.getE()).getScope().getParent() instanceof ClassSymbol) {
				eType = ((ClassSymbol) ((Id) dispatch.getE()).getScope().getParent()).getType();
			}
		}

		if (eType != null) {
			var eTypeSymbol = SymbolTable.globals.lookup(eType.getName());

			if (eTypeSymbol == null) {
				SymbolTable.error(ctx, dispatch.getE().getToken(),
						"Dispatch on undefined class " + eType.getName());
				return null;
			}

			if (SymbolTable.isUndefinedMethod(eType, dispatch.getToken().getText())) {
				SymbolTable.error(ctx, dispatch.getToken(),
						"Undefined method " + dispatch.getToken().getText() + " in class " + eType.getName());
				return null;
			}

			if (!((FunctionSymbol) eTypeSymbol).symbols.isEmpty()
					&& ((ClassSymbol) eTypeSymbol).symbols.get(dispatch.getToken().getText()) != null
					&& dispatch.getArgs().size() - 1 != ((FunctionSymbol)((ClassSymbol) eTypeSymbol).symbols.get(dispatch.getToken().getText())).symbols.size()) {
				SymbolTable.error(ctx, dispatch.getToken(),
						"Method " + dispatch.getToken().getText() + " of class " + eTypeSymbol
							+ " is applied to wrong number of arguments");
				return null;
			}

			List<TypeSymbol> list1 = dispatch.getArgs().subList(1, dispatch.getArgs().size()).stream()
					.map(x -> x.accept(this))
					.toList();
			Object[] result = SymbolTable.isCallingMethodWithDifferentTypeOfFormals(list1, eType.getName(), dispatch.getToken().getText());
			if ((boolean)result[0]) {
				var pos = (int)result[3];
				SymbolTable.error(ctx, dispatch.getArgs().get(pos + 1).getToken(),
					"In call to method " + dispatch.getToken().getText() + " of class " + eTypeSymbol
						+ ", actual type " + list1.get(pos).getName() + " of formal parameter " + result[1]
						+ " is incompatible with declared type " + result[2]);
			}

			if (dispatch.getE() instanceof Id) {
				if (((Id) dispatch.getE()).getScope() instanceof FunctionSymbol) {
					return ((FunctionSymbol) ((Id) dispatch.getE()).getScope()).getType();
				}
			}
		}

		return null;
	}

	@Override
	public TypeSymbol visit(FuncDef funcDef) {
		var ctx = funcDef.getCtx();
		var id = funcDef.getId();
		var type = funcDef.getType();

		if (SymbolTable.isOverridingMethodWithDifferentNumberOfFormals(id)) {
			SymbolTable.error(ctx, id.getToken(),
					"Class " + id.getScope().getParent().toString() + " overrides method "
							+ id.getToken().getText() + " with different number of formal parameters");
			return null;
		}

		Object[] result = SymbolTable.isOverridingMethodWithDifferentTypeOfFormals(id);
		if ((boolean)result[0]) {
			var idSymbolName = ((IdSymbol) result[2]).getName();
			var token = ctx.formals.stream()
					.filter(x -> x.name.getText().equals(idSymbolName))
					.findFirst()
					.get();
			SymbolTable.error(ctx, token.type,
					"Class " + id.getScope().getParent().toString() + " overrides method "
							+ id.getToken().getText() + " but changes type of formal parameter "
							+ ((IdSymbol)result[2]).getName() + " from " + ((IdSymbol)result[1]).getType().getName()
							+ " to " + ((IdSymbol)result[2]).getType().getName());
			return null;
		}

		result = SymbolTable.isOverridingMethodWithDifferentReturnType(id);
		if ((boolean)result[0]) {
			SymbolTable.error(ctx, ctx.type,
					"Class " + id.getScope().getParent().toString() + " overrides method "
							+ id.getToken().getText() + " but changes return type from "
							+ ((TypeSymbol)result[1]).getName() + " to "
							+ ((TypeSymbol)result[2]).getName());
			return null;
		}

		for (var param : funcDef.getParams()) {
			param.accept(this);
		}

		var bodyType = funcDef.getBody().accept(this);
		TypeSymbol idType = SymbolTable.getTypeSymbol(type);

		if (!type.getToken().getText().equals("Object") && bodyType != null && idType != null
				&& SymbolTable.areIncompatibleTypes(idType, bodyType)) {
			SymbolTable.error(ctx, funcDef.getBody().getToken(),
					"Type " + bodyType.getName() + " of the body of method " + id.getToken().getText()
							+ " is incompatible with declared return type " + type.getToken().getText());
		}

		return null;
	}

	@Override
	public TypeSymbol visit(Stringg stringg) {
		return TypeSymbol.STRING;
	}

	@Override
	public TypeSymbol visit(While whilee) {
		var condType = whilee.getCond().accept(this);

		if (condType != TypeSymbol.BOOL) {
			SymbolTable.error(whilee.getCtx(), whilee.getCond().getToken(),
					"While condition has type " + condType.getName() + " instead of Bool");
		}

		return TypeSymbol.OBJECT;
	}

	@Override
	public TypeSymbol visit(VarDef varDef) {
		var ctx = varDef.getCtx();
		var type = varDef.getType();
		var id = varDef.getId();

		if (id.getScope() != null) {
			if (id.getScope().lookup(type.getToken().getText()) == null) {
				SymbolTable.error(ctx, type.getToken(),
						"Class " + id.getScope().toString() + " has attribute " + ctx.name.getText()
								+ " with undefined type " + type.getToken().getText());
				return null;
			}
		}

		if (SymbolTable.isRedefinedInheritedAttribute(id)) {
			SymbolTable.error(ctx, id.getToken(),
					"Class " + id.getScope().toString() + " redefines inherited attribute " + id.getToken().getText());
			return null;
		}

		if (id.getScope() != null) {
			id.getSymbol().setType((TypeSymbol) id.getScope().lookup(type.getToken().getText()));
		}
		if (varDef.getInit() != null) {
			var varType = varDef.getInit().accept(this);
			TypeSymbol idType = SymbolTable.getTypeSymbol(type);

			if (!type.getToken().getText().equals("Object") && varType != null  && idType != null
					&& SymbolTable.areIncompatibleTypes(idType, varType)) {
				if (varDef.getInit() instanceof Dispatch) {
					SymbolTable.error(ctx, ((Dispatch) varDef.getInit()).getCtx().start,
							"Type " + varType.getName() + " of initialization expression of attribute " + id.getToken().getText()
									+ " is incompatible with declared type " + type.getToken().getText());
				} else {
					SymbolTable.error(ctx, varDef.getInit().getToken(),
							"Type " + varType.getName() + " of initialization expression of attribute " + id.getToken().getText()
									+ " is incompatible with declared type " + type.getToken().getText());
				}
			}
		}
		return null;
	}

	@Override
	public TypeSymbol visit(Type type) {
		return null;
	}
}