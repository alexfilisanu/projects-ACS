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
