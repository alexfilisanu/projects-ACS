import java.util.Arrays;

public class Book implements Buyer {
    private String title;
    private String[] authors;
    private int year;
    private String publisher;
    private int recommendedAge;
    private float price;

    public Book(String title, int year, String publisher, int recommendedAge, float price) {
        this.title = title;
        this.year = year;
        this.publisher = publisher;
        this.recommendedAge = recommendedAge;
        this.price = price;
        this.authors = new String[2];
    }

    public String getTitle() { return title; }
    public String[] getAuthors() { return authors; }
    public int getYear() { return year; }
    public String getPublisher() { return publisher; }
    public int getRecommendedAge() { return recommendedAge; }
    public float getPrice() { return price; }
    public void setTitle(String title) { this.title = title; }
    public void setYear(int year) { this.year = year; }
    public void setPublisher(String publisher) { this.publisher = publisher; }
    public void setRecommendedAge(int recommendedAge) { this.recommendedAge = recommendedAge; }
    public void setPrice(float price) { this.price = price; }

    static int nr = 0;
    public void addAuthor(String name) {
        String[] authors = getAuthors();
        int len = authors.length;
        if (nr < len) {
            authors[nr++] = name;
        } else {
            String[] newAuth = new String[len * 2];
            System.arraycopy(authors, 0 , newAuth, 0, len);
        }
    }

    @Override
    public String toString() {
        return "Book{" +
                "title='" + title + '\'' +
                ", authors=" + Arrays.toString(authors) +
                ", year=" + year +
                ", publisher='" + publisher + '\'' +
                ", recommendedAge=" + recommendedAge +
                ", price=" + price +
                '}';
    }

    public float buyBook(User user) {
        if (user instanceof Student) {
            return (float) (0.95 * this.price);
        } else if (user instanceof Teacher) {
            return (float) (0.9 * this.price);
        } else if (user instanceof Librarian) {
            return (float) (0.8 * this.price);
        }
        return 0.0f;
    }




}
