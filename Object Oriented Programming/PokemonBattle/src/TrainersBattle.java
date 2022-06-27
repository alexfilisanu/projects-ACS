import pokemon.ArmedPokemon;
import pokemon.Pokemon;
import trainer.PokemonTrainer;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class TrainersBattle {
    private PokemonTrainer trainer1;
    private PokemonTrainer trainer2;
    private Arena arena;

    public TrainersBattle(PokemonTrainer trainer1, PokemonTrainer trainer2, Arena arena){
        this.trainer1 = trainer1;
        this.trainer2 = trainer2;
        this.arena = arena;
    }

    public void startBattle() throws InterruptedException, CloneNotSupportedException {
        ArrayList<Battle> battles = new ArrayList<>();
        while (true) {
            int randomEvent = new Random().nextInt(3) + 1;
            if (randomEvent == 1) {
                battleWithNeutrel(battles, 0);
                for (Battle battle : battles)
                    battle.join();
            }
            if (randomEvent == 2) {
                battleWithNeutrel(battles, 1);
                for (Battle battle : battles)
                    battle.join();
            }
            if (randomEvent == 3) {
                for (int indexBattle = 0; indexBattle < 3; indexBattle++) {
                    createBattleBetween2Pokemons(battles, indexBattle);
                }
                for (Battle battle : battles)
                    battle.join();
                break;
            }
        }
        Collections.sort(trainer1.getArmedPokemons());
        Collections.sort(trainer2.getArmedPokemons());
        createBattleBetween2Pokemons(battles, 2);
    }

    private void createBattleBetween2Pokemons(ArrayList<Battle> battles, int indexBattle) {
        ArmedPokemon pokemon1 = trainer1.getArmedPokemons().get(indexBattle);
        ArmedPokemon pokemon2 = trainer2.getArmedPokemons().get(indexBattle);
        Battle battleThread = new Battle(pokemon1, pokemon2, this.arena);
        battleThread.start();
        battles.add(battleThread);
    }

    private void battleWithNeutrel(ArrayList<Battle> battles, int indexNeutrel) throws CloneNotSupportedException {
        createBattleBetweenPokemonAndNeutrel(battles, indexNeutrel, trainer1);
        createBattleBetweenPokemonAndNeutrel(battles, indexNeutrel, trainer2);
    }

    private void createBattleBetweenPokemonAndNeutrel(ArrayList<Battle> battles, int indexNeutrel, PokemonTrainer trainer1) throws CloneNotSupportedException {
        int randomPokemon1 = new Random().nextInt(3);
        ArmedPokemon pokemon1 = trainer1.getArmedPokemons().get(randomPokemon1);
        ArmedPokemon pokemon2 = new ArmedPokemon((Pokemon) Main.allPokemons.get(indexNeutrel).clone());
        Battle battleThread = new Battle(pokemon1, pokemon2, this.arena);
        battleThread.start();
        battles.add(battleThread);
    }
}
