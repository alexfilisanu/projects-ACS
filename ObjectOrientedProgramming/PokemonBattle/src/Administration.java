import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.google.gson.stream.JsonReader;
import command.*;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import command.BaghetaMagicaCommand;
import command.BradDeCraciunCommand;
import command.PelerinaCommand;
import command.ScutCommand;
import command.VestaCommand;
import command.VitamineCommand;
import pokemon.ArmedPokemon;
import pokemon.Pokemon;
import trainer.PokemonTrainer;
import trainer.PokemonTrainerBuilder;
import trainer.TrainerReading;

public class Administration {
    public List<Pokemon> readAllPokemons() throws FileNotFoundException {
        Type formatType = new TypeToken<List<Pokemon>>() {}.getType();
        Gson gson = new Gson();
        JsonReader reader = new JsonReader(new FileReader("Inputs\\pokemons.json"));
        return gson.fromJson(reader, formatType);
    }

    public List<TrainerReading> readAllTrainers(String input) throws FileNotFoundException {
        Type formatType = new TypeToken<List<TrainerReading>>() {}.getType();
        Gson gson = new Gson();
        JsonReader reader = new JsonReader(new FileReader(input));
        List<TrainerReading> trainers = gson.fromJson(reader, formatType);
        return trainers;
    }

    public PokemonTrainer getPokemonTrainer(TrainerReading trainer, List<Pokemon> allPokemons) {
        List<ArmedPokemon> armedPokemons = new ArrayList<>();
        for (Map.Entry<String, List<String>> entry : trainer.getPokemonsMap().entrySet()) {
            for (int indexPokemon = 0; indexPokemon < allPokemons.size(); indexPokemon++) {
                if (entry.getKey().equals(allPokemons.get(indexPokemon).getName())) {
                    Pokemon pokemon = (Pokemon) allPokemons.get(indexPokemon).clone();
                    upgradePokemon(entry, pokemon);
                    armedPokemons.add(new ArmedPokemon(pokemon));
                    break;
                }
            }
        }
        PokemonTrainer pokemonTrainer = new PokemonTrainerBuilder()
                .withName(trainer.getName())
                .withAge(trainer.getAge())
                .withArmedPokemons(armedPokemons)
                .build();

        return pokemonTrainer;
    }

    private void upgradePokemon(Map.Entry<String, List<String>> entry, Pokemon pokemon) {
        for (String itemName : entry.getValue()) {
            switch (itemName) {
                case "Scut":
                    new ScutCommand().execute(pokemon);
                    break;
                case "Vesta":
                    new VestaCommand().execute(pokemon);
                    break;
                case "Sabiuta":
                    new SabiutaCommand().execute(pokemon);
                    break;
                case "Bagheta Magica":
                    new BaghetaMagicaCommand().execute(pokemon);
                    break;
                case "Vitamine":
                    new VitamineCommand().execute(pokemon);
                    break;
                case "Brad de Craciun":
                    new BradDeCraciunCommand().execute(pokemon);
                    break;
                case "Pelerina":
                    new PelerinaCommand().execute(pokemon);
                    break;
            }
        }
    }
}
