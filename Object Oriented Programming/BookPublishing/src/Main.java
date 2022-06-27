import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Main {
    static List<PublishingRetailer> publishingRetailers = new ArrayList<>();
    static List<Language> languages = new ArrayList<>();
    public static void main(String[] args) {
        List<Book> books = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("init\\books.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                String name = parts[1];
                String subtitle = parts[2];
                String ISBN = parts[3];
                int pageCount = Integer.parseInt(parts[4]);
                String keywords = parts[5];
                int languageID = Integer.parseInt(parts[6]);
                Date createdOn = new SimpleDateFormat("dd.MM.yyyy HH:mm:ss").parse(parts[7]);
                books.add(new Book(ID, name, subtitle, ISBN, pageCount, keywords, languageID, createdOn));
            }
        } catch (IOException | ParseException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        try (BufferedReader br = new BufferedReader(new FileReader("init\\languages.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                languages.add(new Language(ID, parts[1], parts[2]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        List<Author> authors = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("init\\authors.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                authors.add(new Author(ID, parts[1], parts[2]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\books-authors.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDBook = Integer.parseInt(parts[0]);
                int IDAuthor = Integer.parseInt(parts[1]);
                for (Book book: books) {
                    if (book.getID() == IDBook) {
                        for (Author author: authors) {
                            if (author.getID() == IDAuthor) {
                                book.getAuthors().add(author);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        List<EditorialGroup> editorialGroups = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("init\\editorial-groups.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                editorialGroups.add(new EditorialGroup(ID, parts[1]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\editorial-groups-books.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDEditorialGroup = Integer.parseInt(parts[0]);
                int IDBook = Integer.parseInt(parts[1]);
                for (EditorialGroup editorialGroup: editorialGroups) {
                    if (editorialGroup.getID() == IDEditorialGroup) {
                        for (Book book : books) {
                            if (book.getID() == IDBook) {
                                editorialGroup.getBooks().add(book);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        List<PublishingBrand> publishingBrands = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-brands.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                publishingBrands.add(new PublishingBrand(ID, parts[1]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-brands-books.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDPublishingBrand = Integer.parseInt(parts[0]);
                int IDBook = Integer.parseInt(parts[1]);
                for (PublishingBrand publishingBrand: publishingBrands) {
                    if (publishingBrand.getID() == IDPublishingBrand) {
                        for (Book book : books) {
                            if (book.getID() == IDBook) {
                                publishingBrand.getBooks().add(book);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        List<Country> countries = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("init\\countries.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                countries.add(new Country(ID, parts[1]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-retailers.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int ID = Integer.parseInt(parts[0]);
                publishingRetailers.add(new PublishingRetailer(ID, parts[1]));
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-retailers-countries.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDPublishing = Integer.parseInt(parts[0]);
                int IDCountry = Integer.parseInt(parts[1]);
                for (PublishingRetailer publishingRetailer: publishingRetailers) {
                    if (publishingRetailer.getID() == IDPublishing) {
                        for (Country country : countries) {
                            if (country.getID() == IDCountry) {
                                publishingRetailer.getCountries().add(country);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-retailers-books.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDPublishingRetailer = Integer.parseInt(parts[0]);
                int IDBook = Integer.parseInt(parts[1]);
                for (PublishingRetailer publishingRetailer: publishingRetailers) {
                    if (publishingRetailer.getID() == IDPublishingRetailer) {
                        for (Book book : books) {
                            if (book.getID() == IDBook) {
                                publishingRetailer.getBooks().add(book);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-retailers-editorial-groups.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDPublishingRetailer = Integer.parseInt(parts[0]);
                int IDEditorial = Integer.parseInt(parts[1]);
                for (PublishingRetailer publishingRetailer: publishingRetailers) {
                    if (publishingRetailer.getID() == IDPublishingRetailer) {
                        for (EditorialGroup editorialGroup : editorialGroups) {
                            if (editorialGroup.getID() == IDEditorial) {
                                publishingRetailer.getBooks().add(editorialGroup);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }
        try (BufferedReader br = new BufferedReader(new FileReader("init\\publishing-retailers-publishing-brands.in"))) {
            String line = br.readLine();
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("###");
                int IDPublishingRetailer = Integer.parseInt(parts[0]);
                int IDPublishingBrand = Integer.parseInt(parts[1]);
                for (PublishingRetailer publishingRetailer: publishingRetailers) {
                    if (publishingRetailer.getID() == IDPublishingRetailer) {
                        for (PublishingBrand publishingBrand : publishingBrands) {
                            if (publishingBrand.getID() == IDPublishingBrand) {
                                publishingRetailer.getBooks().add(publishingBrand);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Eroare initializare");
            e.printStackTrace();
        }

        /** TESTARE */
        Test.testing();

    }
}
