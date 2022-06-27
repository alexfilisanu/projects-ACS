package command;

import pokemon.Pokemon;

public class SabiutaCommand implements Command{
    public void execute(Pokemon pokemon) {
        if (pokemon.getAttack() != 0)
            pokemon.setAttack(pokemon.getAttack() + 3);
    }
}
