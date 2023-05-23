package trainer;

import pokemon.ArmedPokemon;

import java.util.List;

public class PokemonTrainer {
    private String name;
    private int age;
    private List<ArmedPokemon> armedPokemons;

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }
    public List<ArmedPokemon> getArmedPokemons() { return armedPokemons; }
    public void setArmedPokemons(List<ArmedPokemon> armedPokemons) { this.armedPokemons = armedPokemons; }

    public PokemonTrainer() { }


    @Override
    public String toString() {
        return "trainer.PokemonTrainer{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", armedPokemons=" + armedPokemons +
                '}';
    }
}
