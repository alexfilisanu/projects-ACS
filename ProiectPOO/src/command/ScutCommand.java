package command;
//import pokemon.Pokemon;

import pokemon.Pokemon;

public class ScutCommand implements Command{
    public void execute(Pokemon pokemon) {
        pokemon.setDefense(pokemon.getDefense() + 2);
        pokemon.setSpecialDefense(pokemon.getSpecialDefense() + 2);
    }
}
