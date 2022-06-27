package trainer;

import pokemon.ArmedPokemon;

import java.util.List;

public class PokemonTrainerBuilder {
    private PokemonTrainer pokemonTrainer = new PokemonTrainer();
    public PokemonTrainer build() {
            return pokemonTrainer;
        }

    public PokemonTrainerBuilder withName(String name) {
        pokemonTrainer.setName(name);
        return this;
    }

    public PokemonTrainerBuilder withAge(int age) {
        pokemonTrainer.setAge(age);
        return this;
    }
    public PokemonTrainerBuilder withArmedPokemons(List<ArmedPokemon> armedPokemonsList) {
        pokemonTrainer.setArmedPokemons(armedPokemonsList);
        return this;
    }

}

