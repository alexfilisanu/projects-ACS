package trainer;

import java.util.LinkedHashMap;
import java.util.List;

public class TrainerReading {
    private String name;
    private int age;
    private LinkedHashMap<String, List<String>> pokemonsMap;

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }
    public LinkedHashMap<String, List<String>> getPokemonsMap() { return pokemonsMap; }
    public void setPokemonsMap(LinkedHashMap<String, List<String>> pokemonsMap) { this.pokemonsMap = pokemonsMap; }

    @Override
    public String toString() {
        return "trainer.TrainerReading{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", pokemonsMap=" + pokemonsMap +
                '}';
    }
}
