import pokemon.Pokemon;
import trainer.PokemonTrainer;
import trainer.TrainerReading;

import java.io.IOException;
import java.util.List;
import java.util.Scanner;
import java.util.logging.*;

public class Main {
    private static Main uniqueInstance;
    private Main() {}
    public static Main Instance() {
        if (uniqueInstance == null)
            uniqueInstance = new Main();
        return uniqueInstance;
    }
    static List<Pokemon> allPokemons;
    public List<Pokemon> getAllPokemons() { return allPokemons; }
    public void setAllPokemons(List<Pokemon> allPokemons) { this.allPokemons = allPokemons; }

    final static Logger logger = Logger.getLogger(Main.class.getName());

    public static void main(String[] args) throws IOException, InterruptedException, CloneNotSupportedException {
        Scanner s = new Scanner(System.in);
        System.out.println("Numarul testului ales este(1: 10): ");
        int number = s.nextInt();

        System.out.println("Locul de afisare a bataliei alese este(standard/fisier): ");
        String location = s.next();
        if (location.equals("fisier")) {
            FileHandler fileHandler = new FileHandler(".\\Outputs\\output_test" + number);
            logger.addHandler(fileHandler);
            fileHandler.setFormatter(new SimpleFormatter());
            logger.setUseParentHandlers(false);
        }
            Administration administration = new Administration();
            Main main = Instance();
            main.setAllPokemons(administration.readAllPokemons());
            List<TrainerReading> trainers = administration.readAllTrainers(".\\Tests\\test" + number + ".json");

            PokemonTrainer pokemonTrainer1 = administration.getPokemonTrainer(trainers.get(0), main.getAllPokemons());
            PokemonTrainer pokemonTrainer2 = administration.getPokemonTrainer(trainers.get(1), main.getAllPokemons());

            TrainersBattle trainersBattle = new TrainersBattle(pokemonTrainer1, pokemonTrainer2, new Arena());
            trainersBattle.startBattle();
    }
}
