import pokemon.ArmedPokemon;

import java.util.Random;
import java.util.logging.Level;

public class Battle extends Thread{
    private ArmedPokemon pokemon1;
    private ArmedPokemon pokemon2;
    private Arena arena;

    public Battle(ArmedPokemon pokemon1, ArmedPokemon pokemon2, Arena arena) {
        this.pokemon1 = pokemon1;
        this.pokemon2 = pokemon2;
        this.arena = arena;
    }

    public ArmedPokemon getPokemon1() { return pokemon1; }
    public void setPokemon1(ArmedPokemon pokemon1) { this.pokemon1 = pokemon1; }
    public ArmedPokemon getPokemon2() { return pokemon2; }
    public void setPokemon2(ArmedPokemon pokemon2) { this.pokemon2 = pokemon2; }
    public Arena getArena() { return arena; }
    public void setArena(Arena arena) { this.arena = arena; }

    public void run() {
        arena.enterArena();
        Main.logger.log(Level.INFO, "A inceput batalia dintre " + pokemon1.getPokemon().toString() +
                " \nsi " + pokemon2.getPokemon().toString() + "\n");
        while (pokemon1.isAlive() && pokemon2.isAlive()) {
            int dmg = 0;
            pokemon1.restartChecks();
            int randomAttack = new Random().nextInt(3);
            if (pokemon2.isEnableNextRound())
                dmg = getDmg(dmg, randomAttack, pokemon1);

            int dmg2 = 0;
            pokemon2.restartChecks();
            int randomAttack2 = new Random().nextInt(3);
            if (pokemon1.isEnableNextRound())
                dmg2 = getDmg(dmg2, randomAttack2, pokemon2);

            changesAfterBattle(dmg, pokemon1, pokemon2);
            changesAfterBattle(dmg2, pokemon2, pokemon1);
        }

        Main.logger.log(Level.INFO, "Batalia s-a incheiat\n");
        getWinner();

        resetAfterBattle(pokemon1);
        resetAfterBattle(pokemon2);

        arena.leaveArena();
    }

    private void resetAfterBattle(ArmedPokemon pokemon) {
        pokemon.setCurrentHp(pokemon.getPokemon().getHp());
        pokemon.setCooldownFor1Ability(0);
        pokemon.setCooldownFor2Ability(0);
        pokemon.setEnableNextRound(true);
    }

    private void getWinner() {
        if (pokemon1.isAlive()) {
            if (pokemon1.getPokemon().getAttack() != 0)
                pokemon1.getPokemon().setAttack(pokemon1.getPokemon().getAttack() + 1);
            else
                pokemon1.getPokemon().setSpecialAttack(pokemon1.getPokemon().getSpecialAttack() + 1);

            pokemon1.getPokemon().setHp(pokemon1.getPokemon().getHp() + 1);
            pokemon1.getPokemon().setDefense(pokemon1.getPokemon().getDefense() + 1);
            pokemon1.getPokemon().setSpecialDefense(pokemon1.getPokemon().getSpecialDefense() + 1);

            Main.logger.log(Level.INFO, "Pokemul " + pokemon1.getPokemon().getName() + " a castiagat");
            Main.logger.log(Level.INFO, "Noile sale statistici: " + pokemon1.getPokemon().toString() + "\n");
        }
        else if (pokemon2.isAlive()) {
            if (pokemon2.getPokemon().getAttack() != 0)
                pokemon2.getPokemon().setAttack(pokemon2.getPokemon().getAttack() + 1);
            else
                pokemon2.getPokemon().setSpecialAttack(pokemon1.getPokemon().getSpecialAttack() + 1);

            pokemon2.getPokemon().setHp(pokemon2.getPokemon().getHp() + 1);
            pokemon2.getPokemon().setDefense(pokemon2.getPokemon().getDefense() + 1);
            pokemon2.getPokemon().setSpecialDefense(pokemon2.getPokemon().getSpecialDefense() + 1);

            Main.logger.log(Level.INFO, "Pokemul " + pokemon2.getPokemon().getName() + " a castiagat.");
            Main.logger.log(Level.INFO, "Noile sale statistici: " + pokemon2.getPokemon().toString() + "\n");
        } else {
            Main.logger.log(Level.INFO, "Egalitate\n");
        }
    }

