package command;
import pokemon.Pokemon;

public class BaghetaMagicaCommand implements Command{
    public void execute(Pokemon pokemon) {
        if (pokemon.getSpecialAttack() != 0)
            pokemon.setSpecialAttack(pokemon.getSpecialAttack() + 3);
    }
}
