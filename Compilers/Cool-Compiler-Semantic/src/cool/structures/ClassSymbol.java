package cool.structures;

public class ClassSymbol extends FunctionSymbol {

	public ClassSymbol(String name, Scope parent) {
		super(name, parent);
	}

	@Override
	public boolean add(Symbol sym) {
		if (symbols.containsKey(sym.getName())) {
			var symbol = symbols.get(sym.getName());
			if (symbol.getClass().equals(sym.getClass())) {
				return false;
			}
		}

		symbols.put(sym.getName(), sym);

		return true;
	}
}
