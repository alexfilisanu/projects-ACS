import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.google.gson.stream.JsonReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class Main{
   /** Dictionarul in care salvez drept key traducerea cuvantului in engleza, iar ca value o lista a
     * cuvintelor ce inseamna acelasi lucru + limba acestora(clasa WorldLangauge);
     */
    static HashMap<String, List<WordLanguage>> completeDictionary = new HashMap<>();

    public static void main(String[] args) throws IOException {
        File directoryPath = new File(".\\Dictionaries");
        File[] fileList = directoryPath.listFiles();
        /** Parcurg toate fisierele din directorul "Dictionaries" */
        assert fileList != null;
        for (File file : fileList) {
            Type formatType = new TypeToken<List<Word>>() {}.getType();
            Gson gson = new Gson();
            JsonReader reader = new JsonReader(new FileReader(file.getPath()));
            /** Extrag limba din numele dinctionarului din care citesc: */
            String language = file.getName().substring(0, file.getName().indexOf("_"));
            List<Word> words = gson.fromJson(reader, formatType);
            for (Word word : words) {
                /** Daca cheia deja exista => adaug noul cuvant in continuarea listei */
                if (completeDictionary.containsKey(word.getWord_en())) {
                    completeDictionary.get(word.getWord_en()).add(new WordLanguage(language, word));
                } else {
                    /** Daca nu => creez cheia si lista aferenta acesteia in care adaug cuvantul citit*/
                    ArrayList<WordLanguage> list = new ArrayList<>();
                    list.add(new WordLanguage(language, word));
                    completeDictionary.put(word.getWord_en(), list);
                }
            }
        }
        /** TESTARE */
        Tests tests = new Tests();
        tests.testing();
    }
}
