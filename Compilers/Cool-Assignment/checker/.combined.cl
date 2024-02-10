(*
   The class A2I provides integer-to-string and string-to-integer
conversion routines. To use these routines, either inherit them
in the class where needed, have a dummy variable bound to
something of type A2I, or simpl write (new A2I).method(argument).
*)

(*
    c2i Converts a 1-character string to an integer. Aborts
if the string is not "0" through "9"
*)
class A2I {
    c2i(char : String) : Int {
        if char = "0" then 0 else
        if char = "1" then 1 else
        if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
    };
(*
   i2c is the inverse of c2i.
*)
    i2c(i : Int) : String {
        if i = 0 then "0" else
        if i = 1 then "1" else
        if i = 2 then "2" else
        if i = 3 then "3" else
        if i = 4 then "4" else
        if i = 5 then "5" else
        if i = 6 then "6" else
        if i = 7 then "7" else
        if i = 8 then "8" else
        if i = 9 then "9" else
        { abort(); ""; }  -- the "" is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
    };
(*
   a2i converts an ASCII string into an integer. The empty string
is converted to 0.  Signed and unsigned strings are handled. The
method aborts if the string does not represent an integer. Very
long strings of digits produce strange answers because of arithmetic 
overflow.
*)
    a2i(s : String) : Int {
        if s.length() = 0 then 0 else
            if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
                if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
                    a2i_aux(s)
        fi fi fi
    };

(*
    a2i_aux converts the usigned portion of the string. As a programming
example, this method is written iteratively.
*)
    a2i_aux(s : String) : Int {
        (let int : Int <- 0 in  
            {    
                (let j : Int <- s.length() in
                    (let i : Int <- 0 in
                        while i < j loop
                        {
                            int <- int * 10 + c2i(s.substr(i,1));
                            i <- i + 1;
                        }
                        pool
                    )
                );
                int;
            }
        )
    };
(*
    i2a converts an integer to a string. Positive and negative 
numbers are handled correctly.  
*)
    i2a(i : Int) : String {
        if i = 0 then "0" else 
            if 0 < i then i2a_aux(i) else
                "-".concat(i2a_aux(i * ~1)) 
        fi fi
    };
    
(*
    i2a_aux is an example using recursion.
*)      
    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
            (let next : Int <- i / 10 in
                i2a_aux(next).concat(i2c(i - next * 10))
            )
        fi
    };
(*
    b2a converts a boolean to a string.
*)
    b2a(b : Bool) : String {
        if b then "true" else "false" fi
    };
(*
    a2b converts a string to a boolean.
*)
    a2b(s : String) : Bool {
        if s = "true" then true else
            if s = "false" then false else
                { abort(); false; }
            fi
        fi
    };
};
class List inherits IO {
    head : Object; 
    tail : List;

    init(hd : Object, tl : List) : SELF_TYPE {
        {
            head <- hd;
            tail <- tl;
            self;
        }
    };

    isEmpty() : Bool { isvoid head };

    head() : Object { if self.isEmpty() then { abort(); new Object; } else head fi };

    tail() : List { if self.isEmpty() then { abort(); new List; } else tail fi };

    add(o : Object) : List { new List.init(o, self) };

    toString() : String {
        if self.isEmpty() then ""
        else let str : String <- 
                case self.head() of
                    p : Product => p.toString();
                    r : Rank => r.toString();
                    l : List => l.toString();
                    s : String => "String(".concat(s).concat(")");
                    n : Int => "Int(".concat(new A2I.i2a(n)).concat(")");
                    b: Bool => "Bool(".concat(new A2I.b2a(b)).concat(")");
                    i : IO => "IO()";
                esac 
            in 
                if self.tail().isEmpty() then str else str.concat(", ").concat(self.tail().toString()) fi
        fi
    };

    merge(other : List) : List { 
        if self.isEmpty() then other else new List.init(head, tail.merge(other)) fi
    };

    reverse() : List {
        if self.isEmpty() then self else tail.reverse().merge(new List.add(head)) fi
    };

    filterBy(f : Filter) : List {
        if self.isEmpty() then self else
            case head of
                l : List => 
                    if f.filter(l.head()) then new List.init(l.head(), l.tail().filterBy(f)) else l.tail().filterBy(f) fi;
                o : Object =>
                    if f.filter(o) then new List.init(o, tail.filterBy(f)) else tail.filterBy(f) fi;
            esac
        fi
    };

    sortBy(c : Comparator) : List {
        if self.isEmpty() then self
        else
            case head of
                l : List => {
                    if l.tail().isEmpty() then new List.insertSorted(l.head(), c)
                    else 
                        let sortedTail : List <- l.tail().sortBy(c)
                        in 
                            sortedTail.insertSorted(l.head(), c)
                    fi;
                };
                o : Object => {
                    if tail.isEmpty() then new List.insertSorted(o, c)
                    else
                        let sortedTail : List <- tail.sortBy(c)
                        in 
                            sortedTail.insertSorted(o, c)
                    fi;
                };
                esac
        fi
    };

