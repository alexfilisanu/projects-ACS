/** Am definit clasa WorldLanguage ce contine 2 atribute: String language si Word word pentru
 * a putea salva alaturi de fiecare cuvant si limba acestuia;
 */
public class WordLanguage {
    private String language;
    private Word word;

    public String getLanguage() { return language; }
    public void setLanguage(String language) { this.language = language; }
    public Word getWordLangauge() { return word; }
    public void setWords(Word word) { this.word = word; }

    public WordLanguage(String language, Word word) {
        this.language = language;
        this.word = word;
    }
}
