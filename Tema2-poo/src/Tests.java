import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Tests {
    public void testing() {
        try {
            Methods methods = new Methods();
            FileWriter fileWriter = new FileWriter("Output\\tests_output.txt");

            fileWriter.write("Teste pentru boolean addWord(Word word, String language):\n");
            fileWriter.write("\tTest 1:");
            ArrayList<String> listSingular1 = new ArrayList<>();
            ArrayList<String> listPlural1 = new ArrayList<>();
            ArrayList<String> listText1 = new ArrayList<>();
            ArrayList<Definition> definitions1 = new ArrayList<>();
            ArrayList<String> listSynonyms1 = new ArrayList<>();
            listSingular1.add("monedă");
            listPlural1.add("monede");
            listText1.add("Ban de metal, rar de hârtie, care are sau a avut curs legal pe teritoriul unui stat");
            listText1.add("Ban de metal de valoare mică");
            definitions1.add(new Definition("Dicționarul explicativ al limbii române ediția a II-a",
                    "definitions", 2009, listText1));
            listSynonyms1.add("ban");
            listSynonyms1.add("piesă");
            listSynonyms1.add("para");
            definitions1.add(new Definition("Dictionar de sinonime", "synonyms", 2002, listSynonyms1));
            fileWriter.write(" " + methods.addWord(new Word("monedă", "coin", "noun", listSingular1,
                    listPlural1, definitions1), "ro") + "\n");
            fileWriter.write("\tTest 2:");
            ArrayList<String> listSingular2 = new ArrayList<>();
            ArrayList<String> listPlural2 = new ArrayList<>();
            ArrayList<String> listText2 = new ArrayList<>();
            ArrayList<Definition> definitions2 = new ArrayList<>();
            ArrayList<String> listSynonyms2 = new ArrayList<>();
            listSingular2.add("mănânc");
            listSingular2.add("mănânci");
            listSingular2.add("mănâncă");
            listPlural2.add("mâncăm");
            listPlural2.add("mâncați");
            listPlural2.add("mănâncă");
            listText2.add("A mesteca un aliment în gură și a-l înghiți; a folosi în alimentație, a consuma");
            listText2.add("A se hrăni, a se alimenta");
            listText2.add("A lua, a-și însuși (pe nedrept) un bun material; a cheltui, a risipi");
            definitions2.add(new Definition("Dicționarul explicativ al limbii române ediția a II-a", "definitions",
                    2009, listText2));
            listSynonyms2.add("cheltui");
            listSynonyms2.add("consuma");
            listSynonyms2.add("irosi");
            listSynonyms2.add("hali");
            listSynonyms2.add("hrăni");
            definitions2.add(new Definition("Dictionar de sinonime", "synonyms", 2002, listSynonyms2));
            fileWriter.write(" " + methods.addWord(new Word("mânca", "eat", "verb", listSingular2,
                    listPlural2, definitions2), "ro") + "\n");
            fileWriter.write("\tTest 3:");
            ArrayList<String> listSingular3 = new ArrayList<>();
            ArrayList<String> listPlural3 = new ArrayList<>();
            ArrayList<String> listText3 = new ArrayList<>();
            ArrayList<Definition> definitions3 = new ArrayList<>();
            ArrayList<String> listSynonyms3 = new ArrayList<>();
            listSingular3.add("câine");
            listPlural3.add("câini");
            listText3.add("Animal mamifer carnivor, domesticit, folosit pentru pază, vânătoare etc.");
            definitions3.add(new Definition("Dicționarul explicativ al limbii române ediția a II-a", "definitions",
                    2009, listText3));
            listSynonyms3.add("lătrător");
            listSynonyms3.add("dulău");
            definitions3.add(new Definition("Dictionar de sinonime", "synonyms", 1998, listSynonyms3));
            fileWriter.write(" " + methods.addWord(new Word("câine", "dog", "noun", listSingular3,
                    listPlural3, definitions3), "ro") + "\n");

            fileWriter.write("\nTeste pentru boolean removeWord(String word, String language):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.removeWord("monedă", "ro") + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.removeWord("câine", "ro") + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.removeWord("hartie", "ro") + "\n");

            fileWriter.write("\nTeste pentru boolean addDefinitionForWord(String word, String language, Definition definition):\n");
            fileWriter.write("\tTest 1:");
            ArrayList<String> listSynonyms4 = new ArrayList<>();
            listSynonyms4.add("a se deplasa");
            listSynonyms4.add("a umbla");
            listSynonyms4.add("a se mișca");
            fileWriter.write(" " + methods.addDefinitionForWord("merge", "ro", new Definition(
                    "Dictionar de sinonime", "synonyms", 2002, listSynonyms4)) + "\n");
            fileWriter.write("\tTest 2:");
            ArrayList<String> listText4 = new ArrayList<>();
            listText4.add("A mesteca în gură, înghițind (pentru a-și menține existența fizică)");
            fileWriter.write(" " + methods.addDefinitionForWord("mânca", "ro", new Definition(
                    "Noul dicționar explicativ al limbii române", "definitions", 2002, listText4)) + "\n");
            fileWriter.write("\tTest 3:");
            ArrayList<String> listSynonyms5 = new ArrayList<>();
            listSynonyms5.add("a se deplasa");
            listSynonyms5.add("a umbla");
            listSynonyms5.add("a se mișca");
            fileWriter.write(" " + methods.addDefinitionForWord("merge", "ro", new Definition(
                    "Dictionar de sinonime", "synonyms", 2002, listSynonyms5)) + "\n");

            fileWriter.write("\nTeste pentru boolean removeDefinition(String word, String language, String dictionary):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.removeDefinition("mânca","ro", "Noul dicționar " +
                    "explicativ al limbii române") + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.removeDefinition("merge","ro", "Dicționarul " +
                    "explicativ al limbii române (ediția a II-a revăzută și adăugită)") + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.removeDefinition("monedă","ro", "Noul dicționar" +
                    " explicativ al limbii române") + "\n");

            fileWriter.write("\nTeste pentru String translateWord(String word, String fromLanguage, String toLanguage):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.translateWord("chats", "fr", "ro") + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.translateWord("mănânci", "ro", "fr") + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.translateWord("merge", "ro", "fr") + "\n");

            fileWriter.write("\nTeste pentru String translateSentence(String sentence, String fromLanguage, String toLanguage):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.translateSentence("chats mangent", "fr", "ro") + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.translateSentence("mănânci pisică", "ro", "fr") + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.translateSentence("pisici merg", "ro", "fr") + "\n");

            fileWriter.write("\nTeste pentru ArrayList<String> translateSentences(String sentence, String fromLanguage, String toLanguage):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.translateSentences("chat manger", "fr", "ro") + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.translateSentences("mânca pisică", "ro", "fr") + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.translateSentences("merge pisică", "ro", "fr") + "\n");

            fileWriter.write("\nTeste pentru ArrayList<Definition> getDefinitionsForWord(String word, String language):\n");
            fileWriter.write("\tTest 1:");
            fileWriter.write(" " + methods.getDefinitionsForWord("pisică", "ro").toString() + "\n");
            fileWriter.write("\tTest 2:");
            fileWriter.write(" " + methods.getDefinitionsForWord("merge", "ro").toString() + "\n");
            fileWriter.write("\tTest 3:");
            fileWriter.write(" " + methods.getDefinitionsForWord("câine", "ro").toString() + "\n");

            fileWriter.write("\nTeste pentru ArrayList<Definition> getDefinitionsForWord(String word, String language):\n");
            fileWriter.write("\tTest 1: output_ro.json\n");
            methods.exportDictionary("ro");
            fileWriter.write("\tTest 2: output_fr.json\n");
            methods.exportDictionary("fr");
            fileWriter.write("\tTest 3: output_esp.json");
            methods.exportDictionary("esp");

            fileWriter.close();
        } catch (IOException e) {
            System.out.println("Eroare: fisierul cu output-urile testelor nu a putut fi creat");
            e.printStackTrace();
        }
    }
}
