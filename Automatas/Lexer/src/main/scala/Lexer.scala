case class Lexer (spec: String) {
  /*
    This is the main function of the lexer, it splits the given word into a list of lexems
    in the format (LEXEM, TOKEN)
  */

  // functie ce primeste lista de forma (token, regex) si returneaza (regex1)|(regex2)|...
  def createRegex(list: List[(String, String)]): String = {
    list match {
      case Nil => ""
      case ::(head, Nil) => "(".+(head._2).+(")")
      case ::(head, next) => "(".+(head._2).+(")|")+createRegex(next)
    }
  }

  // functie ce cauta in lista (token, regex) un regex si ii returneaza token ul, daca exista,
  // iar daca nu returneaza o eroare
  def getToken(regex: String, listPatterns: List[(String, String)]): String = {
    listPatterns match {
      case Nil => "Error: No Pattern"
      case ::(head, next) => if (Dfa.fromPrenex(Regex.toPrenex(head._2)).accepts(regex)) head._1 else getToken(regex, next)
    }
  }

  // transforma spec ul citit intr o lista de forma (token, regex)
  val list: List[(String, String)] = spec.split(";\n").map((x: String)
  => (x.split(": ")(0), x.split(": ")(1))).toList

  // dfa ul creat din regex1|regex2|...
  val dfa: Dfa[Int] = Dfa.fromPrenex(Regex.toPrenex(createRegex(list)))

  def lex(word: String): Either[String, List[(String, String)]] = {
    // imi definesc o functie auxiliara pe care sa o apelez recursiv, adaugandu i in plus parametrii:
    // - currentState - retine starea curenta din dfa
    // - lexeme - retine ultima lexema care poate fi afisata pana in momentul respectiv
    // - currentLexeme - retine lexema actuala(dar aceasta trebuie verificata -> daca ultimul caracter adaugat o
    // duce intr o stare finala lexeme preia valoarea acesteia, iar daca nu se modifica doar valoarea acesteia)
    // - remainingWord - retine word ul care era in momentul ultimei lexeme acceptate, fiind posibil sa ma
    // intorc ulterior in el
    // - indexNumber - retine indexul de pe randul pe care ma aflu
    // - lineNumber - retine randul pe care ma aflu
    def auxLex(word: String, currentState: Int, lexeme: String, currentLexeme: String,
               remainingWord: String, indexNumber: Int, lineNumber: Int): List[String] = {
      word.toList match {
        // daca word ul s a terminat si lexema nu exista -> s a ajuns la EOF si nu poate fi acceptata
        case Nil => if (getToken(lexeme, list) == "Error: No Pattern")
          List[String]("No viable alternative at character EOF, line " + lineNumber)
        // altfel, daca remainingWord este gol -> accept lexema; altfel -> afisez lexema si repelez functia
        else if (remainingWord.isEmpty) List[String](lexeme)
          else lexeme::auxLex(remainingWord, dfa.getInitialState, "", "", remainingWord, indexNumber, lineNumber)
        // daca word ul nu s a terminat
        case ::(head, next) =>
          // daca noul caracter duce intr un sinkState: lexema goala -> eroare, altfel afisez lexema si reapelez functia
          if (dfa.isSinkState(dfa.next(currentState, head))) {
            if (lexeme.isEmpty) List[String]("No viable alternative at character " + indexNumber + ", line " + lineNumber)
            else lexeme::auxLex(remainingWord, dfa.getInitialState, "", "", remainingWord,
              indexNumber + lexeme.length - currentLexeme.length, lineNumber)
          }
          // daca noul caracter duce intr o stare finala care poate fi si acceptata -> modific lexeme in currentLexeme si apelez
          // recursiv functia (daca noul caracter este '\n' incrementez numarul liniei in apelul recursiv si index = 0)
          else if (list.exists(x => Dfa.fromPrenex(Regex.toPrenex(x._2)).accepts(currentLexeme + head))
            && dfa.isFinal(dfa.next(currentState, head))) {
            if (head == '\n') auxLex(next.mkString, dfa.next(currentState, head),
              currentLexeme + head, currentLexeme + head, next.mkString, 0, lineNumber + 1)
            else auxLex(next.mkString, dfa.next(currentState, head),
              currentLexeme + head, currentLexeme + head, next.mkString, indexNumber + 1, lineNumber)
          // in orice alt caz apelez recursiv (aceleasi actiuni in cazul in care caracterul nou este '\n')
          } else {
            if (head == '\n') auxLex(next.mkString, dfa.next(currentState, head),
              lexeme, currentLexeme + head, remainingWord, 0, lineNumber + 1)
            else auxLex(next.mkString, dfa.next(currentState, head),
              lexeme, currentLexeme + head, remainingWord, indexNumber + 1, lineNumber)
          }
      }
    }

    // apelul initial al functiei recursive
    val lexemeList: List[String] = auxLex(word, dfa.getInitialState, "", "", "", 0, 0)

    // daca ultimul String adaugat este de forma Erorii -> Left(eroare); altfel -> Right(Lexem, Token)
    if (lexemeList.last.contains("No viable alternative at character ")) Left(lexemeList.last)
    else Right(lexemeList.map(x => (x, getToken(x, list))))
  }
}