    private void changesAfterBattle(int dmg, ArmedPokemon pokemon1, ArmedPokemon pokemon2) {
        // Daca pokemonul1 e stun => nu poate ataca data viitoare, dar runda aceasta da
        if (pokemon2.isEnableNextRound()) {
            // Daca al 2lea pokemon nu e dodge => ii scade viata
            if (!pokemon2.isCheckDodge()) {
                // Daca atacul a fost normal
                if (pokemon1.isCheckNormalAttack()) {
                    Main.logger.log(Level.INFO, pokemon1.getPokemon().getName() + ": atac normal cu dmg: " + dmg);
                    if (dmg > pokemon2.getPokemon().getDefense())
                        pokemon2.setCurrentHp(pokemon2.getCurrentHp() - dmg + pokemon2.getPokemon().getDefense());
                }
                // Daca atacul a fost special
                if (pokemon1.isCheckSpecialAttack()) {
                    Main.logger.log(Level.INFO, pokemon1.getPokemon().getName() + ": atac special cu dmg: " + dmg);
                    if (dmg > pokemon2.getPokemon().getSpecialDefense())
                        pokemon2.setCurrentHp(pokemon2.getCurrentHp() - dmg + pokemon2.getPokemon().getSpecialDefense());
                }
                // Daca atacul a fost abilitate
                if (pokemon1.isCheckAbility()) {
                    Main.logger.log(Level.INFO, pokemon1.getPokemon().getName() + ": atac abilitate cu dmg: " + dmg);
                    pokemon2.setCurrentHp(pokemon2.getCurrentHp() - dmg);
                }
            }
            else
            {
                Main.logger.log(Level.INFO, pokemon2.getPokemon().getName() + " s-a ferit");
            }
            if (pokemon1.getCooldownFor1Ability() > 0) {
                Main.logger.log(Level.INFO, "Cooldown abilite 1: " + pokemon1.getCooldownFor1Ability());
                pokemon1.setCooldownFor1Ability(pokemon1.getCooldownFor1Ability() - 1);
            }
            if (pokemon1.getCooldownFor2Ability() > 0) {
                Main.logger.log(Level.INFO, "Cooldown abilite 2: " + pokemon1.getCooldownFor2Ability());
                pokemon1.setCooldownFor2Ability(pokemon1.getCooldownFor2Ability() - 1);
            }
            Main.logger.log(Level.INFO, pokemon2.getPokemon().getName() + ": HP ramas: " + pokemon2.getCurrentHp() + "\n");
        } else {
            Main.logger.log(Level.INFO, pokemon1.getPokemon().getName() + " este imobilizat\n");
            pokemon2.setEnableNextRound(true);
        }
        if (pokemon2.isCheckStun()) {
            pokemon2.setEnableNextRound(false);
        }
    }

    private int getDmg(int dmg, int randomAttack, ArmedPokemon pokemon1) {
        if (randomAttack == 0) {
            dmg = getNormalDmg(pokemon1);
        }
        if (randomAttack == 1) {
            if (pokemon1.getCooldownFor1Ability() == 0 && pokemon1.getPokemon().getFirstAbility() != null) {
                dmg = getFirstAbilityDmg(pokemon1);
            } else {
                do {
                    randomAttack = new Random().nextInt(3);
                } while (randomAttack == 1);
                if (randomAttack == 0) {
                    dmg = getNormalDmg(pokemon1);
                }
                if (randomAttack == 2) {
                    if (pokemon1.getCooldownFor2Ability() == 0 && pokemon1.getPokemon().getSecondAbility() != null) {
                        dmg = getSecondAbilityDmg(pokemon1);
                    } else {
                        dmg = getNormalDmg(pokemon1);
                    }
                }
            }
        }
        if (randomAttack == 2) {
            if (pokemon1.getCooldownFor2Ability() == 0 && pokemon1.getPokemon().getSecondAbility() != null) {
                dmg = getSecondAbilityDmg(pokemon1);
            } else {
                do {
                    randomAttack = new Random().nextInt(3);
                } while (randomAttack == 2);
                if (randomAttack == 0) {
                    dmg = getNormalDmg(pokemon1);
                }
                if (randomAttack == 1) {
                    if (pokemon1.getCooldownFor1Ability() == 0 && pokemon1.getPokemon().getFirstAbility() != null) {
                        dmg = getFirstAbilityDmg(pokemon1);
                    } else {
                        dmg = getNormalDmg(pokemon1);
                    }
                }
            }
        }
        return dmg;
    }

    private int getSecondAbilityDmg(ArmedPokemon pokemon) {
        int dmg;
        pokemon.setCheckAbility(true);
        dmg = pokemon.getPokemon().getSecondAbility().getDamage();
        if (pokemon.getPokemon().getSecondAbility().isDodge()) {
            pokemon.setCheckDodge(true);
        }
        if (pokemon.getPokemon().getSecondAbility().isStun()) {
            pokemon.setCheckStun(true);
        }
        pokemon.setCooldownFor2Ability(pokemon.getPokemon().getSecondAbility().getCooldown());
        return dmg;
    }

    private int getFirstAbilityDmg(ArmedPokemon pokemon) {
        int dmg;
        pokemon.setCheckAbility(true);
        dmg = pokemon.getPokemon().getFirstAbility().getDamage();
        if (pokemon.getPokemon().getFirstAbility().isDodge())
            pokemon.setCheckDodge(true);
        if (pokemon.getPokemon().getFirstAbility().isStun())
            pokemon.setCheckStun(true);
        pokemon.setCooldownFor1Ability(pokemon.getPokemon().getFirstAbility().getCooldown());
        return dmg;
    }

    private int getNormalDmg(ArmedPokemon pokemon) {
        int dmg;
        if (pokemon.getPokemon().getAttack() != 0) {
            dmg = pokemon.getPokemon().getAttack();
            pokemon.setCheckNormalAttack(true);
        } else {
            dmg = pokemon.getPokemon().getSpecialAttack();
            pokemon.setCheckSpecialAttack(true);
        }
        return dmg;
    }

}
