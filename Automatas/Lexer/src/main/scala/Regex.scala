import scala.collection.mutable

object Regex {
  // functie ce verifica daca trebuie inserata operatia CONCAT in lista
  // -> daca lista primita nu e vida si primul element este a-z / A-Z / 0-9 / ( / \', atunci
  // trebuie inserata operatia CONCAT
  def verifyConcat(s: List[Char]): Boolean = {
    s.nonEmpty && ((s.head <= 'z' && s.head >= 'a') || s.head == '(' || s.head == '[' ||
      (s.head <= 'Z' && s.head >= 'A') || (s.head <= '9' && s.head >= '0') || s.head == '\'')
  }

  // functie ce primeste un input de forma [a-z] / [0-9] etc. si returneaza o List[Either[Char, Char]]
  // -> prelucreaza input ul in (a | b | ... | z) si verifica daca urmatorul element din lista trebuie
  // sa fie CONCAT sau nu
  def range(s: List[Char]): List[Either[Char, Char]] = {
    // generez lista avand extremitatile primite in input pe pozitiile 0 si 2
    val rangeList = (s(0) to s(2)).toList
    // inserez operatia UNION intre toate elementele
    val resultList: List[Either[Char, Char]] = rangeList.foldLeft(List[Either[Char, Char]]())((acc, x)
      => acc++(Left(x)::List(Right('|')))).init
    // verific daca dupa terminare operatiei range trebuie inserat CONCAT, sau nu
    // operatia range este efectuata intre ( )
    if (verifyConcat(s.drop(3)))
      Right('(')::resultList++List(Right(')'), Right('C')) else Right('(')::resultList++List(Right(')'))
  }

  /*
  This function should:
  -> Classify input as either character(or string) or operator
  -> Convert special inputs like [0-9] to their correct form
  -> Convert escaped characters
  */

  // -> opeartiile(inclusiv parantezele) le salevez de forma Right(simbol), iar pentru CONCAT,
  // care nu are un simbol propriu am folosit 'C', eps il salvez ca Right('E'), iar restul
  // atomilor ca Left(c); un atom escapat este salvat sub forma Left('), Left(c), Left(')
  // -> inainte de a adauga un nou element in lista verific daca trebuie inserata operatia CONCAT
  def preprocess(s: List[Char]): List[Either[Char, Char]] = {
    s match {
      case Nil => Nil
      case head::tail =>
        head match {
          // in cazul in care primul caracter este e verific daca urmatoarele sunt p si s, caz in care
          // trebuie tratat caracterul eps(am folosit codificarea Right('E') pentru acesta)
          case 'e' => if (tail.nonEmpty && tail.head == 'p' && tail.tail.nonEmpty && tail.tail.head == 's')
            Right('E')::preprocess(tail.drop(2)) else
              if (verifyConcat(tail)) Left(head)::Right('C')::preprocess(tail) else Left(head)::preprocess(tail)
          // in cazul in care am intalnit carcterul [ este clar ca urmeaza o operatie de tipul range
          // pe care o efectuez in functia range, deja implementata
          case '[' => range(tail)++preprocess(tail.drop(4))
          // in cazul in care am intalnit caracterul \' este clar ca urmeaza un caracter escapat
          case '\'' => if (verifyConcat(tail.drop(2)))
            Left(head)::List(Left(tail.head), Left(tail.tail.head), Right('C'))++preprocess(tail.drop(2)) else
            Left(head)::List(Left(tail.head), Left(tail.tail.head))++preprocess(tail.drop(2))
          case '+' => if (verifyConcat(tail))
            Right(head)::List(Right('C'))++preprocess(tail) else Right(head)::preprocess(tail)
          case '?' => if (verifyConcat(tail))
            Right(head)::List(Right('C'))++preprocess(tail) else Right(head)::preprocess(tail)
          case '*' => if (verifyConcat(tail))
            Right(head)::List(Right('C'))++preprocess(tail) else Right(head)::preprocess(tail)
          case '|' => Right(head)::preprocess(tail)
          case '(' => Right(head)::preprocess(tail)
          case ')' => if (verifyConcat(tail))
            Right(head)::List(Right('C'))++preprocess(tail) else Right(head)::preprocess(tail)
          case _ => if (verifyConcat(tail))
            Left(head)::List(Right('C'))++preprocess(tail) else Left(head)::preprocess(tail)
        }
    }
  }

