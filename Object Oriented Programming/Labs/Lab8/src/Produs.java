import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public abstract class Produs implements Comparable<Produs> {

    public static void pretRaftMaxim(List<? extends Produs> lista) {
        Optional<? extends Produs> max = lista.stream().max(Produs::compareTo);

        max.ifPresent(Produs::afiseaza);
    }

    public static void pretRaftMaximPerProdus(List<? extends Produs> lista) {
        lista.stream().collect(Collectors.groupingBy(produs -> produs.getClass()))
                .values().forEach(Produs::pretRaftMaxim);
    }

    @Override
    public int compareTo(Produs o) {
        return (int) (this.pretRaft() - o.pretRaft());
    }

    abstract double pretRaft();

    abstract void afiseaza();
}
