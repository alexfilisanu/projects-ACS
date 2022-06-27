package command;

import pokemon.Pokemon;

public class BradDeCraciunCommand implements Command{
    public void execute(Pokemon pokemon) {
        if (pokemon.getAttack() != 0)
            pokemon.setAttack(pokemon.getAttack() + 3);
        pokemon.setDefense(pokemon.getDefense() + 1);
    }
}