    insertSorted(element : Object, c : Comparator) : List {
        if self.isEmpty() then new List.init(element, self)
        else if c.compareTo(head, element) < 0 then new List.init(head, tail.insertSorted(element, c))
        else new List.init(element, self)
        fi fi
    };

    getIndex(index : Int) : Object {
        if index = 0 then head else tail.getIndex(index - 1) fi
    };

    removeIndex(index : Int) : List {
        if index = 0 then tail else new List.init(head, tail.removeIndex(index - 1)) fi
    };

    addAtIndex(index : Int, o : Object) : List {
        if index = 0 then new List.init(o, self) else new List.init(head, tail.addAtIndex(index - 1, o)) fi
    };
};
class Main inherits IO {
    lists : List <- new List;
    looping : Bool <- true;
    somestr : String;
    stringTokenizer : StringTokenizer <- new StringTokenizer;
    a2i : A2I <- new A2I;

    help() : IO {
        out_string("Hi, these are all available commands: help, load, print, merge, filterBy, sortBy.\n")
    };

    load() : Object {
        let auxList : List <- new List,
            loopingLoad : Bool <- true
        in
        {
            while loopingLoad loop {
                somestr <- in_string();
                stringTokenizer <- stringTokenizer.init(somestr, " ");
                while stringTokenizer.hasMoreTokens() loop
                    let token : String <- stringTokenizer.nextToken(),
                        object : Rank
                    in 
                        if token = "Soda" 
                            then auxList <- auxList.add(new Soda.init(stringTokenizer.nextToken(), stringTokenizer.nextToken(), a2i.a2i(stringTokenizer.nextToken())))
                        else if token = "Coffee" 
                            then auxList <- auxList.add(new Coffee.init(stringTokenizer.nextToken(), stringTokenizer.nextToken(), a2i.a2i(stringTokenizer.nextToken())))
                        else if token = "Laptop" 
                            then auxList <- auxList.add(new Laptop.init(stringTokenizer.nextToken(), stringTokenizer.nextToken(), a2i.a2i(stringTokenizer.nextToken())))
                        else if token = "Router" 
                            then auxList <- auxList.add(new Router.init(stringTokenizer.nextToken(), stringTokenizer.nextToken(), a2i.a2i(stringTokenizer.nextToken())))
                        else if token = "Private" 
                            then auxList <- auxList.add(new Private.init(stringTokenizer.nextToken()))
                        else if token = "Corporal" 
                            then auxList <- auxList.add(new Corporal.init(stringTokenizer.nextToken()))
                        else if token = "Sergent" 
                            then auxList <- auxList.add(new Sergent.init(stringTokenizer.nextToken()))
                        else if token = "Officer" 
                            then auxList <- auxList.add(new Officer.init(stringTokenizer.nextToken()))
                        else if token = "String"
                            then auxList <- auxList.add(stringTokenizer.nextToken())
                        else if token = "Int"
                            then auxList <- auxList.add(a2i.a2i(stringTokenizer.nextToken()))
                        else if token = "Bool"
                            then auxList <- auxList.add(a2i.a2b(stringTokenizer.nextToken()))
                        else if token = "IO"
                            then auxList <- auxList.add(new IO)
                        else loopingLoad <- false
                        fi fi fi fi fi fi fi fi fi fi fi fi
                pool;
            } pool;
            lists <- lists.add(auxList.reverse());
        }
    };

    print(index : String) : Object {
        if index = "" then
            let indexInt : Int <- 1,
                list : List <- lists.reverse(),
                auxList : List
            in 
                while not list.isEmpty() loop {
                    auxList <- new List.add(list.head());
                    out_string((new A2I.i2c(indexInt)).concat(": [ ").concat(auxList.toString()).concat(" ]\n"));
                    indexInt <- indexInt + 1;
                    list <- list.tail();
                }
                pool
        else 
            let indexInt : Int <- a2i.a2i(index) - 1,
                auxList : List <- new List.add(lists.reverse().getIndex(indexInt))
            in
                out_string("[ ".concat(auxList.toString()).concat(" ]\n"))
        fi
    };

    merge(index1 : String, index2 : String) : Object {
        let indexInt1 : Int <- a2i.a2i(index1) - 1,
            indexInt2 : Int <- a2i.a2i(index2) - 1,
            list : List <- lists.reverse(),
            auxList1 : List <- new List.add(list.getIndex(indexInt1)),
            auxList2 : List <- new List.add(list.getIndex(indexInt2)),
            auxList3 : List <- auxList1.merge(auxList2)
        in {
            list <- list.removeIndex(indexInt2);
            list <- list.removeIndex(indexInt1);
            lists <- list.reverse().add(auxList3);
        }
    };

