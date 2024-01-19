package cool.structures;

public class IdSymbol extends Symbol {
	protected TypeSymbol type;

	public IdSymbol(String name) {
		super(name);
	}

	public void setType(TypeSymbol type) {
		this.type = type;
	}

	public TypeSymbol getType() {
		return type;
	}
}
