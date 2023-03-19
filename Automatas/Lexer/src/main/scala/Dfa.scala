import scala.collection.mutable

// Dfa[A] este construit, ca in curs, din:
// - states: set de state uri[A]; toate state urile posibile
// - alphabet: Set[Char]
// - delta: map cu key A si value alt map cu key Char si value un state[A]; acesta reprezinta
// tranzitiile de la o stare la alte stari pe baza unui caracter din alfabet
// - initialState: state[A]; state ul initial
// - finalStates: set de state uri[A]; toate state urile finale
// - sinkState: un state in care daca se ajunge nu mai sunt posibile schimbari
class Dfa[A](states: Set[A], alphabet: Set[Char], delta: Map[A, Map[Char, A]], initialState: A,
             finalStates: Set[A], sinkState: A) {
  // The following methods are only the methods directly called by the test suite. You can (and should) define more.

  // returneaza un now Dfa care are toate state urile moodificate conform functiei primite
  def map[B](f: A => B) : Dfa[B] = new Dfa[B](states.map(f), alphabet, delta.map(x => (f(x._1),
    x._2.map(y => (y._1, f(y._2))))), f(initialState), finalStates.map(f), f(sinkState))

  // returneaza state ul urmator cu tranzitie pe c, iar in cazul in care nu exista -> sinkState
  def next(state:A, c: Char): A = {
    if (delta.get(state).foldLeft(false)((acc, i) => acc || i.contains(c))) delta(state)(c)
    else sinkState
  }

  // returneaza daca string ul primit ca parametru este acceptat
  def accepts(str: String): Boolean = {
    // functie auxiliara ce este apelata initial cu state = initialState,
    // iar apoi recursiv pana la terminarea stringului
    def auxAccepts(str: String, state: A): Boolean = {
      str match {
        // daca string ul s a terminat verific daca starea in care ajung este finala
        case "" => isFinal(state)
        // altfel, parcurg toate tranzitiile pana la terminarea string ului
        case s => alphabet.contains(s.head) && auxAccepts(s.tail, next(state, s.head))
      }
    }
    // apel initial
    auxAccepts(str, initialState)
  }

  // getters pentru toti parametrii
  def getStates : Set[A] = states
  def getAlphabet: Set[Char] = alphabet
  def getDelta: Map[A, Map[Char, A]] = delta
  def getInitialState: A = initialState
  def getFinalStates: Set[A] = finalStates
  def getSinkState: A = sinkState

  // verifica daca state ul primit este final
  def isFinal(state: A): Boolean = finalStates.contains(state)
  def isSinkState(state: A):Boolean = state == sinkState
}

// This is a companion object to the Dfa class. This allows us to call the method fromPrenex without instantiating the Dfa class beforehand.
// You can think of the methods of this object like static methods of the Dfa class
object Dfa {
  // genereaza, pentru un state dat, toate state urile in care se poate ajuge prin tranzitii
  // eps(epsilon closure - in curs); in caz ca nu exista -> returneaza un set gol
  def generateEpsClosure(state: Int, epsMap: Map[Int, Set[Int]]): Set[Int] = {
    // simulez un comportament recursiv folosind o coada pentru ca este mai eficient
    var epsClosure = Set[Int]()
    val queue = new scala.collection.mutable.Queue[Int]()
    queue.enqueue(state)
    while(queue.nonEmpty){
      val deqState = queue.dequeue()
      var newStates = Set[Int]()
      if (epsMap.contains(deqState)) newStates = epsMap(deqState) diff epsClosure else newStates = Set[Int]()
      queue ++= newStates
      epsClosure ++= newStates
    }
    epsClosure
  }

  // returnez un set cu toate starile in care se poate ajunge - in acest punct starile sunt sub
  // forma unui set de state uri(in curs: un state de Dfa este un set de state uri de Nfa);
  // folosesc o coada in care adaug noi stari in caz ca acestea nu au fost prelucrate deja
  def findReachableState(alphabet: Set[Char], reachableStates: Set[Set[Int]], queue: mutable.Queue[Set[Int]],
        nonEpsMap: Map[Int, Map[String, Set[Int]]], epsClosure: Map[Int, Set[Int]]): Set[Set[Int]] = {
    // daca coada e goala => toate starile reachable au fost prelucrate
    if (queue.isEmpty)
      reachableStates.foldLeft(reachableStates)((acc, x) => acc--reachableStates.filter(y => y.subsetOf(x) && y != x))
    else {
      // altfel, scot din coada un element pentru care prelucrez toate tranzitiile posibile
      val set = queue.dequeue()
      val reachable: Set[Set[Int]] = alphabet.foldLeft(Set[Set[Int]]())((acc1: Set[Set[Int]], c: Char)
        => acc1+set.foldLeft(Set[Int]())((acc: Set[Int], x: Int) =>
          if (nonEpsMap.contains(x) && nonEpsMap(x).head._1.head == c)
            acc.++(nonEpsMap(x).head._2.foldLeft(Set[Int]())((acc2: Set[Int], y: Int)
              => epsClosure(y)++acc2)) else acc))
      // daca starile nu au fost deja prelucarte le adaug in coada
      reachable.foreach(x => if (reachableStates.contains(x)) queue else queue.enqueue(x))
      // apel recursiv al functiei - adaug in set ul reachableStates noile stari reachable obtinute
      findReachableState(alphabet, reachable.++(reachableStates), queue, nonEpsMap, epsClosure)
    }
  }

