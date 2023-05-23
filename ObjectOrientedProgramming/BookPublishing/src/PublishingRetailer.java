import java.util.ArrayList;
import java.util.List;

public class PublishingRetailer {
    private int ID;
    private String name;
    private List<IPublishingArtifact> books;
    private List<Country> countries;

    public int getID() { return ID; }
    public void setID(int ID) { this.ID = ID; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public List<IPublishingArtifact> getBooks() { return books; }
    public void setBooks(List<IPublishingArtifact> books) { this.books = books; }
    public List<Country> getCountries() { return countries; }
    public void setCountries(List<Country> countries) { this.countries = countries; }

    public PublishingRetailer(int ID, String name) {
        this.ID = ID;
        this.name = name;
        this.books = new ArrayList<>();
        this.countries = new ArrayList<>();
    }
}
