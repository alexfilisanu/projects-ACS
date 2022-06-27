public class Language {
    private int ID;
    private String code;
    private String name;

    public int getID() { return ID; }
    public void setID(int ID) { this.ID = ID; }
    public String getCode() { return code; }
    public void setCode(String code) { this.code = code; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public Language(int ID, String code, String name) {
        this.ID = ID;
        this.code = code;
        this.name = name;
    }

}