  // functie ce calculeaza precedenta operatiilor: + = ? = * > C > |
  def isLowerPrecedence(c1: Char, c2: Char): Boolean = {
    c1 match {
      case '+' => false
      case '?' => false
      case '*' => false
      case 'C' =>
        c2 match {
          case ')' => false
          case '|' => false
          case _ => true
        }
      case '|' =>
        c2 match {
          case ')' => false
          case _ => true
        }
    }
  }

  // functie ce transforma o operatie/eps primita sub forma unui caracter
  // in string ul corespunzator
  def cToString(c: Char): String = {
    c match {
      case 'E' => " eps"
      case 'C' => " CONCAT"
      case '|' => " UNION"
      case '*' => " STAR"
      case '+' => " PLUS"
      case '?' => " MAYBE"
    }
  }

  // functie ce scoate toate elementele din stiva si creeaza un string din toate
  def stackPopAll(stack: mutable.Stack[Char]): String = {
    if (stack.isEmpty) "" else stackPopAll(stack.tail)+cToString(stack.pop())
  }

  // functie ce scoate toate elementele din stiva pana la intalnirea ) si creeaza un string din toate
  def stackPopAllUntil(stack: mutable.Stack[Char]): String = {
    if (stack.head == ')') "" else {
        val str = cToString(stack.pop())
        stackPopAllUntil(stack)+str
      }
  }

  // functie ce scoate toate elementele din stiva care respecta functia isLowerPrecedence
  // si creeaza un string din toate
  def popAllLowerPrecedence(c: Char, stack: mutable.Stack[Char]): String = {
    if (stack.nonEmpty && isLowerPrecedence(c, stack.head)) {
      val cPop = cToString(stack.pop())
      popAllLowerPrecedence(c, stack)+cPop
    } else ""
  }

  // functie ce genereaza string ul prenex
  // -> cu ajutorul unei stive realizez transformarea listei Either[Char, Char] in string
  // -> parcurg lista de la final: daca elementul este de tip Right(operatie) incerc sa l adaug
  // pe stiva(aceasta adaugare se realizeaza doar daca operatia are precedenta mai mica decat
  // top ul stivei); daca elementul este ')' il adaug pur si simplu pe stiva, iar daca este '('
  // scot toate elementele din stiva pana la aparitia '('(inclusiv acesta);
  // -> daca elemtul este de forma Right('E') il afisez la final, procedez la fel si pentru
  // elemente de tipul Left; daca elementele de tipul Left sunt escapate le afisez tot escapate
  def generatePrenex(list: List[Either[Char, Char]], stack: mutable.Stack[Char]): String = {
    list match {
      case Nil => if (stack.isEmpty) "" else stackPopAll(stack)
      case init :+ last =>
        last match {
          case Right(r) =>
            r match {
              case 'E' => generatePrenex(init, stack)+cToString(r)
              case ')' => generatePrenex(init, stack.push(r))
              case '(' =>
                val str = stackPopAllUntil(stack)
                stack.pop()
                generatePrenex(init, stack)+str
              case _ =>
                val str = popAllLowerPrecedence(r, stack)
                generatePrenex(init, stack.push(r))+str
            }
          case Left(l) =>
            l match {
              case '\'' => generatePrenex(init.init.init, stack)+" "+'\''+init.last.merge+'\''
              case _ => generatePrenex(init, stack)+" "+l
            }
        }
    }
  }

  // This function should construct a prenex expression out of a normal one.
  def toPrenex(str: String): String = {
    Regex.generatePrenex(Regex.preprocess(str.replace("\\n", "\n")
      .replace("\\t", "\t").toList), mutable.Stack[Char]()).tail
  }
}