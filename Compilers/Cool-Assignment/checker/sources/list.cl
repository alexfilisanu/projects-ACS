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
