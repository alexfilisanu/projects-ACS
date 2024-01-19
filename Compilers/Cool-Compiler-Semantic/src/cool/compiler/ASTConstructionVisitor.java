package cool.compiler;

import cool.parser.CoolParser;
import cool.parser.CoolParserBaseVisitor;

import java.util.stream.Collectors;

public class ASTConstructionVisitor extends CoolParserBaseVisitor<ASTNode> {
	@Override
	public ASTNode visitAssign(CoolParser.AssignContext ctx) {
		return new Assign(ctx,
				new Id(ctx.name),
				(Expression)visit(ctx.value));
	}

	@Override
	public ASTNode visitBitwiseNot(CoolParser.BitwiseNotContext ctx) {
		return new BitwiseNot(ctx,
				(Expression)visit(ctx.e));
	}

	@Override
	public ASTNode visitBlock(CoolParser.BlockContext ctx) {
		return new Block(ctx.exprs.stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()),
				ctx.start);
	}

	@Override
	public ASTNode visitBool(CoolParser.BoolContext ctx) {
		return new Bool(ctx.BOOL().getSymbol());
	}

	@Override
	public ASTNode visitCall(CoolParser.CallContext ctx) {
		return new Call(ctx,
				ctx.expr().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
	}

	@Override
	public ASTNode visitCase(CoolParser.CaseContext ctx) {
		return new Case(ctx,
				(Expression)visit(ctx.e),
				ctx.caseStatements.stream().map(x -> (CaseStatement) visit(x)).collect(Collectors.toList()));
	}

	@Override
	public ASTNode visitCaseStatement(CoolParser.CaseStatementContext ctx) {
		return new CaseStatement(ctx,
				new Id(ctx.name),
				new Type(ctx.type),
				(Expression) visit(ctx.init));
	}

	@Override
	public ASTNode visitClass(CoolParser.ClassContext ctx) {
		return new Classs(ctx,
				new Id(ctx.type),
				ctx.definition().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
	}

	@Override
	public ASTNode visitDispatch(CoolParser.DispatchContext ctx) {
		return new Dispatch(ctx,
				(Expression)visit(ctx.e),
				new Type(ctx.type),
				ctx.expr().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
	}

	@Override
	public ASTNode visitFormal(CoolParser.FormalContext ctx) {
		return new Formal(ctx,
				new Id(ctx.name),
				new Type(ctx.type));
	}

	@Override
	public ASTNode visitFuncDef(CoolParser.FuncDefContext ctx) {
		return new FuncDef(ctx,
				new Id(ctx.name),
				new Type(ctx.type),
				ctx.formals.stream().map(x -> (Formal) visit(x)).collect(Collectors.toList()),
				(Expression)visit(ctx.body));
	}

	@Override
	public ASTNode visitId(CoolParser.IdContext ctx) {
		return new Id(ctx);
	}

	@Override
	public ASTNode visitIf(CoolParser.IfContext ctx) {
		return new If(ctx,
				(Expression)visit(ctx.cond),
				(Expression)visit(ctx.thenBranch),
				(Expression)visit(ctx.elseBranch));
	}

	@Override
	public ASTNode visitInt(CoolParser.IntContext ctx) {
		return new Int(ctx.INT().getSymbol());
	}

	@Override
	public ASTNode visitIsvoid(CoolParser.IsvoidContext ctx) {
		return new IsVoid(ctx.IS_VOID().getSymbol(),
				(Expression)visit(ctx.e));
	}

	@Override
	public ASTNode visitLet(CoolParser.LetContext ctx) {
		return new Let(ctx,
				ctx.letVars.stream().map(x -> (LetVar) visit(x)).collect(Collectors.toList()),
				(Expression)visit(ctx.body));
	}

	@Override
	public ASTNode visitLetVar(CoolParser.LetVarContext ctx) {
		return ctx.init != null
				? new LetVar(ctx, new Id(ctx.name), new Type(ctx.type), (Expression) visit(ctx.init))
				: new LetVar(ctx, new Id(ctx.name), new Type(ctx.type));
	}

	@Override
	public ASTNode visitMultDiv(CoolParser.MultDivContext ctx) {
		return new MultDiv(ctx,
				(Expression)visit(ctx.left),
				(Expression)visit(ctx.right));
	}

	@Override
	public ASTNode visitNew(CoolParser.NewContext ctx) {
		return new New(ctx,
				new Type(ctx.name));
	}

	@Override
	public ASTNode visitNot(CoolParser.NotContext ctx) {
		return new Not(ctx,
				(Expression)visit(ctx.e));
	}

	@Override
	public ASTNode visitParen(CoolParser.ParenContext ctx) {
		return new Paren((Expression)visit(ctx.e),
				ctx.start);
	}

	@Override
	public ASTNode visitPlusMinus(CoolParser.PlusMinusContext ctx) {
		return new PlusMinus(ctx,
				(Expression)visit(ctx.left),
				(Expression)visit(ctx.right));
	}

	@Override
	public ASTNode visitProgram(CoolParser.ProgramContext ctx) {
		return new Program(ctx.class_().stream().map(x -> (Classs) visit(x)).collect(Collectors.toList()),
				ctx.start);
	}

	@Override
	public ASTNode visitRelational(CoolParser.RelationalContext ctx) {
		return new Relational(ctx,
				(Expression)visit(ctx.left),
				(Expression)visit(ctx.right));
	}

	@Override
	public ASTNode visitString(CoolParser.StringContext ctx) {
		return new Stringg(ctx.STRING().getSymbol());
	}

	@Override
	public ASTNode visitVarDef(CoolParser.VarDefContext ctx) {
		return ctx.init != null
				? new VarDef(ctx,
				new Type(ctx.type),
				new Id(ctx.name),
				(Expression) visit(ctx.init))
				: new VarDef(ctx,
				new Type(ctx.type),
				new Id(ctx.name));
	}

	@Override
	public ASTNode visitWhile(CoolParser.WhileContext ctx) {
		return new While(ctx,
				(Expression)visit(ctx.cond),
				(Expression)visit(ctx.body));
	}
}
