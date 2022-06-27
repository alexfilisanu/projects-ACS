import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

public class Methods {
    /** Metoda pentru adaugarea unui cuvant în dicționar: daca dictionarul contine deja cheia
     * (cuvantul in eng) => verific daca respectivul cuvant in limba specificata exista deja
     * sau nu; da => return false; nu => adaug cuvantul in lista cheii + return true;
     * daca dictionarul nu contine deja cheia => o creez si ii adaug o lista cu cuvantul
     * specificat + return true;
     */
    public boolean addWord(Word word, String language) {
        if (Main.completeDictionary.containsKey(word.getWord_en())) {
            for (WordLanguage wordLanguage : Main.completeDictionary.get(word.getWord_en())) {
                if (wordLanguage.getLanguage().equals(language)) {
                    return false;
                }
            }
            Main.completeDictionary.get(word.getWord_en()).add(new WordLanguage(language, word));
            return true;
        }
        ArrayList<WordLanguage> list = new ArrayList<>();
        list.add(new WordLanguage(language, word));
        Main.completeDictionary.put(word.getWord_en(), list);
        return true;
    }

    /** Metoda pentru ștergerea unui cuvânt din dicționar ce primește ca parametru cuvantul și limba:
     * Parcurg pe rand fiecare lista a fiecarei chei(in momentul in care gasesc limba in fiecare lista,
     * daca cuvantul este identic cu cel cautat il sterg + verific daca lista este goala(daca da o sterg)
     * + return true; iar daca nu inseamna ca nu il gasesc in lista respectiva si trec la urmatoarea);
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => return false;
     */
    public boolean removeWord(String word, String language) {
        Iterator<Map.Entry<String, List<WordLanguage>>> iterator = Main.completeDictionary.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<String, List<WordLanguage>> entry=iterator.next();
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(language)) {
                    if (entry.getValue().get(indexWord).getWordLangauge().getWord().equals(word)) {
                        entry.getValue().remove(indexWord);
                        if (entry.getValue().isEmpty()) {
                            iterator.remove();
                        }
                        return true;
                    }
                    break;
                }
            }
        }
        return false;
    }

    /** Metoda pentru adaugarea unei noi definitii pentru un cuvant dat ca parametru:
     * Parcurg pe rand fiecare lista a fiecarei chei(in momentul in care gasesc limba in fiecare lista,
     * daca cuvantul este identic cu cel cautat ii caut prin definitii sa vad daca mai exista deja una
     * din aceeasi surasa; da => return false; nu => adaug definitia in lista + return true);
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => return false;
     */
    public boolean addDefinitionForWord(String word, String language, Definition definition) {
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(language)) {
                    Word wordIterator = entry.getValue().get(indexWord).getWordLangauge();
                    if (wordIterator.getWord().equals(word)) {
                        for (int indexDef = 0; indexDef < wordIterator.getDefinitions().size(); indexDef++) {
                            if (wordIterator.getDefinitions().get(indexDef).getDict().equals(definition.getDict())) {
                                return false;
                            }
                        }
                        wordIterator.getDefinitions().add(definition);
                        return true;
                    }
                    break;
                }
            }
        }
        return false;
    }

    /** Metoda pentru stergerea unei definitii a unui cuvant dat ca parametru:
     * Parcurg pe rand fiecare lista a fiecarei chei(in momentul in care gasesc limba in fiecare lista,
     * daca cuvantul este identic cu cel cautat ii caut prin definitii sa vad daca exista deja una
     * din aceeasi surasa; da => o sterg + return true; nu => return false);
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => return false;
     */
    public boolean removeDefinition(String word, String language, String dictionary) {
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(language)) {
                    Word wordIterator = entry.getValue().get(indexWord).getWordLangauge();
                    if (wordIterator.getWord().equals(word)) {
                        for (int indexDef = 0; indexDef < wordIterator.getDefinitions().size(); indexDef++) {
                            if (wordIterator.getDefinitions().get(indexDef).getDict().equals(dictionary)) {
                                wordIterator.getDefinitions().remove(indexDef);
                                return true;
                            }
                        }
                        return false;
                    }
                    break;
                }
            }
        }
        return false;
    }

    /** Metoda pentru traducerea unui cuvant: Parcurg pe rand fiecare lista a fiecarei chei(in momentul
     * in care gasesc limba in fiecare lista, daca cuvantul este identic cu cel cautat ii caut prin
     * lista cheii traducerea in limba ceruta(daca exista => return traducere, daca nu => nu exista traducere);
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => nu exista traducere;
     */
    public String translateWord(String word, String fromLanguage, String toLanguage) {
        /** Daca cuvantul este substantiv => caut variantele de singular si plural; */
        /** Daca cuvantul este verb => caut toate cele 3 variantele de singular si plural; */
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(fromLanguage)) {
                    Word wordIterator = entry.getValue().get(indexWord).getWordLangauge();
                    switch (wordIterator.getType()) {
                        /** Daca cuvantul este substantiv => caut variantele de singular si plural; */
                        case "noun" :
                            if (wordIterator.getSingular().get(0).equals(word)) {
                                return getSingularForm(word, toLanguage, entry, 0);
                            }
                            if (wordIterator.getPlural().get(0).equals(word)) {
                                return getPluralForm(word, toLanguage, entry, 0);
                            }
                            break;
                        /** Daca cuvantul este verb => caut toate cele 3 variantele de singular si plural; */
                        case "verb" :
                            for (int verbIndex = 0; verbIndex < 3; verbIndex++) {
                                if (wordIterator.getSingular().get(verbIndex).equals(word)) {
                                    return getSingularForm(word, toLanguage, entry, verbIndex);
                                }
                                if (wordIterator.getPlural().get(verbIndex).equals(word)) {
                                    return getPluralForm(word, toLanguage, entry, verbIndex);
                                }
                            }
                            break;
                        default:
                            throw new IllegalStateException("Unexpected value: " + wordIterator.getType());
                    }
                    break;
                }
            }
        }
        return word + "(fara traducere in " + toLanguage + ")";
    }

    private String getPluralForm(String word, String toLanguage, Map.Entry<String, List<WordLanguage>> entry, int i) {
        for (int indexTranslate = 0; indexTranslate < entry.getValue().size(); indexTranslate++) {
            if (entry.getValue().get(indexTranslate).getLanguage().equals(toLanguage)) {
                return entry.getValue().get(indexTranslate).getWordLangauge().getPlural().get(i);
            }
        }
        return word + "(fara traducere in " + toLanguage + ")";
    }

    private String getSingularForm(String word, String toLanguage, Map.Entry<String, List<WordLanguage>> entry, int i) {
        for (int indexTranslate = 0; indexTranslate < entry.getValue().size(); indexTranslate++) {
            if (entry.getValue().get(indexTranslate).getLanguage().equals(toLanguage)) {
                return entry.getValue().get(indexTranslate).getWordLangauge().getSingular().get(i);
            }
        }
        return word + "(fara traducere in " + toLanguage + ")";
    }

    /** Metoda pentru traducerea unei propozitii: apelez metoda translateWord pentru fiecare
     * cuvant din propozitia data;
     */
    public String translateSentence(String sentence, String fromLanguage, String toLanguage) {
        String strReturn = new String();
        String[] tokens = sentence.split(" ");
        for (String token : tokens) {
            strReturn = strReturn.concat(translateWord(token, fromLanguage, toLanguage));
            strReturn = strReturn.concat(" ");
        }
        return strReturn;
    }

    /** Metoda pentru returnarea tututoror variantelor de traducere a unui cuvant(sinonime) + o folosesc in metodata
     * translateSentances: Parcurg pe rand fiecare lista a fiecarei chei(in momentul in care gasesc limba in fiecare
     * lista, daca cuvantul este identic cu cel cautat ii caut prin lista cheii traducerea in limba ceruta(daca exista
     * => caut sinonimele in definitii + le returnez, daca nu => nu exista traducere);
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => nu exista traducere;
     */
    public ArrayList<String> searchSynonyms(String word, String fromLanguage, String toLanguage) {
        ArrayList<String> synonymsList = new ArrayList<>();
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(fromLanguage)) {
                    Word wordIterator = entry.getValue().get(indexWord).getWordLangauge();
                    if (wordIterator.getWord().equals(word)) {
                        return getSynonymsList(word, toLanguage, synonymsList, entry);
                    }
                    break;
                }
            }
        }
        synonymsList.add(word + "(fara traducere in " + toLanguage + ")");
        return synonymsList;
    }

    private ArrayList<String> getSynonymsList(String word, String toLanguage, ArrayList<String> synonymsList, Map.Entry<String, List<WordLanguage>> entry) {
        for (int indexTranslate = 0; indexTranslate < entry.getValue().size(); indexTranslate++) {
            if (entry.getValue().get(indexTranslate).getLanguage().equals(toLanguage)) {
                Word translateIterator = entry.getValue().get(indexTranslate).getWordLangauge();
                synonymsList.add(translateIterator.getWord());
                for (int indexDef = 0; indexDef < translateIterator.getDefinitions().size(); indexDef++) {
                    if ("synonyms".equals(translateIterator.getDefinitions().get(indexDef).getDictType())) {
                        synonymsList.addAll(translateIterator.getDefinitions().get(indexDef).getText());
                        return synonymsList;
                    }
                }
            }
        }
        synonymsList.add(word + "(fara traducere in " + toLanguage + ")");
        return synonymsList;
    }

    /** Metoda pentru traducerea unei propozitii si furnizarea a 3 variante de traducere folosind sinonimele
     * cuvintelor: apelez metoda searchSynonyms pentru fiecare cuvant din propozitia data; Folosesc primele 3
     * rezultate primite pentru a crea diferite traduceri ale propozitiei;
     */
    public ArrayList<String> translateSentences(String sentence, String fromLanguage, String toLanguage) {
        ArrayList<String> arrReturn = new ArrayList<>();
        boolean first = true;
        String[] tokens = sentence.split(" ");
        for (String token : tokens) {
            ArrayList<String> synonyms  = searchSynonyms(token, fromLanguage, toLanguage);
            /** Daca este primul cuvant: */
            if (first) {
                for (int index = 0; index < 3; index++) {
                    if (synonyms.size() > index) {
                        arrReturn.add(index, synonyms.get(index));
                    } else {
                        arrReturn.add(index, synonyms.get(0));
                    }
                }
                first = false;
            /** Daca cuvantul este pe orice alta pozitie in afara de prima: */
            } else {
                for (int index = 0; index < 3; index++) {
                    if (synonyms.size() > index) {
                        arrReturn.set(index, arrReturn.get(index).concat(" " + synonyms.get(index)));
                    } else {
                        arrReturn.set(index, arrReturn.get(index).concat(" " + synonyms.get(0)));
                    }
                }
            }
        }
        return arrReturn;
    }

    /** Metoda pentru intoarcerea definițiilor si sinonimelor unui cuvant: Parcurg pe rand fiecare lista
     * a fiecarei chei(in momentul in care gasesc limba in fiecare lista, daca cuvantul este identic cu cel
     * cautat ii adaug definitiile intr-o lista pe care o sortez si o returnez;
     * Daca ajung la final si nu am returnat nimic => nu exista cuvantul in dictionar => return lista goala;
     */
    public ArrayList<Definition> getDefinitionsForWord(String word, String language) {
        ArrayList<Definition> arrReturn = new ArrayList<>();
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(language)) {
                    Word wordIterator = entry.getValue().get(indexWord).getWordLangauge();
                    if (wordIterator.getWord().equals(word)) {
                        arrReturn.addAll(wordIterator.getDefinitions());
                        Collections.sort(arrReturn);
                        return arrReturn;
                    }
                    break;
                }
            }
        }
        return arrReturn;
    }

    /** Metoda pentru exportarea unui dictionar in format JSON(folosesc biblioteca gson): Parcurg pe rand
     * fiecare lista a fiecarei chei(in momentul in care gasesc limba adaug cuvantul intr-o lista(in prealabil ii
     * sortez definitiile in fuctie de anul aparitiei)); Sortez lista alfabetic si o export catre un fisier de
     * forma "output_language.json";
     */
    public void exportDictionary(String language) {
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        ArrayList<Word> wordsList = new ArrayList<>();
        for (Map.Entry<String, List<WordLanguage>> entry : Main.completeDictionary.entrySet()) {
            for (int indexWord = 0; indexWord < entry.getValue().size(); indexWord++) {
                if (entry.getValue().get(indexWord).getLanguage().equals(language)) {
                    Word word = entry.getValue().get(indexWord).getWordLangauge();
                    Collections.sort(word.getDefinitions());
                    wordsList.add(word);
                    break;
                }
            }
        }
        Collections.sort(wordsList);
        try (FileWriter writer = new FileWriter("Output\\output_" + language + ".json")) {
            gson.toJson(wordsList, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
