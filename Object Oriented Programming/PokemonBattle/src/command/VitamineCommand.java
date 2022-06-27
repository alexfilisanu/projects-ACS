package command;

import pokemon.Pokemon;

public class VitamineCommand implements Command{
    public void execute(Pokemon pokemon) {
        if (pokemon.getAttack() != 0)
            pokemon.setAttack(pokemon.getAttack() + 2);
        if (pokemon.getSpecialAttack() != 0)
            pokemon.setSpecialAttack(pokemon.getSpecialAttack() + 2);
        pokemon.setHp(pokemon.getHp() + 2);
    }
}
