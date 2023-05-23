import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

public class PublishingBrand implements IPublishingArtifact{
    private int ID;
    private String name;
    private List<Book> books;

    public int getID() { return ID; }
    public void setID(int ID) { this.ID = ID; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public List<Book> getBooks() { return books; }
    public void setBooks(List<Book> books) { this.books = books; }

    public PublishingBrand(int ID, String name) {
        this.ID = ID;
        this.name = name;
        this.books = new ArrayList<>();
    }

    public String Publish() {
        StringBuilder str = new StringBuilder();
        str.append("<xml>\n");
        str.append("\t<publishingBrand\n");
        str.append("\t\t<ID>").append(ID).append("</ID>\n");
        str.append("\t\t<Name>").append(name).append("</Name>\n");
        str.append("\t</publishingBrand>\n");
        str.append("\t<books>\n");
        for(Book book: books) {
            str.append("\t\t<books>\n");
            str.append("\t\t\t<title>").append(book.getName()).append("</title>\n");
            str.append("\t\t\t<subtitle>").append(book.getSubtitle()).append("</subtitle>\n");
            str.append("\t\t\t<isbn>").append(book.getISBN()).append("</isbn>\n");
            str.append("\t\t\t<pageCount>").append(book.getPageCount()).append("</pageCount>\n");
            str.append("\t\t\t<keywords>").append(book.getKeywords()).append("</keywords>\n");
            str.append("\t\t\t<languageID>").append(book.getLanguageID()).append("</languageID>\n");
            str.append("\t\t\t<createdOn>").append(new SimpleDateFormat("dd.MM.yyyy HH:mm:ss").
                    format(book.getCreatedOn())).append("</createdOn>\n");
            str.append("\t\tbook\n");
        }
        str.append("\t</books>\n");
        str.append("</xml>\n");

        return str.toString();
    }
}
