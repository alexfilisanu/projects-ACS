package cool.structures;

import cool.compiler.Compiler;
import cool.compiler.Id;
import cool.compiler.Type;
import cool.parser.CoolParser;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import java.io.File;
import java.util.*;

public class SymbolTable {
    public static Scope globals;
    public static Map<String, String> childToParentMap = new LinkedHashMap<>();

    private static boolean semanticErrors;

    public static void defineBasicClasses() {
        globals = new DefaultScope(null);
        semanticErrors = false;

        globals.add(TypeSymbol.OBJECT);
        globals.add(TypeSymbol.IO);
        globals.add(TypeSymbol.INT);
        globals.add(TypeSymbol.STRING);
        globals.add(TypeSymbol.BOOL);
    }

    /**
     * Displays a semantic error message.
     * 
     * @param ctx Used to determine the enclosing class context of this error,
     *            which knows the file name in which the class was defined.
     * @param info Used for line and column information.
     * @param str The error message.
     */
    public static void error(ParserRuleContext ctx, Token info, String str) {
        while (! (ctx.getParent() instanceof CoolParser.ProgramContext))
            ctx = ctx.getParent();
        
        String message = "\"" + new File(Compiler.fileNames.get(ctx)).getName()
                + "\", line " + info.getLine()
                + ":" + (info.getCharPositionInLine() + 1)
                + ", Semantic error: " + str;
        
        System.err.println(message);
        
        semanticErrors = true;
    }
    
    public static void error(String str) {
        String message = "Semantic error: " + str;
        
        System.err.println(message);
        
        semanticErrors = true;
    }

    public static boolean isPrimitiveType(TypeSymbol typeSymbol) {
        return typeSymbol == TypeSymbol.INT
                || typeSymbol == TypeSymbol.STRING
                || typeSymbol == TypeSymbol.BOOL;
    }

    public static boolean isIllegalParent(Token parent) {
        return parent.getText().equals("Int")
                || parent.getText().equals("String")
                || parent.getText().equals("Bool")
                || parent.getText().equals("SELF_TYPE");
    }

    public static boolean isUndefinedToken(Token token) {
        return globals.lookup(token.getText()) == null;
    }

    public static boolean isInheritanceCycle(Token child) {
        Set<String> visited = new HashSet<>();
        var current = child.getText();

        while (current != null) {
            if (!visited.add(current)) {
                return true;
            }

            var next = childToParentMap.get(current);
            if (next == null) {
                return false;
            }
            if (next.equals(child.getText())) {
                return true;
            }

            current = next;
        }

        return false;
    }

    public static boolean isRedefinedInheritedAttribute(Id id) {
        var scope = id.getScope();

        while (scope != null) {
            var next = childToParentMap.get(scope.toString());

            if (next == null || globals.lookup(next) instanceof TypeSymbol) {
                return false;
            }

            if (((ClassSymbol)globals.lookup(next)).symbols.containsKey(id.getToken().getText())) {
                return true;
            }

            scope = (ClassSymbol)globals.lookup(next);
        }

        return false;
    }

    public static TypeSymbol getTypeSymbol(Type type) {
        var typeGlobal = SymbolTable.globals.lookup(type.getToken().getText());
        if (typeGlobal instanceof ClassSymbol) {
            return ((ClassSymbol) typeGlobal).getType();
        } else if (typeGlobal instanceof TypeSymbol) {
            return (TypeSymbol) typeGlobal;
        }

        return null;
    }


    public static boolean isOverridingMethodWithDifferentNumberOfFormals(Id id) {
        var scope = id.getScope();

        while (scope != null) {
            if (scope instanceof ClassSymbol) {
                var next = childToParentMap.get(scope.toString());

                if (next == null || globals.lookup(next) instanceof TypeSymbol) {
                    return false;
                }

                if (((ClassSymbol) globals.lookup(next)).symbols.containsKey(id.getToken().getText())) {
                    var method = (FunctionSymbol) ((ClassSymbol) globals.lookup(next)).symbols.get(id.getToken().getText());

                    if (id.getSymbol() instanceof FunctionSymbol) {
                        if (method.getFormals().size() != ((FunctionSymbol) id.getSymbol()).symbols.size()) {
                            return true;
                        }
                    }
                }
                scope = (ClassSymbol)globals.lookup(next);
            }
            scope = scope.getParent();
        }

        return false;
    }

