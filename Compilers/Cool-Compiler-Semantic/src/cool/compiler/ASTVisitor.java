package cool.compiler;

public interface ASTVisitor<T> {
	T visit(Assign assign);
	T visit(BitwiseNot bitwiseNot);
	T visit(Block block);
	T visit(Bool bool);
	T visit(Call call);
	T visit(Case casee);
	T visit(CaseStatement caseStatement);
	T visit(Classs classs);
	T visit(Dispatch dispatch);
	T visit(Formal formal);
	T visit(FuncDef funcDef);
	T visit(Id id);
	T visit(If iff);
	T visit(Int intt);
	T visit(IsVoid isVoid);
	T visit(Let let);
	T visit(LetVar letVar);
	T visit(MultDiv multDiv);
	T visit(New neww);
	T visit(Not not);
	T visit(Paren paren);
	T visit(PlusMinus plusMinus);
	T visit(Program program);
	T visit(Relational relational);
	T visit(Stringg string);
	T visit(Type type);
	T visit(VarDef varDef);
	T visit(While whilee);
}
