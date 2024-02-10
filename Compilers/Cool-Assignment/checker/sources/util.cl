class Comparator {
    compareTo(o1 : Object, o2 : Object) : Int { 0 };
};

class PriceComparator inherits Comparator {
    compareTo(o1 : Object, o2 : Object) : Int {
        case o1 of
            p1 : Product => case o2 of
                p2 : Product => p1.getprice() - p2.getprice();
                o2 : Object => { abort(); 0; };
            esac;
            o1 : Object => { abort(); 0; };
        esac
    };
};

class RankComparator inherits Comparator {
    compareTo(o1 : Object, o2 : Object) : Int {
        case o1 of
            r1 : Rank => case o2 of
                r2 : Rank => r1.getrank() - r2.getrank();
                o2 : Object => { abort(); 0; };
            esac;
            o1 : Object => { abort(); 0; };
        esac
    };
};

class AlphabeticComparator inherits Comparator {
    compareTo(o1 : Object, o2 : Object) : Int {
        case o1 of
            s1 : String => case o2 of
                s2 : String => strcmp(s1, s2);
                o2 : Object => { abort(); 0; };
            esac;
            o1 : Object => { abort(); 0; };
        esac
    };

    strcmp(str1 : String, str2 : String) : Int {
        let result : Int <- 0,
            i : Int,
            len1 : Int <- str1.length(),
            len2 : Int <- str2.length(),
            min_len : Int <- if len1 < len2 then len1 else len2 fi
        in
        {
            let char1 : String,
                char2 : String
            in
                while i < min_len loop {
                    char1 <- str1.substr(i, 1);
                    char2 <- str2.substr(i, 1);

                    if char1 < char2 then { i <- min_len; result <- 0 - 1; }
                    else if char2 < char1 then { i <- min_len; result <- 1; }
                    else i <- i + 1 
                    fi fi;
                }
            pool;
            if result = 0 then 
                if len1 < len2 then result <- 0 - 1 else result <- 1 fi
            else result <- result
            fi;
            result;
        }
    };
};


class Filter {
    filter(o : Object) : Bool { true };
};

class ProductFilter inherits Filter {
    filter(o : Object) : Bool {
        case o of
            p : Product => true;
            o : Object => false;
        esac
    };
};

class RankFilter inherits Filter {
    filter(o : Object) : Bool {
        case o of
            p : Rank => true;
            o : Object => false;
        esac
    };
};

class SamePriceFilter inherits ProductFilter {
    filter(o : Object) : Bool {
        case o of
            p : Product => p.getprice() = p@Product.getprice();
            o : Object => false;
        esac
    };
};