    public static Object[] isOverridingMethodWithDifferentTypeOfFormals(Id id) {
        var result = new Object[]{false, null, null};
        var scope = id.getScope();

        while (scope != null) {
            if (scope instanceof ClassSymbol) {
                var next = childToParentMap.get(scope.toString());

                if (next == null || globals.lookup(next) instanceof TypeSymbol) {
                    return result;
                }

                if (((ClassSymbol) globals.lookup(next)).symbols.containsKey(id.getToken().getText())) {
                    var method = (FunctionSymbol) ((ClassSymbol) globals.lookup(next)).symbols.get(id.getToken().getText());

                    if (id.getSymbol() instanceof FunctionSymbol) {
                        var formals = ((FunctionSymbol)((ClassSymbol) scope).symbols.get(id.getToken().getText())).symbols.values().toArray();
                        var methodFormals = method.symbols.values().toArray();

                        for (int i = 0; i < formals.length; i++) {
                            if (!((IdSymbol)formals[i]).getType().equals(((IdSymbol)methodFormals[i]).getType())) {
                                return new Object[]{true, methodFormals[i], formals[i]};
                            }
                        }
                    }
                }
                scope = (ClassSymbol)globals.lookup(next);
            }
            scope = scope.getParent();
        }

        return result;
    }

    public static Object[] isOverridingMethodWithDifferentReturnType(Id id) {
        var result = new Object[]{false, null, null};
        var scope = id.getScope();

        while (scope != null) {
            if (scope instanceof ClassSymbol) {
                var next = childToParentMap.get(scope.toString());

                if (next == null || globals.lookup(next) instanceof TypeSymbol) {
                    return result;
                }

                if (((ClassSymbol) globals.lookup(next)).symbols.containsKey(id.getToken().getText())) {
                    var method = (FunctionSymbol) ((ClassSymbol) globals.lookup(next)).symbols.get(id.getToken().getText());

                    if (id.getSymbol() instanceof FunctionSymbol) {
                        if (!id.getSymbol().getType().equals(method.getType())) {
                            return new Object[]{true, method.getType(), id.getSymbol().getType()};
                        }
                    }
                }
                scope = (ClassSymbol)globals.lookup(next);
            }
            scope = scope.getParent();
        }
        return result;
    }

    public static boolean isInheritedClass(TypeSymbol childType, TypeSymbol parentType) {
        var current = childToParentMap.get(childType.getName());
        while (current != null) {
            if (current.equals(parentType.getName())) {
                return true;
            }
            current = childToParentMap.get(current);
        }

        return false;
    }

    public static boolean areIncompatibleTypes(TypeSymbol parentType, TypeSymbol childType) {
        return !parentType.getName().equals(childType.getName())
                && !SymbolTable.isInheritedClass(childType, parentType);
    }

    public static TypeSymbol getMostInheritedClass(ArrayList<TypeSymbol> typesList) {
        var types = new ArrayList<>(typesList);
        var current = types.get(0);

        for (int i = 1; i < types.size(); i++) {
            if (isInheritedClass(current, types.get(i))) {
                current = types.get(i);
            }
        }

        return current;
    }

    public static boolean isUndefinedMethod(TypeSymbol type, String methodName) {
        var current = type.getName();

        while (current != null) {
            if (globals.lookup(current) instanceof TypeSymbol) {
                return true;
            }

            if (((ClassSymbol)globals.lookup(current)).symbols.containsKey(methodName)) {
                return false;
            }

            current = childToParentMap.get(current);
        }

        return true;
    }

    public static Object[] isCallingMethodWithDifferentTypeOfFormals(List<TypeSymbol> typesList, String className, String methodName) {
        var result = new Object[]{false, null, null, null};
        var current = className;

        while (current != null) {
            if (globals.lookup(current) instanceof TypeSymbol) {
                return result;
            }

            if (((ClassSymbol)globals.lookup(current)).symbols.containsKey(methodName)) {
                Optional<Symbol> functionSymbol = ((ClassSymbol) globals.lookup(current)).symbols.values().stream()
                        .filter(sym -> sym.getName().equals(methodName))
                        .findFirst();

                if (functionSymbol.isPresent()) {
                    var currentTypes = ((FunctionSymbol) functionSymbol.get()).symbols.values().stream()
                            .map(x -> ((IdSymbol) x).getType())
                            .toList();
                    var currentIds = ((FunctionSymbol) functionSymbol.get()).symbols.keySet().stream().toList();
                    var minSize = Math.min(currentTypes.size(), typesList.size());

                    for (int i = 0; i < minSize; i++) {
                        if (!typesList.get(i).equals(currentTypes.get(i))
                                && !SymbolTable.isInheritedClass(typesList.get(i), currentTypes.get(i))) {
                            return new Object[]{true, currentIds.get(i), currentTypes.get(i), i};
                        }
                    }
                }
            }

            current = childToParentMap.get(current);
        }

        return result;
    }

    public static boolean hasSemanticErrors() {
        return semanticErrors;
    }
}
