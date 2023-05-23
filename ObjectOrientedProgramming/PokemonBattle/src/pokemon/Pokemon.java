package pokemon;

public class Pokemon implements Prototype {
    private String name;
    private int hp;
    private int attack;
    private int specialAttack;
    private int defense;
    private int specialDefense;
    private Ability firstAbility;
    private Ability secondAbility;

    public Pokemon() {}

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public int getHp() { return hp; }
    public void setHp(int hp) { this.hp = hp; }
    public int getAttack() { return attack; }
    public void setAttack(int attack) { this.attack = attack; }
    public int getSpecialAttack() { return specialAttack; }
    public void setSpecialAttack(int specialAttack) { this.specialAttack = specialAttack; }
    public int getDefense() { return defense; }
    public void setDefense(int defense) { this.defense = defense; }
    public int getSpecialDefense() { return specialDefense; }
    public void setSpecialDefense(int specialDefense) { this.specialDefense = specialDefense; }
    public Ability getFirstAbility() { return firstAbility; }
    public void setFirstAbility(Ability firstAbility) { this.firstAbility = firstAbility; }
    public Ability getSecondAbility() { return secondAbility; }
    public void setSecondAbility(Ability secondAbility) { this.secondAbility = secondAbility; }

    @Override
    public String toString() {
        return "pokemon.Pokemon{" +
                "name='" + name + '\'' +
                ", hp=" + hp +
                ", attack=" + attack +
                ", specialAttack=" + specialAttack +
                ", defense=" + defense +
                ", specialDefense=" + specialDefense +
                ", firstAbility=" + firstAbility +
                ", secondAbility=" + secondAbility +
                '}';
    }

    @Override
    public Prototype clone() {
        Pokemon pokemon = new PokemonBuilder()
                .withName(name)
                .withHp(hp)
                .withAttack(attack)
                .withSpecialAttack(specialAttack)
                .withDefense(defense)
                .withSpecialDefense(specialDefense)
                .withFirstAbility(firstAbility)
                .withSecondAbility(secondAbility)
                .build();
        return pokemon;
    }

}
