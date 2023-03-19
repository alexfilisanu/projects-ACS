import scala.collection.mutable

// Nfa[A] este construit, ca in curs, din:
// - states: set de state uri[A]; toate state urile posibile
// - alphabet: Set[Char]
// - delta: map cu key A si value alt map cu key String si value un set de state uri[A]; acesta
// reprezinta tranzitiile de la o stare la alte stari pe baza unui caracter din alfabet sau "eps"
// - initialState: state[A]; state ul initial
// - finalStates: set de state uri[A]; toate state urile finale
class Nfa[A](states: Set[A], alphabet: Set[Char], delta: Map[A, Map[String, Set[A]]], initialState: A,
             finalStates: Set[A]) {
  // The following methods are only the methods directly called by the test suite. You can (and should) define more.
  // returneaza un now Nfa care are toate state urile moodificate conform functiei primite
  def map[B](f: A => B): Nfa[B] = new Nfa[B](states.map(f), alphabet, delta.map(x => (f(x._1),
    x._2.map(y => (y._1, y._2.map(f))))), f(initialState), finalStates.map(f))

  // returneaza toate state urile urmatoare unui state cu tranzitie pe c sau "eps"
  def next(state:A, c: Char): Set[A] = nextCTransition(state, c.toString).++(nextCTransition(state, "eps"))

  // returneaza toate state urile urmatoare unui state cu tranzitie pe c (apelata in next cu c si "eps")
  def nextCTransition(state: A, c: String): Set[A] = {
    // daca exista tranzitii pe c le returnez, altfel returnez un Set[A] gol
    if (delta.get(state).foldLeft(false)((acc, i) => acc || i.contains(c))) delta.get(state).get(c)
    else Set[A]()
  }

  // returneaza daca string ul primit ca parametru este acceptat
  def accepts(str: String): Boolean = {
    // functie auxiliara ce este apelata initial cu state = initialState,
    // iar apoi recursiv pana la terminarea stringului
    def auxAccepts(str: String, state: A): Boolean = {
      str match {
        // daca string ul s a terminat, parcurg toate tranzitiile pe eps posibile si verific,
        // la fiecare pas, daca starea in care ajung este finala
        case "" => nextCTransition(state, "eps").foldLeft(false)((acc: Boolean, nextState: A) =>
          acc || auxAccepts("", nextState)) || isFinal(state)
        // altfel, parcurg toate tranzitiile pe c si eps pana la terminarea string ului
        case s => nextCTransition(state, s.head.toString).foldLeft(false)((acc: Boolean, nextState: A) =>
          acc || auxAccepts(s.tail, nextState)) ||
          nextCTransition(state, "eps").foldLeft(false)((acc: Boolean, nextState: A) => acc || auxAccepts(s, nextState))
      }
    }
    // apel initial
    auxAccepts(str, initialState)
  }

  // getters pentru toti parametrii
  def getStates: Set[A] = states
  def getAlphabet: Set[Char] = alphabet
  def getDelta: Map[A, Map[String, Set[A]]] = delta
  def getInitialState: A = initialState
  def getFinalStates: Set[A] = finalStates

  // verifica daca state ul primit este final
  def isFinal(state: A): Boolean = finalStates.contains(state)
}

// This is a companion object to the Nfa class. This allows us to call the method fromPrenex without instantiating the Nfa class beforehand.
// You can think of the methods of this object like static methods of the Nfa class
object Nfa {
  // reaalizeaza opeartia de CONCAT pentru 2 nfa uri
  def concatOperation(nfa1: Nfa[Int], nfa2: Nfa[Int]): Nfa[Int] = {
    // incrementez toate state urile din al 2lea Nfa cu numarul de state uri din primul
    val nfaMapped: Nfa[Int] = nfa2.map(x => x + nfa1.getStates.size)
    // in plus fata de tranzitiile deja existente o sa apara una eps dintre finalState ul
    // primului Nfa si initialState ul celui de al 2lea modificat
    val epsTransition: Map[Int, Map[String, Set[Int]]] = Map(nfa1.getFinalStates.head -> Map("eps" -> Set(nfaMapped.getInitialState)))

    // crearea noului Nfa
    new Nfa[Int](nfa1.getStates.++(nfaMapped.getStates), nfa1.getAlphabet.++(nfaMapped.getAlphabet),
      nfa1.getDelta.++(nfaMapped.getDelta).++(epsTransition), nfa1.getInitialState, nfaMapped.getFinalStates)
  }

  // realizeaza operatia de UNION pentru 2 nfa uri
  def unionOperation(nfa1: Nfa[Int], nfa2: Nfa[Int]): Nfa[Int] = {
    // incrementez toate state urile din primul Nfa cu 1
    val nfaMapped1: Nfa[Int] = nfa1.map(x => x + 1)
    // incrementez toate state urile din al 2lea Nfa cu numarul de state uri din primul + 1
    val nfaMapped2: Nfa[Int] = nfa2.map(x => x + 1 + nfa1.getStates.size)
    // in plus fata de tranzitiile deja existente o sa apara 2 eps intre noul initialState si cele
    // doua initialState uri ale Nfa urilor modificate si 2 eps intre finalState urile Nfa urilor
    // modificate si noul finalState
    val epsTransition: Map[Int, Map[String, Set[Int]]] = Map(nfa1.getInitialState ->
      Map("eps" -> Set(nfaMapped1.getInitialState, nfaMapped2.getInitialState)),
      nfaMapped1.getFinalStates.head -> Map("eps" -> Set(nfaMapped2.getFinalStates.head + 1)),
      nfaMapped2.getFinalStates.head -> Map("eps" -> Set(nfaMapped2.getFinalStates.head + 1)))

    // crearea noului Nfa - 2 stari in plus fata de cele deja existente(noul initialState si noul finalState)
    new Nfa[Int](nfaMapped1.getStates.++(nfaMapped2.getStates).+(nfa1.getInitialState)
      .+(nfaMapped2.getFinalStates.head + 1), nfaMapped1.getAlphabet.++(nfaMapped2.getAlphabet),
      nfaMapped1.getDelta.++(nfaMapped2.getDelta).++(epsTransition), nfa1.getInitialState,
      nfaMapped2.getFinalStates.map(x => x + 1))
  }

