import java.text.SimpleDateFormat;
import java.util.*;

public class Book implements IPublishingArtifact {
    private int ID;
    private String name;
    private String subtitle;
    private String ISBN;
    private int pageCount;
    private String keywords;
    private int languageID;
    private Date createdOn;
    private List<Author> authors;

    public Book(int ID, String name, String subtitle, String ISBN, int pageCount, String keywords, int languageID, Date createdOn) {
        this.ID = ID;
        this.name = name;
        this.subtitle = subtitle;
        this.ISBN = ISBN;
        this.pageCount = pageCount;
        this.keywords = keywords;
        this.languageID = languageID;
        this.createdOn = createdOn;
        this.authors = new ArrayList<>();
    }

    public Book(int ID) {
        this.ID = ID;
    }
    public int getID() {
        return ID;
    }
    public void setID(int ID) {
        this.ID = ID;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getSubtitle() {
        return subtitle;
    }
    public void setSubtitle(String subtitle) {
        this.subtitle = subtitle;
    }
    public String getISBN() {
        return ISBN;
    }
    public void setISBN(String ISBN) {
        this.ISBN = ISBN;
    }
    public int getPageCount() {
        return pageCount;
    }
    public void setPageCount(int pageCount) {
        this.pageCount = pageCount;
    }
    public String getKeywords() {
        return keywords;
    }
    public void setKeywords(String keywords) {
        this.keywords = keywords;
    }
    public int getLanguageID() {
        return languageID;
    }
    public void setLanguageID(int languageID) {
        this.languageID = languageID;
    }
    public Date getCreatedOn() { return createdOn; }
    public void setCreatedOn(Date createdOn) {
        this.createdOn = createdOn;
    }
    public List<Author> getAuthors() {
        return authors;
    }
    public void setAuthors(List<Author> authors) {
        this.authors = authors;
    }

    public String Publish() {
        return  "<xml>\n" +
                    "\t<title>" + name + "</title>\n" +
                    "\t<subtitle>" + subtitle + "</subtitle>\n" +
                    "\t<isbn>" + ISBN + "</isbn>\n" +
                    "\t<pageCount>" + pageCount + "</pageCount>\n" +
                    "\t<keywords>" + keywords + "</keywords>\n" +
                    "\t<languageID>" + languageID + "</languageID>\n" +
                    "\t<createdOn>" + new SimpleDateFormat("dd.MM.yyyy HH:mm:ss").format(createdOn)
                        + "</createdOn>\n" +
                    "\t<authors>" + authors + "</authors>\n" +
                "</xml>\n";
    }
}
