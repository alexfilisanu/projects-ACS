package pokemon;

public class ArmedPokemon implements Comparable<ArmedPokemon>{
    private Pokemon pokemon;
    private int currentHp;
    private int cooldownFor1Ability;
    private int cooldownFor2Ability;
    private boolean checkNormalAttack;
    private boolean checkSpecialAttack;
    private boolean checkAbility;
    private boolean checkStun;
    private boolean checkDodge;
    private boolean enableNextRound;

    public ArmedPokemon(Pokemon pokemon) {
        this.pokemon = pokemon;
        this.currentHp = this.pokemon.getHp();
        this.cooldownFor1Ability = 0;
        this.cooldownFor2Ability = 0;
        this.checkNormalAttack = false;
        this.checkSpecialAttack = false;
        this.checkAbility = false;
        this.checkStun = false;
        this.checkDodge = false;
        this.enableNextRound = true;
    }

    public Pokemon getPokemon() { return pokemon; }
    public void setPokemon(Pokemon pokemon) { this.pokemon = pokemon; }
    public boolean isCheckNormalAttack() { return checkNormalAttack; }
    public void setCheckNormalAttack(boolean checkNormalAttack) { this.checkNormalAttack = checkNormalAttack; }
    public boolean isCheckSpecialAttack() { return checkSpecialAttack; }
    public void setCheckSpecialAttack(boolean checkSpecialAttack) { this.checkSpecialAttack = checkSpecialAttack; }
    public boolean isCheckAbility() { return checkAbility; }
    public void setCheckAbility(boolean checkAbility) { this.checkAbility = checkAbility; }
    public boolean isCheckStun() { return checkStun; }
    public void setCheckStun(boolean checkStun) { this.checkStun = checkStun; }
    public boolean isCheckDodge() { return checkDodge; }
    public void setCheckDodge(boolean checkDodge) { this.checkDodge = checkDodge; }
    public int getCurrentHp() { return currentHp; }
    public void setCurrentHp(int currentHp) { this.currentHp = currentHp; }
    public int getCooldownFor1Ability() { return cooldownFor1Ability; }
    public void setCooldownFor1Ability(int cooldownFor1Ability) { this.cooldownFor1Ability = cooldownFor1Ability; }
    public int getCooldownFor2Ability() { return cooldownFor2Ability; }
    public void setCooldownFor2Ability(int cooldownFor2Ability) { this.cooldownFor2Ability = cooldownFor2Ability; }
    public boolean isEnableNextRound() { return enableNextRound;}
    public void setEnableNextRound(boolean enableNextRound) { this.enableNextRound = enableNextRound; }

    @Override
    public String toString() {
        return "pokemon.ArmedPokemon{" +
                "pokemon=" + pokemon +
                '}';
    }

    public boolean isAlive() {
        return currentHp > 0;
    }


    public int getScore() {
        return this.pokemon.getAttack() + this.pokemon.getSpecialAttack() + this.pokemon.getDefense() +
                this.pokemon.getSpecialDefense() + this.pokemon.getHp();
    }

    @Override
    public int compareTo(ArmedPokemon o) {
         if (this.getScore() == o.getScore())
             return this.getPokemon().getName().compareTo(o.getPokemon().getName());
         return this.getScore() - o.getScore();
    }

    public void restartChecks() {
        this.checkNormalAttack = false;
        this.checkSpecialAttack = false;
        this.checkAbility = false;
        this.checkStun = false;
        this.checkDodge = false;
    }
}
