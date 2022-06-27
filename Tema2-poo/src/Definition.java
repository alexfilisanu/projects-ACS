import java.util.List;

public class Definition implements Comparable<Definition>{
    private String dict;
    private String dictType;
    private Integer year;
    private List<String> text;

    public String getDict() { return dict; }
    public void setDict(String dict) { this.dict = dict; }
    public String getDictType() { return dictType; }
    public void setDictType(String dictType) { this.dictType = dictType; }
    public Integer getYear() { return year; }
    public void setYear(Integer year) { this.year = year; }
    public List<String> getText() { return text; }
    public void setText(List<String> text) { this.text = text; }

    public Definition(String dict, String dictType, Integer year, List<String> text) {
        this.dict = dict;
        this.dictType = dictType;
        this.year = year;
        this.text = text;
    }

    /** Pentru sortarea in functie de anul aparitiei a definitiilor: */
    @Override
    public int compareTo(Definition o) {
        return this.getYear().compareTo(o.getYear());
    }

    @Override
    public String toString() {
        return "Definition{" + "dict=" + dict + ", year=" + year + "}";
    }
}
