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
