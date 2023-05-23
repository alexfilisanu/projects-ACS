package command;

import pokemon.Pokemon;

public class PelerinaCommand implements Command{
    public void execute(Pokemon pokemon) {
        pokemon.setSpecialDefense(pokemon.getSpecialDefense() + 3);
    }
}