  // realizeaza operatia de STAR pentru un nfa
  def starOperation(nfa: Nfa[Int]): Nfa[Int] = {
    // incrementez toate state urile din Nfa cu 1
    val nfaMapped: Nfa[Int] = nfa.map(x => x + 1)
    // in plus fata de tranzitiile deja existente o sa apara eps intre noul initialState si cel al Nfa ului,
    // 2 eps de la finalState ul primului Nfa la noul finalState si la initialState ul Nfa ului, o eps intre
    // noul initialState si noul finalState
    val epsTransition: Map[Int, Map[String, Set[Int]]] = Map(nfa.getInitialState ->
      Map("eps" -> Set(nfaMapped.getInitialState, nfaMapped.getFinalStates.head + 1)),
      nfaMapped.getFinalStates.head -> Map("eps" -> Set(nfaMapped.getInitialState, nfaMapped.getFinalStates.head + 1)))

    // crearea noului Nfa - 2 stari in plus fata de cele deja existente(noul initialState si noul finalState)
    new Nfa[Int](nfaMapped.getStates.+(nfa.getInitialState).+(nfaMapped.getFinalStates.head + 1), nfaMapped.getAlphabet,
      nfaMapped.getDelta.++(epsTransition), nfa.getInitialState, nfaMapped.getFinalStates.map(x => x + 1))
  }

  // parcurg lista de string uri de la final la inceput pana la terminarea acesteia;
  // cu ajutorul unei stive realizez operatiile definite anterior(in cazul unei operatii
  // scot de pe stiva numarul de parametrii corespunzatori operatiei, o aplic, iar apoi
  // adaug rezultatul in stiva; in cazul unui simplu caracter/eps ii creez Nfa ul si il
  // adaug in stiva, iar in cazul void returnez direct Nfa ul corespunzator)
  def generateNfa(strList: List[String], stack: mutable.Stack[Nfa[Int]]): Nfa[Int] = {
    strList match {
      case Nil => stack.pop()
      case init :+ last =>
        last match {
          case "void" => new Nfa[Int](Set[Int](1, 2), Set[Char](), Map[Int, Map[String, Set[Int]]](),
            1, Set[Int](2))
          case "CONCAT" => generateNfa(init, stack.push(concatOperation(stack.pop(), stack.pop())))
          case "UNION" => generateNfa(init, stack.push(unionOperation(stack.pop(), stack.pop())))
          case "STAR" => generateNfa(init, stack.push(starOperation(stack.pop())))
          case "PLUS" =>
            val nfaPop = stack.pop()
            generateNfa(init, stack.push(concatOperation(nfaPop, starOperation(nfaPop))))
          case "MAYBE" => generateNfa(init, stack.push(unionOperation(stack.pop(), new Nfa[Int](Set[Int](1), Set[Char](),
            Map[Int, Map[String, Set[Int]]](), 1, Set[Int](1)))))
          case "eps" => generateNfa(init, stack.push(new Nfa[Int](Set[Int](1), Set[Char](),
            Map[Int, Map[String, Set[Int]]](), 1, Set[Int](1))))
          // in cazul unui caracter simplu exista mai multe posibilitati: - caracterul sa fie o ghilimea,
          // caz in care este clar ca urmatorul caracter este tot o ghilimea si in locul celor 2 creez un
          // Nfa cu alfabet space; - caracterul sa fie escapat de 2 ghilimele, caz in care creez Nfa cu
          // caracterul escapat; - cazul in care caracterul este normal si creez un Nfa cu el
          case c => c match {
            case "\'" => generateNfa(init.init, stack.push(new Nfa[Int](Set[Int](1, 2), Set[Char](' '),
              Map[Int, Map[String, Set[Int]]](1 -> Map[String, Set[Int]](" " -> Set(2))), 1, Set[Int](2))))
            case e => e.head match {
              case '\'' => generateNfa(init, stack.push(new Nfa[Int](Set[Int](1, 2), Set[Char](e.tail.init.head),
                  Map[Int, Map[String, Set[Int]]](1 -> Map[String, Set[Int]](e.tail.init -> Set(2))), 1, Set[Int](2))))
              case s => generateNfa(init, stack.push(new Nfa[Int](Set[Int](1, 2), Set[Char](s),
                Map[Int, Map[String, Set[Int]]](1 -> Map[String, Set[Int]](e -> Set(2))), 1, Set[Int](2))))
            }
          }
      }
    }
  }

  def fromPrenex(str: String): Nfa[Int] = {
    generateNfa(str.split(' ').toList, mutable.Stack[Nfa[Int]]())
  }
}