  // functie ce genereaza tranzitiile unui Dfa stiind statesCode - codificarea dintre un state Dfa
  // si toate starile Nfa din care e compus, starea initiala pentru o tranzitia a Nfa ului, starea
  // finala si caracterul pe care este realizata tranzitia
  def generateTransition(statesCode: Map[Int, Set[Int]], initialState: Int, finalState: Int,
                         c: Char): Map[Int, Map[Char, Int]] = {
    // creez toate starile de pornire pe tranzitie ale Dfa ului
    val initialStates: Set[Int] = statesCode.filter(x => x._2.contains(initialState)).keySet
    // creez toate starile de sosire pe tranzitie ale Dfa ului
    val finalStates: Set[Int] = statesCode.filter(x => x._2.contains(finalState)).keySet

    // creeaza toate tranzitiile de forma pornire -> [caracter, sosire] pentru o stare
    // de pornire si un caracter
    def auxGenerate(x: Int, set: Set[Int], c: Char): (Int, Map[Char, Int]) = {
      x -> set.foldLeft(Map[Char, Int]())((acc: Map[Char, Int], y: Int) => acc+(c -> y))
    }

    // apelez auxGenerate pentru fiecare stare din initialStates
    initialStates.foldLeft(Map[Int, Map[Char, Int]]())((acc: Map[Int, Map[Char, Int]], x: Int)
      => acc+auxGenerate(x, finalStates, c))
  }

  // functie ce creeaza toate tranzitiile descrise anterior: preiau fiecare tranzitie din
  // nonEpsMap si calculez generateTransitions pentru elementele corespunzatoare
  def getTransitions(statesCode: Map[Int, Set[Int]], nonEpsMap: Map[Int, Map[String, Set[Int]]])
  : Map[Int, Map[Char, Int]] = {
    val mapTransitions: Map[Int, Map[Char, Int]] =  nonEpsMap.flatMap(x => x._2.flatMap(y => y._2.map(z
      => generateTransition(statesCode, x._1, z, y._1.head)))).flatten.foldLeft(Map[Int, Map[Char, Int]]())((acc, x)
      => if (acc.contains(x._1)) acc.updated(x._1, acc(x._1)++x._2) else acc.+(x))

    mapTransitions
  }

  // functie pentru generarea Dfa ului
  def generateDfa(nfa: Nfa[Int]): Dfa[Int] = {
    // epsMap - un map ce contine doar tranzitiile cu eps preluate din delta
    val epsMap: Map[Int, Set[Int]] = nfa.getDelta.filter(x => x._2.contains("eps")).foldLeft(Map[Int,
      Set[Int]]())((acc: Map[Int, Set[Int]], x: (Int, Map[String, Set[Int]])) => acc.+(x._1 -> x._2.head._2))

    // nonEpsMap - un map ce contine doar tranzitiile cu eps preluate din delta
    val nonEpsMap: Map[Int, Map[String, Set[Int]]] = nfa.getDelta.filter(x => !x._2.contains("eps"))

    // epsClosure - contine epsClosure penntru toate state urile
    val epsClosure: Map[Int, Set[Int]] = nfa.getStates.foldLeft(Map[Int, Set[Int]]())((acc: Map[Int, Set[Int]],
      x: Int) => acc.+(x -> generateEpsClosure(x, epsMap).+(x)))

    // reachableStates - toate starile in care se ajunge prin tranzitii nonEps
    val reachableStates: Set[Set[Int]] = findReachableState(nfa.getAlphabet, Set[Set[Int]](epsClosure(1)),
      mutable.Queue[Set[Int]](epsClosure(1)), nonEpsMap, epsClosure)

    def compareLists(list1: List[Int], list2: List[Int]): Boolean = {
      if (list1.head == list2.head) compareLists(list1.tail, list2.tail) else list1.head < list2.head
    }
    // atribui fiecarei stari din reachableStates o stare Dfa
    val dfaStatesCode: Map[Int, Set[Int]] = reachableStates.toList.sortWith((x: Set[Int], y:Set[Int])
      => compareLists(x.toList.sorted, y.toList.sorted)).foldLeft(Map[Int, Set[Int]]())((acc: Map[Int,
      Set[Int]], x: Set[Int]) => acc+(acc.size + 1 -> x))

    // calculez noul Map de tranzitii
    val dfaDelta: Map[Int, Map[Char, Int]] = getTransitions(dfaStatesCode, nonEpsMap)

    // calculez noile stari finale(o stare Dfa este finala daca contine cel putin o stare Nfa
    // finala in structura sa)
    val dfaFinalStates: Set[Int] = dfaStatesCode.filter(x => x._2.contains(nfa.getFinalStates.head)).keySet

    // noua starea initiala este starea ce contine in structura sa starea Nfa initiala
    val dfaInitialState: Int = dfaStatesCode.filter(x => x._2.contains(nfa.getInitialState)).keySet.head

    new Dfa[Int](dfaStatesCode.keySet, nfa.getAlphabet, dfaDelta, dfaInitialState, dfaFinalStates, Int.MinValue)
  }

  def fromPrenex(str: String): Dfa[Int] = {
    generateDfa(Nfa.fromPrenex(str))
  }
}