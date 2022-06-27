import java.io.FileWriter;
import java.io.IOException;

public class Test{

    static void testing() {
        try {
            FileWriter fileWriter = new FileWriter("tests_output.txt");
            fileWriter.write("Test for getBooksForPublishingRetailerID():\n");
            fileWriter.write("\tTest 1:\n");
            for (Book book : Administration.getBooksForPublishingRetailerID(1)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 2:\n");
            for (Book book : Administration.getBooksForPublishingRetailerID(2)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 3:\n");
            for (Book book : Administration.getBooksForPublishingRetailerID(3)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 4:\n");
            for (Book book : Administration.getBooksForPublishingRetailerID(4)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 5:\n");
            for (Book book : Administration.getBooksForPublishingRetailerID(5)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }

            fileWriter.write("\nTest for getLanguagesForPublishingRetailerID():\n");
            fileWriter.write("\tTest 1:\n");
            for (Language language : Administration.getLanguagesForPublishingRetailerID(1)) {
                fileWriter.write("\t\t" + language.getName() + "\n");
            }
            fileWriter.write("\tTest 2:\n");
            for (Language language : Administration.getLanguagesForPublishingRetailerID(2)) {
                fileWriter.write("\t\t" + language.getName() + "\n");
            }
            fileWriter.write("\tTest 3:\n");
            for (Language language : Administration.getLanguagesForPublishingRetailerID(3)) {
                fileWriter.write("\t\t" + language.getName() + "\n");
            }
            fileWriter.write("\tTest 4:\n");
            for (Language language : Administration.getLanguagesForPublishingRetailerID(4)) {
                fileWriter.write("\t\t" + language.getName() + "\n");
            }
            fileWriter.write("\tTest 5:\n");
            for (Language language : Administration.getLanguagesForPublishingRetailerID(5)) {
                fileWriter.write("\t\t" + language.getName() + "\n");
            }

            fileWriter.write("\nTest for getCountriesForBookID():\n");
            fileWriter.write("\tTest 1:\n");
            for (Country country : Administration.getCountriesForBookID(357)) {
                fileWriter.write("\t\t" + country.getCountryCode() + "\n");
            }
            fileWriter.write("\tTest 2:\n");
            for (Country country : Administration.getCountriesForBookID(7259)) {
                fileWriter.write("\t\t" + country.getCountryCode() + "\n");
            }
            fileWriter.write("\tTest 3:\n");
            for (Country country : Administration.getCountriesForBookID(14312)) {
                fileWriter.write("\t\t" + country.getCountryCode() + "\n");
            }
            fileWriter.write("\tTest 4:\n");
            for (Country country : Administration.getCountriesForBookID(591)) {
                fileWriter.write("\t\t" + country.getCountryCode() + "\n");
            }
            fileWriter.write("\tTest 5:\n");
            for (Country country : Administration.getCountriesForBookID(357)) {
                fileWriter.write("\t\t" + country.getCountryCode() + "\n");
            }

            fileWriter.write("\nTest for getCommonBooksForRetailerIDs():\n");
            fileWriter.write("\tTest 1:\n");
            for (Book book : Administration.getCommonBooksForRetailerIDs(1, 5)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 2:\n");
            for (Book book : Administration.getCommonBooksForRetailerIDs(2, 7)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 3:\n");
            for (Book book : Administration.getCommonBooksForRetailerIDs(3, 10)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 4:\n");
            for (Book book : Administration.getCommonBooksForRetailerIDs(4, 15)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 5:\n");
            for (Book book : Administration.getCommonBooksForRetailerIDs(6, 17)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }

            fileWriter.write("\nTest for getAllBooksForRetailerIDs():\n");
            fileWriter.write("\tTest 1:\n");
            for (Book book : Administration.getAllBooksForRetailerIDs(1, 5)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 2:\n");
            for (Book book : Administration.getAllBooksForRetailerIDs(2, 7)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 3:\n");
            for (Book book : Administration.getAllBooksForRetailerIDs(3, 10)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 4:\n");
            for (Book book : Administration.getAllBooksForRetailerIDs(4, 15)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.write("\tTest 5:\n");
            for (Book book : Administration.getAllBooksForRetailerIDs(6, 17)) {
                fileWriter.write("\t\t" + book.getName() + " " + book.getSubtitle() + "\n");
            }
            fileWriter.close();

        } catch (IOException e) {
            System.out.println("Eroare: fisierul cu output-urile testelor nu a putut fi creat");
            e.printStackTrace();
        }
    }
}
