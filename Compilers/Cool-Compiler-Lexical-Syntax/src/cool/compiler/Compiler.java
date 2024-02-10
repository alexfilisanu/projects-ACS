package cool.compiler;

import cool.lexer.CoolLexer;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import cool.parser.*;

import java.io.*;
import java.util.Objects;
import java.util.stream.Collectors;


public class Compiler {
    // Annotates class nodes with the names of files where they are defined.
    public static ParseTreeProperty<String> fileNames = new ParseTreeProperty<>();

    public static void main(String[] args) throws IOException {
        if (args.length == 0) {
            System.err.println("No file(s) given");
            return;
        }

        CoolLexer lexer = null;
        CommonTokenStream tokenStream = null;
        CoolParser parser = null;
        ParserRuleContext globalTree = null;

        // True if any lexical or syntax errors occur.
        boolean lexicalSyntaxErrors = false;

        // Parse each input file and build one big parse tree out of
        // individual parse trees.
        for (var fileName : args) {
            var input = CharStreams.fromFileName(fileName);

            // Lexer
            if (lexer == null)
                lexer = new CoolLexer(input);
            else
                lexer.setInputStream(input);

            // Token stream
            if (tokenStream == null)
                tokenStream = new CommonTokenStream(lexer);
            else
                tokenStream.setTokenSource(lexer);
                
            /*
            // Test lexer only.
            tokenStream.fill();
            List<Token> tokens = tokenStream.getTokens();
            tokens.stream().forEach(token -> {
                var text = token.getText();
                var name = CoolLexer.VOCABULARY.getSymbolicName(token.getType());

                System.out.println(text + " : " + name);
                //System.out.println(token);
            });
            */

            // Parser
            if (parser == null)
                parser = new CoolParser(tokenStream);
            else
                parser.setTokenStream(tokenStream);

            // Customized error listener, for including file names in error
            // messages.
            var errorListener = new BaseErrorListener() {
                public boolean errors = false;

                @Override
                public void syntaxError(Recognizer<?, ?> recognizer,
                                        Object offendingSymbol,
                                        int line, int charPositionInLine,
                                        String msg,
                                        RecognitionException e) {
                    String newMsg = "\"" + new File(fileName).getName() + "\", line " +
                            line + ":" + (charPositionInLine + 1) + ", ";

                    Token token = (Token) offendingSymbol;
                    if (token.getType() == CoolLexer.ERROR)
                        newMsg += "Lexical error: " + token.getText();
                    else
                        newMsg += "Syntax error: " + msg;

                    System.err.println(newMsg);
                    errors = true;
                }
            };

            parser.removeErrorListeners();
            parser.addErrorListener(errorListener);

            // Actual parsing
            var tree = parser.program();
            if (globalTree == null)
                globalTree = tree;
            else
                // Add the current parse tree's children to the global tree.
                for (int i = 0; i < tree.getChildCount(); i++)
                    globalTree.addAnyChild(tree.getChild(i));

            // Annotate class nodes with file names, to be used later
            // in semantic error messages.
            for (int i = 0; i < tree.getChildCount(); i++) {
                var child = tree.getChild(i);
                // The only ParserRuleContext children of the program node
                // are class nodes.
                if (child instanceof ParserRuleContext) {
                    fileNames.put(child, fileName);
                }
            }

            // Record any lexical or syntax errors.
            lexicalSyntaxErrors |= errorListener.errors;

            // Stop before semantic analysis phase, in case errors occurred.
            if (lexicalSyntaxErrors) {
                System.err.println("Compilation halted");
                return;
            }

            var astConstructionVisitor = new CoolParserBaseVisitor<ASTNode>() {
                @Override
                public ASTNode visitAssign(CoolParser.AssignContext ctx) {
                    return new Assign(ctx.ASSIGN().getSymbol(),
                            ctx.name,
                            (Expression)visit(ctx.value));
                }

                @Override
                public ASTNode visitBitwiseNot(CoolParser.BitwiseNotContext ctx) {
                    return new BitwiseNot(ctx.TILDE().getSymbol(),
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
                    return new Call(ctx.name,
                            ctx.expr().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
                }

                @Override
                public ASTNode visitCase(CoolParser.CaseContext ctx) {
                    return new Case((Expression)visit(ctx.e),
                            ctx.caseStatements.stream().map(x -> (CaseStatement) visit(x)).collect(Collectors.toList()),
                            ctx.start);
                }

                @Override
                public ASTNode visitCaseStatement(CoolParser.CaseStatementContext ctx) {
                    return new CaseStatement(ctx.name,
                            ctx.type,
                            (Expression) visit(ctx.init));
                }

                @Override
                public ASTNode visitClass(CoolParser.ClassContext ctx) {
                    return new Classs(ctx.type,
                            ctx.inherit,
                            ctx.definition().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
                }

                @Override
                public ASTNode visitDispatch(CoolParser.DispatchContext ctx) {
                    return new Dispatch(ctx.name,
                            (Expression)visit(ctx.e),
                            ctx.type,
                            ctx.expr().stream().map(x -> (Expression) visit(x)).collect(Collectors.toList()));
                }

                @Override
                public ASTNode visitFormal(CoolParser.FormalContext ctx) {
                    return new Formal(ctx.name,
                            ctx.type,
                            ctx.start);
                }

                @Override
                public ASTNode visitFuncDef(CoolParser.FuncDefContext ctx) {
                    return new FuncDef(ctx.name,
                            ctx.type,
                            ctx.formals.stream().map(x -> (Formal) visit(x)).collect(Collectors.toList()),
                            (Expression)visit(ctx.body),
                            ctx.start);
                }

                @Override
                public ASTNode visitId(CoolParser.IdContext ctx) {
                    return new Id(ctx.ID().getSymbol());
                }

                @Override
                public ASTNode visitIf(CoolParser.IfContext ctx) {
                    return new If((Expression)visit(ctx.cond),
                            (Expression)visit(ctx.thenBranch),
                            (Expression)visit(ctx.elseBranch),
                            ctx.start);
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
                    return new Let(ctx.letVars.stream().map(x -> (LetVar) visit(x)).collect(Collectors.toList()),
                            (Expression)visit(ctx.body),
                            ctx.start);
                }

                @Override
                public ASTNode visitLetVar(CoolParser.LetVarContext ctx) {
                    return ctx.init != null
                            ? new LetVar(ctx.name,
                                ctx.type,
                                (Expression) visit(ctx.init),
                                ctx.start)
                            : new LetVar(ctx.name,
                                ctx.type,
                                ctx.start);
                }

                @Override
                public ASTNode visitMultDiv(CoolParser.MultDivContext ctx) {
                    return new MultDiv((Expression)visit(ctx.left),
                            ctx.op,
                            (Expression)visit(ctx.right));
                }

                @Override
                public ASTNode visitNew(CoolParser.NewContext ctx) {
                    return new New(ctx.NEW().getSymbol(),
                            ctx.name);
                }

                @Override
                public ASTNode visitNot(CoolParser.NotContext ctx) {
                    return new BitwiseNot(ctx.NOT().getSymbol(),
                            (Expression)visit(ctx.e));
                }

                @Override
                public ASTNode visitParen(CoolParser.ParenContext ctx) {
                    return new Paren((Expression)visit(ctx.e),
                            ctx.start);
                }

                @Override
                public ASTNode visitPlusMinus(CoolParser.PlusMinusContext ctx) {
                    return new PlusMinus((Expression)visit(ctx.left),
                            ctx.op,
                            (Expression)visit(ctx.right));
                }

                @Override
                public ASTNode visitProgram(CoolParser.ProgramContext ctx) {
                    return new Program(ctx.class_().stream().map(x -> (Classs) visit(x)).collect(Collectors.toList()),
                            ctx.start);
                }

                @Override
                public ASTNode visitRelational(CoolParser.RelationalContext ctx) {
                    return new Relational((Expression)visit(ctx.left),
                            ctx.op,
                            (Expression)visit(ctx.right));
                }

                @Override
                public ASTNode visitSelf(CoolParser.SelfContext ctx) {
                    return new Self(ctx.SELF().getSymbol());
                }

                @Override
                public ASTNode visitString(CoolParser.StringContext ctx) {
                    return new Stringg(ctx.STRING().getSymbol());
                }

                @Override
                public ASTNode visitVarDef(CoolParser.VarDefContext ctx) {
                    return ctx.init != null
                            ? new VarDef(ctx.name,
                                ctx.type,
                                (Expression) visit(ctx.init),
                                ctx.start)
                            : new VarDef(ctx.name,
                                ctx.type,
                                ctx.start);
                }

                @Override
                public ASTNode visitWhile(CoolParser.WhileContext ctx) {
                    return new While((Expression)visit(ctx.cond),
                            (Expression)visit(ctx.body),
                            ctx.start);
                }
            };

            var printVisitor = new ASTVisitor<Void>() {
                int indent = 0;

                @Override
                public Void visit(Assign assign) {
                    printIndent(assign.token.getText());

                    indent++;
                    printIndent(assign.name.getText());
                    assign.value.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(BitwiseNot bitwiseNot) {
                    printIndent(bitwiseNot.token.getText());

                    indent++;
                    bitwiseNot.e.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Block block) {
                    printIndent("block");

                    indent++;
                    block.exprs.forEach(x -> x.accept(this));
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Bool bool) {
                    printIndent(bool.token.getText());

                    return null;
                }

                @Override
                public Void visit(Call call) {
                    printIndent("implicit dispatch");

                    indent++;
                    printIndent(call.token.getText());
                    call.args.forEach(x -> x.accept(this));
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Case casee) {
                    printIndent("case");

                    indent++;
                    casee.e.accept(this);
                    casee.caseStatements.forEach(caseStatement -> caseStatement.accept(this));
                    indent--;

                    return null;
                }

                @Override
                public Void visit(CaseStatement caseStatement) {
                    printIndent("case branch");

                    indent++;
                    printIndent(caseStatement.name.getText());
                    printIndent(caseStatement.type.getText());
                    caseStatement.init.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Classs classs) {
                    printIndent("class");

                    indent++;
                    printIndent(classs.token.getText());
                    if (classs.inherit != null) {
                        printIndent(classs.inherit.getText());
                    }
                    classs.definitions.forEach(definition -> definition.accept(this));
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Dispatch dispatch) {
                    printIndent(".");

                    indent++;
                    dispatch.e.accept(this);
                    if (Objects.nonNull(dispatch.type)) {
                        printIndent(dispatch.type.getText());
                    }
                    printIndent(dispatch.token.getText());
                    // sublist(1,) because e(first element of args) is already accepted before
                    dispatch.args.subList(1, dispatch.args.size())
                            .forEach(x -> x.accept(this));
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Formal formal) {
                    printIndent("formal");

                    indent++;
                    printIndent(formal.name.getText());
                    printIndent(formal.type.getText());
                    indent--;

                    return null;
                }

                @Override
                public Void visit(FuncDef funcDef) {
                    printIndent("method");

                    indent++;
                    printIndent(funcDef.name.getText());
                    funcDef.params.forEach(definition -> definition.accept(this));
                    printIndent(funcDef.type.getText());
                    if (Objects.nonNull(funcDef.body)) {
                        funcDef.body.accept(this);
                    }
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Id id) {
                    printIndent(id.token.getText());

                    return null;
                }

                @Override
                public Void visit(If iff) {
                    printIndent("if");

                    indent++;
                    iff.cond.accept(this);
                    iff.thenBranch.accept(this);
                    iff.elseBranch.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Int intt) {
                    printIndent(intt.token.getText());

                    return null;
                }

                @Override
                public Void visit(IsVoid isVoid) {
                    printIndent(isVoid.token.getText());

                    indent++;
                    isVoid.e.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Let let) {
                    printIndent("let");

                    indent++;
                    let.letVars.forEach(varDef -> varDef.accept(this));
                    let.body.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(LetVar letVar) {
                    printIndent("local");

                    indent++;
                    printIndent(letVar.name.getText());
                    printIndent(letVar.type.getText());
                    if (letVar.init != null) {
                        letVar.init.accept(this);
                    }
                    indent--;

                    return null;
                }

                @Override
                public Void visit(MultDiv multDiv) {
                    printIndent(multDiv.token.getText());

                    indent++;
                    multDiv.left.accept(this);
                    multDiv.right.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(New neww) {
                    printIndent(neww.token.getText());

                    indent++;
                    printIndent(neww.name.getText());
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Not not) {
                    printIndent(not.token.getText());

                    indent++;
                    not.e.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Paren paren) {
                    paren.e.accept(this);

                    return null;
                }

                @Override
                public Void visit(PlusMinus plusMinus) {
                    printIndent(plusMinus.token.getText());

                    indent++;
                    plusMinus.left.accept(this);
                    plusMinus.right.accept(this);
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
                    printIndent(relational.token.getText());

                    indent++;
                    relational.left.accept(this);
                    relational.right.accept(this);
                    indent--;

                    return null;
                }

                @Override
                public Void visit(Self self) {
                    printIndent(self.token.getText());

                    return null;
                }

                @Override
                public Void visit(Stringg stringg) {
                    // replace for special characters
                    String interpretedString = stringg.token.getText()
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
                public Void visit(VarDef varDef) {
                    printIndent("attribute");

                    indent++;
                    printIndent(varDef.name.getText());
                    printIndent(varDef.type.getText());
                    if (varDef.init != null) {
                        varDef.init.accept(this);
                    }
                    indent--;

                    return null;
                }

                @Override
                public Void visit(While whilee) {
                    printIndent("while");

                    indent++;
                    whilee.cond.accept(this);
                    whilee.body.accept(this);
                    indent--;

                    return null;
                }

                private void printIndent(String str) {
                    for (int i = 0; i < indent; i++)
                        System.out.print("  ");
                    System.out.println(str);
                }
            };

            var ast = astConstructionVisitor.visit(tree);
            ast.accept(printVisitor);
        }
    }
}
