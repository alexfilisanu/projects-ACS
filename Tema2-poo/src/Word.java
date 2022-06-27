import java.util.List;

public class Word implements Comparable<Word>{
    private String word;
    private String word_en;
    private String type;
    private List<String> singular;
    private List<String> plural;
    private List<Definition> definitions;

    public String getWord() { return word; }
    public void setWord(String word) { this.word = word; }
    public String getWord_en() { return word_en; }
    public void setWord_en(String word_en) { this.word_en = word_en; }
    public String getType() { return type; }
    public void setType(String type) { this.type = type; }
    public List<String> getSingular() { return singular; }
    public void setSingular(List<String> singular) { this.singular = singular; }
    public List<String> getPlural() { return plural; }
    public void setPlural(List<String> plural) { this.plural = plural; }
    public List<Definition> getDefinitions() { return definitions; }
    public void setDefinitions(List<Definition> definitions) { this.definitions = definitions; }

    public Word(String word, String word_en, String type, List<String> singular, List<String> plural, List<Definition> definitions) {
        this.word = word;
        this.word_en = word_en;
        this.type = type;
        this.singular = singular;
        this.plural = plural;
        this.definitions = definitions;
    }

    /** Pentru sortarea alfabetica a cuvintelor: */
    @Override
    public int compareTo(Word o) {
        return this.getWord().compareTo(o.getWord());
    }
}
