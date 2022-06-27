import java.util.*;

public class Main {
    Map<Integer, ArrayList<Student>> m =
            new TreeMap<Integer, ArrayList<Student>>(new Comparator<Integer>(){
        public int compare(Integer o1, Integer o2) {
            return o2.compareTo(o1);
        }
    });

    public void adaugaStudent(Student s) {
        ArrayList<Student> lista;
        if(m.containsKey(Math.round(s.getMedie()))) {
            lista = m.get(Math.round(s.getMedie()));
            lista.add(s);
        } else {
            lista = new ArrayList<Student>();
            lista.add(s);
            m.put(Math.round(s.getMedie()), lista);
        }

    }

    public static void main(String[] args) {
        Main catalog = new Main();
        catalog.adaugaStudent(new Student("Radu", 5.2f));
        catalog.adaugaStudent(new Student("Andrei", 5.4f));
        catalog.adaugaStudent(new Student("Mihai", 6.5f));
        catalog.adaugaStudent(new Student("Alex", 7.3f));
        catalog.adaugaStudent(new Student("Rares", 7.4f));

        for(Map.Entry<Integer, ArrayList<Student> > intrare : catalog.m.entrySet())
        {
            System.out.print(intrare.getKey() + " ");
            System.out.println(intrare.getValue());
        }

    }



}
