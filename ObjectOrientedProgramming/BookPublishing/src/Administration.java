import java.util.ArrayList;
import java.util.List;

public class Administration extends Main{
    /** intoarce o lista de carti unice ale unui anumit PublishingRetailer;
     * odata ce am gasit obiectul cautat in lista publishingRetailers,
     * verific fiecare obiect din lista books al acestuia, iar daca cartea
     * nu este deja in lista ce va fi returnata, o adaug;
     */
    static List<Book> getBooksForPublishingRetailerID(int publishingRetailerID) {
        List<Book> booksList = new ArrayList<>();
        int ok;
        for (PublishingRetailer publishingRetailer : publishingRetailers) {
            if (publishingRetailer.getID() == publishingRetailerID) {
                for (IPublishingArtifact iPublishingArtifact: publishingRetailer.getBooks()) {
                    ok = 1;
                    if (iPublishingArtifact instanceof Book) {
                        for (Book book : booksList) {
                            if (book.getID() == ((Book) iPublishingArtifact).getID()) {
                                ok = 0;
                                break;
                            }
                        }
                        if (ok == 1) {
                            booksList.add((Book) iPublishingArtifact);
                        }
                    } else if (iPublishingArtifact instanceof EditorialGroup) {
                        for (Book bookEditorialGroup : ((EditorialGroup) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Book book : booksList) {
                                if (book.getID() == bookEditorialGroup.getID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                booksList.add(bookEditorialGroup);
                            }
                        }
                    } else if (iPublishingArtifact instanceof PublishingBrand) {
                        for (Book bookPublishingBrand : ((PublishingBrand) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Book book : booksList) {
                                if (book.getID() == bookPublishingBrand.getID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                booksList.add(bookPublishingBrand);
                            }
                        }
                    }
                }
                break;
            }
        }
        return booksList;
    }

    /** intoarce o lista de limbi ale unui anumit PublishingRetailer;
     * odata ce am gasit obiectul cautat in lista publishingRetailers,
     * verific fiecare obiect din lista books al acestuia si implicit
     * languageID-ul acesteia; daca limba nu este deja in lista de
     * languagesID, o adaug; la final am o lista de languageID unice
     * si caut corespondentul acestora in lista languages pentru a crea
     * o lista de limbi pe care sa o returnez;
     */
    static List<Language> getLanguagesForPublishingRetailerID(int publishingRetailerID) {
        List<Language> languagesList = new ArrayList<>();
        List<Integer> languagesIDList = new ArrayList<>();
        int ok;
        for (PublishingRetailer publishingRetailer: publishingRetailers) {
            if (publishingRetailer.getID() == publishingRetailerID) {
                for (IPublishingArtifact iPublishingArtifact : publishingRetailer.getBooks()) {
                    ok = 1;
                    if (iPublishingArtifact instanceof Book) {
                        for (Integer languageID : languagesIDList) {
                            if (languageID == ((Book) iPublishingArtifact).getLanguageID()) {
                                ok = 0;
                                break;
                            }
                        }
                        if (ok == 1) {
                            languagesIDList.add(((Book) iPublishingArtifact).getLanguageID());
                        }
                    } else if (iPublishingArtifact instanceof EditorialGroup) {
                        for (Book bookEditorialGroup : ((EditorialGroup) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Integer languageID : languagesIDList) {
                                if (languageID == bookEditorialGroup.getLanguageID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                languagesIDList.add(bookEditorialGroup.getLanguageID());
                            }
                        }
                    } else if (iPublishingArtifact instanceof PublishingBrand) {
                        for (Book bookPublishingBrand : ((PublishingBrand) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Integer languageID : languagesIDList) {
                                if (languageID == bookPublishingBrand.getLanguageID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                languagesIDList.add(bookPublishingBrand.getLanguageID());
                            }
                        }
                    }
                }
                break;
            }
        }

        for (Integer languageID : languagesIDList) {
            for (Language language : languages) {
                if (languageID == language.getID()) {
                    languagesList.add(language);
                    break;
                }
            }
        }

        return languagesList;
    }

    /** intoarce o listă de tari unice ale unei anumite carti;
     * parcurg fiecare carte din sistem (lista publishingRetailers), iar
     * odata ce cartea a fost gasita, preiau tara in care respectivul
     * PublishingRetailer publica si verific ca aceasta sa nu fie deja
     * adaugata in lista tarilor pe care o returnez la final, caz in care
     * o adaug;
     */
    static List<Country> getCountriesForBookID(int bookID) {
        List<Country> countryList = new ArrayList<>();
        int ok;
        for (PublishingRetailer publishingRetailer: publishingRetailers) {
            for (IPublishingArtifact iPublishingArtifact: publishingRetailer.getBooks()) {
                if (iPublishingArtifact instanceof Book) {
                    if (bookID == ((Book) iPublishingArtifact).getID()) {
                        for (Country countryPublishingRetailer : publishingRetailer.getCountries()) {
                            ok = 1;
                            for (Country country : countryList) {
                                if (country.getID() == countryPublishingRetailer.getID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                countryList.add(countryPublishingRetailer);
                            }
                        }
                    }
                } else if (iPublishingArtifact instanceof EditorialGroup) {
                    for (Book bookEditorialGroup : ((EditorialGroup) iPublishingArtifact).getBooks()) {
                        if (bookID == bookEditorialGroup.getID()) {
                            for (Country countryPublishingRetailer : publishingRetailer.getCountries()) {
                                ok = 1;
                                for (Country country : countryList) {
                                    if (country.getID() == countryPublishingRetailer.getID()) {
                                        ok = 0;
                                        break;
                                    }
                                }
                                if (ok == 1) {
                                    countryList.add(countryPublishingRetailer);
                                }
                            }
                        }
                    }
                } else if (iPublishingArtifact instanceof PublishingBrand) {
                    for (Book bookPublishingBrand : ((PublishingBrand) iPublishingArtifact).getBooks()) {
                        if (bookID == bookPublishingBrand.getID()) {
                            for (Country countryPublishingRetailer : publishingRetailer.getCountries()) {
                                ok = 1;
                                for (Country country : countryList) {
                                    if (country.getID() == countryPublishingRetailer.getID()) {
                                        ok = 0;
                                        break;
                                    }
                                }
                                if (ok == 1) {
                                    countryList.add(countryPublishingRetailer);
                                }
                            }
                        }
                    }
                }
            }
        }
        return countryList;
    }

    /**
     * intoarce o lista de carti comune intre retaileri;
     * pentru inceput adaug in lista booksList1 toate cartile primului
     * PublishingRetailer; pentru cel de al 2lea parcurg toate cartile,
     * iar daca una dintre acestea este deja in lista mentionata anterior,
     * o adaug in lista booksListCommon, pe care o si returnez la final;
     */
    static List<Book> getCommonBooksForRetailerIDs(int retailerID1, int retailerID2) {
        List<Book> booksList1 = new ArrayList<>();
        List<Book> booksListCommon = new ArrayList<>();
        for (PublishingRetailer publishingRetailer: publishingRetailers) {
            if ((publishingRetailer.getID() == retailerID1)) {
                for (IPublishingArtifact iPublishingArtifact : publishingRetailer.getBooks()) {
                    if (iPublishingArtifact instanceof Book) {
                        booksList1.add((Book) iPublishingArtifact);
                    } else if (iPublishingArtifact instanceof EditorialGroup) {
                        booksList1.addAll(((EditorialGroup) iPublishingArtifact).getBooks());
                    } else if (iPublishingArtifact instanceof PublishingBrand) {
                        booksList1.addAll(((PublishingBrand) iPublishingArtifact).getBooks());
                    }
                }
                break;
            }
        }

        for (PublishingRetailer publishingRetailer: publishingRetailers) {
            if ((publishingRetailer.getID() == retailerID2)) {
                for (IPublishingArtifact iPublishingArtifact : publishingRetailer.getBooks()) {
                    if (iPublishingArtifact instanceof Book) {
                        for (Book book : booksList1) {
                            if (book.getID() == ((Book) iPublishingArtifact).getID()) {
                                booksListCommon.add((Book) iPublishingArtifact);
                                break;
                            }
                        }
                    } else if (iPublishingArtifact instanceof EditorialGroup) {
                        for (Book bookEditorialGroup : ((EditorialGroup) iPublishingArtifact).getBooks()) {
                            for (Book book : booksList1) {
                                if (book.getID() == bookEditorialGroup.getID()) {
                                    booksListCommon.add(bookEditorialGroup);
                                    break;
                                }
                            }
                        }
                    } else if (iPublishingArtifact instanceof PublishingBrand) {
                        for (Book bookPublishingBrand : ((PublishingBrand) iPublishingArtifact).getBooks()) {
                            for (Book book : booksList1) {
                                if (book.getID() == bookPublishingBrand.getID()) {
                                    booksListCommon.add(bookPublishingBrand);
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            }
        }

        return booksListCommon;
    }

    /**
     * intoarce o lista de carti intre retaileri (uniunea celor doua liste);
     * parcurg pe rand fiecare carte din sisitem si o adaug in lista ce va fi
     * returnata in caz ca aceasta nu este deja acolo;
     */
    static List<Book> getAllBooksForRetailerIDs(int retailerID1, int retailerID2) {
        List<Book> booksList = new ArrayList<>();
        int ok;
        for (PublishingRetailer publishingRetailer: publishingRetailers) {
            if ((publishingRetailer.getID() == retailerID1) || (publishingRetailer.getID() == retailerID2)) {
                for (IPublishingArtifact iPublishingArtifact: publishingRetailer.getBooks()) {
                    ok = 1;
                    if (iPublishingArtifact instanceof Book) {
                        for (Book book : booksList) {
                            if (book.getID() == ((Book) iPublishingArtifact).getID()) {
                                ok = 0;
                                break;
                            }
                        }
                        if (ok == 1) {
                            booksList.add((Book) iPublishingArtifact);
                        }
                    } else if (iPublishingArtifact instanceof EditorialGroup) {
                        for (Book bookEditorialGroup : ((EditorialGroup) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Book book : booksList) {
                                if (book.getID() == bookEditorialGroup.getID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                booksList.add(bookEditorialGroup);
                            }
                        }
                    } else if (iPublishingArtifact instanceof PublishingBrand) {
                        for (Book bookPublishingBrand : ((PublishingBrand) iPublishingArtifact).getBooks()) {
                            ok = 1;
                            for (Book book : booksList) {
                                if (book.getID() == bookPublishingBrand.getID()) {
                                    ok = 0;
                                    break;
                                }
                            }
                            if (ok == 1) {
                                booksList.add(bookPublishingBrand);
                            }
                        }
                    }
                }
            }
        }
        return booksList;
    }

}
