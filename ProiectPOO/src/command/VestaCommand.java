package command;

import pokemon.Pokemon;

public class VestaCommand implements Command{
    public void execute(Pokemon pokemon) {
        pokemon.setHp(pokemon.getHp() + 10);
    }
}