    filterBy(index : String, filterType : String) : Object {
        let indexInt : Int <- a2i.a2i(index) - 1,
            filter : Filter,
            list : List <- lists.reverse(),
            auxList : List <- new List.add(list.getIndex(indexInt))
        in {
            if filterType = "ProductFilter" then filter <- new ProductFilter
            else if filterType = "RankFilter" then filter <- new RankFilter
            else if filterType = "SamePriceFilter" then filter <- new SamePriceFilter
            else filter <- new Filter
            fi fi fi;

            auxList <- auxList.filterBy(filter);
            list <- list.removeIndex(indexInt);
            lists <- list.reverse().add(auxList);
        }
    };

    sortBy(index : String, comparatorType : String, sortType : String) : List {
        let indexInt : Int <- a2i.a2i(index) - 1,
            comparator : Comparator,
            list : List <- lists.reverse(),
            auxList : List <- new List.add(list.getIndex(indexInt))
        in {
            if comparatorType = "PriceComparator" then comparator <- new PriceComparator
            else if comparatorType = "RankComparator" then comparator <- new RankComparator
            else if comparatorType = "AlphabeticComparator" then comparator <- new AlphabeticComparator
            else comparator <- new Comparator
            fi fi fi;

            if sortType = "ascendent" then auxList <- auxList.sortBy(comparator)
            else if sortType = "descendent" then auxList <- auxList.sortBy(comparator).reverse()
            else auxList <- auxList
            fi fi;

            list <- list.removeIndex(indexInt);
            lists <- list.reverse().add(auxList);
        }
    };
    
    main() : Object {
        {
            load();
            while looping loop {
                somestr <- in_string();
                stringTokenizer <- stringTokenizer.init(somestr, " ");
                while stringTokenizer.hasMoreTokens() loop
                    let token : String <- stringTokenizer.nextToken()
                    in
                        if token = "help" then help() 
                        else if token = "load" then load()
                        else if token = "print" then print(stringTokenizer.nextToken())
                        else if token = "merge" then merge(stringTokenizer.nextToken(), stringTokenizer.nextToken())
                        else if token = "filterBy" then filterBy(stringTokenizer.nextToken(), stringTokenizer.nextToken())
                        else if token = "sortBy" then sortBy(stringTokenizer.nextToken(), stringTokenizer.nextToken(), stringTokenizer.nextToken())
                        else looping <- false 
                        fi fi fi fi fi fi
                pool;
            } pool;
        }
    };
};
class StringTokenizer inherits IO {
    str : String;
    delimiter : String;
    currentPos : Int;
    length : Int;
    
    init(inputStr: String, inputDelimeter: String) : StringTokenizer {
        {
            str <- inputStr;
            delimiter <- inputDelimeter;
            currentPos <- 0;
            length <- str.length();
            self;
        }
    };
    
    hasMoreTokens(): Bool {
        currentPos < length
    };
    
    nextToken(): String {
        let token : String <- "",
        auxPos : Int <- currentPos
        in 
        {
            while auxPos < length loop
                if str.substr(currentPos, 1) = delimiter then 
                    {
                        currentPos <- currentPos + 1;
                        auxPos <- length;
                    }
                else
                    {
                        token <- token.concat(str.substr(currentPos, 1));
                        currentPos <- currentPos + 1;
                        auxPos <- currentPos;
                    }
                fi
            pool;
            token;
        }
    };
};
(*******************************
 *** Classes Product-related ***
 *******************************)
class Product {
    name : String;
    model : String;
    price : Int;

    init(n : String, m: String, p : Int) : SELF_TYPE {
        {
            name <- n;
            model <- m;
            price <- p;
            self;
        }
    };

    getprice() : Int { price * 119 / 100 };

    toString() : String {
        type_name().concat("(").concat(name).concat(",").concat(model).concat(")")
    };
};

class Edible inherits Product {
    -- VAT tax is lower for foods
    getprice() : Int { price * 109 / 100 };
};

class Soda inherits Edible {
    -- sugar tax is 20 bani
    getprice() : Int { price * 109 / 100 + 20};
};

class Coffee inherits Edible {
    -- this is technically poison for ants
    getprice() : Int { price * 119 / 100};
};

class Laptop inherits Product {
    -- operating system cost included
    getprice() : Int { price * 119 / 100 + 499};
};

class Router inherits Product {};

(****************************
 *** Classes Rank-related ***
 ****************************)
class Rank {
    name : String;

    init(n : String) : SELF_TYPE {
        {
            name <- n;
            self;
        }
    };

    getrank() : Int { 0 };

    toString() : String {
        type_name().concat("(").concat(name).concat(")")
    };
};

class Private inherits Rank {
    getrank() : Int { 1 };
};

class Corporal inherits Private {
    getrank() : Int { 2 };
};

class Sergent inherits Corporal {
    getrank() : Int { 3 };
};

class Officer inherits Sergent {
    getrank() : Int { 4 };
};
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
